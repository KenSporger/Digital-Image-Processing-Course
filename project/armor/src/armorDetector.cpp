#include "armorDetector.h"


using namespace std;
using namespace cv;

namespace rm
{


ArmorDetector::ArmorDetector()
{
    _roi = Rect(cv::Point(0, 0), _srcImg.size());
    _status = ARMOR_NONE;
#if defined(DEBUG_DETECTION) || defined(SHOW_RESULT)
    _debugWindowName = "debug";
    namedWindow(_debugWindowName);
#endif
}

ArmorDetector::ArmorDetector(const ArmorParam &armorParam)
{
    _param = armorParam;
    _roi = Rect(cv::Point(0, 0), _srcImg.size());
    _status = ARMOR_NONE;
#if defined(DEBUG_DETECTION) || defined(SHOW_RESULT)
    _debugWindowName = "debug";
    namedWindow(_debugWindowName);
#endif
}

//设置装甲颜色
void ArmorDetector::setEnemyColor(int enemy_color)
{
    _enemy_color = enemy_color;
    _self_color = enemy_color == ARMOR_RED ? ARMOR_BLUE : ARMOR_RED;
}

//读取帧，并确定检测区域
void ArmorDetector::loadImg(const Mat &srcImg)
{
    _srcImg = srcImg;
#if defined(DEBUG_DETECTION) || defined(SHOW_RESULT)
    _debugImg = srcImg.clone();
#endif
    //srcImg的图像框
    Rect imgBound = Rect(Point(0, 0), _srcImg.size());

    //处于追踪某块装甲板（不需要搜索全图）
    if (_status == ARMOR_LOCAL)
    {
        Rect bRect = boundingRect(_targetArmor.vertex) + _roi.tl();
        //以中心为锚点放大2倍
        bRect = cvex::scaleRect(bRect, Vec2f(3,2));
        //通过与操作还解决了坐标溢出的问题
        _roi = bRect & imgBound;
        //由于_srcImg是对srcImg的引用，不能改变const对象
        // _srcImg(_roi).copyTo(_roiImg);
        _roiImg = _srcImg(_roi).clone();
    }
    else
    {
        //未来检索的区域取决于_roi矩形框
        _roi = imgBound;
        // srcImg.copyTo(_roiImg);
        _roiImg = srcImg.clone();

    }
#if defined(DEBUG_DETECTION) 
    //绘制_roi
    rectangle(_debugImg, _roi, cvex::WHITE);
#endif
    
}

//armor的vertex的坐标系是_roiImg，要进行转换平移
const std::vector<cv::Point2f> ArmorDetector::getArmorVertex() const
{
	vector<cv::Point2f> realVertex;
	for (int i = 0; i < 4; i++)
	{
		realVertex.emplace_back(Point2f(_targetArmor.vertex[i].x + _roi.tl().x,
										_targetArmor.vertex[i].y + _roi.tl().y));
	}
	return realVertex;
}


int ArmorDetector::detect()
{
    //清空所有装甲
    _armors.clear();
    //视野内的灯条
    vector<LightDescriptor> lightInfos;

    /*
    * 一、预处理(二值化获取灯条掩码)
    */
#if defined(TIME_COUNT)
    preProcessTS.clockStart();
#endif
    vector<Mat> channels;
    //rb通道差
    split(_roiImg, channels);
    _grayImg = _enemy_color == ARMOR_RED ? 
                    channels[2] - channels[0] : channels[0] - channels[2]; 
    Mat binBrightImg;
    threshold(_grayImg, binBrightImg, _param.brightness_threshold, 255, THRESH_BINARY);
    //膨胀获取完整的灯条
    Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    dilate(binBrightImg, binBrightImg, element);
#if defined(TIME_COUNT)
    preProcessTS.clockEnd();
#endif
#if defined(DEBUG_PRETREATMENT)
    cout << "显示预处理二值结果..." << endl;
    imshow("binBrightImg", binBrightImg);
    waitKey(0);
#endif
    /*
    * 二、查找灯条轮廓,并进行筛选
    */
#if defined(TIME_COUNT)
    findContourTS.clockStart();
#endif
    vector<vector<Point>> lightContours;
    //只检测外轮廓，仅保存轮廓的拐点
    findContours(binBrightImg, lightContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
#if defined(TIME_COUNT)
    findContourTS.clockEnd();
#endif
#if defined(DEBUG_DETECTION)
    //预览所有轮廓
    cout << "显示待筛选灯条轮廓..." << endl;
    cvex::showContours(_debugWindowName, _debugImg, _debugImg, lightContours, cvex::YELLOW, 0, _roi.tl());
#endif
#if defined(TIME_COUNT)
    contourFilterTS.clockStart();
#endif
    for (const auto &contour : lightContours)
    {
        //计算轮廓本身的面积
        float lightContourArea = contourArea(contour);
        //筛选条件1：椭圆拟合需要至少6个点
        //筛选条件2：轮廓面积
        if (contour.size() <= 5 ||
            lightContourArea < _param.light_min_area) continue;
        
        //最小二乘拟合椭圆，要求拟合的点至少为6个点
        //返回椭圆的最小外接矩形
        //RotatedRect包含外接矩形和拟合椭圆信息
        //fitEllipse和minAreaRect相似，但是minAreaRect的短边可能是height
        //fitEllipse结果的短边为width，长边为height
        //rect的角度取长边在行增加方向部分与列增加方向的夹角，范围：[0,180]
        RotatedRect lightRotateRect = fitEllipse(contour);
        //角度问题有待研究
        adjustRec(lightRotateRect);
        //筛选条件3:宽长比
        //筛选条件4:矩形度 
        if (lightRotateRect.size.height / lightRotateRect.size.width < _param.light_min_ratio ||
            lightContourArea / lightRotateRect.size.area() < _param.light_min_rectangularity)
            continue;

        //扩展旋转包围框的大小
        lightRotateRect.size.width *= _param.light_rect_extend_ratio;
        lightRotateRect.size.height *= _param.light_rect_extend_ratio;

        //以下由于原算法中一开始使用灰度化，灯条检测最后才用ＲＢ通道差，
        //所以这里获取外接矩形无用
        //获取外接矩形
        Rect lightBoundRect = lightRotateRect.boundingRect();
        //与当前兴趣区域交集(两个矩形重叠的矩形)
        //注意不能用roi,其坐标系不是同一个
        const Rect srcBound(Point(0, 0), _roiImg.size());
        lightBoundRect &= srcBound;

        if (lightBoundRect.empty()) continue;

        //获取灯条区域
        // Mat lightImg = _roiImg(lightBoundRect);
        //与外界矩形相同大小的灯条掩码
        Mat lightMask = Mat::zeros(lightBoundRect.size(), CV_8UC1);
        // //获取灯条包围矩形的顶点
        Point2f lightVertexArray[4];
        lightRotateRect.points(lightVertexArray);
        //顶点的坐标需要转换到lightMask坐标系
        vector<Point> lightVertex;
        for (int i = 0; i < 4; ++i)
        {
            lightVertex.emplace_back(lightVertexArray[i].x - lightBoundRect.tl().x, 
                                     lightVertexArray[i].y - lightBoundRect.tl().y);
                                                 //显示攻击的装甲
        }
        //填充多边形区域（在lightMask分割出灯条区域）
		fillConvexPoly(lightMask, lightVertex, 255);
        //该处筛选的位置和必要性？
        // if(lightImg.size().area() <= 0 || lightMask.size().area() <= 0) continue;
        dilate(lightMask, lightMask, element);

        lightInfos.emplace_back(lightRotateRect);

    }

#if defined(TIME_COUNT)
    contourFilterTS.clockEnd();
#endif
    //未检测到灯条
    if (lightInfos.empty())
    {
        return _status = ARMOR_NONE;
    }

#if defined(DEBUG_DETECTION)
   {
        //绘制筛选后的灯条
        vector<RotatedRect> _debugLights;
        for (auto &light : lightInfos)
        {
            _debugLights.push_back(light.rec());
        }
        //lightInfos中的lightRotateRect是基于_roiImg提取的，而_roiImg比srcImg小
        cout << "显示筛选后的灯条包围框..." << endl;
        cvex::showRectangles(_debugWindowName, _debugImg, _debugImg, _debugLights, cvex::GREEN, 0, _roi.tl());
   }
#endif

    /*
    * 三、同装甲灯条匹配
    */
#if defined(TIME_COUNT)
    armorFilterTS.clockStart();
#endif
   //按X坐标升序，lambda函数
    sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor &l1, const LightDescriptor &l2)->bool{
        return l1.center.x < l2.center.x;
    });

