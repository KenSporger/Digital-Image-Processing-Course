#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//非剪裁旋转，扩大显示范围
int main()
{
    Mat img = imread("img/lena.jpg");
    Mat dst;
    //旋转角度、缩放比例
    float angle = 20, scale = 1.0;
    //变换中心
    Point2f center(img.cols / 2, img.rows / 2);
    
    //获取变换矩阵
    Mat affineM = getRotationMatrix2D(center, angle, scale);

    //获取旋转后的外接四边形
    Rect box = RotatedRect(center, img.size(), angle).boundingRect();

    //显示范围放大会改变中心位置
    affineM.at<double>(0, 2) += box.width / 2.0 - center.x;
    affineM.at<double>(1, 2) += box.height / 2.0 - center.y ;

    //仿射变换,扩大显示范围
    warpAffine(img, dst, affineM, box.size());

    imshow("img", img);
    imshow("dst", dst);

    waitKey(0);

}