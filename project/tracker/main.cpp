#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
using namespace std;
using namespace cv;

int main( int argc, char** argv ){

    VideoCapture cap("../../../video/vtest.avi");
    Mat frame;
    Rect2d roi;
    Ptr<Tracker> tracker = TrackerKCF::create();
    cap >> frame;
    roi=selectROI("tracker",frame);
    // initialize the tracker
    tracker->init(frame,roi);
    while(1)
    {
        cap >> frame;
        // stop the program if no more images
        if(frame.rows==0 || frame.cols==0)
        break;
        // update the tracking result
        auto flag = tracker->update(frame,roi);
        if (!flag) 
        {
            roi=selectROI("tracker",frame);
            // initialize the tracker 
            tracker = TrackerKCF::create();
            tracker->init(frame,roi);
        }
        // draw the tracked object
        rectangle( frame, roi, Scalar( 255, 0, 0 ), 2, 1 );
        // show image with the tracked object
        imshow("tracker",frame);
        
        waitKey(30);
    }

}