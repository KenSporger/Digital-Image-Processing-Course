#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Mat img = imread("img/die_on_chip.png");
    Mat gray, bin;
    // Mat mask(img.rows, img.cols, CV_8UC1, Scalar(0));
    cvtColor(img, gray, CV_BGR2GRAY);
    
    threshold(gray, bin, 0, 255, THRESH_OTSU);
    
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
        if (prop > 0.9 && prop < 1.1)
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