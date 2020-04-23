#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Canny检测
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
        Sobel(frame, edgeX, CV_16SC1, 1, 0);
        //检测水平边缘
        Sobel(frame, edgeY, CV_16SC1, 0, 1);
        
        //重载函数
        //直接对原图求Canny,双阈值
        // Canny(frame, edge, 100, 200); 
        //对索贝尔梯度结果求Canny
        Canny(edgeX, edgeY, edge,100, 200);

        imshow("frame", frame);
        imshow("edge", edge);
        
        key = waitKey(30);
    }
}