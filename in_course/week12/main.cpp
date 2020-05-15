#include "gamma.h"
#include "equalizeHist.h"

using namespace cv;
using namespace std;

void gammaTest(float gamma)
{
	Mat img = imread("../../../img/face.jpg", IMREAD_GRAYSCALE);
	Mat dst;
	
	imshow("img", img);
	gammaCorrect(img, dst,gamma);

	imshow("gamma", dst);
	waitKey(0);
	destroyAllWindows();
}


void mycallback(int size, void *data)
{
    Mat src = *(Mat *)data;
    Mat dst; 

	gammaCorrect(src, dst, (float)size /100.0);

    imshow("img", dst);
}


void gammaTest2()
{
	Mat img = imread("../../../img/gtest.jpg", IMREAD_GRAYSCALE);
	
	int minSize = 1;
	int maxSize = 100;
	
	imshow("img", img);
	createTrackbar("gamma value", "img", &minSize, maxSize, mycallback, &img);
	waitKey(0);
	destroyAllWindows();
}

void histTest()
{
	Mat img = imread("../../../img/etest.jpg");
	Mat dst;
	
	imshow("img", img);
	equalizeHistBGR(img, dst);

	imshow("equalize", dst);
	waitKey(0);
	destroyAllWindows();
}



int main()
{
	//gamma矫正演示
	gammaTest(1 / 2.2);
	gammaTest(2.2);

	//查找装逼信息
	gammaTest2();

	//三通道直方图均衡化
	histTest();


    return 0;
}