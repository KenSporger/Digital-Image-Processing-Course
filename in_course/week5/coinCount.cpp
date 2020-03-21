#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 硬币区域轮廓标记，计数

int main()
{
    Mat img = imread("img/coin.png", IMREAD_GRAYSCALE);
    Mat dst, labels, stats, centers;
    threshold(img, dst, 0, 255, THRESH_OTSU);

    // 二值图连通域标记
    // labels：大小同src，灰度值代表标号，0为背景
    // stats： 每一行都是一个连通域的特征参数:x,y,width,height,area;利用x,y,width,height可以绘制包围矩形框
    // centers：中心坐标
    // stats和centers的第一行都是背景连通域
    // cnt 连通域个数（包含背景
    int cnt = connectedComponentsWithStats(dst, labels, stats, centers);

    Rect box;
    for (int i = 1; i < stats.rows; i++)
    {
        box.x = stats.at<int>(i, 0);
        box.y = stats.at<int>(i, 1);
        box.width = stats.at<int>(i, 2);
        box.height = stats.at<int>(i, 3);

        rectangle(img, box, CV_RGB(255, 255, 255));

    }

    cout << cnt -1;
    imshow("img", img);
    waitKey(0);
}