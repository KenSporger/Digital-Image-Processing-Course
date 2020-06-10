#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

//模板函数，将相同操作应用于不同类型

template<typename T>
Point_<T> pointScale(Point_<T> pt1, float den)
{
    pt1.x /= den;
    pt1.y /= den;
    return pt1;
}

int main()
{
    Point2f pt1, pt2;
    pt1.x = 100;
    pt1.y = 100;
    //自动识别类型
    pt2 = pointScale(pt1, 2.2);
    cout << pt2 << endl;
    //显示
    pt2 = pointScale<int>(pt1, 2.2);
    cout << pt2 << endl;
    return 0;
}