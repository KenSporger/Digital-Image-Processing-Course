#include<iostream>
#include<string.h>
#include<time.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 直方图计算
float calcuGrayHistgram(Mat src, float *hist)
{
    CV_Assert(src.type() == CV_8UC1);

    int total = src.total();
    float mean = 0;
    uchar *srcPtr = src.data;

    memset(hist, 0, 256 * sizeof(float));
    for (int i = 0; i < total; i++){
        hist[srcPtr[i]]++;
    }

    // normalize
    for (int i = 0; i < 256; i++){
        hist[i] /= total;
        mean += hist[i] * i;
    }

    return mean;
} 

void Ostu(Mat src)
{
    CV_Assert(src.type() == CV_8UC1);
    // 归一化的直方图
    float hist[256]; 
    // 图像总体均值
    float meanImgGray = 0;
    // 背景均值(<thres)
    float meanBgGray = 0;
    float k = 0;
    // 背景比例
    float propBgPixel = 0;
    // 类间方差
    float maxStd = 0;
    float std=0;
    float thres = 0;
    // 获取直方图和图像均值
    meanImgGray = calcuGrayHistgram(src, hist);

    for (int i = 0; i < 256; i++){
        // 累积计算背景比例
        propBgPixel += hist[i];
        // k = 背景的总灰度值 / 图像像素总数
        k += hist[i] * i;
        // 背景均值 = k / (背景像素数 / 图像像素总数) = 背景的总灰度值 / 背景像素数
        meanBgGray = k / propBgPixel;
        // 公式优化：g = w1*w2*(u1-u2)^2 = w1 /(1-w1)*(u1-u0)^2
        std = propBgPixel / (1 - propBgPixel) * (meanBgGray - meanImgGray) * (meanBgGray - meanImgGray);
        //找到最大类间方差对应的灰度
        if (std > maxStd){
            maxStd = std;
            thres = i;
        }
    }

    // 二值化
    for (int i = 0; i < src.rows; i++){
        for (int j = 0; j < src.cols; j++){
           if (src.at<uchar>(i, j) <= thres){
               src.at<uchar>(i, j) = 0;
           }else{
               src.at<uchar>(i, j) = 255;
           }
        }
    }

}

int main()
{
    Mat img = imread("img/coin.jpg", IMREAD_GRAYSCALE);

    Ostu(img);
    
    imshow("img", img);
    waitKey(0);


}