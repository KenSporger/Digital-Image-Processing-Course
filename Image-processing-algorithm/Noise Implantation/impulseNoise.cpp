#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

// 椒盐噪声，n决定了噪声的数量
void addImpulseNoise(Mat dst, int n)
{
    CV_Assert(dst.type() == CV_8UC3 || dst.type() == CV_8UC1);

    for (int k = 0; k < n; k++){

        int i = rand() % dst.rows;
        int j = rand() % dst.cols;
        int value = (rand() % 2) * 255;

        if (dst.type() == CV_8UC1){
            dst.at<uchar>(i, j) = value;
        }
        else if (dst.type() == CV_8UC3){
            dst.at<Vec3b>(i, j)[0] = value;
            dst.at<Vec3b>(i, j)[1] = value;
            dst.at<Vec3b>(i, j)[2] = value;
        }
    }

    return;
}

int main()
{
    Mat img = imread("img/numbers.jpg");

    addImpulseNoise(img, 10000);

    imshow("img", img);
    waitKey(0);

}