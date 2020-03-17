#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

// 颜色减淡(增亮)进行图层混合
void colorDodge(Mat src, Mat dst)
{
    CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 );
    CV_Assert(dst.type() == CV_8UC1 || dst.type() == CV_8UC3 );

    int total = src.rows * src.cols * src.channels();
    uchar *srcPtr = src.data;
    uchar *dstPtr = dst.data;
    
    for (int i = 0; i < total; i++){
        //dst>0,src会有一定程度的增亮
        //dst=255时，src最亮 
        //dst灰度值为0，不影响src
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
    Mat img2(img.rows, img.cols, CV_8UC3, Scalar(255, 255, 255));
    Mat dodge1 = img.clone();
    Mat dodge2 = img.clone();
    
    colorDodge(dodge1, img);
    colorDodge(dodge2, img2);

    imshow("img", img);
    imshow("dodge1", dodge1);
    imshow("dodge2", dodge2);
    waitKey(0);
}