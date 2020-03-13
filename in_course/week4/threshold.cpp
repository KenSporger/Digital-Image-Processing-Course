#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("img/paper.jpg", IMREAD_GRAYSCALE);
    Mat dstGlobal, dstLocal;
    
    threshold(img, dstGlobal, 0, 255, THRESH_OTSU);
    adaptiveThreshold(img, dstLocal, 255, THRESH_BINARY, ADAPTIVE_THRESH_MEAN_C, 7, 10);

    imshow("local", dstLocal);
    imshow("global", dstGlobal);
    waitKey(0);


}
