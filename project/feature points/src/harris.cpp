#include "featurePoints.h"
//Harris角点检测,具备光照不变、旋转不变性

using namespace std;
using namespace cv;

void cornerHarrisDetect(const Mat &img)
{
    Mat cornerStrength;

    //Harris角点检测
    //https://www.cnblogs.com/ronny/p/4009425.html
    //blockSize:领域窗口大小 ksize:梯度微分算子大小 k:影响角点响应值，越大，检测的角点越多 cornerStrength:浮点mat
    cornerHarris(img, cornerStrength, 3, 3, 0.05);

    //找到响应最大值和最小值
    double minVal, maxVal;
    minMaxLoc(cornerStrength, &minVal, &maxVal);

    //阈值筛选出角点
    double th = (maxVal - minVal)*0.7 + minVal;
    Mat bin;
    threshold(cornerStrength, bin, th, 255, THRESH_BINARY);
    //二值图的元素类型：CV_32F
    cout << "二值化后的元素类型：" << bin.type() << endl;
    bin.convertTo(bin, CV_8UC1);
    //成功转到CV_8UC1
    cout << "转换后的元素类型"<<bin.type() << endl;
    //利用泛型算法,理想为8个,实测17个，需要非极大值抑制 为啥时waitKey结束后才显示值？
    cout <<"角点个数：" << count(bin.begin<uchar>(), bin.end<uchar>(), 255);
}

void cornerHarrisDetectDilate(const Mat &img)
{
    Mat cornerStrength;

    //Harris角点检测
    //https://www.cnblogs.com/ronny/p/4009425.html
    //blockSize:领域窗口大小 ksize:梯度微分算子大小 k:影响角点响应值，越大，检测的角点越多 cornerStrength:浮点mat
    cornerHarris(img, cornerStrength, 3, 3, 0.05);

    //找到响应最大值和最小值
    double minVal, maxVal;
    minMaxLoc(cornerStrength, &minVal, &maxVal);

    //阈值筛选出角点
    double th = (maxVal - minVal)*0.7 + minVal;
    Mat bin;
    threshold(cornerStrength, bin, th, 255, THRESH_BINARY);
    bin.convertTo(bin, CV_8UC1);

    Mat dilateMat;
    //灰度图膨胀取窗口元素和结构元素值之和，Mat()全为0，则取窗口最大值
    //膨胀+compare就等价于了非极大值抑制
    dilate(cornerStrength, dilateMat, Mat());
    Mat eqMask;
    //膨胀前后不变值就是极大值
    compare(cornerStrength, dilateMat, eqMask, CMP_EQ);
    bitwise_and(bin, eqMask, bin);
    //正确地检查到了8个角点
    cout <<"角点个数：" << count(bin.begin<uchar>(), bin.end<uchar>(), 255);  
    imshow("bin", bin);
    waitKey(0);
}

void herrisDemo()
{
    Mat img = imread("../../../img/metaltemp.png", IMREAD_GRAYSCALE);
    imshow("img", img);
    cornerHarrisDetect(img);
    cornerHarrisDetectDilate(img);

    //Harris算法不具备尺度不变形
    resize(img, img, Size(500, 150));
    cornerHarrisDetectDilate(img);   

    destroyAllWindows();
}