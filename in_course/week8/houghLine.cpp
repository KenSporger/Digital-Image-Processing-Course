#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//霍夫直线变换
int main()
{
    Mat img = imread("img/metal.png");
    Mat gray, edge, lines;
    
    cvtColor(img, gray, CV_BGR2GRAY);
    //Canny获取边缘
    Canny(gray, edge, 100, 200);

    //霍夫直线变换
    HoughLines(edge, lines, 1, CV_PI / 180, 100);

    int rho, theta;
    Point pt1, pt2;
    //y根线：1*y*Vec2f(r,theta)
    for (int i = 0; i < lines.rows; i++)
    {
        for (int j = 0; j < lines.cols; j++)
        {
            //(r,theta)为过原点与直线垂直直线之间的交点
            float rho = lines.at<Vec2f>(i, j)[0];
            float theta = lines.at<Vec2f>(i, j)[1];
            double a = cos(theta);
            double b = sin(theta);
            double x0 = rho * a;
            double y0 = rho * b;
            //找出直线上两点        
            pt1.x = saturate_cast<int>(x0 - 1000 * b);
            pt1.y = saturate_cast<int>(y0 +  1000 * a);
            pt2.x = saturate_cast<int>(x0 + 1000 * b);
            pt2.y = saturate_cast<int>(y0 - 1000 * a);

            line(img, pt1, pt2, CV_RGB(255, 255, 0));
        }
    }

    imshow("img", img);

    waitKey(0);

}