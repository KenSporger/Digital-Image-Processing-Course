#include "bgSubDemo.h"

using namespace cv;
using namespace std;

int main()
{
	//开始计时
	double start = static_cast<double>(cvGetTickCount());

	//该demo验证并演示，视频中的像素灰度值变换是否呈高斯分布
	// verifyGaussian();
    // bgSub_demo();
    bgSubGaussian_demo();

	//结束计时
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//显示时间
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//等待键盘响应，按任意键结束程序
	system("pause");
    return 0;
}