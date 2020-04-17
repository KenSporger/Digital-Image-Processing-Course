#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("img/topic1.JPG");
    Mat hsv, gray, bin1, bin2, bin;
    // Mat mask(img.rows, img.cols, CV_8UC1, Scalar(0));
    resize(img, img, Size(400, 400));
    cvtColor(img, hsv, CV_BGR2HSV);

    int minH = 0;
    int maxH = 20;
    int minS = 43;
    int maxS = 255;
    int minV = 55;
    int maxV = 255;
    
    inRange(hsv, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), bin1);
    minH = 160;
    maxH = 180;
    inRange(hsv, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), bin2);

    bitwise_or(bin1, bin2, bin);
    
    vector<vector<Point>> contours;
    findContours(bin, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    Point2f vtx[4];
    for (int i = 0; i < contours.size(); i++){
        drawContours(img, contours, i, Scalar(255, 0, 0), 3, 8);

        RotatedRect rbox = minAreaRect(contours[i]);
        rbox.points(vtx);
        float width = rbox.size.width;
        float height = rbox.size.height;
        float prop = width / height;
        float area = width * height;
        if (area > 400)
        {
            for (int j = 0; j < 4; j++)
            {
            
            line(img, vtx[j], vtx[(j + 1) % 4], Scalar(255 ,255, 255), 2, CV_AA);
            }


        }
        

    }


    imshow("img", img);
    waitKey(0);



}