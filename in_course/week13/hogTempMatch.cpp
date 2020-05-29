#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int cellSize = 16;
const int cellDims = 8;

//获取HOG直方图
//src: 灰度图像 
//hist: 已经分配好的数组 
//histDims: hist元素个数
//cellSize：cell的大小，正方形
//cellDims: 角度量化维数
void getHogHist(const Mat &src, float *hist, int histDims ,int cellSize, int cellDims)
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
    for (int i = 0; i < src.rows; i += cellSize)
    {
        for (int j = 0; j < src.cols; j += cellSize)
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


void hogTempMatch(const Mat &src, const Mat &temp, Rect &rec)
{
       //截取有效的模板区域
    Mat tempValide = temp(Rect(0 ,0, (temp.cols / cellSize) * cellSize, (temp.rows / cellSize) * cellSize));

    //维度数
    const int histDims = (tempValide.cols / cellSize) * (tempValide.rows / cellSize) * cellDims;

    float *tempHist = new float[histDims];
    getHogHist(tempValide, tempHist, histDims, cellSize, cellDims);

    const int maxCol = src.cols - tempValide.cols;
    const int maxRow = src.rows - tempValide.rows;

    Mat srcRoi;
    float *roiHist = new float[histDims];
    Point minLoc(0, 0);
    double minVal = DBL_MAX;
    double curVal;

    for (int i = 0; i < maxCol; i++)
    {
        for (int j = 0; j < maxRow; j++)
        {
            srcRoi = src(Rect(i , j, tempValide.cols, tempValide.rows));
            getHogHist(srcRoi, roiHist, histDims, cellSize, cellDims);
            calcuDistance(roiHist, tempHist, curVal, histDims);
            if (curVal < minVal)
            {
                minVal = curVal;
                minLoc.x = i;
                minLoc.y = j;
            }
        }
    }

    rec = Rect(minLoc.x, minLoc.y, tempValide.cols, tempValide.rows);

}


void hogDemo()
{
    Mat src = imread("img/metal.png", IMREAD_GRAYSCALE);
    //读取模板
    Mat temp = imread("img/metaltemp.png", IMREAD_GRAYSCALE);

    //截取有效的模板区域
    Mat tempValide = temp(Rect(0 ,0, (temp.cols / cellSize) * cellSize, (temp.rows / cellSize) * cellSize));

    Rect roi;
    hogTempMatch(src, tempValide, roi);

    Mat dst(src);
    rectangle(dst, roi, Scalar(255, 0, 0));

    imshow("dst", dst);
    waitKey(0);
    destroyAllWindows();
}

void pyramidHogDemo(int scale)
{
    Mat src = imread("img/metal.png", IMREAD_GRAYSCALE);
    //读取模板
    Mat temp = imread("img/metaltemp.png", IMREAD_GRAYSCALE);

    Mat tempDown, srcDown;
    resize(temp, tempDown, Size(temp.cols / scale, temp.rows / scale));
    // pyrUp(tempTop, temp);
    resize(src, srcDown, Size(src.cols / scale, src.rows / scale));
    
    //截取有效的模板区域
    Mat tempValide = tempDown(Rect(0 ,0, (tempDown.cols / cellSize) * cellSize, (tempDown.rows / cellSize) * cellSize));

    Rect roi;
    hogTempMatch(srcDown, tempValide, roi);

    tempValide = temp(Rect(0 ,0, (temp.cols / cellSize) * cellSize, (temp.rows / cellSize) * cellSize));
    
    roi.x *= scale;
    roi.y *= scale;
    roi.width = tempValide.cols * 2;
    roi.height = tempValide.rows * 2;
    Mat srcRect = src(roi);

    Rect roi2;
    hogTempMatch(srcRect, tempValide, roi2);

    roi2.x += roi.x;
    roi2.y += roi.y;

    Mat dst(src);
    rectangle(dst, roi2, Scalar(255, 0, 0));


    imshow("dst", dst);
    waitKey(0);
    destroyAllWindows();

}



int main()
{
    // hogDemo();
    pyramidHogDemo(4);

}