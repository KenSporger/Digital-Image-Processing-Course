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
    Mat edge1, edge2, edge;

    while(key != 'q'){
        
        cap.read(frame);
        //sobel检测边缘
        Sobel(frame, edge1, CV_64FC1, 1, 0);
        Sobel(frame, edge2, CV_64FC1, 0, 1);
        // 取绝对值并转256图
        convertScaleAbs(edge1, edge1);
        convertScaleAbs(edge2, edge2);
        addWeighted(edge1, 0.5, edge2, 0.5, 0, edge);

        imshow("frame", frame);
        imshow("edge1", edge1);
        imshow("edge2", edge2);
        imshow("edge", edge);
        
        key = waitKey(30);
    }
}