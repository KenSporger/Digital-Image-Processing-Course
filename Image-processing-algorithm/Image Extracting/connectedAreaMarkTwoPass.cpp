#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

// 查找每一行的子区域
// numberOfArea：子区域总数 stArea：子区域开始位置  enArea：子区域结束位置  rowArea：子区域所在行号
void searchArea(const Mat src, int &numberOfArea, vector<int> &stArea, vector<int> &enArea, vector<int> &rowArea)
{
    for (int row = 0; row < src.rows; row++)
    {
        // 行指针
        const uchar *rowData = src.ptr<uchar>(row);
        // 判断行首是否是子区域的开始点
        if (rowData[0] == 255){
            numberOfArea++;
            stArea.push_back(0);
        }
        
        for (int col = 1; col < src.cols; col++)
        {
            // 子区域开始位置的判断：前像素为背景，当前像素是前景
            if (rowData[col - 1] == 0 && rowData[col] == 255)
            {
                numberOfArea++;
                stArea.push_back(col);   
            // 子区域结束位置的判断：前像素是前景，当前像素是背景             
            }else if (rowData[col - 1] == 255 && rowData[col] == 0)
            {
                // 更新结束位置vector、行号vector
                enArea.push_back(col - 1);
                rowArea.push_back(row);
            }
        }
        // 结束位置在行末
        if (rowData[src.cols - 1] == 255)
        {
            enArea.push_back(src.cols - 1);
            rowArea.push_back(row);
        }
    }
}

// 初步标签,获取等价对
// labelOfArea:子区域标签值， equalLabels：等价标签对 offset:0为四连通,1为8连通
void markArea(int numberOfArea, vector<int> stArea, vector<int> enArea, vector<int> rowArea, vector<int> &labelOfArea, vector<pair<int, int>> &equalLabels, int offset)
{
    int label = 1;
    // 当前所在行   
    int curRow = 0;
    // 当前行的第一个子区域位置索引
    int firstAreaCur = 0;
    // 前一行的第一个子区域位置索引
    int firstAreaPrev = 0;
    // 前一行的最后一个子区域位置索引
    int lastAreaPrev = 0;

    // 初始化标签都为0
    labelOfArea.assign(numberOfArea, 0);
    
    // 遍历所有子区域并标记
    for (int i = 0; i < numberOfArea; i++)
    {
        // 行切换时更新状态变量
        if (curRow != rowArea[i])
        {
            curRow = rowArea[i];
            firstAreaPrev = firstAreaCur;
            lastAreaPrev = i - 1;
            firstAreaCur = i;
        }
        // 相邻行不存在子区域
        if (curRow != rowArea[firstAreaPrev] + 1)
        {
            labelOfArea[i] = label++;
            continue;
        }
        // 对前一行进行迭代
        for (int j = firstAreaPrev; j <= lastAreaPrev; j++)
        {
            // 判断是否相连
            if (stArea[i] <= enArea[j] + offset && enArea[i] >= stArea[j] - offset)
            {
                // 之前没有标记过
                if (labelOfArea[i] == 0)
                    labelOfArea[i] = labelOfArea[j];
                // 之前已经被标记，保存等价对
                else if (labelOfArea[i] != labelOfArea[j])
                    equalLabels.push_back(make_pair(labelOfArea[i], labelOfArea[j]));
            }else if (enArea[i] < stArea[j] - offset)
            {
                // 为当前行下一个子区域缩小上一行的迭代范围
                firstAreaPrev = max(firstAreaPrev, j - 1);
                break;
            }
        }
        // 与上一行不存在相连
        if (labelOfArea[i] == 0)
        {
            labelOfArea[i] = label++;
        }
    }
}

// 等价对处理，标签重映射
void replaceEqualMark(vector<int> &labelOfArea, vector<pair<int, int>> equalLabels)
{
    int maxLabel = *max_element(labelOfArea.begin(), labelOfArea.end());

    // 等价标签矩阵，值为true表示这两个标签等价
    vector<vector<bool>> eqTab(maxLabel, vector<bool>(maxLabel, false));
    // 将等价对信息转移到矩阵上
    vector<pair<int, int>>::iterator labPair;
    for (labPair = equalLabels.begin(); labPair != equalLabels.end(); labPair++)
    {
        eqTab[labPair->first -1][labPair->second -1] = true;
        eqTab[labPair->second -1][labPair->first -1] = true;
    }
    // 标签映射
    vector<int> labelMap(maxLabel + 1, 0);
    // 等价标签队列
    vector<int> tempList;
    // 当前使用的标签
    int curLabel = 1;

    for (int i = 1; i <= maxLabel; i++)
    {
        // 如果该标签已被映射，直接跳过
        if (labelMap[i] != 0)
        {
            continue;
        }

        labelMap[i] = curLabel;
        tempList.push_back(i);

        for (int j = 0; j < tempList.size(); j++)
        {
            // 在所有标签中寻找与当前标签等价的标签 
            for (int k = 1; k <= maxLabel; k++)
            {
                // 等价且未访问
                if (eqTab[tempList[j] - 1][k - 1] && labelMap[k] == 0)
                {
                    labelMap[k] = curLabel;
                    tempList.push_back(k);
                }
            }
        }

        curLabel++;
        tempList.clear();
    }

    // 根据映射修改标签
    vector<int>::iterator label;
    for (label = labelOfArea.begin(); label != labelOfArea.end(); label++)
    {
        *label = labelMap[*label];
    }

    return;
}

// label with different colors
void paintArea(Mat src, Mat &dst, vector<int> stArea, vector<int> enArea, vector<int> rowArea, vector<int> labelOfArea, int numberOfArea)
{
    Mat img(src.rows, src.cols, CV_8UC3, CV_RGB(0, 0, 0));
    cvtColor(img, dst, CV_RGB2HSV);
    
    int maxLabel = *max_element(labelOfArea.begin(), labelOfArea.end());
    vector<uchar> hue;
    for (int i = 1; i<= maxLabel; i++)
    {
        // HSV color-mode 
        hue.push_back(uchar(180.0 * (i - 1) / (maxLabel + 1)));
    }

    for (int i = 0; i < numberOfArea; i++)
    {
        for (int j = stArea[i]; j <= enArea[i]; j++)
        {
            dst.at<Vec3b>(rowArea[i], j)[0] = hue[labelOfArea[i]];
            dst.at<Vec3b>(rowArea[i], j)[1] = 255;
            dst.at<Vec3b>(rowArea[i], j)[2] = 255;
        }
    }

    cvtColor(dst, dst, CV_HSV2BGR);
} 


int main()
{

    Mat img = imread("img/qrcode.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    threshold(img, img, 0, 255, THRESH_OTSU);
    //7s for 1000 times, not including painting
    int numberOfArea=0;
    vector<int> stArea;
    vector<int> enArea;
    vector<int> rowArea;
    vector<int> AreaLabels;
    vector<pair<int, int>> equalLabels;
    int offset=0;

    searchArea(img, numberOfArea, stArea, enArea, rowArea);
    markArea(numberOfArea, stArea, enArea, rowArea, AreaLabels, equalLabels, offset);

    replaceEqualMark(AreaLabels, equalLabels);
    paintArea(img, dst, stArea, enArea, rowArea, AreaLabels, numberOfArea);

    imshow("dst", dst);
    waitKey(0);

}