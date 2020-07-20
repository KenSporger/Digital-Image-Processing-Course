#ifndef _BGSUBGMM_H
#define _BGSUBGMM_H

#include<iostream>
#include<opencv2/opencv.hpp>

class bgSubGMM
{
    public:
        //默认构造函数
        bgSubGMM();
        //用户构造函数
        bgSubGMM(int _history, float varThreshold);
        //训练器
        void apply(InputArray image, OutputArray fmask);

    private:
        //最大模型数量
        int nmixtures;
        //学习率
        float learningRate;
        //帧计数
        int nframes;
        //更新帧数
        int history;
        //背景模型匹配阈值
        float varThL;
        //背景像素阈值
        float varThH;
        //背景模型累积权重
        float backgroundRatio;
        //初始方差
        float varInit;
        //最大方差
        float varMax;
        //最小方差
        float varMin;
        //权重衰减
        float weightCT;   

        //背景模型
        cv::Mat bgmodel;
        //当前模型数量
        cv::Mat modelAmount;

};

#endif
