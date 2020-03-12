#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    cv::Mat dst(300, 300, CV_8UC3, Scalar(255, 255, 255));
    cv::Point pt;

    pt.x = 100;
    pt.y = 100;

    // center:(100,100) radius= 50, border thickness=1
    circle(dst, pt, 50, CV_RGB(255, 0, 0), 1, 8, 0);
    imshow("dst", dst);
    waitKey(0);

    // 点+半径 -1实心
    circle(dst, pt, 50, CV_RGB(255, 0, 0), -1, 8, 0);
    imshow("dst", dst);
    waitKey(0);

    // paint line
    // Point(x, y)
    line(dst, Point(50, 100), Point(50, 50), CV_RGB(0, 255, 0), 2);
    imshow("dst", dst);
    waitKey(0);

    // 两点一矩形
    rectangle(dst, Point(50, 50), Point(100, 100), CV_RGB(0, 0, 255), -1);
    imshow("dst", dst);
    waitKey(0);

    // 中心+长短轴+旋转角=椭圆
    // center Size(long axis, short axis, rotate, 0, 360)
    ellipse(dst, Point(150, 150), Size(100, 50), 45, 0, 360, CV_RGB(0, 0, 0), 1);
    ellipse(dst, Point(150, 150), Size(100, 50), -45, 0, 360, CV_RGB(0, 0, 0), 1);
    imshow("dst", dst);
    waitKey(0);

}