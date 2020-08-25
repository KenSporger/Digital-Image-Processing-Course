#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;




int main()
{
    Mat img = imread("../../../img/ocr.jpg", 0);
    Rect numRoi;
    Mat numMat, dst;
    numRoi = selectROI(img);
    numMat = img(numRoi);
    numMat.copyTo(dst);
    // threshold(numMat, dst, 100, 255,THRESH_BINARY);
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




// int main()
// {
//     char *outText;

//     tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
//     // Initialize tesseract-ocr with English, without specifying tessdata path
//     if (api->Init(NULL, "eng")) {
//         fprintf(stderr, "Could not initialize tesseract.\n");
//         exit(1);
//     }

//     // Open input image with leptonica library
//     Pix *image = pixRead("img/ocr.jpg");
//     api->SetImage(image);
//     // Get OCR result
//     outText = api->GetUTF8Text();
//     printf("OCR output:\n%s", outText);

//     // Destroy used object and release memory
//     api->End();
//     delete api;
//     delete [] outText;
//     pixDestroy(&image);

//     return 0;
// }