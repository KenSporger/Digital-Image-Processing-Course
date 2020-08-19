#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

#include <stdio.h>

using namespace cv;


enum { STEREO_BM = 0, STEREO_SGBM = 1, STEREO_HH = 2, STEREO_VAR = 3, STEREO_3WAY = 4 };
 
void calDispWithSGBM(Mat imgL, Mat imgR, Mat &imgDisparity8U)
{
	cv::Size imgSize = imgL.size();
	int numberOfDisparities = ((imgSize.width / 8) + 15) & -16;
    /*
    * minDisparity : 最小可能的视差值
    * numDisparities ： 最大可能的视差值-最小可能的视差值
    * blockSize : 匹配的块大小(SGM原算法是单像素匹配，blockSize=1)
    * Birchfield-Tomasi sub-pixel metric 替代 Mutual information cost function 
    * P1：视差平滑度系数
    * P2：视差平滑度系数，P2>P1
    * disp12MaxDiff:左右视差一致性检查的最大差异值（非正值禁用检查）
    * preFilterCap：预处理滤波器的截断值，主要用于Birchfield-Tomasi
    * uniquenessRatio ： 最小代价超越次佳值的百分比
    * speckleWindowSize ： 散斑过滤相关
    * speckleRange ：散斑过滤相关
    * mode ：:MODE_HH 双通道动态编程算法
    */
	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 16, 3);
 
	sgbm->setPreFilterCap(63);
 
	int SADWindowSize = 9;
	int sgbmWinSize = SADWindowSize > 0 ? SADWindowSize : 3;
	sgbm->setBlockSize(sgbmWinSize);
 
	int cn = imgL.channels();
	sgbm->setP1(8 * cn*sgbmWinSize*sgbmWinSize);
	sgbm->setP2(32 * cn*sgbmWinSize*sgbmWinSize);
 
	sgbm->setMinDisparity(0);
	sgbm->setNumDisparities(numberOfDisparities);
	sgbm->setUniquenessRatio(10);
	sgbm->setSpeckleWindowSize(100);
	sgbm->setSpeckleRange(32);
	sgbm->setDisp12MaxDiff(1);
 
	int alg = STEREO_SGBM;
	if (alg == STEREO_HH)
		sgbm->setMode(cv::StereoSGBM::MODE_HH);
	else if (alg == STEREO_SGBM)
		sgbm->setMode(cv::StereoSGBM::MODE_SGBM);
	else if (alg == STEREO_3WAY)
		sgbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);
 
	Mat imgDisparity16S = Mat(imgL.rows, imgL.cols, CV_16S);
	sgbm->compute(imgL, imgR, imgDisparity16S);
 
	//--Display it as a CV_8UC1 image：16位有符号转为8位无符号
	imgDisparity16S.convertTo(imgDisparity8U, CV_8U, 255 / (numberOfDisparities*16.));
}
 
int main()
{
	//--读取图像
	Mat imgL = imread("../../../img/ConesL.png", 0);    
	Mat imgR = imread("../../../img/ConesR.png", 0);
 
	//--And create the image in which we will save our disparities
	Mat imgDisparity8U = Mat(imgL.rows, imgL.cols, CV_8UC1);
 
	//calDispWithBM(imgL, imgR, imgDisparity8U);
	calDispWithSGBM(imgL, imgR, imgDisparity8U);
    imshow("imgL", imgL);
    imshow("imgR", imgR);
    imshow("Disparity", imgDisparity8U);
    waitKey(0);
}

