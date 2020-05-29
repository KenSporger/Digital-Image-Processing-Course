#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;



//获取HOG直方图
//src: 灰度图像 
//hist: 已经分配好的数组 
//endSrcRow: src最大row, 剩余部分直接忽略
//endSrcCol: src最大col， 剩余部分直接忽略
//histDims: hist元素个数
//cellSize：cell的大小，正方形
//cellDims: 角度量化维数
void getHogHist(const Mat &src, float *hist, int endSrcRow, int endSrcCol, int histDims,int cellSize, int cellDims)
{
    //清零
    memset(hist, 0, sizeof(float) * histDims);

    Mat gx, gy, mag, angle;
    
    //索贝尔计算XY梯度，并转化到极坐标
    Sobel(src, gx, CV_32F, 1, 0, 1);
    Sobel(src, gy, CV_32F, 0, 1, 1);
    //mag: 模长(梯度强度)
    //angle： 角度0~360
    //true为角度值，false为弧度制
    cartToPolar(gx, gy, mag, angle, true);

    int stepAngle = 360 / cellDims;

    int index = 0;
    //将每个cell的直方图拼接成一个大直方图
    for (int i = 0; i < endSrcRow; i += cellSize)
    {
        for (int j = 0; j < endSrcCol; j += cellSize)
        {
            //cell的终止row和终止col
            int endCellRow = i + cellSize;
            int endCellCol = j + cellSize;

            for (int k = i; k < endCellRow; k++)
            {
                for (int m = j; m < endCellCol; m++)
                {
                    //根据角度所在量化区域，将梯度强度叠加到直方图上
                    int dim = angle.at<float>(k ,m) / stepAngle;
                    hist[index + dim] = mag.at<float>(k ,m);
                }
                
            }

            index += cellDims;
        }
    }

}


//直方图之间的欧几里得距离
void calcuDistance(float *arrayA, float *arrayB, double &dist, int length)
{
    double sum = 0;
    
    for (int i = 0; i < length; i++)
    {
        sum += (arrayA[i] -arrayB[i]) * (arrayA[i] -arrayB[i]);
    }

    dist = sqrt(sum);
}


int main()
{
    Mat src = imread("img/metal.png");
    //读取模板
    Mat temp = imread("img/metaltemp.png");

    const int cellSize = 16;
    const int cellDims = 8;
    const int tempW = temp.cols;
    const int tempH = temp.rows;
    const int srcW = src.cols;
    const int srcH = src.rows;\

    const int histDims;

    float *tempHist = new float[histDims];
    
    Point oldP(0, 0);
    Point newP(0, 0);

    for (int i = )





}