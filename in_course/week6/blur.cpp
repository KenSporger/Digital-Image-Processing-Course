#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap;

    cap.open(0);

    if (!cap.isOpened())
    {
        return -1;
    }

    uchar key=0;
    Mat frame;
    Mat median, mean, gaus;

    while(key != 'q'){
        
        cap.read(frame);
        //中值滤波
        medianBlur(frame, median, 7);
        //均值滤波
        blur(frame, mean, Size(7, 7));
        //高斯滤波
        GaussianBlur(frame, gaus, Size(7, 7), 5.0);

        imshow("frame", frame);
        imshow("median", median);
        imshow("mean", mean);
        imshow("gauss", gaus);
        
        key = waitKey(30);
    }
}