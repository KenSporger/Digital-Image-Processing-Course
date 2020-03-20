#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("img/circuit.JPG", IMREAD_GRAYSCALE);
    Mat dst, labels, stats, centers;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(11, 11));
    threshold(img, dst, 100, 255, THRESH_BINARY_INV);

    morphologyEx(dst, dst, MORPH_OPEN, kernel);
    int cnt = connectedComponentsWithStats(dst, labels, stats, centers);


    cout << cnt -1;
    imshow("img", dst);
    waitKey(0);
}