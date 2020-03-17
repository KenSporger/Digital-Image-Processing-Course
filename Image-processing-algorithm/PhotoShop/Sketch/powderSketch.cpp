#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

//粉末素描滤镜
void powderSketch(Mat src)
{    
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3);

    Mat dst = src.clone();
    int total = dst.rows * dst.cols * dst.channels();
    uchar *dstPtr = dst.data;
    uchar *srcPtr = src.data;
    
    // 颜色反转 
    for (int i = 0; i < total; i++){
        dstPtr[i] = 255 - dstPtr[i];
    }

    // 高斯低通滤波
    GaussianBlur(dst, dst, Size(3,3), 1.0);

    // 颜色减淡
    for (int i = 0; i < total; i++){
        int val = srcPtr[i] + srcPtr[i] * dstPtr[i] / (256.0 - dstPtr[i]);
        if (val > 255){
            val = 255;
        }
        srcPtr[i] = val;
    }
}

int main()
{
    Mat img = imread("img/numbers.jpg");

    powderSketch(img);
    imshow("sketch", img);
    waitKey(0);
}