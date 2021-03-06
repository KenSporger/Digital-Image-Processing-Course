#include "primitiveDectect.h"

using namespace std;
using namespace cv;

inline bool check(int r, int c, int cols, int rows)
{
    return c >= 0 && c <= cols && r >= 0 && r < rows;
}

inline float distance(int x, int y, float a, float b, int i, int j)
{
    return abs(a*x + b*y - (a*i + b*j)) / sqrt(a*a + b*b+1e-5);
}

int calcuAngle(int x, int y, int angle)
{
    if (x >= 0 && y >= 0)
    {
        return angle;
    }
    else if (x >= 0 && y <= 0)
    {
        return 360 + angle;
    }
    else if (x <= 0 && y >= 0)
    {
        return 180 + angle;
    }
    else
    {
        return 180 + angle;
    }
}

Mat paintHistgram(float *hist, int n)
{
    cv::Mat dst(500, 1200, CV_8UC3, Scalar(255, 255, 255));
    
    // 找到直方图中最大值
    float maxVal=0;
    for (int i = 0; i < n; i++){
        maxVal = (hist[i] > maxVal) ? hist[i] : maxVal;
    }
    // 按最大值将比例缩放到合适大小
    float maxHeight = 400 / maxVal;
    rectangle(dst, Point(50,50), Point((n-1)*3 + 50,450), CV_RGB(100, 100, 100), 1);
    // 绘制
    for (int i = 0; i< n; i++){
        float X = 50 + i*3;
        line(dst, Point(X, 450), Point(X, 450 - hist[i] * maxHeight), CV_RGB(0, 255, 0), 1);
        if (i % 45 == 0)
            putText(dst, to_string(i), Point(X-5, 452), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
    }

    // 边框

    return dst;
}


void cornorDetect()
{
    Mat img = imread("../../../img/star.jpeg");
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    int rows = img.rows;
    int cols = img.cols;
    Mat gradX, gradY;
    Mat gradXY(rows, cols, CV_16SC1, Scalar::all(0));
    Sobel(gray, gradX, CV_16SC1, 1, 0);
    Sobel(gray, gradY, CV_16SC1, 0, 1);
    Mat edge(rows, cols, CV_8UC1, Scalar::all(0));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            gradXY.at<int16_t>(i, j) = sqrt(gradX.at<int16_t>(i, j)*gradX.at<int16_t>(i, j)+
                                            gradY.at<int16_t>(i, j)*gradY.at<int16_t>(i, j));
            // cout << gradXY.at<int16_t>(i, j) << endl;
            if (gradXY.at<int16_t>(i, j) > 300) edge.at<uchar>(i, j) = 255; 
        }
    }  
    const int radius = 10;
    float Td = 1;
    float sigma = radius /2.0;
    float Eth1 = 4000, Eth2 =1.0, Eth3 =600;
    vector<vector<int>> keyPoints;
    Mat EaMat(rows, cols, CV_32FC1, Scalar::all(0));
    Mat ErMat(rows, cols, CV_32FC1, Scalar::all(0));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (edge.at<uchar>(i, j) != 255)
                continue;
            // float hp[360] = {0};
            float hp[180] = {0};
            for (int m = -radius; m <= radius; m++)
            {
                for (int n = -radius; n <= radius; n++)
                {
                    if (m == 0 && n == 0) continue;
                    if (check(m+i, n+j, cols, rows))
                    {
                        if (edge.at<uchar>(i+m, j+n) != 255)    continue;
                        float d = distance(j, i, gradX.at<int16_t>(i+m, j+n), gradY.at<int16_t>(i+m, j+n), j+n, i+m);
                        if (d < Td)
                        {
                            float dx = gradX.at<int16_t>(i+m, j+n);
                            float dy = gradY.at<int16_t>(i+m, j+n);
                            float w = sqrt(dx*dx + dy*dy) * exp(-(m*m + n*n + d*d)/2/(sigma*sigma));
                            // int angle = atan((float)m / (n+1e-5)) / 2.0/3.1415*360;
                            // angle = calcuAngle(n, m, angle);    
                            // there can't be 1e-5
                            int angle = atan(-dx / dy) / 2.0/3.1415*360;
                            angle = angle < 0 ? angle+180 : angle;
                            hp[angle] += w;
                        }
                    }
                }
            }
            float Et = accumulate(begin(hp), end(hp), float(0));
            int mainAngle = max_element(begin(hp), end(hp)) - begin(hp);
            // int beg1 = mainAngle >= 5 ? mainAngle - 5 : 0;
            // int end1 = mainAngle <= 175 ? mainAngle + 5: 180;
            // int beg2 = mainAngle < 5 ? mainAngle - 5 : 0;
            int beg1 = mainAngle - 5;
            int end1 = mainAngle + 5;
            float Em;
            if (beg1 >= 0 && end1 <= 180)
                Em = accumulate(begin(hp) + beg1, begin(hp) + end1, float(0));
            else if (beg1 < 0)
            {
                Em = accumulate(begin(hp), begin(hp) + end1, float(0));
                Em = accumulate(end(hp) + beg1, end(hp), Em);
            }
            else
            {
                Em = accumulate(begin(hp) + beg1, end(hp), float(0));
                Em = accumulate(begin(hp), begin(hp) + end1 - 180, Em);               
            }

            float Ea = Et - Em;
            float Er = Ea  / Em;
            if (Ea >Eth1 && Er > Eth2)
            {
                cout << Ea << "," <<Er << endl;
                vector<int> p={j, i};
                bool flag = false;
                for (int z = 0; z < 180; z++)
                {
                    if (hp[z] > Eth3)
                    {
                        flag = true;
                        p.push_back(z);
                        // cout << z <<" ";
                    }
                }
                // cout << endl;
                if (flag)
                {
                    // cout << i << "," << j << endl;
                    // circle(img, Point2d(j, i), 4, Scalar(255, 0, 0), 1);
                    // Mat hist = paintHistgram(hp, 180);
                    // imshow("hist", hist);
                    // imshow("img", img);
                    // waitKey(0);
                    // keyPoints.push_back(p);
                    EaMat.at<float>(i, j) = Ea;
                    ErMat.at<float>(i, j) = Er;
                }   
            }

            // 360
            // float Et = accumulate(begin(hp), end(hp), float(0));
            // int mainAngle = max_element(begin(hp), end(hp)) - begin(hp);
            // int mainAngle2 = (mainAngle+180) % 360;
            // int beg1 = mainAngle >= 1 ? mainAngle - 1 : 0;
            // int end1 = mainAngle <= 358 ? mainAngle + 1: 360;
            // int beg2 = mainAngle2 >= 1 ? mainAngle2 - 1 : 0;
            // int end2 = mainAngle2 <= 358 ? mainAngle2 + 1: 360;
            // float Em = accumulate(begin(hp) + beg1, begin(hp) + end1, float(0))+
            //             accumulate(begin(hp) + beg2, begin(hp) + end2, float(0));
            // float Ea = Et - Em;
            // float Er = Ea  / Em;
            // if (Ea >Eth1 && Er > Eth2)
            // {
            //     // cout << Ea << "," <<Er << endl;
            //     vector<int> p={j, i};
            //     bool flag = false;
            //     for (int z = 0; z < 360; z++)
            //     {
            //         if (hp[z] > Eth3)
            //         {
            //             flag = true;
            //             p.push_back(z);
            //             cout << z <<" ";
            //         }
            //     }
            //     cout << endl;
            //     if (flag)
            //     {
            //         circle(img, Point2d(j, i), 4, Scalar(255, 0, 0), 1);
            //         Mat hist = paintHistgram(hp, 360);
            //         imshow("hist", hist);
            //         imshow("img", img);
            //         waitKey(0);
            //         keyPoints.push_back(p);
            //     }   
            // }

        }
    }


    Mat EaMat_dilated;
    dilate(EaMat, EaMat_dilated, getStructuringElement(MORPH_RECT, Size(3, 3)));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (EaMat.at<float>(i, j) > 0 && EaMat.at<float>(i, j) == EaMat_dilated.at<float>(i, j))
                    circle(img, Point2d(j, i), 4, Scalar(255, 0, 0), 1);
        }
    }
    // cout << keyPoints.size() << endl;
    imshow("img", img);
    waitKey(0);

    // imshow("gray", gray);
    
}


