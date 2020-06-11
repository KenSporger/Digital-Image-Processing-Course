#include<iostream>
#include<opencv2/opencv.hpp>
#include "dft.h"

using namespace std;
using namespace cv;


bool polynomial_curve_fit(std::vector<cv::Point>& key_point, int n, cv::Mat& A)
{
    //Number of key points
    int N = key_point.size();

    //构造矩阵X
    cv::Mat X = cv::Mat::zeros(n + 1, n + 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            for (int k = 0; k < N; k++)
            {
                X.at<double>(i, j) = X.at<double>(i, j) +
                    std::pow(key_point[k].x, i + j);
            }
        }
    }

    //构造矩阵Y
    cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    for (int i = 0; i < n + 1; i++)
    {
        for (int k = 0; k < N; k++)
        {
            Y.at<double>(i, 0) = Y.at<double>(i, 0) +
                std::pow(key_point[k].x, i) * key_point[k].y;
        }
    }

    A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    //求解矩阵A
    cv::solve(X, Y, A, cv::DECOMP_LU);
    return true;
}


 //难点：
 //提取干净的水柱：
 //1、先HSV去掉非水珠区域
 //2、背景差分获取水柱区域，和上一步取交集
 //3、用膨胀法将断开的水柱连通起来
 //4、祸夫直线检测可以检测水柱上的短直线
 //5、傅里叶变换可以获得水柱，关键在于频谱区域选择
 //6、第一帧获取ROI之后可以在ROI区域内检测
 //7、一开始使用高斯模糊噪声会少
int main()
{
	VideoCapture video;
	video.open("../../../video/fire.mp4");

	if (!video.isOpened())
	{
		cerr << "can't read video !" << endl;
		return -1;
	}

	Mat frame, fireMask, watermask;
	Mat gray;
	Mat hsvMat;
	Mat foregroundMask, foreground;
	Mat background;
	Mat labels, stats, centers, connect;

    //背景
	video >> background;
	cvtColor(background, background, CV_BGR2GRAY);
	
    //混合高斯模型
	Ptr<BackgroundSubtractor> model;
	model = createBackgroundSubtractorMOG2();
	
	Point2f vtx[4];

	uchar key = 0;
	int cnt = 0;
	while (key != 'q')
	{
		video >> frame;

		if (frame.empty())
		{
			cerr << "can't read frame !" << endl;
            return -1;
		}


		cvtColor(frame, hsvMat, CV_BGR2HSV);
		cvtColor(frame, gray, CV_BGR2GRAY);
		// absdiff(gray, background, foregroundMask);
        // threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);

		inRange(hsvMat, Scalar(0, 25, 100), Scalar(255, 150, 255), fireMask);
		//预筛选
        inRange(hsvMat, Scalar(0, 0, 150), Scalar(100, 25, 255), watermask);


		GaussianBlur(frame, frame, Size(3, 3), 1.0, 1.0);
		model->apply(frame, foregroundMask);
		bitwise_and(foregroundMask, watermask, foregroundMask);
        threshold(foregroundMask, foregroundMask, 200, 255, THRESH_BINARY);
        //膨胀获取较完整的轮廓
        dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_RECT, Size(3, 3)));

        // 连通域
        // labels：大小同src，灰度值代表标号，0为背景
        // stats： 每一行都是一个连通域的特征参数:x,y,width,height,area;利用x,y,width,height可以绘制包围矩形框
        // centers：中心坐标
        // stats和centers的第一行都是背景连通域
        // cnt 连通域个数（包含背景
        // if (++cnt >= 100)
        // {
        //     cnt = 100;
        // }
        vector<Point> points;
        vector<vector<Point>> contours;
        Point2f vtx[4];
		findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for (const auto &contour : contours)
        {
            RotatedRect waterRotateRect = minAreaRect(contour);
            if (waterRotateRect.angle < -85 || 
                waterRotateRect.angle > -25 ||
                waterRotateRect.size.width > 10 ||
                waterRotateRect.size.height / waterRotateRect.size.width < 2 
                ) 
                continue;

            waterRotateRect.points(vtx);
            points.push_back(vtx[0]);
            points.push_back(vtx[1]);
            points.push_back(vtx[2]);
            points.push_back(vtx[3]);
            // for (int j = 0; j < 4; j++)
            // {
            //     line(frame, vtx[j], vtx[(j+1) % 4], Scalar(255, 0, 0));
            // }

        }

        if (points.size() < 12)
            continue;



		// //每次都要清零，copyto会叠加上次的结果
		// foreground = Scalar::all(0);
		// frame.copyTo(foreground, foregroundMask);


        // // Rect box;
        // vector<Point> points;
        // for (int i = 1; i < stats.rows; i++)
        // {
        //     // box.x = stats.at<int>(i, 0);
        //     // box.y = stats.at<int>(i, 1);
        //     // box.width = stats.at<int>(i, 2);
        //     // box.height = stats.at<int>(i, 3);
        //     if (stats.at<float>(i, 4) > )

        //     // rectangle(frame, box, CV_RGB(255, 255, 255));
        // }

		
        // for (int i = 0; i < foregroundMask.rows; i++)
        // {
        //     for (int j = 0; j < foregroundMask.cols; j++)
        //     {
        //         if (foregroundMask.at<uchar>(i,j))
        //             points.push_back(Point(j, i));
        //     }
        // }
        
        Mat A;
		polynomial_curve_fit(points, 2, A);
        std::vector<cv::Point> points_fitted;

        for (int x = 0; x < 1000; x++)
        {
            double y = A.at<double>(0, 0) + A.at<double>(1, 0) * x +
                A.at<double>(2, 0)*std::pow(x, 2) + A.at<double>(3, 0)*std::pow(x, 3);

            points_fitted.push_back(cv::Point(x, y));
        }
        polylines(frame, points_fitted, false, cv::Scalar(0, 255, 0), 3, 8, 0);


		// vector<vector<Point>> contours;
    	// //输入二值图，输出shape(a,b),a表示轮廓数量,b表示每条轮廓包含的点
    	// findContours(fireMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


		// for (int i = 0; i < contours.size(); i++)
		// {
		// 	// drawContours(frame, contours, i, Scalar(255, 0, 0), 3);
		// 	Rect box = boundingRect(contours[i]);
		// 	rectangle(frame, box, Scalar(255, 0, 0));
		// }

		imshow("mask", foregroundMask);
		// imshow("fire", fireMask);
        // imshow("dst", dst);
        imshow("frame", frame);
		// imshow("waterMask", watermask);


		key = waitKey(30);
		
	}
	
}
 