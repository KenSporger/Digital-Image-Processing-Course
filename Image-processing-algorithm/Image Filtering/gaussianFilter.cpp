#include<iostream>
#include<math.h>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;


double gaussianCalcu(double x, double sigma = 1.0)
{
    return exp( -(x * x) / (2 * sigma *sigma) );
}

// 根据核大小计算方差 
double getStandardSigma(int length)
{
    return 0.3 * ( (length - 1) * 0.5 - 1) + 0.8;
}

// 获取一维卷积核
void gaussianKernel1D(double kernel[],int length, double sigma = 1.0)
{
    int center = length / 2; 
    double sum = 0; 
    
    for (int i = 0; i < length; i++){
        kernel[i] = gaussianCalcu(center - i, sigma);
        sum += kernel[i];
    }
    //归一化卷积核
    for (int i = 0; i < length; i++) kernel[i] /= sum;

    return;
}

// 支持单通道和三通道图像
void separateGaussianFilter(Mat src, Mat dst, int ksize, double sigma)
{
    CV_Assert(src.channels()==1 || src.channels() == 3);
    // 动态分配
    double *kernel = new double[ksize];
    // 获取一维卷积核
    gaussianKernel1D(kernel,ksize,sigma);

    
    Mat srcEtd;
    src.copyTo(srcEtd);
    int border = ksize / 2; 
    // 扩展边界
    copyMakeBorder(src, srcEtd, border, border, border, border, BORDER_REPLICATE);
    int channels = srcEtd.channels();
    int rows = srcEtd.rows - border;
    int cols = srcEtd.cols - border;
    
    
    // 水平方向卷积
    for (int i = border; i < rows; i++){
        for (int j = border; j < cols; j++){
            
            double sum[3] = {0};
            // convolusion kernel:0~2*border , srcEtd: center-border ~ center+border
            for (int k = -border; k <= border; k++){
                if (channels == 1){
                    sum[0] += kernel[border + k] * srcEtd.at<uchar>(i, j + k);
                }else if (channels == 3){
                    sum[0] += kernel[border + k] * srcEtd.at<Vec3b>(i, j + k)[0];
                    sum[1] += kernel[border + k] * srcEtd.at<Vec3b>(i, j + k)[1];
                    sum[2] += kernel[border + k] * srcEtd.at<Vec3b>(i, j + k)[2];
                }
            }
            // 卷积结果替换核中心，并且滤波时不建立副本，直接在原图上操作
            if (channels == 1)
                srcEtd.at<uchar>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3)
                srcEtd.at<Vec3b>(i , j) = {static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2])};

        }
    }
    // 垂直方向卷积
    for (int i = border; i < rows; i++){
        for (int j = border; j < cols; j++){
            
            double sum[3] = {0};

            for (int k = -border; k <= border; k++){
                if (channels == 1){
                    sum[0] += kernel[border + k] * srcEtd.at<uchar>(i + k, j);
                }else if (channels == 3){
                    sum[0] += kernel[border + k] * srcEtd.at<Vec3b>(i + k, j)[0];
                    sum[1] += kernel[border + k] * srcEtd.at<Vec3b>(i + k, j)[1];
                    sum[2] += kernel[border + k] * srcEtd.at<Vec3b>(i + k, j)[2];
                }
            }

            if (channels == 1)
                srcEtd.at<uchar>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3)
                srcEtd.at<Vec3b>(i , j) = {static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2])};
                
        }
    }

    for (int i = border; i < rows; i++)
        for (int j = border; j < cols; j++)
            dst.at<Vec3b>(i - border, j - border) = srcEtd.at<Vec3b>(i,j);
    //收回动态分配
    delete [] kernel;
}


int main()
{
    Mat img = imread("img/numbers.jpg");
    Mat dst(img.rows, img.cols, CV_8UC3);
    double sigma = getStandardSigma(15);

    imshow("img",img);
    separateGaussianFilter(img,dst,15,sigma);
    imshow("dst1",dst);
    GaussianBlur(img,dst,Size(15,15),0,0);
    imshow("dst2",dst);
    waitKey(0);

}