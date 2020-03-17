#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

int main(){
    //assign and define together  cols,rows scalar(one-dimensional vector)
    cv::Mat M1(3,2,CV_8UC3,Scalar(0,0,255));
    std::cout << "M1=" << std::endl;
    std::cout << M1 << std::endl;

    //define and assign later
    cv::Mat M2;
    M2.create(3,2,CV_32FC1);
    std::cout << "M2=" << std::endl;
    std::cout << M2 << std::endl;

    //get properties of Mat
    int height = M1.rows;
    int width = M1.cols;

    //print in one line 
    std::cout << "size:" << height << "," << width << std::endl;

    cv::Mat M3,M4,M5;
    //shallow copy , point to same addrenss
    M3=M1;
    M5=M1;
    //deep copy
    M1.copyTo(M4);

    //M3 point to M1 ,M1 point to M2,M3 != M2
    M1 = M2;

    std::cout << "M1=" << std::endl;
    std::cout << M1 << std::endl;
    std::cout << "M3:" << std::endl;
    std::cout << M3 << std::endl;
    std::cout << "M4:" << std::endl;
    std::cout << M4 << std::endl;

    //generate random number(uchar)
    randu(M3,0,255);
    std::cout << "M5:" << std::endl;
    std::cout << M5 << std::endl;  





}