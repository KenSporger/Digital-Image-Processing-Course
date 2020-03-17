#include<iostream>
#include<string.h>
#include<time.h>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// ordinary median filter
void medianFilter(const Mat src, Mat &dst, int kSize)
{
    CV_Assert(src.type() == CV_8UC1);

    Mat srcEtd;
    int border = kSize / 2;
    int midIndex = kSize * kSize / 2; 
    vector<uchar> kernel;

    src.copyTo(dst);
    copyMakeBorder(src, srcEtd, border, border, border, border, BORDER_REFLECT);

    int rows = srcEtd.rows - border;
    int cols = srcEtd.cols - border;

    for (int i = border; i < rows; i++){
        for (int j = border; j < cols; j++){

            for (int m = -border; m <= border; m++)
                for (int n = -border; n <= border; n++)
                    kernel.push_back( srcEtd.at<uchar>(i + m, j + n) );

            sort(kernel.begin(), kernel.end());

            dst.at<uchar>(i - border, j - border) = kernel[midIndex];
            kernel.clear();
        }
    }
}

// count pixels in hist to find median value 
uchar getMedianValue(const int hist[], int thres)
{
    int sum = 0;
    for (int i = 0; i < 256; i++){
        sum += hist[i];
        // the number of pixels couted is up to thres(usually harf of the total)
        if (sum >= thres)   return i;
    }
    return 0;
}

// use histogram statistic to accelerate the computation
void huangMedianFilter(const Mat src, Mat &dst, int kSize)
{
    CV_Assert(src.type() == CV_8UC1);

    Mat srcEtd;
    int border = kSize / 2;

    src.copyTo(dst);
    copyMakeBorder(src, srcEtd, border, border, border, border, BORDER_REFLECT);

    int rows = srcEtd.rows - border;
    int cols = srcEtd.cols - border;
    int hist[256] = {0};
    //harf of the total
    int thres = kSize * kSize / 2 + 1;

    for (int i = border; i < rows; i++){
        for (int j = border; j < cols; j++){

            if (j == border){
                // reset histogram in the beginning of each row
                memset(hist, 0, sizeof(hist));
                
                for (int m = -border; m <= border; m++){
                    for (int n = -border; n <= border; n++){
                        uchar val = srcEtd.at<uchar>(i + m, j + n);
                        hist[val]++;
                    }
                }
            }else{
                // add the rightmost col,delete the leftmost col
                for (int m = -border; m <= border; m++){
                    uchar rgiht = srcEtd.at<uchar>(i + m, j + border);
                    uchar left = srcEtd.at<uchar>(i + m, j - border -1);
                    hist[rgiht]++;
                    hist[left]--;
                }
            }
            // find median value 
            dst.at<uchar>(i - border, j - border) = getMedianValue(hist, thres);
        }
    }
}



int main()
{
    Mat img = imread("img/numbers.jpg", IMREAD_GRAYSCALE);
    Mat dst,dst1,dst2;


    clock_t t0 = clock();
    medianBlur(img, dst, 9);
    clock_t t1 = clock();
    medianFilter(img, dst1, 9);
    clock_t t2 = clock();
    huangMedianFilter(img, dst2, 9);
    clock_t t3 = clock();

    // compare the running time
    cout << (t1-t0) << endl;
    cout << (t2-t1) << endl;
    cout << (t3-t2) << endl;

    imshow("img", img);
    imshow("dst", dst);
    imshow("dst1", dst1);
    imshow("dst2", dst2);
    waitKey(0);

}