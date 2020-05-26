#include "bgSub.h"

using namespace cv;


//计算高斯模型的均值图像和方差图像
void calcuGaussianBgModel(const std::vector<Mat> &frames, Mat &mean_mat, Mat &var_mat)
{
	int rows = frames[0].rows;
	int cols = frames[0].cols;
	
	mean_mat.create(rows, cols, CV_8UC1);
	var_mat.create(rows, cols, CV_32FC1);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			//像素各自累加计算历史均值
			int sum = 0;
			for (Mat frame : frames)
			{
				sum += frame.at<uchar>(i, j);
			}

			mean_mat.at<uchar>(i, j) = sum / frames.size();

			//像素各自累加计算历史方差
			float var = 0;
			for (Mat frame : frames)
			{
				//自动整形提升
				var += pow(frame.at<uchar>(i, j) - mean_mat.at<uchar>(i, j), 2);
			}

			var_mat.at<float>(i, j) = var / frames.size();
		}
	}

}

//利用高斯模型进行背景差分
void gaussianBgModelDiff(const Mat &mean_mat, const Mat &var_mat, const Mat &frame, Mat &sub_mat, float weight)
{
	absdiff(frame, mean_mat, sub_mat);

	//创建迭代器
	MatIterator_<uchar> it1, end1;
	MatConstIterator_<float> it2, end2;
	it1 = sub_mat.begin<uchar>();
	end1 = sub_mat.end<uchar>();
	it2 = var_mat.begin<float>();
	end2 = var_mat.end<float>();
	
	while (it1 != end1)
	{
		//th = weight * var
		int thresh = weight * (*it2);
		*it1 = *it1 >= thresh ? 255 : 0;
		++it1;++it2;
	}
}
