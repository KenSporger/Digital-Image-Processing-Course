#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap;

    int minH = 0;
    int maxH = 20;
    int minS = 80;
    int maxS = 255;
    int minV = 80;
    int maxV = 255;

    cap.open(0);

    if (!cap.isOpened())
    {
        return -1;
    }

    uchar key=0;
    Mat frame, hsv, mask;
    Mat blur, dst;

    while(key != 'q'){
        
        cap.read(frame);

        cvtColor(frame, hsv, CV_BGR2HSV);
        // 提取肤色掩码图像
        inRange(hsv, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), mask);

        //中值滤波
        medianBlur(frame, blur, 7);

        frame.copyTo(dst);
        //掩码，仅复制皮肤部分
        blur.copyTo(dst, mask);
      

        imshow("frame", frame);
        imshow("mask", mask);
        imshow("buff", dst);
        
        key = waitKey(30);
    }
}