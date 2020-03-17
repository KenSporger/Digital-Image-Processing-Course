#include<iostream>
#include<omp.h>
#include<opencv2/opencv.hpp>
using namespace cv;

//移位代替乘除法，使用多线程加速
Mat speed_rgb2gray(Mat src){
    
    Mat dst(src.rows,src.cols,CV_8UC1);
    //use openmp multithreading
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < src.rows; i++){
        for (int j = 0; j < src.cols; j++){
            //等价于 0.299R+0.587G+0.114B,(R*313524 + G*615514 + B*119538) >> 20
            dst.at<uchar>(i,j) = ((
                (src.at<Vec3b>(i,j)[0] << 18) + (src.at<Vec3b>(i,j)[0] << 15) + (src.at<Vec3b>(i,j)[0] << 14) +
                (src.at<Vec3b>(i,j)[0] << 11) + (src.at<Vec3b>(i,j)[0] << 7 ) + (src.at<Vec3b>(i,j)[0] << 7 ) +
                (src.at<Vec3b>(i,j)[0] << 5 ) + (src.at<Vec3b>(i,j)[0] << 4 ) + (src.at<Vec3b>(i,j)[0] << 2 ) +
                
                (src.at<Vec3b>(i,j)[1] << 19) + (src.at<Vec3b>(i,j)[1] << 16) + (src.at<Vec3b>(i,j)[1] << 14) +
                (src.at<Vec3b>(i,j)[1] << 13) + (src.at<Vec3b>(i,j)[1] << 10) + (src.at<Vec3b>(i,j)[1] << 8 ) +
                (src.at<Vec3b>(i,j)[1] << 4 ) + (src.at<Vec3b>(i,j)[1] << 3 ) + (src.at<Vec3b>(i,j)[1] << 1 ) +

                (src.at<Vec3b>(i,j)[2] << 16) + (src.at<Vec3b>(i,j)[2] << 15) + (src.at<Vec3b>(i,j)[2] << 14) +
                (src.at<Vec3b>(i,j)[2] << 12) + (src.at<Vec3b>(i,j)[2] << 9 ) + (src.at<Vec3b>(i,j)[2] << 7 ) +
                (src.at<Vec3b>(i,j)[2] << 5 ) + (src.at<Vec3b>(i,j)[2] << 4 ) + (src.at<Vec3b>(i,j)[2] << 1 )   ) >> 20);
        }
    }

    return dst;
}


int main()
{
    Mat img,gray1,gray2;

    img = imread("img/blue.jpg");
    // BGR->RGB
    cvtColor(img, img, COLOR_BGR2RGB);

    cvtColor(img,gray1,COLOR_RGB2GRAY);
    gray2 = speed_rgb2gray(img);

    imshow("gray1",gray1);
    imshow("gray2",gray2);
    waitKey(0);    

    return 0;
}