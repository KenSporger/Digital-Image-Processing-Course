#include "bgSubGMM.h"

using namespace std;
using namespace cv;


//默认参数
//默认最大模型数量
const int defaultNMixtures = 5;
//默认动态学习率建模帧数
const int defaultHistory = 500;
//默认模型匹配的方差系数阈值
const float defaultVarThl = 3.0f * 3.0f;
//默认背景元素的方差系数阈值
const float defaultVarThh = 4.0f * 4.0f;
//默认背景权重累积阈值
const float defaultBackgroundRatio = 0.9f;
//默认模型初始化方差
const float defaultVarInit = 15.0f;
//默认模型方差最大值
const float defaultVarMax = 5 * defaultVarInit;
//默认模型方差最小值
const float defaultVarMin = 4.0f;
//默认模型衰减值
const float defaultWeightCT = 0.05f;


//默认构造函数
bgSubGMM::bgSubGMM()
{
    nmixtures = defaultNMixtures;
    nframes = 0;
    history = defaultHistory;
    varThL = defaultVarThl;
    varThH = defaultVarThh;
    backgroundRatio = defaultBackgroundRatio;
    varInit = defaultVarInit;
    varMax = defaultVarMax;
    varMin = defaultVarMin;
    weightCT = defaultWeightCT;
}

//用户构造函数
//输入_history影响学习率，varThreshold影响背景分割
bgSubGMM::bgSubGMM(int _history, float varThreshold)
{
    nmixtures = defaultNMixtures;
    nframes = 0;
    history = _history;
    varThL = defaultVarThl;
    varThH = varThreshold;
    backgroundRatio = defaultBackgroundRatio;
    varInit = defaultVarInit;
    varMax = defaultVarMax;
    varMin = defaultVarMin;
    weightCT = defaultWeightCT;
}


struct GMM
{
    float weight;
    float variance;
};


class GMMParallelBody: public ParallelLoopBody
{
    public:
        GMMParallelBody(
            const Mat &_img, Mat &_dst, GMM* _gmm, 
            float* _mean, uchar* _modelAmount, 
            int _nmixtures, int _nchannels, float _varThL, 
            float _varThH, float _bgRatio, float _alpha,
            float _CT, float _varInit, float _varMax, 
            float _varMin)
        {
            img = &_img;
            dst = &_dst;
            gmm0 = _gmm;
            mean0 = _mean;
            modelAmount0 = _modelAmount;
            nmixtures = _nmixtures;
            nchannels = _nchannels;
            varThL = _varThL;
            varThH = _varThH;
            bgRatio = _bgRatio;
            alpha = _alpha;
            CT = _CT;
            varInit = _varInit;
            varMax = _varMax;
            varMin = _varMin;

        };
        void operator()(const Range &range)
        {
            int r0 = range.start, r1 = range.end;
            int cols = img->cols, rows = img->rows;

            float dData[3];

            float wCT = alpha * CT;
            float alpha1 = 1 - alpha; 

            //对行迭代
            for(int i = r0; i != r1; i++)
            {   
                //获取行首指针
                GMM* gmm = gmm0 + i * cols * nmixtures;
                float *mean = mean0 + i * cols * nmixtures * nchannels;
                uchar *modelAmount = modelAmount0 + i * cols;
                uchar *mask = dst->ptr<uchar>(i);
                const uchar* data = img->ptr<uchar>(i);

                int meanStep = nmixtures * nchannels;

                //对列迭代
                for (int j = 0; j < cols; j++, data+=nchannels, gmm+=nmixtures, mean+=meanStep)
                {
                    
                    bool fitModel = false;
                    bool bgflag = false;
                    float totalWeight = 0;
                    int swap_cnt = 0;

                    //对模型迭代
                    uchar models = modelAmount[j];
                    for (int k = 0; k < models; k++, mean+=nchannels)
                    {
                        //权重衰减
                        //w = (1-alpha)*w -alpha*CT
                        float weight = alpha1 * gmm[k].weight - wCT;

                        float var = gmm[k].variance;

                        //判断是否已经匹配模型
                        if (!fitModel)
                        {

                            //计算像素和模型间的距离
                            float dist = 0;
                            //数据类型不对
                            if (nchannels == 1)
                            {
                                dData[0] = (data[0] - mean[0]);
                                dist = dData[0] * dData[0];
                            }
                            else 
                            {
                                dData[0] = (data[0] - mean[0]);
                                dData[1] = (data[1] - mean[1]);
                                dData[2] = (data[2] - mean[2]);
                                dist = dData[0]*dData[0] + dData[1]*dData[1] + dData[2]*dData[2];
                            }

                            //判断是否是背景
                            if (dist < varThH*var && totalWeight < bgRatio) 
                            {
                                bgflag = true;
                            }

                            //判断是否与当前模型匹配
                            if (dist < varThL*var)
                            {
                                fitModel = true;

                                //权重更新
                                //w= w + alpha(1-w) - alpha*Ct
                                weight += alpha;

                                float alpha2 = alpha / gmm[k].weight;

                                //方差更新
                                //v = v + (alpha/w)*(dist-v)
                                float varnew = var + alpha2 * (dist - var);
                                varnew = max(varnew, varMin);
                                varnew = min(varnew, varMax);
                                gmm[k].variance = varnew;
                                
                                
                                //均值向量更新
                                //u = u + (alpha/w)*(x-u)
                                for (int n = 0; n < nchannels; n++)
                                {
                                    mean[n] -= alpha2 * dData[n];
                                }

                                //匹配的模型其权重一定会比原来的大
                                //从大到小按权重排序
                                for (int m = k; m > 0; m--)
                                {
                                    //直到前面没有比它更大的权重
                                    if (weight < gmm[m-1].weight)
                                        break;

                                    swap(gmm[m], gmm[m-1]);
                                    swap_cnt++;
                                    for (int n = 0; n < nchannels; n++)
                                        swap(mean[m*nchannels + n], mean[(m-1)*nchannels + n]);
                                }
                            }
                        }

                        //如果模型的权重小于-wCT，下一次迭代就会小于0，因此现在删除
                        //如果模型的权重小于-wCT，那么它必然是模型集中最小的（在尾部）
                        if (weight < -wCT)
                        {
                            weight = 0;
                            models--;
                        }

                        gmm[k-swap_cnt].weight = weight;
                        totalWeight += weight;

                    }
                    
                    //归一化权重
                    float invWeight = 0;
                    if (totalWeight > 0)
                        invWeight = 1.0 / totalWeight;
                    
                    for (int k = 0; k < models; k++)
                    {
                        gmm[k].weight *= invWeight;
                    }

                    //新建模型
                    if (!fitModel)
                    {
                        //模型数达到最大，最替换权重最小的模型，否则在尾部新建
                        int index = models == nmixtures ? nmixtures - 1 : models++;

                        if (models == 1)
                            gmm[0].weight = 1;
                        else
                        {
                            gmm[index].weight = alpha;
                            //归一化权重，剩余模型分享1-alpha的权重
                            for (int n = 0; n < index; n++)
                                gmm[n].weight *= alpha1;
                        }

                        gmm[index].variance = varInit;

                        for (int n = 0; n < nchannels; n++)
                            mean[index * nmixtures + n] = data[n];                        

                        //从大到小按权重排序
                        for (int m = index; m > 0; m--)
                        {
                            //直到前面没有比它更大的权重
                            if (alpha < gmm[m-1].weight)
                                break;

                            swap(gmm[m], gmm[m-1]);
                            for (int n = 0; n < nchannels; n++)
                                swap(mean[m*nchannels + n], mean[(m-1)*nchannels + n]);
                        }
                    }

                    modelAmount[j] = models;
                    mask[j] = bgflag ? 0 : 255;
                }




            }

        }

