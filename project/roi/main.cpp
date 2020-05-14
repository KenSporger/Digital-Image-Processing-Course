#include "roi.h"

using namespace std;
using namespace cv;


//鼠标响应函数
void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{

	Mat hh;
	hh = *(Mat*)userdata;
	Point p(x, y);
	switch (EVENT)
	{
	case EVENT_LBUTTONDOWN:
	{
		// points.x = x;
		// points.y = y;
		// mousePoints.push_back(points);
		circle(hh, p, 4, cvScalar(255, 255, 255), -1);
		imshow("img", hh);
	}
	break;
	}

}
 
int main()
{
	Mat img = imread("/home/ken/Documents/Digital-Image-Processing-Course/img/lena.jpg");
	ROI myroi(img);
	Mat mask;

	myroi.selectPolygon(mask);

	imshow("mask",mask);
	waitKey(0);

}