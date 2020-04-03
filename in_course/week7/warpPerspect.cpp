#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//投影变换有8个未知数，通过指定四点的映射确定变换矩阵
int main()
{
    Mat img = imread("img/lena.jpg");
    Mat dst;
    
    //原图像四点
    const Point2f srcPt[] = {
        Point2f(150, 150),
        Point2f(150, 300),
        Point2f(350, 300),
        Point2f(350, 150)
    };
    //变换后对应的四点
    const Point2f dstPt[] = {
        Point2f(200, 150),
        Point2f(200, 300),
        Point2f(340, 270),
        Point2f(340, 180)
    };

    //自动计算透视矩阵
    const Mat affineM = getPerspectiveTransform(srcPt, dstPt);
    //透视变换
    warpPerspective(img, dst, affineM, img.size());

    imshow("img", img);
    imshow("dst", dst);

    waitKey(0);

}