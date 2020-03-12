#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

void pixelsAverage(cv::Mat &src, uchar thres)
{
    int rows = src.rows;
    int cols = src.cols;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){

            uchar avg = (src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2]) / 3;

            if (avg > thres) avg = 255;
            else avg = 0;

            src.at<Vec3b>(i, j)[0] = avg;
            src.at<Vec3b>(i, j)[1] = avg;
            src.at<Vec3b>(i, j)[2] = avg;

        }
    }
} 


int main()
{
    cv::Mat srcMat = imread("img/numbers.jpg");
    cv::Mat deepMat, shallowMat;
    // copyto是深复制
    srcMat.copyTo(deepMat);
    //浅复制
    shallowMat = srcMat;

    pixelsAverage(srcMat, 100);

    imshow("deepMat", deepMat);
    imshow("shallowMat", shallowMat);

    waitKey(0); 
}