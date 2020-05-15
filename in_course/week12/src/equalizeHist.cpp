#include "equalizeHist.h"


void equalizeHistBGR(Mat src, Mat &dst)
{
    CV_Assert(src.channels() == 3);
    Mat hsv, h, s, v;
    vector<cv::Mat> channels;

    //先转为HSV通道
    cvtColor(src, hsv, COLOR_BGR2HSV);
    split(hsv, channels);
    // h = channels.at(0);
    // s = channels.at(1);
    // v = channels.at(2);

    equalizeHist(channels[2], channels[2]);

    merge(channels, hsv);

    cvtColor(hsv, dst, COLOR_HSV2BGR);

}