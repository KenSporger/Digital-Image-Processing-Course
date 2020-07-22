#include "featurePoints.h"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

void siftDetect()
{
    Mat imga = imread("../../../img/rmbuff1.png");
    Mat imgb = imread("../../../img/rmbuff2.png");

    Ptr<SIFT> detector = SIFT::create(20);
    //特征点查找
    vector<KeyPoint> keypointsA, keypointsB;
    detector->detect(imga, keypointsA, Mat());
    detector->detect(imgb, keypointsB, Mat());
    //绘制特征点
    Mat keypointsMatA, keypointsMatB;
    drawKeypoints(imga, keypointsA, keypointsMatA);
    drawKeypoints(imgb, keypointsB, keypointsMatB);

    Ptr<SiftDescriptorExtractor> extractor = SiftDescriptorExtractor::create(20);
    //特征直方图向量
    Mat descriptorA,descriptorB;
    //特征点描述
    extractor->compute(imga, keypointsA, descriptorA);
    extractor->compute(imgb, keypointsB, descriptorB);
    cout << "A向量大小： " << descriptorA.size() << endl;
    cout << "B向量大小： " << descriptorB.size() << endl;

    //特征点匹配
    BFMatcher matcher(NORM_L2);
    std::vector< DMatch > matches;
    matcher.match(descriptorA, descriptorB, matches);

    //绘制匹配图
    Mat img_matches;
    drawMatches(imga, keypointsA, imgb, keypointsB, matches, img_matches);

    imshow("imga", imga);
    imshow("imgb", imgb);
    imshow("keyimga", keypointsMatA);
    imshow("keyimgb", keypointsMatB);
    imshow("desca", descriptorA);
    imshow("descb", descriptorB);
    imshow("match", img_matches);
    waitKey(0);


}

