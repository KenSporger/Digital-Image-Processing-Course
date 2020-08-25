#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

char ss[4] = {'A', 'B','C','D'};

void printP(Point2f srcP[4])
{
    static int cnt = 0;
    for (int i = 0; i < 4; i++)
        cout << ss[i] << cnt << "="<< "("<< srcP[i].x << ","<< srcP[i].y<<")"<< endl;
    cnt++;
}

void transformP(Point2f srcP[4], Mat &affineM)
{
    Point2f temp;
    double s;
    for (int i = 0; i < 4; i++)
    {
        temp.x = srcP[i].x * affineM.at<double>(0, 0) + srcP[i].y * affineM.at<double>(0, 1) + affineM.at<double>(0, 2);
        temp.y = srcP[i].x * affineM.at<double>(1, 0) + srcP[i].y * affineM.at<double>(1, 1) + affineM.at<double>(1, 2);
        s = srcP[i].x * affineM.at<double>(2, 0) + srcP[i].y * affineM.at<double>(2, 1) + affineM.at<double>(2, 2);
        srcP[i] = temp / s;
    }

}


// epsilon = +1/-1
void equilongTransform(double epsilon=1, double theta=0.3925, double tx=50, double ty=50)
{
    Point2f srcP[4]={Point2f(100, 100), Point2f(300, 100), Point2f(300, 200), Point2f(100, 200)};
    Mat affineM(3, 3, CV_64FC1, Scalar(0));
    affineM.at<double>(0, 0) = epsilon * cos(theta);
    affineM.at<double>(0, 1) = - sin(theta);
    affineM.at<double>(0, 2) = tx;
    affineM.at<double>(1, 0) = epsilon * sin(theta);
    affineM.at<double>(1, 1) = cos(theta);
    affineM.at<double>(1, 2) = ty;
    affineM.at<double>(2, 0) = 0;
    affineM.at<double>(2, 1) = 0;
    affineM.at<double>(2, 2) = 1;

    transformP(srcP, affineM);
    cout << "equilongTransform" << endl;
    printP(srcP);
    cout << endl;
}

void similarTransform(double s=0.5, double theta=0.3925, double tx=50, double ty=50)
{
    Point2f srcP[4]={Point2f(100, 100), Point2f(300, 100), Point2f(300, 200), Point2f(100, 200)};
    Mat affineM(3, 3, CV_64FC1, Scalar(0));
    affineM.at<double>(0, 0) = s * cos(theta);
    affineM.at<double>(0, 1) = -s * sin(theta);
    affineM.at<double>(0, 2) = tx;
    affineM.at<double>(1, 0) = s * sin(theta);
    affineM.at<double>(1, 1) = s * cos(theta);
    affineM.at<double>(1, 2) = ty;
    affineM.at<double>(2, 0) = 0;
    affineM.at<double>(2, 1) = 0;
    affineM.at<double>(2, 2) = 1;

    transformP(srcP, affineM);
    cout << "similarTransform" << endl;
    printP(srcP);
    cout << endl;
}


void affineTransform(double a11=0.1, double a12=0.2, double a21=0.3, double a22=0.4, double theta=0.3925, double tx=50, double ty=50)
{
    Point2f srcP[4]={Point2f(100, 100), Point2f(300, 100), Point2f(300, 200), Point2f(100, 200)};
    Mat affineM(3, 3, CV_64FC1, Scalar(0));
    affineM.at<double>(0, 0) = a11 ;
    affineM.at<double>(0, 1) = a12;
    affineM.at<double>(0, 2) = tx;
    affineM.at<double>(1, 0) = a21;
    affineM.at<double>(1, 1) = a22;
    affineM.at<double>(1, 2) = ty;
    affineM.at<double>(2, 0) = 0;
    affineM.at<double>(2, 1) = 0;
    affineM.at<double>(2, 2) = 1;

    transformP(srcP, affineM);
    cout << "affineTransform" << endl;
    printP(srcP);
    cout << endl;
}

void projectTransform(double a11=0.1, double a12=0.2, double a21=0.3, double a22=0.4, double theta=0.3925, double tx=50, double ty=50, double v1=0.5, double v2=0.3)
{
    Point2f srcP[4]={Point2f(100, 100), Point2f(300, 100), Point2f(300, 200), Point2f(100, 200)};
    Mat affineM(3, 3, CV_64FC1, Scalar(0));
    affineM.at<double>(0, 0) = a11 ;
    affineM.at<double>(0, 1) = a12;
    affineM.at<double>(0, 2) = tx;
    affineM.at<double>(1, 0) = a21;
    affineM.at<double>(1, 1) = a22;
    affineM.at<double>(1, 2) = ty;
    affineM.at<double>(2, 0) = v1;
    affineM.at<double>(2, 1) = v2;
    affineM.at<double>(2, 2) = 1;

    transformP(srcP, affineM);
    cout << "projectTransform" << endl;
    printP(srcP);
    cout << endl;
}



int main()
{
    equilongTransform();
    equilongTransform(-1);
    similarTransform();
    affineTransform();
    projectTransform();


}