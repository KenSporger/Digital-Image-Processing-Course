#include "gamma.h"

//全局gamma矫正
void gammaCorrect(const Mat src, Mat &dst, float gamma)
{
    //生成灰度级Gamma映射
    uchar map[256];
    for (int i = 0; i < 256; i++)
    {
        map[i] = static_cast<uchar>(pow((float)i / 255, gamma) * 255);
    }

    src.copyTo(dst);

    //Mat迭代器
    //与通道数量无关，可以直接迭代
    MatIterator_<uchar> it,end;
    for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
    {
        *it = map[*it];
    }

}

