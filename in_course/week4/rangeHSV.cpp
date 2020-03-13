#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    VideoCapture video;
    Mat frame, hsv, dst;
    video.open(0);

    int minH = 0;
    int maxH = 20;
    int minS = 43;
    int maxS = 255;
    int minV = 55;
    int maxV = 255;

    char key = 0;
    while(key != 'q')
    {
        if (video.read(frame))
        {
            cvtColor(frame, hsv, CV_BGR2HSV);
            
            
            // 提取肤色，输出dst掩码图像
            inRange(hsv, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), dst);

            imshow("frame", frame);
            imshow("dst", dst);
            key = waitKey(30);
        }
    }
}