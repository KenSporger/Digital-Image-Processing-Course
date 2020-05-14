#include<iostream>
#include<opencv2/opencv.hpp>

//交互式ROI
class ROI
{
    public:
        //构造函数
        ROI(const cv::Mat src);
        cv::String windowName = "defalut";
        //焦点大小
        int pointSize = 4;
        //轮廓粗细
        int contourThickness = 2;
        //轮廓颜色
        cv::Scalar displayColor = cv::Scalar(255, 0, 0);
        //多边形ROI选择
        void selectPolygon(cv::Mat &dstMat);
    private:
        //ROI源图
        cv::Mat srcMat;
        //鼠标采集点
        std::vector<cv::Point> mousePoints;
        std::vector<std::vector<cv::Point>> contours;
        void mouseCallback(int EVENT, int x, int y, int flags, void* userdata);
};



