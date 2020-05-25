#include "dft.h"
#include "roi.h"
#include "hough.h"

using namespace std;
using namespace cv;

//图像混合demo：大象-豹子
//输入：dxl/dyl为低通ROI尺寸、dxh/dyh为高通ROI尺寸
void mixDemo(int dxl, int dyl, int dxh, int dyh)
{

	//高频图像：豹子
    Mat hp = imread("../../../img/leopard.jpg", IMREAD_GRAYSCALE);
	//低频图像：大象
	Mat lp= imread("../../../img/elephant.jpeg", IMREAD_GRAYSCALE);

    //分别存储幅度谱、相位谱
	Mat lpmats[2], hpmats[2];
	//DFT变换获取幅度相位谱
	dftConvert(lp, lpmats[0], lpmats[1]);
	dftConvert(hp, hpmats[0], hpmats[1]);

	//获取高通和低通掩码
	Mat lpmask(lpmats[0].rows, lpmats[0].cols, CV_32F, Scalar::all(0));
	Mat hpmask(hpmats[0].rows, hpmats[0].cols, CV_32F, Scalar::all(1));
	Mat q0 = lpmask(Rect(0, 0, dxl, dyl));
	Mat q1 = lpmask(Rect(lpmats[0].cols - dxl, 0, dxl, dyl));
	Mat q2 = lpmask(Rect(lpmats[0].cols - dxl, lpmats[0].rows - dyl, dxl, dyl));
	Mat q3 = lpmask(Rect(0, lpmats[0].rows - dyl, dxl, dyl));
	q0 = 1;q1 = 1; q2 = 1; q3 = 1; 

	lpmats[0] = lpmats[0].mul(lpmask);
    lpmats[1] = lpmats[1].mul(lpmask);

	q0 = hpmask(Rect(0, 0, dxh, dyh));
	q1 = hpmask(Rect(hpmats[0].cols - dxh, 0, dxh, dyh));
	q2 = hpmask(Rect(hpmats[0].cols - dxh, hpmats[0].rows - dyh, dxh, dyh));
	q3 = hpmask(Rect(0, hpmats[0].rows - dyh, dxh, dyh));
	q0 = 0;q1 = 0; q2 = 0; q3 = 0; 

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

//交互DFT变换Demo
void dftDemo()
{
	Mat src= imread("../../../img/menglu.jpeg", IMREAD_GRAYSCALE);	
	Mat dft_mats[3];
	dftInteractiveFilter(src, dft_mats[0], dft_mats[1], dft_mats[2]);
	imshow("img", src);
	imshow("dst", dft_mats[2]);
	waitKey(0);
	destroyAllWindows();
}

//旋转频谱对比Demo
void rotateDemo()
{
	Mat src= imread("../../../img/text.png", IMREAD_GRAYSCALE);
	Mat rotate;

	//对原图顺时针旋转90
	float angle = -90, scale = 0.7;
	Point2f center(src.cols / 2, src.rows / 2);
	const Mat M = getRotationMatrix2D(center, angle, scale);
	warpAffine(src, rotate, M, src.size(), 1, BORDER_REPLICATE);

	//观察原图频谱
	Mat dft_mats[3];
	dftConvert(src, dft_mats[0], dft_mats[1]);
	diagonExchange(dft_mats[0], dft_mats[2]);
	dftNormalize(dft_mats[2], dft_mats[2]);
	imshow("src", dft_mats[2]);

	//观察旋转图频谱
	Mat dft_mats2[3];
	dftConvert(rotate, dft_mats2[0], dft_mats2[1]);
	diagonExchange(dft_mats2[0], dft_mats2[2]);
	dftNormalize(dft_mats2[2], dft_mats2[2]);
	imshow("rotate", dft_mats2[2]);

	imshow("img", rotate);
	waitKey(0);
	destroyAllWindows();

}



//输入：二值化阈值、霍夫变换投票、角度阈值（弧度制）默认(float)CV_PI/90
void textDemo(int thresh, int vote, float angle_thresh = (float)CV_PI/90)
{
	Mat src= imread("../../../img/text.png", IMREAD_GRAYSCALE);	

	Mat mag, bin, dst;
	//DFT获取幅度谱
	dftConvert(src, mag);
	//对角互换
	diagonExchange(mag, mag);
	//归一化
	dftNormalize(mag, mag);
	//二值化
	threshold(mag, bin, thresh, 255, THRESH_BINARY);
	//霍夫直线变换
	Mat lines;
	//rho=1,theta=CV_PI / 180为精度
    HoughLines(bin, lines, 1, CV_PI / 180, vote);
	drawHoughLines(lines, mag, Scalar(255, 0, 0), 2);
	imshow("mag", mag);

	//使用平均值进行角度估算
	float angle = 0;
	//有效line计数
	float line_cnt = 0;
	float pi2 = CV_PI/2;
	//y根线：1*y*Vec2f(r,theta)
    for (int i = 0; i < lines.rows; ++i)
    {
        for (int j = 0; j < lines.cols; ++j)
        {
			//获取直线角度
            float theta = lines.at<Vec2f>(i, j)[1];
			//忽略小于阈值的角度
			if (theta < angle_thresh || abs(pi2 - theta) < angle_thresh || (CV_PI - theta) < angle_thresh)
			{
				continue;
			}
			else
			{
				//只有输入图像是正方形时，检测到的角才是文本真正旋转的角度。但我们的输入图像不一定是正方形的，所以要根据图像的长宽比改变这个角度
				theta = theta<pi2 ? theta : theta-CV_PI;
				float angelT = src.rows*tan(theta)/src.cols;
				theta = atan(angelT);
				float angelD = theta*180/(float)CV_PI;

				//对符合条件的直线角度相加
				++line_cnt;
				angle += angelD; 
			}
			
        }
    }

	//没有找到合适直线
	if (line_cnt == 0)
	{
		std::cout << "No useful line found!" << std::endl;
		return;
	}
	else
	{
		std::cout << "useful line found: " << line_cnt << std::endl;
		//求平均值
		angle /= line_cnt;
	}

	//旋转
	Point2f center(src.cols / 2, src.rows / 2);
	Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
	warpAffine(src, dst, rotMat, src.size(), 1, BORDER_REPLICATE);
	imshow("src", src);
	imshow("result",dst);
	waitKey(0);
	destroyAllWindows();

}

 
int main()
{

	// mixDemo(10, 10, 30, 30);
	// dftDemo();
	// rotateDemo();
	textDemo(150, 60);

}
 