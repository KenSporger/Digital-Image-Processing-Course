#include <opencv2/opencv.hpp>
#include <iostream>
// set cv as default namespace below
using namespace cv;

int main()
{
    cv::Mat img=cv::imread("img/blue.jpg");
    // BGR not RGB
    std::cout << img.at<Vec3b>(0,0) << std::endl;
    cv::imshow("image",img);
    waitKey(0);
    return 0;
}
 
