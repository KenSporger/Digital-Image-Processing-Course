#include "roi.h"

//构造函数
ROI::ROI(const cv::Mat src)
{
    if (!src.empty())
    {
        src.copyTo(srcMat);
    }
    else
    {
        std::cout << "failed to read image!:" << std::endl;
        return ;
    }
}


//鼠标交互相应
void ROI::mouseCallback(int EVENT, int x, int y, int flags, void* userdata)
{
    //获取窗口显示图像
    cv::Mat img = *(cv::Mat*) userdata;
    //获取坐标
    cv::Point pos(x,y);

    /*
    EVENT: 
    EVENT_MOUSEMOVE滑动
    EVENT_LBUTTONDOWN 左击
    EVENT_RBUTTONDOWN 右击
    EVENT_MBUTTONDOWN中键点击
    EVENT_LBUTTONUP 左键放开
    EVENT_RBUTTONUP 右键放开
    EVENT_LBUTTONDBLCLK左键双击
    EVENT_RBUTTONDBLCLK 右键双击
    EVENT_MBUTTONDBLCLK 中键双击
    EVENT_FLAG_LBUTTON 左键拖拽
    FLAGS:
    EVENT_FLAG_RBUTTON 右键拖拽
    EVENT_FLAG_MBUTTON 中键拖拽
    EVENT_FLAG_CTRLKEY 按住Ctrl不放
    EVENT_FLAG_SHIFTKEY 按住Shift不放
    EVENT_FLAG_ALTKEY 按住Alt不放
    */
    switch (EVENT)
    {
        //鼠标左键点击确认角点
        case CV_EVENT_LBUTTONDOWN:
        {
            mousePoints.push_back(pos);
            //绘制点以反馈
            cv::circle(img, pos, pointSize, displayColor, -1);
            // case CV_EVENT_MOUSEMOVE
            break;
        }

    }

    cv::imshow(windowName, img);
}

//ROI多边形区域选择
void ROI::selectPolygon(cv::Mat &dstMat)
{
    cv::Mat selectMat;
    //掩码图像
    cv::Mat mask(srcMat.rows, srcMat.cols, 0);
    srcMat.copyTo(selectMat);

    //鼠标左键选择角点，任意按键结束选择
    cv::imshow(windowName, srcMat);
    std::cout << "ok" << std::endl;
    cv::setMouseCallback(windowName, (cv::MouseCallback)&ROI::mouseCallback, &selectMat);
    cv::waitKey(0);
    cv::destroyAllWindows();

    //判断是否能构成多边形
    if (mousePoints.size() < 3)
    {
        std::cout << "points are too little!:" << std::endl;
		return;
    }
    
    //存储边界
    contours.push_back(mousePoints);
    //实心roi掩码
    cv::drawContours(mask, contours, contours.size() - 1, 255, -1);
    mask.copyTo(dstMat);

}


