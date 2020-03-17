#include<iostream>
#include<time.h>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// generate value of noise
// Marsaglia polar method
double generateGaussianNoise(double mu, double sigma)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
    double U1,U2;
    if ( phase == 0 ) {
        do {
            // generate 2 independent variables that follow U(0,1)
            U1 = (double)rand() / RAND_MAX;
            U2 = (double)rand() / RAND_MAX;

            // change range to [-1,1] 
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            // limit in unit circle
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else{
        X = V2 * sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;
    return mu+sigma*X;
}

// mu determines the brightness of noise
// sigma and k affect the intensity of noise
// noise = k*(mu+sigma*random)
void addGaussianNoise(Mat &src, double mu, double sigma, double k)
{
    int rows = src.rows;
    int cols = src.cols;
    int total = rows * cols * src.channels();

    uchar* head = src.data;

    for (int i = 0; i < total; i++){
        //color dithering
        int val = *head + k*generateGaussianNoise(mu, sigma);
    
        if (val < 0) {
            val = 0;
        }else if (val > 255){
            val = 255;
        }

        *head++ = (uchar)val;
    }
}

int main()
{
    Mat img = imread("img/numbers.jpg");
    clock_t t1 = clock();
    addGaussianNoise(img, 0, 1.0, 60);
    clock_t t2 = clock();
    cout << t2 -t1;
    imshow("img", img);
    waitKey(0);
}