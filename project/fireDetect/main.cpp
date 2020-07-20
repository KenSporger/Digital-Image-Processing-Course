#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat frame, fireMask, watermask;
Mat gray;
Mat hsvMat;
Mat foregroundMask;
Mat background;
int frame_cnt;
Point gun(173, 34);


void polynomialCurvefit(std::vector<cv::Point>& points, cv::Mat& A, int n)
{
    int N = points.size();

    //构造矩阵X
    cv::Mat X = cv::Mat::zeros(n + 1, n + 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            for (int k = 0; k < N; k++)
            {
                X.at<double>(i, j) = X.at<double>(i, j) + std::pow(points[k].x, i + j);
            }
        }
    }


    //构造矩阵Y
    cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++)
    {
        for (int k = 0; k < N; k++)
        {
            Y.at<double>(i, 0) = Y.at<double>(i, 0) + std::pow(points[k].x, i) * points[k].y;
        }
    }


    A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    //求解矩阵A
    cv::solve(X, Y, A, cv::DECOMP_SVD);
}

//火焰检测
void fireDectect()
{
    /*
    * HSV空间特征筛选
    */
   	cvtColor(frame, hsvMat, CV_BGR2HSV);
    inRange(hsvMat, Scalar(0, 100, 180), Scalar(30, 150, 255), fireMask);
    //膨胀获取较完整的火焰
    dilate(fireMask, fireMask, getStructuringElement(MORPH_RECT, Size(3, 3)));
    // morphologyEx(fireMask, fireMask, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));
    
    /*
    * 轮廓提取与筛选
    */
    vector<vector<Point>> fireCntr;
    findContours(fireMask, fireCntr, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //最大的外接矩形作为定位框    
    Rect maxBox = boundingRect(fireCntr[0]);
    for (int i = 1; i < fireCntr.size(); i++)
    {
        Rect box = boundingRect(fireCntr[i]);
        if (box.area() > 100 && box.area() > maxBox.area())
        {
            maxBox.x = box.x;
            maxBox.y = box.y;
            maxBox.width = box.width;
            maxBox.height = box.height;
        }

    }

    /*
    * 绘制定位框
    */   
    rectangle(frame, maxBox, Scalar(255, 0, 0));   
}


void waterDectect()
{

    //灰度图
    cvtColor(frame, gray, CV_BGR2GRAY);

    //背景建立
    if (frame_cnt == 0)
    {
        gray.copyTo(background);
        return;
    }

    /*
    * HSV筛选
    */
    cvtColor(frame, hsvMat, CV_BGR2HSV);
    inRange(hsvMat, Scalar(0, 0, 150), Scalar(140, 50, 255), watermask);

    /*
    * 背景差分筛选
    */    
    absdiff(gray, background, foregroundMask);
    medianBlur(foregroundMask, foregroundMask, 3);
    threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);  

    /*
    * HSV与背景差分交集
    */         
    bitwise_and(foregroundMask, watermask, foregroundMask);
    morphologyEx(foregroundMask, foregroundMask, MORPH_OPEN, getStructuringElement(MORPH_CROSS, Size(3, 3)));

    /*
    * 轮廓特征筛选
    */    
    vector<vector<Point>> contours;
    vector<Point> points;
    vector<Point> real_points;
    Point2f vtx[4];
    findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours)
    {
        if (contourArea(contour) < 10) continue;
        RotatedRect waterRotateRect = minAreaRect(contour);
        //角度限制、长宽比限制
        if (waterRotateRect.angle < -70 || 
            waterRotateRect.angle > -10 ||
            waterRotateRect.size.height / waterRotateRect.size.width < 0.7
            ) 
            continue;

        {
            //中心点作为拟合点集
            points.push_back(waterRotateRect.center);
        }

        waterRotateRect.points(vtx);
        for (int i = 0; i < 4; i++)
        {
            line(frame, vtx[i], vtx[(i+1)%4], Scalar(0,255,0), 2);
        }
    }


    /*
    * 根据相对位置删除假点
    */    
    sort(points.begin(), points.end(), [](const Point &p1, const Point &p2){
        return p1.x < p2.x;
    });
    real_points.push_back(gun);
    float rate = 0;
    for (const auto &p : points)
    {
        if (real_points.size() == 1)
        {
            rate = (gun.y - p.y) / (gun.x - p.x);
            real_points.push_back(p);
        }
        else
        {
            //当前点与上一点构成的斜率
            float _rate = (float)(real_points.back().y - p.y) / (real_points.back().x - p.x);
            if ((_rate - rate) < 1 && (_rate - rate) > 0)
            {
                real_points.push_back(p);
                rate = _rate;
            }
        }
        
    }

    //点集较少时不拟合
    if (real_points.size() > 4)
    {
        Mat A;
        polynomialCurvefit(real_points, A, 2);
        std::vector<cv::Point> points_fitted;
        for (int x = gun.x ; x < frame.cols; x++)
        {
            double y = A.at<double>(0, 0) + A.at<double>(1, 0) * x +
                A.at<double>(2, 0)*std::pow(x, 2) + A.at<double>(3, 0)*std::pow(x, 3);

            if (y < gun.y || y >= frame.rows) continue;
            points_fitted.push_back(cv::Point(x, y));
        }
        // polylines(frame, points_fitted, false, cv::Scalar(0, 255, 0), 2, 8, 0);
    }


}



int main()
{
	VideoCapture video;
	video.open("video/fire.mp4");

	if (!video.isOpened())
	{
		cerr << "can't read video !" << endl;
		return -1;
	}

	uchar key = 0;
    //历史处理时间
    double time_sum = 0;
	while (key != 'q')
	{
        
		video >> frame;

		if (frame.empty())
		{
			cerr << "can't read frame !" << endl;
            break;
		}
        double start = static_cast<double>(cvGetTickCount());
        //火焰检测定位
        // fireDectect();
        //水柱检测拟合
        waterDectect();
        double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
        cout << "processing time:" << time / 1000 << "ms" << endl;

        imshow("frame", frame);
		key = waitKey(0);
        ++frame_cnt;		
        time_sum += time / 1000;
	}
    cout << "总帧数: "<< frame_cnt << endl;
    cout << "平均处理时间:　"<< time_sum / frame_cnt << endl;
	
}
 