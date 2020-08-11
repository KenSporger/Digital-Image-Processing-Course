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

void primitiveDectectDemo()
{
    Mat img = imread("../../../img/polygon.jpg");
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    Mat gradX, gradY, edge;
    Sobel(gray, gradX, CV_16SC1, 1, 0);
    Sobel(gray, gradY, CV_16SC1, 0, 1);
    Canny(gradX, gradY, edge,50, 150);

    int rows = img.rows;
    int cols = img.cols;
    const int radius = 10;
    float Td = 2;
    float sigma = radius /2.0;
    float Eth1 = 500, Eth2 = 1.0, Eth3 = 100;
    vector<vector<int>> keyPoints;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (edge.at<uchar>(i, j) != 255)
                continue;
            float hp[360] = {0};
            for (int m = -radius; m < radius; m++)
            {
                for (int n = -radius; n < radius; n++)
                {
                    if (m == 0 && n == 0) continue;
                    if (check(m+i, n+j, cols, rows))
                    {
                        float d = distance(j+n, i+m, gradX.at<int16_t>(i, j), gradY.at<int16_t>(i, j), j, i);
                        if (d < Td)
                        {
                            float dx = gradX.at<int16_t>(i+m, j+n);
                            float dy = gradY.at<int16_t>(i+m, j+n); 
                            float w = sqrt(dx*dx + dy*dy) * exp(-(m*m + n*n)/2/(sigma*sigma));
                            int angle = atan((float)m / (n+1e-5)) / 2.0/3.1415*360;
                            angle = calcuAngle(n, m, angle);            
                            hp[angle] += w;
                        }
                    }
                }
            }

            // 
            float Et = accumulate(begin(hp), end(hp), float(0));
            int mainAngle = max_element(begin(hp), end(hp)) - begin(hp);
            int mainAngle2 = (mainAngle+180) % 360;
            int beg1 = mainAngle >= 1 ? mainAngle - 1 : 0;
            int end1 = mainAngle <= 358 ? mainAngle + 1: 360;
            int beg2 = mainAngle2 >= 1 ? mainAngle2 - 1 : 0;
            int end2 = mainAngle2 <= 358 ? mainAngle2 + 1: 360;
            float Em = accumulate(begin(hp) + beg1, begin(hp) + end1, float(0))+
                        accumulate(begin(hp) + beg2, begin(hp) + end2, float(0));
            float Ea = Et - Em;
            float Er = Ea  / Em;
            // cout << Em << endl;
            if (Ea >Eth1 && Er > Eth2)
            {
                vector<int> p={j, i};
                bool flag = false;
                for (int z = 0; z < 360; z++)
                {
                    if (hp[z] > Eth3)
                    {
                        flag = true;
                        p.push_back(z);
                    }
                }
                if (flag)
                {
                    circle(img, Point2d(j, i), 4, Scalar(255, 0, 0), 1);
                    keyPoints.push_back(p);
                }   
            }

        }
    }

    cout << keyPoints.size() << endl;

    // imshow("gray", gray);
    imshow("edge", edge);
    imshow("img", img);
    waitKey(0);
}