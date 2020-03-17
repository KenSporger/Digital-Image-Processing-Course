#include<iostream>
#include<string.h>
#include<time.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 单通道图的积分图计算
void getGrayIntegralImage(Mat src, Mat &dst)
{
    CV_Assert(src.type() == CV_8UC1);
    //积分图比图像要多一行一列
    Mat integral(src.rows + 1, src.cols + 1, CV_32S, Scalar(0));
    
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0, sum = 0; j < src.cols; j++)
        {
            //沿着行方向对列进行累加
            sum += src.at<uchar>(i, j);
            //当前积分=同列上一行的积分+本行的列积分
            integral.at<int>(i + 1, j +1) = integral.at<int>(i, j +1) + sum;
        }
    } 

    integral.copyTo(dst);
}

// 动态阈值=窗口均值-常数C
void localBinarize(Mat src, Mat &dst, int Ksize, double C)
{
    CV_Assert(src.type() == CV_8UC1);
    int rows = src.rows;
    int cols = src.cols;
    int radius = Ksize / 2;
    Mat itgl;
    src.copyTo(dst);
    getGrayIntegralImage(dst, itgl);

    for (int i = 0; i < rows; i++)
    {
        int rowL = max(i - radius, 0);
        int rowH = min(i + radius + 1, rows); 
        
        //利用积分图快速计算窗口均值
        for (int j = 0; j < cols; j++)
        {
            int colL = max(j - radius, 0);
            int colH = min(j + radius + 1, cols);
            //窗口有效像素数量
            int pixels = (colH - colL) * (rowH - rowL);
            //窗口积分=右下角-左下角-右上角+左上角
            int mean = itgl.at<int>(rowH, colH) - itgl.at<int>(rowL, colH) 
                            - itgl.at<int>(rowH, colL) + itgl.at<int>(rowL, colL);
            //移位实现过半进位
            mean = (mean + (pixels >> 1)) / pixels - C;
            
            if (dst.at<uchar>(i, j) <= mean)
            {
                dst.at<uchar>(i, j) = 0;
            }
            else
            {
                dst.at<uchar>(i, j) = 255;
            }
        }
    }
}


int main()
{
    VideoCapture video;
    Mat img;
    Mat dst;

    video.open("video/papers.avi");
    int frames = video.get(CAP_PROP_FRAME_COUNT);
    for (int i = 0; i < frames; i++)
    {
        if (video.read(img))
        {
            cvtColor(img, img, CV_BGR2GRAY);
            localBinarize(img, dst, 7, 12);
            imshow("img", img);
            imshow("dst", dst);
            waitKey(0);
        }
    }
}