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
void getHogHist(Mat src, float *hist, int endSrcRow, int endSrcCol, int histDims,int cellSize, int cellDims)
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
    //参考图像
    Mat imgA = imread("img/hogTemplate.jpg", IMREAD_GRAYSCALE);
    //类似图像
    Mat imgB = imread("img/img1.jpg", IMREAD_GRAYSCALE);
    //无关图像
    Mat imgC = imread("img/img2.jpg", IMREAD_GRAYSCALE);
    //imgA的旋转图像
    Mat imgD;

    Point2f center(imgA.cols / 2, imgA.rows / 2);
    const Mat affineM = getRotationMatrix2D(center, 10, 1.0);
    warpAffine(imgA, imgD, affineM, imgA.size());


    int cellSize = 16;
    int cellDims = 8;
    int cellCol = imgA.cols / cellSize;
    int cellRow = imgA.rows / cellSize;
    int endSrcCol = cellCol * cellSize;
    int endSrcRow = cellRow * cellSize;
    int histDims = cellRow * cellCol * cellDims;

    float *histA = new float[histDims];
    float *histB = new float[histDims];
    float *histC = new float[histDims];
    float *histD = new float[histDims];

    getHogHist(imgA, histA, endSrcRow, endSrcCol, histDims, cellSize, cellDims);
    getHogHist(imgB, histB, endSrcRow, endSrcCol, histDims, cellSize, cellDims);
    getHogHist(imgC, histC, endSrcRow, endSrcCol, histDims, cellSize, cellDims);
    getHogHist(imgD, histD, endSrcRow, endSrcCol, histDims, cellSize, cellDims);

    double distBtoA, distCtoA, distDtoA;
    calcuDistance(histA, histB, distBtoA, histDims);
    calcuDistance(histA, histC, distCtoA, histDims);
    //HOG不具备旋转不变性
    calcuDistance(histA, histD, distDtoA, histDims);

    cout << "distance img1 to temp: " << distBtoA << endl;
    cout << "distance img2 to temp: " << distCtoA << endl;
    cout << "distance temp-rotated to temp: " << distDtoA << endl;

    delete[] histA, histB, histC, histD;

    return 0;
}