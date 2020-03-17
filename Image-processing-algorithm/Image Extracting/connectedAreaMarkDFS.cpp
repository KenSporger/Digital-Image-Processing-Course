#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;


void seedFilling(Mat &src)
{
    // 标签容器，初始化为标记0
    vector<vector<int>> labels(src.rows, vector<int>(src.cols, 0));
    // 当前的种子标签
    int curLabel = 1;
    // 四连通位置偏移
    pair<int, int> offset[4] = {make_pair(0, 1), make_pair(1, 0), make_pair(-1, 0), make_pair(0, -1)};
    // 当前连通域中的单位域队列
    vector<pair<int, int>> tempList;

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            // 当前单位域已被标记或者属于背景区域, 则跳过
            if (labels[i][j] != 0 || src.at<uchar>(i, j) == 0)
            {
                continue;
            }
            // 当前单位域未标记并且属于前景区域, 用种子为其标记
            labels[i][j] = curLabel;
            // 加入单位域队列
            tempList.push_back(make_pair(i, j));

            // 遍历单位域队列
            for (int k = 0; k < tempList.size(); k++)
            {
                // 四连通范围内检查未标记的前景单位域
                for (int m = 0; m < 4; m++)
                {
                    int row = offset[m].first + tempList[k].first;
                    int col = offset[m].second + tempList[k].second;
                    // 防止坐标溢出图像边界 
                    row = (row < 0) ? 0: ((row >= src.rows) ? (src.rows - 1): row);
                    col = (col < 0) ? 0: ((col >= src.cols) ? (src.cols - 1): col);
                    // 邻近单位域未标记并且属于前景区域, 标记并加入队列
                    if (labels[row][col] == 0 && src.at<uchar>(row, col) == 255)
                    {
                        labels[row][col] = curLabel;
                        tempList.push_back(make_pair(row, col));                       
                    }
                }
            }
            // 一个完整连通域查找完毕，标签更新
            curLabel++;
            // 清空队列
            tempList.clear();
        }
    }
    return;
}




int main()
{

    Mat img = imread("img/qrcode2.jpg", IMREAD_GRAYSCALE);
    threshold(img, img, 0, 255, THRESH_OTSU);

    // 4.5s for 1000 times 
    seedFilling(img);
}