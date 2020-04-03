#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//非剪裁旋转
int main()
{
    Mat img = imread("img/lena.jpg");
    Mat dst;
    //旋转角度、缩放比例
    float angle = 20;
    //旋转后的窗口大小变为：
    // nH = W*sin(a)+H*cos(a)
    // nW = W*cos(a)+H*sin(a)
    float scale = 1/( abs( sin( 6.283 *angle / 360 ) ) + abs( cos( 6.283 *angle / 360 ) ) );

    //变换中心
    Point2f center(img.cols / 2, img.rows / 2);
    
    //获取变换矩阵
    Mat affineM = getRotationMatrix2D(center, angle, scale);



    //仿射变换
    warpAffine(img, dst, affineM, img.size());

    imshow("img", img);
    imshow("dst", dst);

    waitKey(0);

}