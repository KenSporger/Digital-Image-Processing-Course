#ifndef _BGSUBDEMO_H
#define _BGSUBDEMO_H

#include<iostream>
#include<opencv2/opencv.hpp>


void on_mouse(int EVENT, int x, int y, int flags, void* userdata);
int drawHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght);
void gaussianModelDiff(const cv::Mat &mean_mat, cv::Mat &var_mat, const cv::Mat &frame, float th, cv::Mat &sub_bin);
void calcuGaussianModle(const std::vector<cv::Mat> &mat_save, cv::Mat &mean_mat, cv::Mat &var_mat);


int verifyGaussian();
int bgSub_demo();
int bgSubGaussian_demo();

#endif 
