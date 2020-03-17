#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>
#include<omp.h>
#include<string.h>

using namespace std;
using namespace cv;


// get integral image from single-channel array or Mat
void getGrayIntegralImage(unsigned char *src, int *integral, int rows, int cols)
{
    //reset the first row in integral image
    memset(integral, 0, (cols + 1)* sizeof(int));
    for (int row = 0; row < rows; row++){
        // the first colume of current row in source image 
        unsigned char *srcCurRow = src + row * cols;
        // the second colume of previous row in integral image
        int *dstPrevRow = integral + row * (cols + 1) + 1;
        // the second colume of current row in integral image
        int *dstCurRow = integral + (row + 1) * (cols + 1) + 1;
        // reset the first colume in integral image
        dstCurRow[-1] = 0;

        for (int col = 0, sum = 0; col < cols; col++){
            // accumulate alone the row 
            sum += srcCurRow[col];
            // integral[row][col] = src[i][j] for i in range(0,row) for j in range(0,col)
            dstCurRow[col] = dstPrevRow[col] + sum;
        }
    }
}


//use integral image to calculate median value
void integralBoxFilter(unsigned char *src, unsigned char *dst, int rows, int cols, int kSize,int srcType)
{
    CV_Assert(srcType == CV_8UC1);
    int radius = kSize / 2;
    // allocate space for integral image
    int *integral = (int *)malloc((rows + 1) * (cols + 1) * sizeof(int));
    // update integral image
    getGrayIntegralImage(src, integral, rows, cols);

    #pragma omp parallel for
    for (int row = 0; row < rows; row++){
        // example: ksize =3, radius =1, row=1, col =1
        // the follow is integral image:
        //r\c  0  1  2  3
        // 0   A  -  -  B YL
        // 1   -  *  *  *
        // 2   -  *  *  * 
        // 3   D  *  *  C YH
        //     XL       XH
        // sum of kernel is equal to (C-B-D+A)
        int YL = max(row - radius, 0);
        int YH = min(row + 1 + radius, rows);
        unsigned char *dstCurRow = dst + row * cols;
        int *rowL = integral + YL * (cols + 1);
        int *rowH = integral + YH * (cols + 1);

        for (int col = 0; col < cols; col++){
            int XL = max(col - radius, 0);
            int XH = min(col + 1 + radius, cols);
            int kernelSum = rowH[XH] - rowH[XL] - rowL[XH] + rowL[XL];
            int pixelsCnt = (XH - XL) * (YH - YL);
            dstCurRow[col] = (kernelSum + (pixelsCnt >> 1)) / pixelsCnt;
        }
    }
    
    // free space of integral image
    free(integral);
}

int main()
{
    Mat img = imread("img/numbers.jpg", IMREAD_GRAYSCALE);
    Mat dst(img.rows, img.cols, CV_8UC1);
    Mat dst2(img.rows, img.cols, CV_8UC1);

    clock_t t0 = clock();
    integralBoxFilter(img.data, dst.data, img.rows, img.cols, 15, img.type());
    clock_t t1 = clock();
    boxFilter(img, dst2, -1, Size(15,15));
    clock_t t2 = clock();

    cout << t1 - t0 << endl;
    cout << t2 - t1 << endl;

    imshow("dst", dst);
    imshow("dst2", dst2);
    waitKey(0);
}