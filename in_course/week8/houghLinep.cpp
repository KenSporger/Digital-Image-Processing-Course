#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//概率霍夫直线变换
int main()
{
    Mat img = imread("img/metal.png");
    Mat gray, edge, lines;
    
    cvtColor(img, gray, CV_BGR2GRAY);
    //Canny获取边缘
    Canny(gray, edge, 100, 200);

    //概率霍夫直线变换，投票，最小直线长度，最大点间隔
    HoughLinesP(edge, lines, 1, CV_PI / 180, 20, 100.0, 10.0);

    //直线数量
    cout << lines.total();
    Point pt1, pt2;
    //y根线：1*y*Vec2f(r,theta)
    for (int i = 0; i < lines.rows; i++)
    {
        for (int j = 0; j < lines.cols; j++)
        {
            //直线上两点
            pt1.x = lines.at<Vec4i>(i, j)[0];
            pt1.y = lines.at<Vec4i>(i, j)[1];
            pt2.x = lines.at<Vec4i>(i, j)[2];
            pt2.y = lines.at<Vec4i>(i, j)[3];

            line(img, pt1, pt2, CV_RGB(255, 255, 0));
        }
    }

    imshow("img", img);

    waitKey(0);

}