//x y  dx dy weight
void cornorFit(vector<vector<float>> keyPoints, Mat &P)
{
    int N = keyPoints.size();

    //构造矩阵X
    cv::Mat X = cv::Mat::zeros(2, 2, CV_64FC1);
    cv::Mat Y = cv::Mat::zeros(2, 1, CV_64FC1);

    for (int i = 0; i < N; i++)
    {
        float a = keyPoints[i][2] / keyPoints[i][4];
        float b = keyPoints[i][3] / keyPoints[i][4];
        float c = (keyPoints[i][0] * keyPoints[i][2] +
                            keyPoints[i][1] * keyPoints[i][3]) / keyPoints[i][4];
        X.at<double>(0, 0) += a * a;
        X.at<double>(0, 1) += a * b;
        X.at<double>(1, 0) += a * b;
        X.at<double>(1, 1) += b * b;
        Y.at<double>(0, 0) += a * c;
        Y.at<double>(1, 0) += b * c;       
    }

    P = cv::Mat::zeros(2, 1, CV_64FC1);
    //求解矩阵A
    cv::solve(X, Y, P, cv::DECOMP_SVD);
}

float smooth(float x, float alpha, float beta)
{
    return alpha - (alpha - 1)*exp(beta*(1-x));
}


void cornorLocate()
{
    Mat img = imread("../../../img/star.jpeg");
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    int rows = img.rows;
    int cols = img.cols;
    Mat gradX, gradY;
    Mat gradXY(rows, cols, CV_16SC1, Scalar::all(0));
    Sobel(gray, gradX, CV_16SC1, 1, 0);
    Sobel(gray, gradY, CV_16SC1, 0, 1);
    Mat edge(rows, cols, CV_8UC1, Scalar::all(0));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            gradXY.at<int16_t>(i, j) = sqrt(gradX.at<int16_t>(i, j)*gradX.at<int16_t>(i, j)+
                                            gradY.at<int16_t>(i, j)*gradY.at<int16_t>(i, j));
            // cout << gradXY.at<int16_t>(i, j) << endl;
            if (gradXY.at<int16_t>(i, j) > 300) edge.at<uchar>(i, j) = 255; 
        }
    }  
    const int radius = 10;
    float Td = 1;
    float sigma = radius /2.0;
    float Eth1 = 4000, Eth2 =1.0, Eth3 =600;
    Mat EaMat(rows, cols, CV_32FC1, Scalar::all(0));
    Mat ErMat(rows, cols, CV_32FC1, Scalar::all(0));
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (edge.at<uchar>(i, j) != 255)
                continue;
            vector<vector<float>> keyPoints;
            float hp[180] = {0};
            float meanMag = 0;
            for (int m = -radius; m <= radius; m++)
            {
                for (int n = -radius; n <= radius; n++)
                {
                    if (m == 0 && n == 0) continue;
                    if (check(m+i, n+j, cols, rows))
                    {
                        if (edge.at<uchar>(i+m, j+n) != 255)    continue;
                        float d = distance(j, i, gradX.at<int16_t>(i+m, j+n), gradY.at<int16_t>(i+m, j+n), j+n, i+m);
                        if (d < Td)
                        {
                            float dx = gradX.at<int16_t>(i+m, j+n);
                            float dy = gradY.at<int16_t>(i+m, j+n); 
                            float w1 = sqrt(dx*dx + dy*dy) * exp(-(m*m + n*n + d*d)/2/(sigma*sigma));
                            float w2 = sqrt(m*m + n*n) * exp(-(m*m + n*n + d*d)/2/(sigma*sigma));
                            float mag = sqrt(dx*dx + dy*dy);
                            float ln = sqrt(dx*dx + dy*dy + (dx*(j+n) + dy*(i+m))*(dx*(j+n) + dy*(i+m)));
                            int angle = atan(-dx / dy) / 2.0/3.1415*360;
                            angle = angle < 0 ? angle+180 : angle;
                            hp[angle] += w1;
                            meanMag += mag;
                            vector<float> p = {(float)(j+n), (float)(i+m), dx, dy, w2, mag, ln};
                            keyPoints.push_back(p);
                        }
                    }
                }
            }

            float Et = accumulate(begin(hp), end(hp), float(0));
            int mainAngle = max_element(begin(hp), end(hp)) - begin(hp);
            int beg1 = mainAngle >= 5 ? mainAngle - 5 : 0;
            int end1 = mainAngle <= 175 ? mainAngle + 5: 180;
            float Em;
            if (beg1 >= 0 && end1 <= 180)
                Em = accumulate(begin(hp) + beg1, begin(hp) + end1, float(0));
            else if (beg1 < 0)
            {
                Em = accumulate(begin(hp), begin(hp) + end1, float(0));
                Em = accumulate(end(hp) + beg1, end(hp), Em);
            }
            else
            {
                Em = accumulate(begin(hp) + beg1, end(hp), float(0));
                Em = accumulate(begin(hp), begin(hp) + end1 - 180, Em);               
            }
            float Ea = Et - Em;
            float Er = Ea  / Em;

            // meanMag /= keyPoints.size();
            if (Ea >Eth1 && Er > Eth2)
            {
                EaMat.at<float>(i, j) = Ea;
                ErMat.at<float>(i, j) = Er;
                // for (auto &p : keyPoints)
                // {
                //     p[4] = p[6] / (smooth(p[5] / meanMag, 2.0, 1.0) *p[4]);
                // }
                // Mat cornor;
                // cornorFit(keyPoints, cornor);
                // // if ()
                // circle(img, Point2d(cornor.at<double>(0,0), cornor.at<double>(1,0)), 4, Scalar(255, 0, 0), 1);
            }

        }
    }

    // Mat EaMat_dilated;
    // dilate(EaMat, EaMat_dilated, getStructuringElement(MORPH_RECT, Size(3, 3)));
    // for (int i = 0; i < rows; i++)
    // {
    //     for (int j = 0; j < cols; j++)
    //     {
    //         if (EaMat.at<float>(i, j) > 0 && EaMat.at<float>(i, j) == EaMat_dilated.at<float>(i, j))
    //     }
    // }

    
    // cout << keyPoints.size() << endl;

    // imshow("gray", gray);
    // imshow("edge", edge);
    // imshow("img", img);
    // waitKey(0);
}