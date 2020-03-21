#include<iostream>
#include<time.h>
#include<string.h>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 利用曼哈顿距离计算像素离255像素的最小距离，便于形态学处理
// 适用于十字，多次形态学运算的场景
// 腐蚀则要计算离0的最小距离

class DistanceMap
{
    public:
        // 离255的最短距离
        Mat dist;
        int total;
        int rows;
        int cols;
        
        // 构造函数
        DistanceMap(int rows, int cols)
        {
            this->total = rows * cols;

            dist.create(rows, cols, CV_32SC1);

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    // 初始化距离为最大值
                    dist.at<int>(i, j) = this->total;
                }
            }

            this->cols = cols;
            this->rows = rows;
        }  

        // 获取元素距离
        int getDist(int row, int col)
        {
            // 超出边界
            if (row < 0 || row >= rows || col < 0 || col >= cols)
            {
                return total;
            }
            else
            {
                return dist.at<int>(row, col);
            }
        } 

        // 设置距离
        void setDist(int row, int col, int value)
        {
            dist.at<int>(row, col) = value;
        }

        // 利用DP算法更新距离Map
        void updateMap(Mat src)
        {
            CV_Assert(src.type() == CV_8UC1 && src.rows == this->rows && src.cols == this->cols);

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    // 当前点为255，距离设为0
                    if (src.at<uchar>(i, j ) == 255)
                    {
                        setDist(i, j, 0);
                    }
                    else
                    {
                        // 考察左边和上边
                        int d0 = getDist(i - 1, j);
                        int d1 = getDist(i, j - 1);
                        setDist(i, j, min(d0, d1) + 1);
                    }
                    
                }
            }

            // 考察下边和右边
            for (int i = rows - 1; i >= 0 ; i--)
            {
                for (int j = cols - 1; j >= 0 ; j--)
                {
                    if (src.at<uchar>(i, j ) != 255)
                    {
                        int d0 = getDist(i + 1, j);
                        int d1 = getDist(i, j + 1);
                        // 还要考察当前点，避免距离覆盖
                        int cur = getDist(i, j);
                        setDist(i, j, min(min(d0, d1) + 1, cur));
                    }
                }
            }

        }
    
};

// 十字膨胀
void dilateCross(Mat &src, Mat dist,int kSize)
{
    int radius = kSize / 2; 
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (dist.at<int>(i, j ) <= radius)
            {
                src.at<uchar>(i, j) = 255;
            }
        }
    }
}

int main()
{
    Mat img = imread("img/circuit.JPG", IMREAD_GRAYSCALE);
    threshold(img, img, 100, 255, THRESH_BINARY_INV);

    DistanceMap map(img.rows, img.cols);
    map.updateMap(img);
    Mat dist = map.dist;

    Mat dst = img.clone();

    dilateCross(dst, dist, 3);     

    imshow("img", img);
    imshow("dist", dist);
    waitKey(0);   

}