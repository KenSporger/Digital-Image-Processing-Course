#include<iostream>
#include<time.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//use at to access
void func1(Mat img)
{
    int rows = img.rows;
    int cols = img.cols;
    Mat dst(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            dst.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
        }
    }
}

//use pointer to access
void func2(Mat img)
{
    int rows = img.rows;
    int cols = img.cols;
    int length = cols * img.channels();
    Mat dst(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++){
        uchar* pdst = dst.ptr<uchar>(i);
        uchar* psrc = img.ptr<uchar>(i); 
        for (int j = 0; j < length; j++){
            pdst[j] = psrc[j];
        }
    }
}


// see it as one-demension array
void func3(Mat img)
{
    int rows = img.rows;
    int cols = img.cols;
    int total = rows * cols * img.channels();
    Mat dst(rows, cols, CV_8UC3); 

    // get address of the first element  
    uchar* phead = dst.data;
    uchar* shead = img.data;

    for (int i = 0; i < total; i++){
        *phead++ = *shead++;
    }

}

int main()
{
    Mat img = imread("img/numbers.jpg");
    //func2/func3 is a little faster than func1
    clock_t t0 = clock();
    for (int i = 0; i < 1000; i++)
        func1(img);
    clock_t t1 = clock();
    for (int i = 0; i < 1000; i++)
        func2(img);
    clock_t t2 = clock();
    for (int i = 0; i < 1000; i++)
        func3(img);
    clock_t t3 = clock();


    cout << t1 - t0 << endl;
    cout << t2 - t1 << endl;
    cout << t3 - t2 << endl;
}
