#include "bgSub.h"

using namespace cv;
using namespace std;



//最简单的背景差分
void bgSubDemo()
{
    // VideoCapture video("../../../video/vtest.avi");
    VideoCapture video;
    video.open(0);

    if (!video.isOpened())
    {
        cerr << "open video failed! " << endl;
        return;
    }

    //背景图像
    Mat bg_mat;
    video >> bg_mat;
    cvtColor(bg_mat, bg_mat, CV_BGR2GRAY);

    Mat frame, sub_mat;
    int thresh = 100;
    uchar key = 0;
    //创建阈值滑动条
    namedWindow("bgSub");
    //由于视频实时，故不设定回调
    createTrackbar("thresh", "bgSub", &thresh, 255);

    while (key != 'q')
    {
        //读取一帧
        video >> frame;
        if (frame.empty())
        {
            cerr << "read frame failed! " << endl;
            return;
        }
        cvtColor(frame, frame, CV_BGR2GRAY);
        //绝对值差分
        absdiff(frame, bg_mat, sub_mat);
        threshold(sub_mat, sub_mat, thresh, 255, THRESH_BINARY);
        imshow("bgSub", sub_mat);
        imshow("video", frame);
        key = waitKey(30);
    }

    destroyAllWindows();
    
}


//单一高斯背景差分，可去除
void gaussianBgSubDemo(float weight = 2, int bg_num = 100)
{
    VideoCapture video;
    video.open(0);

    if (!video.isOpened())
    {
        cerr << "open camera failed! " << endl;
        return;
    }

    //背景图像
    Mat bg_mat;
    vector<Mat> bg_frames;
    //高斯模型
    Mat mean_mat, var_mat;
    Mat frame, sub_mat;
    uchar key = 0;
    int cnt = 0;
    while (key != 'q')
    {
        //读取一帧
        video >> frame;
        if (frame.empty())
        {
            cerr << "read frame faile10d! " << endl;
            return;
        }
        cvtColor(frame, frame, CV_BGR2GRAY);
        
        if (cnt < bg_num)
        {
            //将前bg_num张作为样本
            bg_frames.push_back(frame);
        }
        else if (cnt == bg_num)
        {
            //高斯模型计算
            calcuGaussianBgModel(bg_frames, mean_mat, var_mat);
        }
        else
        {
            //计算背景差分
            gaussianBgModelDiff(mean_mat, var_mat, frame, sub_mat, weight);
            imshow("bgSub", sub_mat);
            key = waitKey(30);
        }        
        ++cnt;        
    }

    destroyAllWindows();
    
}


//调用opencv的背景差分函数方法
int opencvBgSubDemo(int method = 0)
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo("../../../video/vtest.avi");
    // VideoCapture capVideo;
    // capVideo.open(0);
	
    Mat inputFrame, frame, foregroundMask, foreground, background;
	Ptr<BackgroundSubtractor> model;
    
    if (method == 0)
        //K邻近算法
        model = createBackgroundSubtractorKNN();
    else
        //高斯混合模型分离算法
	    model = createBackgroundSubtractorMOG2();
    
    uchar key = 0;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	while (key != 'q') {
		capVideo >> frame;

		//获取前景掩码
		model->apply(frame, foregroundMask);
		// GaussianBlur(foregroundMask, foregroundMask, Size(3, 3), 1.5, 1.5);
		// threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);
        erode(foregroundMask, foregroundMask,getStructuringElement(MORPH_RECT, Size(5, 5)));
        dilate(foregroundMask, foregroundMask,getStructuringElement(MORPH_RECT, Size(9, 9)));
        if (foreground.empty())
		    foreground.create(frame.size(), frame.type());
		foreground = Scalar::all(0);
        //获取前景图像
		frame.copyTo(foreground, foregroundMask);
		//获取背景图像
		model->getBackgroundImage(background);

        imshow("image", frame);
		imshow("foreground mask", foregroundMask);
        imshow("mean background image", background);
		imshow("foreground image", foreground);
        key = waitKey(30);
	}
	return 0;
}




int main()
{
    // bgSubDemo();
    // gaussianBgSubDemo(3);
    // opencvBgSubDemo();
    return 0;
}