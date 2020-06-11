#ifndef _DFT_H
#define _DFT_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include "roi.h"

void dftConvert(const cv::Mat &src, cv::Mat &mag_mat, cv::Mat &ph_mat);
void dftConvert(const cv::Mat &src, cv::Mat &mag_mat);
void diagonExchange(cv::Mat src, cv::Mat &dst);
void dftNormalize(const cv::Mat &mag_mat, cv::Mat &normalized_mat);
void idftConvert(const cv::Mat &mag_mat, const cv::Mat &ph_mat, int dst_rows, int dst_cols, cv::Mat &dst);
void dftInteractiveFilter(const cv::Mat &src, cv::Mat &mag_mat, cv::Mat &ph_mat, cv::Mat &dst);


#endif 