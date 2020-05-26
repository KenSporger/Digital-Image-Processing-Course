#ifndef _BGSUB_H
#define _BGSUB_H

#include<iostream>
#include<opencv2/opencv.hpp>


void calcuGaussianBgModel(const std::vector<cv::Mat> &frames, cv::Mat &mean_mat, cv::Mat &var_mat);
void gaussianBgModelDiff(const cv::Mat &mean_mat, const cv::Mat &var_mat, const cv::Mat &frame, cv::Mat &sub_mat, float weight = 2.0);


#endif 