    for (size_t i = 0; i < lightInfos.size(); i++)
    {
        for (size_t j = i+1; j < lightInfos.size(); j++)
        {
            const LightDescriptor &leftLight = lightInfos[i];
            const LightDescriptor &rightLight = lightInfos[j];

            //匹配筛选1：角度差
            float angleDiff = abs(leftLight.angle - rightLight.angle);
            //匹配筛选2：长度差相对值
            float lenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
            if(angleDiff > _param.light_max_angle_diff ||
                lenDiff_ratio > _param.light_max_height_diff_ratio) continue;
        
            //匹配筛选3：中心距相对值
            float dist = cvex::caluDistance(leftLight.center, rightLight.center);
            float meanLen = (leftLight.length + rightLight.length) / 2;
            float dist_ratio = dist / meanLen;
            //匹配筛选4：中心纵向距离相对值
            float yDiff = abs(leftLight.center.y - rightLight.center.y);
            float yDiff_ratio = yDiff / meanLen;
            //匹配筛选4：中心横向距离相对值
            float xDiff = abs(leftLight.center.x - rightLight.center.x);
            float xDiff_ratio = xDiff / meanLen;

            if(yDiff_ratio > _param.light_max_y_diff_ratio ||
                xDiff_ratio < _param.light_min_x_diff_ratio ||
                dist_ratio > _param.armor_max_dist_ratio ||
                dist_ratio < _param.armor_min_dist_ratio)
            {
                continue;
            }

            //距离/灯长不随距离远近而明显变化，因此可作为装甲板大小的衡量标准
            int armorType = dist_ratio > _param.armor_big_armor_ratio ? BIG_ARMOR : SMALL_ARMOR;
            
            //rotationScore没看懂
            float ratiOff = (armorType == BIG_ARMOR) ? 
                            max(_param.armor_big_armor_ratio - dist_ratio, float(0)) : 
                            max(_param.armor_small_armor_ratio - dist_ratio, float(0));
            float yOff = yDiff / meanLen;
            float rotationScore = -(ratiOff * ratiOff + yOff * yOff);            

            //存储装甲信息
            _armors.emplace_back(leftLight, rightLight, armorType, rotationScore, _grayImg, _param);
            //一旦找到第一个匹配，就不在寻找
            break;
        }
    }
#if defined(TIME_COUNT)
    armorFilterTS.clockEnd();
#endif

