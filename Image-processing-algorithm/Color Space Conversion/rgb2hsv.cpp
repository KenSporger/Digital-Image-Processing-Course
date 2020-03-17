#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

// RGB模型转到HSV模型
Mat RGB2HSV(Mat src)
{
    int rows = src.rows;
    int cols = src.cols;
    Mat dst(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++){
        for (int j=0; j< cols; j++){

            uchar r = src.at<Vec3b>(i, j)[0];
            uchar g = src.at<Vec3b>(i, j)[1];
            uchar b = src.at<Vec3b>(i, j)[2];
            uchar maxVal = max(r, max(g, b));
            uchar minVal = min(r, min(g, b));
            float diffMaxMin = maxVal - minVal;
            float hue;

            if (maxVal == r) hue = 30 * (g - b) / diffMaxMin;
            else if (maxVal == g) hue = 60 + 30 * (b - r) / diffMaxMin;
            else hue = 120 + 30 * (r - g) / diffMaxMin;
            //Opencv中S和V的范围为0~255，而H的范围是0~180
            //V = max(r,g,b)
            //V:[0,255]
            dst.at<Vec3b>(i, j)[2] = maxVal;
            //S = (V-min(r,g,b))/V
            //S:[0,1] -> 255*S:[0,255]
            dst.at<Vec3b>(i, j)[1] = (maxVal == 0) ? 0 : 255 * (diffMaxMin / maxVal);
            //H = 30*(g-b)/(V-min(r,g,b)) ,V=r
            //H = 60+30*(b-r)/(V-min(r,g,b)) ,V=g
            //H = 120+30*(r-g)/(V-min(r,g,b)) ,V=b
            //H = H+180 if H<0;
            //H:[0,180] 
            dst.at<Vec3b>(i, j)[0] = (hue < 0) ? (180 + hue + 0.5) : hue;
        }
    }
    
    return dst;
}

//HSV转到RGB,过程就是公式的逆推
Mat HSV2RGB(Mat src)
{
    int rows = src.rows;
    int cols = src.cols;
    // 本程序使用6个level表示色调范围 0:[0,30] 1:[30,60] 2:[60,90] 3:[90,120] 4:[120,150] 5:[150,180]
    // 定义各level对应的色调值
    int standard[6] = {0, 60, 60, 120, 120, 180};
    //定义各level对应的符号
    int symbol[6] = {1, -1, 1, -1, 1, -1};
    Mat dst(rows, cols, CV_8UC3);

    for (int i = 0; i < rows; i++){
        for (int j=0; j< cols; j++){
            
            uchar b, g, r;
            uchar h = src.at<Vec3b>(i, j)[0];
            uchar s = src.at<Vec3b>(i, j)[1];
            uchar v = src.at<Vec3b>(i, j)[2];   

            // s=255*(max-min)/max 
            float diffMaxMin = s * v / 255.0; 
            // 计算所属level
            int level = h / 30;
            // 以level3,level4为例，使用的公式为H = 120+30*(r-g)/(V-min(r,g,b)) ,V=b
            // standard=120,则(h - standard[level]) / 30.0* diffMaxMin等于(r-g)
            // diffMidMin=midVal - minVal 必须是大于零的
            // 当level=3时，r-g<0,而symbol=-1,使得diffMidMin=g-r大于了0
            // 当level=5时，r-g>0,而symbol=1,使得diffMidMin=r-g大于了0
            int diffMidMin = symbol[level] * (h - standard[level]) / 30.0  * diffMaxMin;
            int maxVal = v;
            int minVal = v - diffMaxMin;
            // diffMidMin = midVal - minVal
            int midVal = minVal + diffMidMin;

            // 0和5的level红色是最大值，1和2的level绿色是最大值，3和4的level蓝色是最大值
            switch (level){
                case 0: r = maxVal; g = midVal; b = minVal; break;
                case 1: r = midVal; g = maxVal; b = minVal; break;
                case 2: r = minVal; g = maxVal; b = midVal; break;
                case 3: r = minVal; g = midVal ; b = maxVal; break;
                case 4: r = midVal; g = minVal ; b = maxVal; break;
                case 5: r = maxVal; g = minVal ; b = midVal; break;
                default: break;
            }

            dst.at<Vec3b>(i, j)[0] = r;
            dst.at<Vec3b>(i, j)[1] = g;
            dst.at<Vec3b>(i, j)[2] = b;
        }
    }

    return dst;

}

int main()
{
    Mat img = imread("img/numbers.jpg");
    Mat hsv,reImg; 
    Mat hsv_channels[3];

    //BGR->RGB
    cvtColor(img, img, COLOR_BGR2RGB);

    // cvtColor(img,hsv,COLOR_RGB2HSV);
    hsv = RGB2HSV(img);
    // cvtColor(hsv,reImg,COLOR_HSV2RGB);
    reImg = HSV2RGB(hsv);
    
    split(hsv,hsv_channels);
    imshow("img",img);
    imshow("h",hsv_channels[0]);
    imshow("s",hsv_channels[1]);
    imshow("v",hsv_channels[2]);
    imshow("reImg",reImg);
    waitKey(0);

}