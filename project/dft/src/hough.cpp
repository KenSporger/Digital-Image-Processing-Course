#include "hough.h"

using namespace cv;

//绘制霍夫变换直线
void drawHoughLines(const Mat &lines, Mat &dst, Scalar color, int thick)
{
    Point pt1, pt2;
    //画布
    
    for (int i = 0; i < lines.rows; ++i)
    {
        for (int j = 0; j < lines.cols; ++j)
        {
            //(r,theta)为过原点与直线垂直直线之间的交点的极坐标
            //原点为图像左上角点,X轴正方向为右，Y轴正方向向下
            //theta范围0-PI,是垂足点和原点连线落在Y轴正平面部分与X轴正方向的夹角
            float rho = lines.at<Vec2f>(i, j)[0];
            float theta = lines.at<Vec2f>(i, j)[1];
            double a = cos(theta);
            double b = sin(theta);
            double x0 = rho * a;
            double y0 = rho * b;
            //找出直线上两点        
            pt1.x = saturate_cast<int>(x0 - 1000 * b);
            pt1.y = saturate_cast<int>(y0 +  1000 * a);
            pt2.x = saturate_cast<int>(x0 + 1000 * b);
            pt2.y = saturate_cast<int>(y0 - 1000 * a);

            line(dst, pt1, pt2, color, thick);
        }
    }

}