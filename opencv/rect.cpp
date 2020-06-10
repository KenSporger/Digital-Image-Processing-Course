#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{
    Rect r1(Point(10,10), Size(20, 20));
    Rect r2(Point(0,0), Size(20, 20));
    Rect r3;
    //矩形的与操作，相当于取重叠区域
    //[（10,10）(20,20)]
    r3 = r2 & r1;
    cout << "-------------1-------------" << endl;
    cout << r3.x << endl << r3.y << endl;
    cout << r3.width << endl << r3.height << endl;

    //空集
    //[(0,0),(0,0)]
    Rect r4(Point(40,40), Size(50, 50));
    cout << r4.empty() << endl;
    r4 &= r1;
    cout << "-------------2-------------" << endl;
    cout << r4.x << endl << r4.y << endl;
    cout << r4.width << endl << r4.height << endl;
    cout << r4.empty() << endl;
    //矩形与点运算，相当于平移
    //[(0,0),(20,20)] -> [(10,10),(30,30)]
    r3 = r2 + r1.tl();
    cout << "-------------3-------------" << endl;
    cout << r3.x << endl << r3.y << endl;
    cout << r3.width << endl << r3.height << endl;

    



}