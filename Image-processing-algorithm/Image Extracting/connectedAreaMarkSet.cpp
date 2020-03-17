#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

class AreaMark
{
    public:
        AreaMark(const Mat src,int offset);
        int getMarkedArea(vector<vector<int>> &area); 
        void getMarkedImage(Mat &dst);

    private:
        Mat src; 
        int offset;
        int numberOfArea=0;
        vector<int> labelMap;
        vector<int> labelRank;
        vector<int> stArea; 
        vector<int> enArea;
        vector<int> rowArea;
        vector<int> labelOfArea;
        vector<pair<int, int>> equalLabels;
        
        void markArea();
        void searchArea();
        void setInit(int n);
        int findRoot(int label);
        void unite(int labelA, int labelB);
        void replaceEqualMark();
};

// 构造函数
// imageInput:输入待标记二值图像    offsetInput:0为四连通，1为八连通
AreaMark::AreaMark(Mat imageInput,int offsetInput)
{
    src = imageInput;
    offset = offsetInput;
}

// 使用可区分的颜色标记连通域
void AreaMark::getMarkedImage(Mat &dst)
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

// 获取标记过的各行子区域
int AreaMark::getMarkedArea(vector<vector<int>> &area)
{
    searchArea();
    markArea();
    replaceEqualMark();
    area.push_back(rowArea);
    area.push_back(stArea);
    area.push_back(enArea);
    area.push_back(labelOfArea);
    return numberOfArea;
}

// 查找每一行的子区域
// numberOfArea：子区域总数 stArea：子区域开始位置  enArea：子区域结束位置  rowArea：子区域所在行号
void AreaMark::searchArea()
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



void AreaMark::markArea()
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

//集合初始化
void AreaMark::setInit(int n)
{
    for (int i = 0; i <= n; i++)
    {
        labelMap.push_back(i);
        labelRank.push_back(0);
    }
}

//查找树根
int AreaMark::findRoot(int label)
{
    if (labelMap[label] == label)
    {
        return label;
    }
    else
    {
        // path compression
        return labelMap[label] = findRoot(labelMap[label]);
    }
}

// 合并集合
void AreaMark::unite(int labelA, int labelB)
{
    labelA = findRoot(labelA);
    labelB = findRoot(labelB);

    if (labelA == labelB)
    {
        return;
    }
    // rank optimization:tree with high rank merge tree with low rank
    if (labelRank[labelA] < labelRank[labelB])
    {
        labelMap[labelA] = labelB;
    }
    else
    {
        labelMap[labelB] = labelA;
        if (labelRank[labelA] == labelRank[labelB])
        {
            labelRank[labelA]++;
        }
    }
    
}

// 等价对处理，标签重映射
void AreaMark::replaceEqualMark()
{
    int maxLabel = *max_element(labelOfArea.begin(), labelOfArea.end());
    
    setInit(maxLabel);
    
    // 合并等价对，标签初映射
    vector<pair<int, int>>::iterator labPair;
    for (labPair = equalLabels.begin(); labPair != equalLabels.end(); labPair++)
    {
        unite(labPair->first, labPair->second);
    }

    // 标签重映射，填补缺失标签
    int newLabel=0;
    vector<int> labelReMap(maxLabel + 1, 0);
    vector<int>::iterator old;
    for (old = labelMap.begin(); old != labelMap.end(); old++)
    {
        if (labelReMap[findRoot(*old)] == 0)
        {
            labelReMap[findRoot(*old)] = newLabel++;
        }
    }
    
    // 根据重映射结果修改标签
    vector<int>::iterator label;
    for (label = labelOfArea.begin(); label != labelOfArea.end(); label++)
    {
        *label = labelReMap[findRoot(*label)];
    }  

}


int main()
{

    Mat img = imread("img/qrcode.jpg", IMREAD_GRAYSCALE);
    threshold(img, img, 0, 255, THRESH_OTSU);

    
    AreaMark marker(img, 0);
    vector<vector<int>> area;
    int amount;
    // 1s for 1000 times
    amount = marker.getMarkedArea(area);
    Mat dst;
    marker.getMarkedImage(dst);

    imshow("img", img);
    imshow("dst", dst);
    waitKey(0);
}