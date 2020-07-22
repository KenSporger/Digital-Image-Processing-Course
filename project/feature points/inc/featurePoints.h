#ifndef _FEATUREPOINTS_H
#define _FEATUREPOINTS_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include<algorithm>

void cornerHarrisDetect(const cv::Mat &img);
void cornerHarrisDetectDilate(const cv::Mat &img);
void herrisDemo();
void siftDetect();

#endif