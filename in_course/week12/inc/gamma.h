#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void gammaCorrect(const Mat src, Mat &dst, float gamma);