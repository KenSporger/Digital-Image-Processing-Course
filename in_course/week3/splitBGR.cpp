#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    cv::Mat img = imread("img/solarPanel.png");
    std::vector<cv::Mat> channels;
    
    //用vector作为输出容器
    split(img, channels);

    // at访问或者使用[0]
    cv::Mat B = channels.at(0);
    cv::Mat G = channels.at(1);
    cv::Mat R = channels.at(2);

    imshow("img", img);
    imshow("B", B);
    imshow("G", G);
    imshow("R", R);
    waitKey(0);

}