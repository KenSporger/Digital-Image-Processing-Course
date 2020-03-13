#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

String windowName = "dst";

void binarize(int size, void *data)
{
    if (size % 2 == 0 || size < 3) return;
    Mat src = *(Mat *)data;
    Mat dst; 
    adaptiveThreshold(src, dst, 255, THRESH_BINARY, ADAPTIVE_THRESH_MEAN_C, size, 10);

    imshow(windowName, dst);
}


int main()
{
    Mat img = imread("img/paper.jpg", IMREAD_GRAYSCALE);

    int minSize = 3;
    int maxSize = 29;
    int minC = 0;
    int maxC = 255;

    imshow(windowName, img);
    createTrackbar("Ksize", windowName, &minSize, maxSize, binarize, &img);
    waitKey(0);

}