    //未识别到装甲
    if (_armors.empty())
    {
        return _status = ARMOR_NONE;
    }
    //未理解
	// _armors.erase(remove_if(_armors.begin(), _armors.end(), [](ArmorDescriptor& i)
	// {
	// 	return !(i.isArmorPattern());
	// }), _armors.end());


    if (_armors.empty())
    {
        //无目标状态
        _targetArmor.clear();
        
        //从锁定目标到丢失目标
        if (_status == ARMOR_LOCAL)
        {
            return _status = ARMOR_LOST;
        }
        else
        {
            return _status = ARMOR_NONE;
        } 
    }

#ifdef DEBUG_DETECTION
    {
        //显示所有确定的装甲板轮廓
        vector<vector<Point>> armorContours;
        for (const auto &armor : _armors)
        {
            vector<Point> contour;
            for(const auto& point : armor.vertex)
            {
                contour.emplace_back(Point(point.x,point.y));
            }
            armorContours.emplace_back(contour);
        }
        cout << "显示所有检测到的装甲板轮廓" << endl;
        cvex::showContours(_debugWindowName, _debugImg, _debugImg, armorContours, cvex::MAGENTA, 0, _roi.tl());    
    }
#endif

    //最终分数为各项分数之和
    for(auto & armor : _armors)
	{
		armor.finalScore = armor.sizeScore + armor.distScore + armor.rotationScore;
	}
    //按分数降序排列
    sort(_armors.begin(), _armors.end(), [](const ArmorDescriptor &a1, const ArmorDescriptor &a2){
        return a1.finalScore > a2.finalScore; 
    });

