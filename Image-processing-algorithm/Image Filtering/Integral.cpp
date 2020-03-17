#include<iostream>
#include<string.h>
using namespace std;

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

int main()
{
unsigned char img[10][10];
    int dst[11][11];
    
    memset(img, 1, sizeof(img));
    getGrayIntegralImage(img[0], dst[0], 10, 10);

    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++){
            printf("%d ",dst[i][j]);
        }
        printf("\n");
    }

    
}