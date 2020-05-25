#ifndef _HOUGH_H
#define _HOUGH_H

#include<iostream>
#include<opencv2/opencv.hpp>

void drawHoughLines(const cv::Mat &lines, cv::Mat &dst, cv::Scalar color, int thick);

#endif