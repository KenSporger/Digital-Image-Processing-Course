// dip-class-demos.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include "functions.h"

using namespace cv;
using namespace std;

int main()
{
	//开始计时
	double start = static_cast<double>(getTickCount());
	//dftDemo();
	//mouseROI();
	ifftDemo();
	//结束计时
	double time = ((double)getTickCount() - start) / getTickCount();
	//显示时间
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//等待键盘响应，按任意键结束程序
	system("pause");
    return 0;
}