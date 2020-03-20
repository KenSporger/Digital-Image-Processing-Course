#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("img/coin.png", IMREAD_GRAYSCALE);
    Mat dst, labels, stats, centers;
    threshold(img, dst, 0, 255, THRESH_OTSU);

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