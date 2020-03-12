#include<iostream>
#include<string.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat paintHistgram(float *hist)
{
    cv::Mat dst(500, 865, CV_8UC3, Scalar(255, 255, 255));
    
    // 找到直方图中最大值
    float maxVal=0;
    for (int i = 0; i < 256; i++){
        maxVal = (hist[i] > maxVal) ? hist[i] : maxVal;
    }
    // 按最大值将比例缩放到合适大小
    float maxHeight = 400 / maxVal;

    // 绘制
    for (int i = 0; i< 256; i++){
        float X = 50 + i*3;
        line(dst, Point(X, 450), Point(X, 450 - hist[i] * maxHeight), CV_RGB(150, 150, 150), 1);
    }

    // 边框
    rectangle(dst, Point(50,50), Point(255*3 + 50,450), CV_RGB(100, 100, 100), 2);

    return dst;
}

void calcuGrayHistgram(Mat src, float *hist)
{
    CV_Assert(src.type() == CV_8UC1);

    int total = src.total();
    uchar *srcPtr = src.data;

    memset(hist, 0, 256 * sizeof(float));
    for (int i = 0; i < total; i++){
        hist[srcPtr[i]]++;
    }

    // 归一化
    for (int i = 0; i < 256; i++){
        hist[i] /= total;
    }
} 

int main()
{
    Mat img = imread("img/numbers.jpg", IMREAD_GRAYSCALE);
    float hist[256];
    
    calcuGrayHistgram(img, hist);
    Mat dst = paintHistgram(hist);

    imshow("hist", dst);
    waitKey(0);


}