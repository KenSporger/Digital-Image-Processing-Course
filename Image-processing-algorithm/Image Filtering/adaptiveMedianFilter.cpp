#include<iostream>
#include<vector>
#include<algorithm>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//加入随机的椒盐噪声 
void addImpulseNoise(Mat dst, int n)
{
    CV_Assert(dst.type() == CV_8UC3 || dst.type() == CV_8UC1);

    for (int k = 0; k < n; k++){

        int i = rand() % dst.rows;
        int j = rand() % dst.cols;
        int value = (rand() % 2) * 255;

        if (dst.type() == CV_8UC1){
            dst.at<uchar>(i, j) = value;
        }
        else if (dst.type() == CV_8UC3){
            dst.at<Vec3b>(i, j)[0] = value;
            dst.at<Vec3b>(i, j)[1] = value;
            dst.at<Vec3b>(i, j)[2] = value;
        }
    }

    return;
}

//查找中值
uchar adaptiveProcess(Mat &src, int row, int col, int kSize, int maxSize)
{
    int radius = kSize / 2;
    vector<uchar> pixels;
    //加入窗口元素
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
            pixels.push_back( src.at<uchar>(i + row, j + col) );
    
    //排序
    sort(pixels.begin(), pixels.end());

    uchar minVal = pixels[0];
    uchar maxVal = pixels[pixels.size() - 1];
    uchar midVal = pixels[pixels.size() / 2];
    uchar curVal = src.at<uchar>(row, col);

    //中值属于非噪声
    if (midVal > 0 && midVal < 255){
        if (curVal > 0 && curVal < 255)
            //锚点不是噪声
            return curVal;
        else 
            //锚点是噪声
            return midVal;
    }else{
        //中值是噪声
        kSize += 2;
        if (kSize <= maxSize)
            //增大窗口尺寸
            return adaptiveProcess(src, row, col, kSize, maxSize);
        else
            return midVal;
    }
}

// 自适应中值滤波
void adaptiveMedianFilter(Mat src, Mat &dst, int maxSize)
{
    CV_Assert(src.type() == CV_8UC1);
    src.copyTo(dst);
    Mat srcEtd;
    int border = maxSize / 2;
    //边界扩展
    copyMakeBorder(src, srcEtd, border, border, border, border, BORDER_REPLICATE);

    int rows = srcEtd.rows - border;
    int cols = srcEtd.cols - border;

    for (int i = border; i < rows; i++)
        for (int j = border; j < cols; j++)
            srcEtd.at<uchar>(i, j) = adaptiveProcess(srcEtd, i, j, 3, maxSize);
    
    // 输出除掉边界的有效区域
    for (int i = border; i < rows; i++)
        for (int j = border; j < cols; j++)
            dst.at<uchar>(i - border, j - border) = srcEtd.at<uchar>(i, j);

}

int main()
{
    Mat img = imread("img/numbers.jpg", IMREAD_GRAYSCALE);
    Mat dst1,dst2;

    addImpulseNoise(img, 200000);
    // 普通的中值滤波会模糊边缘
    medianBlur(img, dst1, 7);
    // 保护边缘但仅适用于0和255的椒盐噪声
    adaptiveMedianFilter(img, dst2, 7);

    imshow("img", img);
    imshow("dst1", dst1);
    imshow("dst2", dst2);

    waitKey(0);


    

}