#ifndef _ROI_H
#define _ROI_H
#include<iostream>
#include<opencv2/opencv.hpp>

void selectPolygon(const cv::Mat &srcMat, cv::Mat &dstMat, bool roi_select = true);

#endif


