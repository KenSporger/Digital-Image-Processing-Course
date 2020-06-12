#ifndef __OPENCV_EXTENDED_H
#define __OPENCV_EXTENDED_H

#include<opencv2/opencv.hpp>

namespace cv
{

//+重载，旋转矩阵平移
template<typename T>
RotatedRect operator+(const RotatedRect &rec, const Point_<T> &p)
{
	return RotatedRect(Point2f(rec.center.x + p.x, rec.center.y + p.y), rec.size, rec.angle);
}

namespace cvex
{

const Scalar BLUE(255, 0, 0);
const Scalar GREEN(0, 255, 0);
const Scalar RED(0, 0, 255);
const Scalar MAGENTA(255, 0, 255);
const Scalar YELLOW(0, 255, 255);
const Scalar CYAN(255, 255, 0);
const Scalar WHITE(255, 255, 255);


//模板函数
//计算两点之间的欧式距离
template<typename T>
float caluDistance(const Point_<T> &pt1, const Point_<T> &pt2)    
{
    return std::sqrt(std::pow(pt1.x - pt2.x, 2) + std::pow(pt1.y - pt2.y, 2)); 
}

//获取直线交点
template<typename ValType>
const cv::Point2f crossPointOf(const std::array<cv::Point_<ValType>, 2>& line1, const std::array<cv::Point_<ValType>, 2>& line2)
{
	ValType a1 = line1[0].y - line1[1].y;
	ValType b1 = line1[1].x - line1[0].x;
	ValType c1 = line1[0].x*line1[1].y - line1[1].x*line1[0].y;

	ValType a2 = line2[0].y - line2[1].y;
	ValType b2 = line2[1].x - line2[0].x;
	ValType c2 = line2[0].x*line2[1].y - line2[1].x*line2[0].y;

	ValType d = a1 * b2 - a2 * b1;

	if(d == 0.0)
	{
		return cv::Point2f(FLT_MAX, FLT_MAX);
	}
	else
	{
		return cv::Point2f(float(b1*c2 - b2 * c1) / d, float(c1*a2 - c2 * a1) / d);
	}
}

//Rect缩放
template<typename T1, typename T2>
const cv::Rect_<T1> scaleRect(const cv::Rect_<T1>& rect, const cv::Vec<T2,2> scale, cv::Point_<T1> anchor = cv::Point_<T1>(-1, -1))
{
	if(anchor == cv::Point_<T1>(-1, -1))
	{
		anchor = cv::Point_<T1>(rect.x + rect.width / 2, rect.y + rect.height / 2);
	}
	cv::Point_<T1> tl((cv::Vec<T1, 2>(rect.tl()) - cv::Vec<T1, 2>(anchor)).mul(scale) + cv::Vec<T1, 2>(anchor));
	cv::Size_<T1> size(rect.width*scale[0], rect.height*scale[1]);
	return cv::Rect_<T1>(tl, size);
}


/*
*调试显示函数
*/

//旋转矩阵绘制
void drawRotateRect(Mat &img, const RotatedRect &rec, const Scalar &color)
{
	Point2f vtx[4];
	rec.points(vtx);
	for (int j = 0; j < 4; j++)
	{
		line(img, vtx[j], vtx[(j+1) % 4], color);
	}
}

//在三通道图上绘制单个矩形
template <typename Rectype>
void showRectangle(const std::string &windowName,
				   const Mat &srcImg, 
				   Mat &dstImg,
				   const Rectype &rec,
				   const Scalar color,
				   const int waitTime = -1,
				   const Point offset = Point(0,0)
				   )
{
	//单通道转成三通道
	if (srcImg.channels() == 1 )	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	else srcImg.copyTo(dstImg);

	//将rec平移(用于坐标系的转换)
	drawRotateRect(dstImg, rec + offset, color);

	imshow(windowName, dstImg);
	
	//等待
	if (waitTime >= 0)
	{
		waitKey(waitTime);
	}

}

//在三通道图上绘制多个矩形
template <typename Recs>
void showRectangles(const std::string &windowName,
				   const Mat &srcImg, 
				   Mat &dstImg,
				   const Recs &recs,
				   const Scalar color,
				   const int waitTime = -1,
				   const Point offset = Point(0,0)
				   )
{
	//单通道转成三通道
	if (srcImg.channels() == 1 )	cvtColor(srcImg, dstImg, CV_GRAY2BGR);
	else srcImg.copyTo(dstImg);

	for (const auto &rec : recs)
	{
		drawRotateRect(dstImg, rec + offset, color);
	}

	imshow(windowName, dstImg);
	//等待
	if (waitTime >= 0)
	{
		waitKey(waitTime);
	}

}


//显示所有轮廓
template<typename ContoursType>
void showContours(const std::string& windowName, 
				  const cv::Mat& srcImg, 
				  cv::Mat&dstImg, 
				  const ContoursType& contours,  
				  const cv::Scalar& color, 
				  const int waitTime = -1, 
				  const cv::Point offset = cv::Point(0, 0))
{
	if(srcImg.channels() == 1)	cvtColor(srcImg, dstImg, cv::COLOR_GRAY2BGR);
	else if(srcImg.data != dstImg.data) srcImg.copyTo(dstImg);

	//-1绘制所有轮廓,
	cv::drawContours(dstImg, contours, -1, color, 1, 8, cv::noArray(), INT_MAX, offset);

	cv::imshow(windowName, dstImg);
	if(waitTime >= 0)
	{
		cv::waitKey(waitTime);
	}
}


}

}


#endif 
