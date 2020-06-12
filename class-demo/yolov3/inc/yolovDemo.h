#pragma once
#include "parameterDemo.h"
#include <opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>

using namespace cv;
using namespace std;
using namespace dnn;


//通过非极大值抑制去掉置信度较低的bouding box
void postprocess(cv::Mat& frame, std::vector<cv::Mat>& outs);

// 获得输出名字
std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);


//绘制检测结果
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

int yoloV3();

int openpose();
int openFace();

