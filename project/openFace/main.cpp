#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
#include<opencv2/freetype.hpp>
#include<iostream>
#include<map>
using namespace cv;
using namespace std;
using namespace dnn;
//使用OpenFace进行人脸检测与特征匹配

String prototxt = "../model/deploy.prototxt";
String weights = "../model/res10_300x300_ssd_iter_140000_fp16.caffemodel";
String facemodel = "../model/openface.nn4.small2.v1.t7";
string dataset = "../test/samples";
string unknown = "../test/unknown";
int inpWidth = 300;        // Width of network's input image
int inpHeight = 300;       // Height of network's input image
float confidenceThresh = 0.5;
// 人脸比对数据库
multimap<string,vector<float>> faceData;

/*
* 脸部特征向量构造
* face:脸部roi
* net: openface.nn4.small2.v1.t7
* fv: 128维特征向量
*/
void getFaceFeatures(Mat &face, Net &net, vector<float> &fv) 
{
	Mat blob = blobFromImage(face, 1.0/255.0, Size(96, 96), Scalar(0, 0, 0), true, false);
	net.setInput(blob);
    // size:1×128
	Mat probMat = net.forward();
    // cout << "netRecogn size: " << probMat.size << endl;
	for (int i = 0; i < probMat.cols; i++) 
    {
		float value = probMat.at<float>(0, i);
		fv.push_back(value);
	}
}

/*
* 脸部识别与定位
* src:含脸部图像
* net: res10_300x300_ssd_iter_140000_fp16.caffemodel
* box: 脸部定位框
*/
void faceDetect(Mat &src, Net &net, vector<Rect> &boxes)
{
    // 图片数据调整
    // inputshape:300*300
    // scale = 1.0
    // mean = Scalar(104.0, 177.0, 123.0)
    Mat blob;
    blobFromImage(src, blob, 1.0, Size(inpWidth, inpHeight), Scalar(104.0, 177.0, 123.0), false, false); 
    // 前向传播
    net.setInput(blob);
    // 1×1×N×7(N为预测的box数量)
    Mat result = net.forward();  
    // cout << "result shape: "<< result.size << endl;
    // 使用指针拷贝图像
    Mat detectionMat(result.size[2], result.size[3], CV_32F, result.ptr<float>());
    boxes.clear();
    for (int i = 0; i < detectionMat.rows; i++)
    {
        //置信度阈值筛选
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > confidenceThresh)
        {
            // 左下角、右上角坐标都是归一化之后的
            int xLeftTop = static_cast<int>(detectionMat.at<float>(i, 3) * src.cols);
            int yLeftTop = static_cast<int>(detectionMat.at<float>(i, 4) * src.rows);
            int xRightBottom = static_cast<int>(detectionMat.at<float>(i, 5) * src.cols);
            int yRightBottom = static_cast<int>(detectionMat.at<float>(i, 6) * src.rows);
            xLeftTop = max(min(xLeftTop, src.cols), 0);
            yLeftTop = max(min(yLeftTop, src.rows), 0);
            xRightBottom = max(min(xRightBottom, src.cols), 0);
            yRightBottom = max(min(yRightBottom, src.rows), 0);
            Rect faceBox(xLeftTop, yLeftTop, xRightBottom - xLeftTop, yRightBottom - yLeftTop);
            boxes.push_back(faceBox);
        }
    }   
}

float compare(vector<float> &fv1, vector<float> fv2) {
    // 计算余弦相似, 0 ~ 1 距离，距离越小越相似，
    // 0表示夹角为0°，1表示夹角为90°
    float dot = 0;
    float sum2 = 0;
    float sum3 = 0;
    int dims = fv1.size();
    for (int i = 0; i < dims; i++) {
        dot += fv1[i] * fv2[i];
        sum2 += pow(fv1[i], 2);
        sum3 += pow(fv2[i], 2);
    }
    float norm = sqrt(sum2)*sqrt(sum3);
    float similarity = dot / norm;
    float dist = acos(similarity) / CV_PI;
    return dist;
}


void openFaceDemo()
{
    // 读取模型和权重
    Net netDetect = dnn::readNetFromCaffe(prototxt, weights);
    Net netFeature = dnn::readNetFromTorch(facemodel);
    
    // 设置计算后台
	netDetect.setPreferableBackend(DNN_BACKEND_DEFAULT);
	netDetect.setPreferableTarget(DNN_TARGET_CPU);
	netFeature.setPreferableBackend(DNN_BACKEND_DEFAULT);
	netFeature.setPreferableTarget(DNN_TARGET_CPU);  

     
    // 读取样本人脸图片，建立数据库
    std::vector<cv::String> filenames;
    glob(dataset, filenames);
    for (auto file : filenames)
    {
        vector<Rect> boxes;
        Mat sample = imread(file);
        faceDetect(sample, netDetect, boxes);
        // 保证每个图片里只有一张脸
        if (boxes.size() > 1) continue;
        Mat faceRoi = sample(boxes[0]);
        // 截取人名（两个汉字）
        string label(file.end()-12, file.end()-6);
        vector<float> features;
        getFaceFeatures(faceRoi, netFeature, features);
        faceData.insert({label,features});
    }

    // 读取测试人脸图片，利用数据库进行身份识别
    filenames.clear();
    glob(unknown, filenames);
    for (auto file : filenames)
    {
        vector<Rect> boxes;
        Mat img = imread(file);
        string true_label(file.end()-12, file.end()-6);
        faceDetect(img, netDetect, boxes);

        for (auto box : boxes)
        {
            Mat faceRoi = img(box);
            vector<float> features;
            getFaceFeatures(faceRoi, netFeature, features);

            //相似度计算，人脸特征匹配
            float minDist = 1000;
            string pred = "unknown";
            for (auto face : faceData)
            {
                float dist = compare(features, face.second);
                if (minDist > dist)
                {
                    minDist = dist;
                    pred = face.first;
                }
            }

            // 显示结果
            rectangle(img, box, Scalar(0, 255, 0), 2);
            cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
            ft2->loadFontData( "../fonts/simhei.ttf", 0 );
            ft2->putText(img, pred, Point(box.x, box.y), 17, Scalar(255, 255, 255), 1, 8, true);
            // 不支持中文
            // putText(img, pred, Point(box.x, box.y),FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
            
        }
        // 窗口名称显示为正确的标签值
        imshow(true_label, img);
        waitKey(0);
    }


}

int main()
{
    openFaceDemo();
}

