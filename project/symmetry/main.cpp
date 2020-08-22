#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#define PI 3.141596

Mat img;


void findSymmetry0(vector<Point> contour, double &k, double &b)
{
    double mean_x=0, mean_y=0;
    double N=0, M=0;
    
    for (auto p:contour)
    {
        mean_x += p.x;
        mean_y += p.y;
    }
    mean_x /= contour.size();
    mean_y /= contour.size();
    
    double temp;
    for (auto p:contour)
    {
        temp = (p.x - mean_x) * (p.y - mean_y);
        M += temp;
        N += (p.x - mean_x) * (p.x - mean_x) - (p.y - mean_y) * (p.y - mean_y);
    }
    
    temp = sqrt(4*M*M + N*N);

    double k1 = (-N+temp) / (2*M);
    double k2 = (-N-temp) / (2*M);
    double b1 = mean_y - k1 * mean_x;
    double b2 = mean_y - k2 * mean_x;
    cout <<"k1: " << k1 << ", k2: " << k2 << endl;
    cout <<"b1: " << b1 << ", b2: " << b2 << endl;

    //debug
    double x1 = -10;
    double y1 = k1 * x1 + b1;
    double y2 = k2 * x1 + b2;
    double x2 = img.cols + 10;
    double y3 = k1 * x2 + b1;
    double y4 = k2 * x2 + b2;

    line(img, Point2f(x1, y1), Point2f(x2, y3), Scalar(0, 0, 255), 2);
    line(img, Point2f(x1, y2), Point2f(x2, y4), Scalar(0, 0, 255), 2);

}


void findSymmetry(vector<Point> contour, double &rho, double &theta)
{
    double mean_x=0, mean_y=0;
    double N=0, M=0;
    
    for (auto p:contour)
    {
        mean_x += p.x;
        mean_y += p.y;
    }
    mean_x /= contour.size();
    mean_y /= contour.size();
    
    double temp1, temp2;
    for (auto p:contour)
    {
        temp1 = (p.x - mean_x);
        temp2 = (p.y - mean_y);
        M += temp1 * temp2;
        N += temp1 * temp1 - temp2 * temp2;
    }
    
    // ignore N=0;
    double theta1, rho1;
    double theta2, rho2;
    theta1 = atan(2*M/N) / 2;
    rho1 = cos(theta1) * mean_x + sin(theta1) * mean_y;
    theta2 = theta1 > 0? theta1 - PI/2: theta1 + PI/2;  
    rho2 = cos(theta2) * mean_x + sin(theta2) * mean_y;

    {
        double a = cos(theta1);
        double b = sin(theta1);
        double x0 = rho1 * a;
        double y0 = rho1 * b;
        //找出直线上两点       
        Point pt1, pt2; 
        pt1.x = saturate_cast<int>(x0 - 1000 * b);
        pt1.y = saturate_cast<int>(y0 +  1000 * a);
        pt2.x = saturate_cast<int>(x0 + 1000 * b);
        pt2.y = saturate_cast<int>(y0 - 1000 * a);

        line(img, pt1, pt2, Scalar(0, 255, 0), 2);
    }

    {
        double a = cos(theta2);
        double b = sin(theta2);
        double x0 = rho2 * a;
        double y0 = rho2 * b;
        //找出直线上两点       
        Point pt1, pt2; 
        pt1.x = saturate_cast<int>(x0 - 1000 * b);
        pt1.y = saturate_cast<int>(y0 +  1000 * a);
        pt2.x = saturate_cast<int>(x0 + 1000 * b);
        pt2.y = saturate_cast<int>(y0 - 1000 * a);

        line(img, pt1, pt2, Scalar(0, 255, 0), 2);
    }

}





int main()
{
    VideoCapture video("./video/buff.mp4"); 
    while(1)
    {
        video >> img;

        Mat bin, hsv;
        cvtColor(img, hsv, cv::COLOR_BGR2HSV);
        cv::Mat temp, tempBinary;
        inRange(hsv, Scalar(0, 60, 80), Scalar(25, 255, 255), temp);
        inRange(hsv, Scalar(156, 60, 80), Scalar(181, 255, 255), tempBinary);
        bin = temp | tempBinary;
        dilate(bin, bin, getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9)));

        vector<vector<Point>> contours;
        double rho, theta;
        findContours(bin, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        for (int i =0; i < contours.size(); i++)
        {
            if (contourArea(contours[i]) > 4000)
            {
                drawContours(img, contours, i, Scalar(255, 0, 0), 2);
                findSymmetry(contours[i], rho, theta);
                findSymmetry0(contours[i], rho, theta);
            }
        }
        imshow("img", img);
        waitKey(0);
    }

}