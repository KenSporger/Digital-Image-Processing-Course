#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Harris角点检测
int main()
{
    Mat img = imread("img/metal.png", IMREAD_GRAYSCALE);
    Mat cornerStrength, corners;

    cornerHarris(img, cornerStrength, 3, 3, 0.01);

    //注意cornerStrength值较小
    threshold(cornerStrength, corners, 0.0001, 255, THRESH_BINARY);

    imshow("img", img);
    imshow("corner", corners);
    waitKey(0);
}