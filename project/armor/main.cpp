#include<iostream>
#include<opencv2/opencv.hpp>
#include "armorDetector.h"

using namespace std;
using namespace cv;
using namespace rm;

//单装甲检测
void simpleDetect()
{

    const Mat img = imread("../../../img/armor01.png");
    Mat dst;
    img.copyTo(dst);
    Mat lightMask, gray;

    cvtColor(img, gray, CV_BGR2GRAY);
    threshold(gray, lightMask, 240, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(lightMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    
    int amount = contours.size();
    cout << amount << endl;
    vector<Point2f> centers;
    for (int i = 0; i < amount; i++)
    {
        RotatedRect rbox = minAreaRect(contours[i]);
        int width = rbox.size.width; 
        int height = rbox.size.height;
        //宽长比
        float prop = height / width;

        if (prop > 5)
        {
            centers.push_back(rbox.center);
        }

    }
    //计算中点
    Point armorCenter;
    if (centers.size() == 2)
    {
        armorCenter.x = (centers[0].x + centers[1].x) / 2;
        armorCenter.y = (centers[0].y + centers[1].y) / 2;
    }
    
    circle(dst, armorCenter, 4, Scalar(0, 0, 255), 2);
    imshow("img", img);
    imshow("dst", dst);
    waitKey(0);

}

//东南大学2018解决方案
//原理：先二值化，再检测单个灯条，进行特征筛选，然后对灯条进行一一匹配，检测出装甲，对装甲进行评估，
//分值最大者作为攻击目标，并且在后面的几帧中缩小检测图像的范围，以达到快速追踪。
void SEUDetect()
{
    ArmorParam param;
    ArmorDetector Arm(param);
    Arm.setEnemyColor(ARMOR_BLUE);
#if defined(TIME_COUNT)
    Arm.timeInit();
#endif
    VideoCapture video("../../../video/armormini.mp4");
    if (!video.isOpened())
    {
        cout << "can't openc video!" << endl;
    }
    Mat frame;
    uchar key = 0;
    while (key != 'q')
    {        
        video >> frame;
        if (frame.empty())
        {
            cout << "can't read frame!" << endl;
            return;
        }
        
        Arm.loadImg(frame);
        Arm.detect();
#if defined(TIME_COUNT)
    Arm.printEchoTimeSummary();
#endif
#ifdef SHOW_RESULT
        Arm.showDetectedArmor();
#endif 
        key = waitKey(30);
    }
#if defined(TIME_COUNT)
    Arm.printHistroySummary();
#endif
}

int main()
{
    // simpleDetect();
    SEUDetect();
}