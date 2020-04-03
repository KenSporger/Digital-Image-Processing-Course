#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("img/lena.jpg");
    Mat dst;
    //旋转角度、缩放比例
    float angle = 45, scale = 0.8;
    //变换中心
    Point2f center(img.cols / 2, img.rows / 2);
    //获取变换矩阵
    const Mat affineM = getRotationMatrix2D(center, angle, scale);
    //仿射变换
    warpAffine(img, dst, affineM, img.size());

    imshow("img", img);
    imshow("dst", dst);

    waitKey(0);

}