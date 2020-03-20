#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main()
{
    Mat img = imread("img/coin.png", IMREAD_GRAYSCALE);
    Mat erodeImg, dilateImg, openImg, closeImg;  
    Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
    
    threshold(img, img, 0, 255, THRESH_OTSU);

    erode(img, erodeImg, kernel);
    dilate(img, dilateImg, kernel);
    morphologyEx(img, openImg, MORPH_OPEN, kernel);
    morphologyEx(img, closeImg, MORPH_CLOSE, kernel);

    imshow("img", img);
    waitKey(0);
    imshow("img", erodeImg);
    waitKey(0);
    imshow("img", dilateImg);
    waitKey(0);
    imshow("img", openImg);
    waitKey(0);
    imshow("img", closeImg);
    waitKey(0);

}
