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

            // if (avg > thres) avg = 255;
            // else avg = 0;

            src.at<Vec3b>(i, j)[0] = avg;
            src.at<Vec3b>(i, j)[1] = avg;
            src.at<Vec3b>(i, j)[2] = avg;

        }
    }
} 


int main()
{
    // String path = "img/numbers.jpg";
    String path = "img/blue.jpg";

    cv::Mat img = imread(path);
    // 0.299R+0.587G+0.114B
    cv::Mat gray = imread(path, IMREAD_GRAYSCALE);
    // 0.333R+0.333G+0.333B
    pixelsAverage(img,100);
    
    
    std::cout << int(gray.at<uchar>(100,100)) << std::endl;
    std::cout << img.at<Vec3b>(100,100) << std::endl;



    imshow("img", img);
    imshow("gray", gray);
    waitKey(0);
}