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
    Mat edgeX, edgeY, edge;

    while(key != 'q'){
        
        cap.read(frame);

        cvtColor(frame, frame, CV_BGR2GRAY);
        
        //检测垂直边缘
        Sobel(frame, edgeX, CV_64FC1, 1, 0);
        //检测水平边缘
        Sobel(frame, edgeY, CV_64FC1, 0, 1);

        // 取绝对值并转256图
        convertScaleAbs(edgeX, edgeX);
        convertScaleAbs(edgeY, edgeY);
        // 全部边缘
        addWeighted(edgeX, 0.5, edgeY, 0.5, 0, edge);

        imshow("frame", frame);
        imshow("edge1", edgeX);
        imshow("edge2", edgeY);
        imshow("edge", edge);
        
        key = waitKey(30);
    }
}