#include "dft.h"

using namespace cv;

//输入：单通道灰度图
// 输出：原始的幅度谱、原始相位谱
void dftConvert(const Mat &src, Mat &mag_mat, Mat &ph_mat)
{
    if (src.empty() ) {
		return;
	}
    //只对单通道灰度图进行处理
    CV_Assert(src.type() == CV_8UC1);

    //获取最佳长宽，加速DFT计算（因数只有2,3,5是最快的）
    int pad_row = getOptimalDFTSize(src.rows);
    int pad_col = getOptimalDFTSize(src.cols);

    //在底部和右部(以便后面位运算进行裁剪)扩展边界,填充常数值0
    Mat pad_mat;
    copyMakeBorder(src, pad_mat, 0, pad_row - src.rows, 0, pad_col - src.cols, BORDER_CONSTANT, Scalar::all(0));
    std::cout << "pad_mat.rows: " << pad_row << std::endl;
    std::cout << "pad_mat.cols: " << pad_col << std::endl;

    //存储实部和虚部(填充0)
    Mat complex[2] = {Mat_<float>(pad_mat), Mat::zeros(pad_mat.size(), CV_32F)};
    Mat complex_mat;
    //合并实部和虚部，建立二通道Mat
    merge(complex, 2, complex_mat);

    //傅里叶变换
    dft(complex_mat, complex_mat);
    //分割实部和虚部
    split(complex_mat, complex);

    //求幅度谱和相位谱
    magnitude(complex[0], complex[1], mag_mat);
    phase(complex[0], complex[1], ph_mat);

    
}



//Mat对角变换
//两次对角变换可以恢复到原图
//dst的大小一定是偶数，与src可能不一样
void diagonExchange(Mat src, Mat &dst)
{

    //扩展确保对称性（cols和rows为偶数）
    copyMakeBorder(src, src, 0, src.rows & 1, 0, src.cols & 1, BORDER_CONSTANT, Scalar::all(0));

    int ctr_x = src.cols / 2;
    int ctr_y = src.rows / 2;
    //图像对角线互换
	/*
	0 | 1         3 | 2 
	-------  ===> -------
	2 | 3         1 | 0
	*/
    //通过ROI获取四块对角区域，并进行引用赋值
    Mat block_0 = src(Rect(0, 0, ctr_x, ctr_y));
    Mat block_1 = src(Rect(ctr_x, 0, ctr_x, ctr_y));
    Mat block_2 = src(Rect(0, ctr_y, ctr_x, ctr_y));
    Mat block_3 = src(Rect(ctr_x, ctr_y, ctr_x, ctr_y));
    Mat temp;
    //交换0-3
    block_3.copyTo(temp);
    block_0.copyTo(block_3);
    temp.copyTo(block_0);
    //交换1-2
    block_2.copyTo(temp);
    block_1.copyTo(block_2);
    temp.copyTo(block_1);    

    src.copyTo(dst);

}



//dft幅度谱归一可视化
//输入：原始dft幅度谱
//输出：归一化的幅度谱
void dftNormalize(const Mat &mag_mat, Mat &normalized_mat)
{
 
    //用最大值对幅度谱进行归一化到[0,1]区间,alpha下限，beta上限
    //NORM_MINMAX：P=(Ak)/(max-min)
    mag_mat.copyTo(normalized_mat);
    //可选，抑制较大值
    // log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    normalized_mat += Scalar::all(1);
    log(normalized_mat, normalized_mat);
    normalize(normalized_mat, normalized_mat, 0, 255, NORM_MINMAX);

    // 输出：归一化的幅度谱、对角互换的幅度谱、原始相位谱
    //转为uchar类型，否则ROI选择时可能出错
   normalized_mat.convertTo(normalized_mat, CV_8UC1);

}


//输入：dft原始幅度谱、原始相位谱, 逆变后的图像大小
//输出：逆变换后的图像
void idftConvert(const Mat &mag_mat, const Mat &ph_mat, int dst_rows, int dst_cols, Mat &dst)
{
    CV_Assert(mag_mat.type() == CV_32F && ph_mat.type() == CV_32F);

    //极坐标转实部和虚部
    Mat complex[2], complex_mat;
    polarToCart(mag_mat, ph_mat, complex[0], complex[1]);
    merge(complex, 2, complex_mat);

    //傅里叶逆变换，仅输出实部，因为正变换中虚部为0
    Mat ifft;
    idft(complex_mat, ifft, DFT_REAL_OUTPUT);

    //归一化
    normalize(ifft, ifft, 0, 255, NORM_MINMAX);
    ifft = ifft(Rect(0, 0, dst_cols, dst_rows));
    // ifft = ifft * 255;
    ifft.convertTo(dst, CV_8UC1);
    
}

//DFT鼠标交互频域滤波
//输入：原图像
//输出：滤波后的幅度谱、相位谱，滤波后的图像
void dftInteractiveFilter(const Mat &src, Mat &mag_mat, Mat &ph_mat, Mat &dst)
{

    if (src.empty() ) {
		return;
	}
    //只对单通道灰度图进行处理
    CV_Assert(src.type() == CV_8UC1 && mag_mat.type() == CV_32F && ph_mat.type() == CV_32F);

    //分别存储幅度谱、相位谱、可视化幅度谱
	Mat mats[3];
    
    //DFT变换获取幅度相位谱
	dftConvert(src, mats[0], mats[1]);
    //记录谱的大小
    int rows = mats[0].rows;
    int cols = mats[1].cols;
    //幅度谱对角线互换
	diagonExchange(mats[0], mats[0]);
    diagonExchange(mats[1], mats[1]);
    //幅度谱归一化
	dftNormalize(mats[0], mats[2]);

    Mat mask;
	selectPolygon(mats[2], mask, false);
	mats[0] = mats[0].mul(mask);
    mats[1] = mats[1].mul(mask);
	diagonExchange(mats[0], mats[0]);
    diagonExchange(mats[1], mats[1]);

    //恢复谱大小
    mats[0] = mats[0](Rect(0, 0, cols, rows));
    mats[1] = mats[1](Rect(0, 0, cols, rows));
    mats[0].copyTo(mag_mat);
    mats[1].copyTo(ph_mat);
	idftConvert(mats[0], mats[1], src.rows, src.cols, dst);

    return;
}





