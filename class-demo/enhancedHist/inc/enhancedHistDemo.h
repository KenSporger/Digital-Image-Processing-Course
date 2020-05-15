#pragma once
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


//gamma矫正演示函数
int gammaMain();
//直方图均衡化演示函数
int equalizeMain();



//gamma矫正功能函数
int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma);


//直方图功能函数
int drawSimpleHist(cv::Mat & histMat, float * srcHist);
int calcSimpleHist(const cv::Mat src, float * dstHist);
int calcSimpleHistWithMask(const cv::Mat src, const cv::Mat mask, float * dstHist);