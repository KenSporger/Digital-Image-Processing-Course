#include "dft.h"
#include "roi.h"

using namespace std;
using namespace cv;


void mixDemo(int dx, int dy)
{

    Mat hp = imread("../../../img/Einstein.jpeg", IMREAD_GRAYSCALE);
	Mat lp= imread("../../../img/menglu.jpeg", IMREAD_GRAYSCALE);

    //分别存储幅度谱、相位谱
	Mat lpmats[2], hpmats[2];
	//DFT变换获取幅度相位谱
	dftConvert(lp, lpmats[0], lpmats[1]);
	dftConvert(hp, hpmats[0], hpmats[1]);

	//获取高通和低通掩码
	Mat lpmask(lpmats[0].rows, lpmats[0].cols, CV_32F, Scalar::all(0));
	Mat hpmask(hpmats[0].rows, hpmats[0].cols, CV_32F, Scalar::all(1));
	Mat q0 = lpmask(Rect(0, 0, dx, dy));
	Mat q1 = lpmask(Rect(lpmats[0].cols - dx, 0, dx, dy));
	Mat q2 = lpmask(Rect(lpmats[0].cols - dx, lpmats[0].rows - dy,dx, dy));
	Mat q3 = lpmask(Rect(0, lpmats[0].rows - dy, dx, dy));
	q0 = 1;q1 = 1; q2 = 1; q3 = 1; 
	subtract(hpmask, lpmask, hpmask);

	lpmats[0] = lpmats[0].mul(lpmask);
    lpmats[1] = lpmats[1].mul(lpmask);
	hpmats[0] = hpmats[0].mul(hpmask);
    hpmats[1] = hpmats[1].mul(hpmask);

	//频谱融合
	add(lpmats[0], hpmats[0], lpmats[0]);
	add(lpmats[1], hpmats[1], lpmats[1]);

	Mat dst;
	idftConvert(lpmats[0], lpmats[1], lp.rows, lp.cols, dst);
	imshow("dst", dst);
	waitKey(0);
	destroyAllWindows();
}

void dftDemo()
{
	Mat src= imread("../../../img/lena.jpg", IMREAD_GRAYSCALE);	
	Mat dft_mats[3];
	dftInteractiveFilter(src, dft_mats[0], dft_mats[1], dft_mats[2]);
	imshow("dst", dft_mats[2]);
	waitKey(0);
	destroyAllWindows();
}

void textDemo()
{
	Mat src= imread("../../../img/text.png", IMREAD_GRAYSCALE);	
}

 
int main()
{

	// mixDemo(15, 15);
	// dftDemo();
	textDemo();
	

}
 