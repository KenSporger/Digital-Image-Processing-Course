#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


void segColor(Mat img, Mat &mask)
{
    int cols = img.cols;
    int rows = img.rows;

    int total = cols * rows;

    //shape :(1, total, 3)
    Mat sample = img.reshape(3, total);
    Mat km_data;
    //必须为浮点型
    sample.convertTo(km_data, CV_32F);

    //终止条件
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 5, 0.1);
    int K = 2;
    Mat labels;
    Mat centers;
    int attempts = 3;
    kmeans(km_data, K, labels, criteria, attempts, KMEANS_PP_CENTERS, centers);

    mask.create(img.size(), CV_8UC1);

    uchar *p = mask.ptr<uchar>();
    uchar fg[2] = { 0,255 };
    for (int i = 0; i < total; i++)
    {
        p[i] = fg[labels.at<int>(i)];
    }
}

int main()
{

    VideoCapture videofg("video/kmeansfg.mp4");
    VideoCapture videobg("video/kmeansbg.mp4");
    VideoWriter output;
    
    output.open("video/week14.avi", CV_FOURCC('M','J','P','G'),videobg.get(CV_CAP_PROP_FPS), 
                    Size(videobg.get(CV_CAP_PROP_FRAME_WIDTH), videobg.get(CV_CAP_PROP_FRAME_HEIGHT)));

    if (!videobg.isOpened() || !videofg.isOpened())
    {
        cout << "open video failed !" << endl;
        return 0;
    }

    uchar key = 0;
    Mat bgframe, fgframe;
    Mat fgmask, bgmask, fg, bg;
    Mat dst;
    while (key != 'q')
    {
        videobg >> bgframe;
        videofg >> fgframe;

        if (bgframe.empty() || fgframe.empty())
        {
            cout << "read video failed ! " << endl;
            return 0;
        }

        segColor(fgframe, fgmask);
        if (fgmask.at<uchar>(0,0) == 255)
            bitwise_not(fgmask, fgmask);
        GaussianBlur(fgmask, fgmask, Size(5, 5), 3, 3);
		threshold(fgmask, fgmask, 240, 255, THRESH_BINARY);
        bitwise_not(fgmask, bgmask);
        fg = Scalar::all(0);
        bg = Scalar::all(0);
        fgframe.copyTo(fg, fgmask);
        bgframe.copyTo(bg, bgmask);
        dst = bg + fg;

        // imshow("fg", fg);
        // imshow("fmask", fgmask);
        // imshow("dst", dst);
        // key = waitKey(1);
        output << dst;
    }
    

}
