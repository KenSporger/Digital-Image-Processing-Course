#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("img/buff.png");
    Mat bin, gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    threshold(gray, bin, 100, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    //每个轮廓包含四个元素值：下一个同级轮廓、上一个同级轮廓、内嵌轮廓，父轮廓
	Mat hierarchy;
    //https://blog.csdn.net/qq_43667130/article/details/105276577
    findContours(bin, contours, hierarchy,RETR_TREE, CHAIN_APPROX_NONE);
    size_t contours_size = contours.size();
    cout << "轮廓总数: " << contours_size << endl;

}