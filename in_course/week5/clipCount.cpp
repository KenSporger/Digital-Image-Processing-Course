#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("img/clip.png", IMREAD_GRAYSCALE);
    Mat dst, labels, stats, centers;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    threshold(img, dst, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);

    morphologyEx(dst, dst, MORPH_OPEN, kernel);
    connectedComponentsWithStats(dst, labels, stats, centers);

    // 左边有干扰区域，回形针面积固定，可以通过面积限制

    float avgArea = 0;
    int area, cnt = 0;
    // 计算连通域平均面积
    for (int i = 1; i < stats.rows; i++)
    {
        area = stats.at<int>(i, 4);
        avgArea += area;
    } 
    avgArea /= stats.rows - 1; 
    // 大于平均面积视为回形针
    for (int i = 1; i < stats.rows; i++)
    {
        area = stats.at<int>(i, 4);
        if (area > avgArea)
        {
            cnt++;
        }
    } 


    cout << cnt;
    imshow("img", dst);
    waitKey(0);
}