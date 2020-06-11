#include "roi.h"

using namespace cv;

std::vector<Point> mousePoints;
const String windowName = "roi";


//鼠标交互相应
void polygonCallback(int EVENT, int x, int y, int flags, void* userdata)
{
    //获取窗口显示图像
    Mat img = *(Mat*) userdata;
    Mat dst;
    img.copyTo(dst);
    //获取坐标
    Point pos(x,y);

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
            circle(img, pos, 4, Scalar(255, 0, 0), -1);
            size_t end = mousePoints.size();
            if (end > 1)
            {
                line(img, mousePoints[end - 2], mousePoints[end - 1], Scalar(255, 0, 0));
                line(dst, mousePoints[end - 2], mousePoints[end - 1], Scalar(255, 0, 0));
            }
            break;
        }
        case CV_EVENT_MOUSEMOVE:
        {
            size_t end = mousePoints.size();
            if (end > 1)
            {
                line(dst, mousePoints[end - 1], pos, Scalar(255, 0, 0));
                line(dst, mousePoints[0], pos, Scalar(255, 0, 0));
            }
        }

    }

    imshow(windowName, dst);
}

//ROI多边形区域选择
//roi_select=false，ROI效果为从原图中去除
//roi_select=true,ROI效果为从原图中提取
void selectPolygon(const Mat &srcMat, Mat &dstMat, bool roi_select)
{
    if (srcMat.empty())
    {
        std::cerr << "srcMat is empty!" << std::endl;
        return;
    }

    //鼠标左键选择角点，任意按键结束选择
    imshow(windowName, srcMat);
    Mat selectMat;
    char key;
    srcMat.copyTo(selectMat);
    std::vector<std::vector<Point>> contours;
    do{
        setMouseCallback(windowName, polygonCallback, &selectMat);
        key = waitKey(0);
        //判断是否能构成多边形
        if (mousePoints.size() < 3)
        {
            std::cout << "points are too little!:" << std::endl;
            mousePoints.clear();
        }
        else
        {
            line(selectMat, mousePoints[0], mousePoints[mousePoints.size() - 1], Scalar(255, 0, 0));
            //存储边界
            contours.push_back(mousePoints);
            mousePoints.clear();
        }
    }while (key != 'q');

    destroyAllWindows();

    //实心roi掩码
    //掩码图像
    Mat mask(srcMat.rows, srcMat.cols, CV_32F, Scalar(!roi_select));
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(mask, contours, i, Scalar(roi_select), -1);
    }
    mask.copyTo(dstMat);
    mousePoints.clear();
}

