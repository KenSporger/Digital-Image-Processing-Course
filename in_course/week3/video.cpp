#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    cv::VideoCapture cap;
    
    // 打开摄像头
    // cap.open(0);
    // 打开视频文件
    cap.open("video/car.mp4");
    // 查看摄像头状态
    if (!cap.isOpened()){
        std::cout << "can't open your camera!" << std::endl;
        return -1;
    }

    uchar key=0;
    // 按q结束
    while(key != 'q'){
        cv::Mat frame;
        // 每次循环读取一帧
        bool rdSuccess = cap.read(frame);
        // 读取状态
        if (!rdSuccess){
            std::cout << "read over !"<< std::endl;
            return -1;
        }else{
            imshow("frame", frame);
        }
        // 延时 30ms 
        key = waitKey(30);
    }
}