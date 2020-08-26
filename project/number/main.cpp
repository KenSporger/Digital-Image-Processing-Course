#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;



//Tesseract 4.0最好在浅色背景上使用深色文本
int demo1()
{
    Mat img = imread("../../../img/ocr.jpg", 0);
    Rect numRoi;
    Mat numMat, dst;
    numRoi = selectROI(img);
    numMat = img(numRoi);
    numMat.copyTo(dst);
    threshold(numMat, dst, 200, 255,THRESH_BINARY_INV);
    imshow("roi", dst);
    waitKey(0);

    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng");
    // tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);
 
    // Get the text
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;
}

//only number
int demo2()
{
    Mat img = imread("../../../img/ocr.jpg", 0);
    Rect numRoi;
    Mat numMat, dst;
    numRoi = selectROI(img);
    numMat = img(numRoi);
    numMat.copyTo(dst);
    threshold(numMat, dst, 200, 255,THRESH_BINARY_INV);
    imshow("roi", dst);
    waitKey(0);

    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng");
    tess.SetVariable("tessedit_char_whitelist", "0123456789");
    tess.SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);
    // Get the text
    char* out = tess.GetUTF8Text();
    std::cout << out << std::endl;
}




int main()
{
    // demo1();
    demo2();
}