    //由于要保证线程的独立性，并行体中的类成员变量不可以被operator()里操作写入
    private:
        const Mat *img;
        Mat *dst;
        GMM* gmm0;
        float* mean0;
        uchar* modelAmount0;
        int nmixtures;
        int nchannels;
        float varThL;
        float varThH;
        float bgRatio;
        float alpha;
        float CT;
        float varInit;
        float varMax;
        float varMin;
};

//适用于单通道或者三通道
void bgSubGMM::apply(InputArray _image, OutputArray _fmask)
{
    Size frameSize = _image.size();
    int frameType = _image.type();
    int nchannels = CV_MAT_CN(frameType);
    CV_Assert(nchannels == 1 || nchannels == 3);

    if (nframes == 0)
    {
        //第一帧初始化背景模型,大小(1,rows*cols*nmixture*(nchannel+GMM))
        bgmodel.create(1, frameSize.height * frameSize.width * nmixtures * (2 + nchannels), CV_32F);
        //初始化背景模型数量
        modelAmount.create(frameSize, CV_8UC1);
        modelAmount = Scalar::all(0);
    }

    ++nframes;

    //学习率先大后减小
    learningRate = 1.0 / min(nframes, history);

    //array->Mat
    Mat image = _image.getMat();
    _fmask.create(frameSize, CV_8UC1);
    //getMat是返回引用
    Mat fmask = _fmask.getMat();

    /*
    const Mat &_img, Mat &_dst, GMM* _gmm, 
            float* _mean, uchar* _modelAmount, 
            int _nmixtures, int _nchannels, float _varThL, 
            float _varThH, float _bgRatio, float _alpha,
            float _CT, float _varInit, float _varMax, 
            float _varMin)
    */

    parallel_for_(Range(0, image.rows), 
                    GMMParallelBody(image, fmask, 
                                    bgmodel.ptr<GMM>(), 
                                    bgmodel.ptr()+sizeof(GMM)*image.rows*image.cols*nmixtures,
                                    modelAmount.ptr(), nmixtures, nchannels, varThL,
                                    varThH, backgroundRatio, learningRate, weightCT, 
                                    varInit, varMax, varMin));
}



