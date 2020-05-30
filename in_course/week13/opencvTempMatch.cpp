#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    cv::VideoCapture cap;
    
    // cap.open(0);
    cap.open("video/vtest.avi");
    if (!cap.isOpened()){
        std::cout << "can't open your camera!" << std::endl;
        return -1;
    }

    Mat frame;

    //ROI选取模板
    cap >> frame;
    Rect2d roi;
    roi = selectROI(frame, true);
    Mat temp; 
    frame(roi).copyTo(temp);

    destroyAllWindows();

    const int match_method = 0;

    // 按q结束
    uchar key=0;
    Mat res, dispMat;
    while(key != 'q'){
        
        cap >> frame;
        if (frame.empty())
        {
            cout << "read frame failed!" << endl;
            return -1;
        }

        matchTemplate(frame, temp, res, match_method);

        normalize(res, res, 0, 1, NORM_MINMAX);

        double minVal, maxVal;
        Point minLoc, maxLoc, matchLoc;

        minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc);

        if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
        {
            matchLoc = minLoc;
        }
        else
        {
            matchLoc = maxLoc;
        }

        frame.copyTo(dispMat);
        rectangle(dispMat, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar(255, 0, 0));

        imshow("dst", dispMat);

        // 延时 30ms 
        key = waitKey(30);
    }
}