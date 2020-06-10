#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//+重载，旋转矩阵平移
template<typename T>
RotatedRect operator+(const RotatedRect &rec, const Point_<T> &p)
{
	return RotatedRect(Point2f(rec.center.x + p.x, rec.center.y + p.y), rec.size, rec.angle);
}

//旋转矩阵绘制
void drawRotateRect(Mat &img, const RotatedRect &rec, Scalar color)
{
	Point2f vtx[4];
	rec.points(vtx);
	for (int j = 0; j < 4; j++)
	{
		line(img, vtx[j], vtx[(j+1) % 4], color, 4);
	}
}

int main()
{
    Mat dst(300, 300, CV_8UC3, Scalar::all(255));
    float angle = 90;
    //center,Size(短轴width，长轴height),短轴与X轴夹角
    //短轴<长轴
    RotatedRect r1(Point2f(150, 150), Size(50, 100), angle);
    drawRotateRect(dst, r1, Scalar(255, 0, 0));
    //短轴>长轴
    RotatedRect r2(Point2f(150, 150), Size(100, 50), angle);
    drawRotateRect(dst, r2, Scalar(0, 255, 0));
    //45度
    angle = 45;
    RotatedRect r3(Point2f(150, 150), Size(50, 100), angle);
    drawRotateRect(dst, r3, Scalar(0, 0, 255));
    //45度
    angle = 135;
    RotatedRect r4(Point2f(150, 150), Size(50, 100), angle);
    drawRotateRect(dst, r4, Scalar(255, 0, 255));

    //矩形平移
    RotatedRect r5 = r1 + Point(50, 50);
    drawRotateRect(dst, r5, Scalar(0, 255, 255));
    imshow("dst", dst);
    waitKey(0);


}