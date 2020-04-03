#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 边界查点
int getPoint(Mat src, uchar *p, int offset, int len)
{
    uchar *cur = p;
    uchar *nxt = p + offset;
    for (int i = 0; i < len - 1; i++)
    {
        if (*cur == 0 && *nxt == 255){
            return i+1;
        }
        cur = nxt;
        nxt += offset;

    }

    return 0;

}

//放射变换有6个未知数，通过指定三点的映射确定变换矩阵
int main()
{
    Mat img = imread("img/lena_rot.jpg");
    Mat gray, bin, dst;

    cvtColor(img, gray, CV_BGR2GRAY);
    threshold(gray, bin, 230, 255, THRESH_BINARY_INV);


    //原图像三点
    const Point2f srcPt[] = {
        Point2f(img.cols - 1, getPoint(bin, bin.ptr<uchar>(0, bin.cols - 1), bin.cols, bin.rows)),
        Point2f(getPoint(bin, bin.ptr<uchar>(img.rows -1 ), 1, bin.cols), img.cols - 1),
        Point2f(0, getPoint(bin, bin.ptr<uchar>(0), bin.cols, bin.rows)),
    };

    //变换后对应的三点
    const Point2f dstPt[] = {
        Point2f(img.cols - 1, 0),
        Point2f(img.cols - 1, img.rows - 1),
        Point2f(0, img.rows - 1),
    };

    //自动计算变换矩阵
    const Mat affineM = getAffineTransform(srcPt, dstPt);
    //仿射变换
    warpAffine(img, dst, affineM, img.size());

    imshow("img", img);
    imshow("bin", bin);
    imshow("dst", dst);

    waitKey(0);

}