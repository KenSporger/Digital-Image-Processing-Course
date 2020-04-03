#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//放射变换有6个未知数，通过指定三点的映射确定变换矩阵
int main()
{
    Mat img = imread("img/lena.jpg");
    Mat dst;
    
    //原图像三点
    const Point2f srcPt[] = {
        Point2f(200, 200),
        Point2f(250, 200),
        Point2f(200, 100),
    };
    //变换后对应的三点
    const Point2f dstPt[] = {
        Point2f(300, 100),
        Point2f(300, 50),
        Point2f(200, 100),
    };

    //自动计算变换矩阵
    const Mat affineM = getAffineTransform(srcPt, dstPt);
    //仿射变换
    warpAffine(img, dst, affineM, img.size());

    imshow("img", img);
    imshow("dst", dst);

    waitKey(0);

}