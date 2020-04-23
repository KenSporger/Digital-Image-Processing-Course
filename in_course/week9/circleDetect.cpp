#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("img/rim.png");
    Mat gray, bin;
    
    cvtColor(img, gray, CV_BGR2GRAY);
    
    threshold(gray, bin, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
    
    vector<vector<Point>> contours;
    //输入二值图，输出shape(a,b),a表示轮廓数量,b表示每条轮廓包含的点
    findContours(bin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    Point2f vtx[4];
    for (int i = 0; i < contours.size(); i++){
        //在图中画出轮廓
        drawContours(img, contours, i, Scalar(255, 0, 0), 3, 8);
        //获取最小包围矩形,不同于boundingbox
        RotatedRect rbox = minAreaRect(contours[i]);
        //获取四个角点
        rbox.points(vtx);
        float width = rbox.size.width;
        float height = rbox.size.height;
        //宽长比
        float prop = width / height;
        //面积
        float area = width * height;
        //限制宽长比和面积
        if (prop > 0.9 && prop < 1.1 && area > 500)
        {
            for (int j = 0; j < 4; j++)
            {
                //画最小包围矩形的四个边
                line(img, vtx[j], vtx[(j + 1) % 4], Scalar(255 ,255, 255), 2, CV_AA);
            }


        }
        

    }


    imshow("img", img);
    waitKey(0);



}