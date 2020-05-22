#include "bgSubDemo.h"

using namespace cv;

//观察的位置
cv::Point vP;

//该demo验证并演示，视频中的像素灰度值变换是否呈高斯分布
int verifyGaussian()
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo("../../../video/vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	int cnt = 0;
	int bin_width = 3;
	int bin_heght = 50;
	float histgram[256] = {0};

	cv::Mat histMat;

	while (1) {

		Mat frame;
		Mat grayMat;
		capVideo >> frame;

		if (frame.empty()) {
			std::cout << "Unable to read frame!" << std::endl;
			return -1;
		}

		//第一帧选取像素
		if (cnt == 0) {
			Mat selectMat;
			frame.copyTo(selectMat);
			namedWindow("mouseCallback");
			imshow("mouseCallback", selectMat);
			setMouseCallback("mouseCallback", on_mouse, &selectMat);
			waitKey(0);
			destroyAllWindows();
		}

		cvtColor(frame,grayMat,COLOR_BGR2GRAY);

		//获得像素灰度值
		int index = grayMat.at<uchar>(vP.y,vP.x);
		//直方图相应的bin加1
		histgram[index]++;

		//绘制直方图
		drawHist(histMat, histgram, bin_width,bin_heght);

		drawMarker(frame, vP, Scalar(255, 255, 255));
		imshow("frame",frame);
		imshow("histMat",histMat);

		waitKey(30);
		cnt++;
	}

	return 0;
}


int bgSub_demo()
{

	//----------------------读取视频文件--------------------------
	VideoCapture capVideo("../../../video/vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	//使用几张图片进行背景建模
	int modelNum = 20;

	int cnt = 0;

	Mat bgModelMat;
	Mat frame, subMat, sub_bin;
	// Mat correct;
	while (1) {

		capVideo >> frame;

		if (frame.empty()) {
			std::cout << "Unable to read frame!" << std::endl;
			return -1;
		}

		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0)
		{
			frame.copyTo(bgModelMat);
		}
		else
		{
			absdiff(frame, bgModelMat, subMat);
			threshold(subMat, sub_bin, 50, 255, THRESH_BINARY);

			// frame.copyTo(correct, sub_bin);
			// addWeighted(bgModelMat, 0.95, frame, 0.05, 0, bgModelMat);

			imshow("sub", subMat);
			imshow("sub_bin", sub_bin);
			// imshow("bg", bgModelMat);
			waitKey(30);
		}

		++cnt;
		
	}

	return 0;
}


int bgSubGaussian_demo()
{

	//----------------------读取视频文件--------------------------
	VideoCapture capVideo("../../../video/vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	//使用几张图片进行背景建模
	const int modelNum = 20;

	int cnt = 0;

	Mat mean_mat, var_mat;
	Mat frame, sub_bin;
	std::vector<Mat> mat_save;
	// Mat correct;
	while (1) {

		capVideo >> frame;

		if (frame.empty()) {
			std::cout << "Unable to read frame!" << std::endl;
			return -1;
		}
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		
		if (cnt < modelNum)
		{
			mat_save.push_back(frame);
		}
		else if (cnt == modelNum)
		{
			calcuGaussianModle(mat_save, mean_mat, var_mat);
		}
		
		else
		{
			gaussianModelDiff(mean_mat, var_mat, frame, 2, sub_bin);
			imshow("sub_bin", sub_bin);
			waitKey(30);
		}

		++cnt;
		
	}

	return 0;
}

//计算高斯模型的均值图像和方差图像
void calcuGaussianModle(const std::vector<Mat> &mat_save, Mat &mean_mat, Mat &var_mat)
{
	int rows = mat_save[0].rows;
	int cols = mat_save[0].cols;
	
	//像素各自累加计算均值
	Mat mean(rows, cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (Mat frame : mat_save)
			{
				mean.at<float>(i, j) += frame.at<uchar>(i, j);
			}
		}
	}
	mean.mul(1.0 / mat_save.size());
	mean.convertTo(mean_mat, CV_8UC1);

	//像素各自计算方差
	Mat variance(rows, cols, CV_32FC1, Scalar(0));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (Mat frame : mat_save)
			{
				variance.at<float>(i, j) += pow(static_cast<double>(frame.at<uchar>(i, j)) - mean.at<float>(i, j), 2);
			}
		}
	}	
	variance.mul(1.0 / mat_save.size());
	variance.copyTo(var_mat);


}

//利用高斯模型进行背景差分
void gaussianModelDiff(const Mat &mean_mat, Mat &var_mat, const Mat &frame, float th, Mat &sub_bin)
{
	Mat sub_mat;
	absdiff(frame, mean_mat, sub_mat);

	sub_mat.copyTo(sub_bin);
	MatIterator_<uchar> it1, end1, it2, end2;
	it1 = sub_bin.begin<uchar>();
	it2 = var_mat.begin<uchar>();
	end1 = sub_bin.end<uchar>();
	end2 = var_mat.end<uchar>();
	while (it1 != end1)
	{
		int thresh = th * (*it2);
		*it1 = *it1 >= thresh ? 255 : 0;
		++it1;++it2;
	}
}


//鼠标响应函数
void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{

	Mat hh;
	hh = *(Mat*)userdata;
	switch (EVENT)
	{
	case EVENT_LBUTTONDOWN:
	{
		vP.x = x;
		vP.y = y;
		drawMarker(hh,vP,Scalar(255,255,255));
		//circle(hh, vP, 4, cvScalar(255, 255, 255), -1);
		imshow("mouseCallback", hh);
		return;
	}
	break;
	}

}

//绘制直方图,某个像素在时间维度上的累加值
int drawHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght)
{
	histMat.create(bin_heght, 256 * bin_width, CV_8UC3);

	histMat = Scalar(255, 255, 255);

	float maxVal = *std::max_element(srcHist, srcHist + 256);

	for (int i = 0; i < 256; i++) {
		Rect binRect;
		binRect.x = i*bin_width;
		float height_i = (float)bin_heght*srcHist[i] / maxVal;
		binRect.height = (int)height_i;
		binRect.y = bin_heght - binRect.height;
		binRect.width = bin_width;
		rectangle(histMat, binRect, CV_RGB(255, 0, 0), -1);
	}

	return 0;
}