    //分数最高者作为攻击目标
    _targetArmor = _armors[0];

#ifdef SHOW_RESULT
    //显示打击的装甲  
    {
        vector<vector<Point>> armorContours;
        vector<Point> contour;
        for(const auto& point : _targetArmor.vertex)
        {
            contour.emplace_back(Point(point.x,point.y));
        }
        armorContours.emplace_back(contour);

        cout << "显示预定打击的装甲板轮廓" << endl;
        if (_enemy_color == ARMOR_BLUE)
            cvex::showContours(_debugWindowName, _debugImg, _debugImg, armorContours, cvex::RED, -1, _roi.tl());
        else if (_enemy_color == ARMOR_RED)
            cvex::showContours(_debugWindowName, _debugImg, _debugImg, armorContours, cvex::BLUE, -1, _roi.tl());
    }
#endif
    return _status = ARMOR_LOCAL;

}


ArmorDescriptor::ArmorDescriptor()
{
    rotationScore = 0;
    sizeScore = 0;
    distScore = 0;
    finalScore = 0;
    type = UNKNOWN_ARMOR;
    vertex.resize(4);
    for(int i = 0; i < 4; i++)
	{
		vertex[i] = Point2f(0, 0);
	}
} 

//不太理解
ArmorDescriptor::ArmorDescriptor(const LightDescriptor &lLight, const LightDescriptor &rLight, const int armorType, const float rotaScore, Mat &grayImg, ArmorParam param)
{

    //获取两个灯条的旋转矩形
    lightPairs[0] = lLight.rec();
    lightPairs[1] = rLight.rec();

    //获取装甲板四个顶点
    cv::Size exLSize(int(lightPairs[0].size.width), int(lightPairs[0].size.height * 2));
	cv::Size exRSize(int(lightPairs[1].size.width), int(lightPairs[1].size.height * 2));
	cv::RotatedRect exLLight(lightPairs[0].center, exLSize, lightPairs[0].angle);
	cv::RotatedRect exRLight(lightPairs[1].center, exRSize, lightPairs[1].angle);

	cv::Point2f pts_l[4];
	exLLight.points(pts_l);
	cv::Point2f upper_l = pts_l[2];
	cv::Point2f lower_l = pts_l[3];

	cv::Point2f pts_r[4];
	exRLight.points(pts_r);
	cv::Point2f upper_r = pts_r[1];
	cv::Point2f lower_r = pts_r[0];

	vertex.resize(4);
	vertex[0] = upper_l;
	vertex[1] = upper_r;
	vertex[2] = lower_r;
	vertex[3] = lower_l;

    type = armorType;
    rotationScore = rotaScore;

    //不清楚为啥要前景图
    getFrontImg(grayImg);

    //计算大小得分
    float normalized_area = contourArea(vertex) / param.area_normalized_base;
	sizeScore = exp(normalized_area);

    //计算距离得分
	Point2f srcImgCenter(grayImg.cols / 2, grayImg.rows / 2);
	float sightOffset = cvex::caluDistance(srcImgCenter, cvex::crossPointOf(array<Point2f, 2>{vertex[0],vertex[2]}, array<Point2f, 2>{vertex[1],vertex[3]}));
	distScore = exp(-sightOffset / param.sight_offset_normalized_base);


}

//未理解
void ArmorDescriptor::getFrontImg(const Mat& grayImg)
{
	using cvex::caluDistance;
	const Point2f&
		tl = vertex[0],
		tr = vertex[1],
		br = vertex[2],
		bl = vertex[3];

	int width, height;
	if(type == BIG_ARMOR)
	{
		width = 92;
		height = 50;
	}
	else
	{
		width = 50;
		height = 50;
	}

	Point2f src[4]{Vec2f(tl), Vec2f(tr), Vec2f(br), Vec2f(bl)};
	Point2f dst[4]{Point2f(0.0, 0.0), Point2f(width, 0.0), Point2f(width, height), Point2f(0.0, height)};
	const Mat perspMat = getPerspectiveTransform(src, dst);
	cv::warpPerspective(grayImg, frontImg, perspMat, Size(width, height));
}



cv::RotatedRect &adjustRec(cv::RotatedRect &rec)
{
	using std::swap;

	float &width = rec.size.width;
	float &height = rec.size.height;
	float &angle = rec.angle;

	while (angle >= 90.0)
		angle -= 180.0;
	while (angle < -90.0)
		angle += 180.0;

	{
		if (angle >= 45.0)
		{
			swap(width, height);
			angle -= 90.0;
		}
		else if (angle < -45.0)
		{
			swap(width, height);
			angle += 90.0;
		}
	}

	return rec;
}

}




