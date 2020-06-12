#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


bool polynomial_curve_fit(std::vector<cv::Point>& key_point, cv::Mat& A, int n, Point anchor)
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
    cv::solve(X, Y, A, cv::DECOMP_SVD);
    return true;
}


template<typename T>
RotatedRect operator+(const RotatedRect &rec, const Point_<T> &p)
{
    return RotatedRect(Point(rec.center.x + p.x, rec.center.y + p.y), rec.size, rec.angle);
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
	video.open("video/fire.mp4");

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
	Mat labels, centers, connect;
    Mat stats;

    //背景
	video >> background;
	cvtColor(background, background, CV_BGR2GRAY);
	
    //混合高斯模型
	Ptr<BackgroundSubtractor> model;
	model = createBackgroundSubtractorMOG2();
	
	Point2f vtx[4];
    Point gun(173, 34);

	uchar key = 0;
	int cnt = 0;
	while (key != 'q')
	{
        double start = static_cast<double>(cvGetTickCount());
		video >> frame;

		if (frame.empty())
		{
			cerr << "can't read frame !" << endl;
            return -1;
		}
        

		cvtColor(frame, hsvMat, CV_BGR2HSV);
		cvtColor(frame, gray, CV_BGR2GRAY);


		inRange(hsvMat, Scalar(0, 25, 100), Scalar(255, 150, 255), fireMask);
		//预筛选
        inRange(hsvMat, Scalar(0, 0, 150), Scalar(140, 50, 255), watermask);


		// GaussianBlur(frame, frame, Size(3, 3), 1.0, 1.0);
        absdiff(gray, background, foregroundMask);
        medianBlur(foregroundMask, foregroundMask, 3);
        threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);
        
		// model->apply(frame, foregroundMask);
		bitwise_and(foregroundMask, watermask, foregroundMask);
        // threshold(foregroundMask, foregroundMask, 200, 255, THRESH_BINARY);
        //膨胀获取较完整的轮廓
        // erode(foregroundMask, foregroundMask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        morphologyEx(foregroundMask, foregroundMask, MORPH_OPEN, getStructuringElement(MORPH_CROSS, Size(3, 3)));
        // dilate(foregroundMask, foregroundMask, getStructuringElement(MORPH_RECT, Size(3, 3)));


        //实测0.4ms
        vector<vector<Point>> contours;
        vector<Point> points;
        vector<Point> real_points;
        Point2f vtx[4];
		findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for (const auto &contour : contours)
        {
            if (contourArea(contour) < 10) continue;
            RotatedRect waterRotateRect = minAreaRect(contour);
            if (waterRotateRect.angle < -70 || 
                waterRotateRect.angle > -10 ||
                waterRotateRect.size.height / waterRotateRect.size.width < 0.7
                ) 
                continue;


            // waterRotateRect.points(vtx);
            // for (const auto &p : contour)
            {
                // points.push_back(p);
                points.push_back(waterRotateRect.center);
                // cout << waterRotateRect.center << endl;
            }
            // for (int j = 0; j < 4; j++)
            // {
            //     line(frame, vtx[j], vtx[(j+1) % 4], Scalar(255, 0, 0));
            // }

        }

        sort(points.begin(), points.end(), [](const Point &p1, const Point &p2){
            return p1.x < p2.x;
        });
        real_points.push_back(gun);
        float rate = 0;
        for (const auto &p : points)
        {
            if (real_points.size() == 1)
            {
                rate = (gun.y - p.y) / (gun.x - p.x);
                real_points.push_back(p);
            }
            else
            {
                float _rate = (float)(real_points.back().y - p.y) / (real_points.back().x - p.x);
                if ((_rate - rate) < 1 && (_rate - rate) > 0)
                {
                    real_points.push_back(p);
                    rate = _rate;
                }
            }
            
        }

        for (const auto &p : real_points)
        {
            circle(frame, p, 1, Scalar(255, 0, 0));
        }

        if (real_points.size() > 4)
        {
            Mat A;
            polynomial_curve_fit(real_points, A, 2, gun);
            std::vector<cv::Point> points_fitted;
            for (int x = gun.x ; x < frame.cols; x++)
            {
                double y = A.at<double>(0, 0) + A.at<double>(1, 0) * x +
                    A.at<double>(2, 0)*std::pow(x, 2) + A.at<double>(3, 0)*std::pow(x, 3);

                if (y < gun.y || y >= frame.rows) continue;
                points_fitted.push_back(cv::Point(x, y));
            }
            polylines(frame, points_fitted, false, cv::Scalar(0, 255, 0), 2, 8, 0);
        }
        double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
        cout << "processing time:" << time / 1000 << "ms" << endl;


		// vector<vector<Point>> contours;
    	// //输入二值图，输出shape(a,b),a表示轮廓数量,b表示每条轮廓包含的点
    	// findContours(fireMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


		// for (int i = 0; i < contours.size(); i++)
		// {
		// 	// drawContours(frame, contours, i, Scalar(255, 0, 0), 3);
		// 	Rect box = boundingRect(contours[i]);
		// 	rectangle(frame, box, Scalar(255, 0, 0));
		// }

        circle(foregroundMask, gun, 4, Scalar(255), 2);
		imshow("mask", foregroundMask);
		// imshow("fire", fireMask);
        // imshow("dst", dst);
        imshow("frame", frame);
		// imshow("waterMask", watermask);


		key = waitKey(30);
		
	}
	
}
 