#include "roi.h"

using namespace std;
using namespace cv;

 
int main()
{
	Mat img = imread("../../../img/lena.jpg");
	Mat mask;
	
	selectPolygon(img, mask);
	imshow("mask",mask);
	waitKey(0);

}