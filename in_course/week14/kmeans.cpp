#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("img/movie.jpg");
    int cols = img.cols;
    int rows = img.rows;

    int total = cols * rows;

    //shape :(1, total, 3)
    Mat sample = img.reshape(3, total);
    Mat km_data;
    //必须为浮点型
    sample.convertTo(km_data, CV_32F);

    //终止条件
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
    int K = 2;
    Mat labels;
    Mat centers;
    int attempts = 2;
    kmeans(km_data, K, labels, criteria, attempts, KMEANS_PP_CENTERS, centers);

    Mat mask(rows, cols, CV_8UC1, Scalar::all(0));

    uchar *p = mask.ptr<uchar>();
    uchar fg[2] = { 0,255 };
    for (int i = 0; i < total; i++)
    {
        p[i] = fg[labels.at<int>(i)];
    }

    imshow("img", img);
    imshow("mask", mask);
    waitKey(0);



}
