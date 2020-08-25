#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;

// 英文字体识别
void demo1()
{
    Mat img = imread("../../../img/ocr.jpg", IMREAD_GRAYSCALE);
    Rect numRoi;
    Mat dst;
    numRoi = selectROI(img);
    dst = img(numRoi);
    imshow("roi", dst);
    waitKey(0);

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    // tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    api->SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);
    // Get the text
    char* out = api->GetUTF8Text();
    std::cout << out << std::endl;
}


// 英文字体识别
void demo2()
{
    Mat img = imread("../../../img/ocr.jpg", IMREAD_GRAYSCALE);
    Rect numRoi;
    Mat dst;
    numRoi = selectROI(img);
    dst = img(numRoi);


    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    // tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    api->SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);
    // Get the text
    api->Recognize(0);
    tesseract::ResultIterator* ri = api->GetIterator();
    tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
    if (ri != 0) {
        do {
        const char* word = ri->GetUTF8Text(level);
        float conf = ri->Confidence(level);
        int x1, y1, x2, y2;
        ri->BoundingBox(level, &x1, &y1, &x2, &y2);
        printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
                word, conf, x1, y1, x2, y2);
        rectangle(img, Point(x1, y1), Point(x2, y2), Scalar(255), 2);
        delete[] word;
        } while (ri->Next(level));
    }

    api->End();
    delete api;
    imshow("img", img);
    waitKey(0);
}


// 单字体分类
void demo3()
{
    Mat img = imread("../../../img/ocr.jpg", IMREAD_GRAYSCALE);
    Rect numRoi;
    Mat dst;
    numRoi = selectROI(img);
    dst = img(numRoi);


    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    // tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    api->SetImage((uchar*)dst.data, dst.cols, dst.rows, 1, dst.cols);
    // Get the text
    api->SetVariable("save_blob_choices", "2");
    // api->Recognize(0);
    // 
    char* out = api->GetUTF8Text();
    std::cout << out << std::endl;
    api->End();
    delete api;
}




int main()
{
    demo1();
    // demo2();
    // demo3();
}

