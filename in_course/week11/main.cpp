#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include "functions.h"

using namespace cv;
using namespace std;

int main()
{
	//开始计时
	double start = static_cast<double>(cvGetTickCount());

	//dftDemo();
	//mouseROI();
	ifftDemo();
	//结束计时,这里的cvGetTickFrequency以us为单位
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//显示时间，us除1000转ms
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//等待键盘响应，按任意键结束程序
	system("pause");
    return 0;
}