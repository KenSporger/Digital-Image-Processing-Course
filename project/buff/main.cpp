#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/video/tracking.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>


using namespace std;
using namespace cv;


const string lenet_model_file = "../lenet/lenet_iter_80000加了负样本.caffemodel";
const string lenet_txt_file = "../lenet/deploy.prototxt";

int main()
{
    VideoCapture video("/home/ken/Documents/Digital-Image-Processing-Course/video/buff.mp4");
    dnn::Net lenet = cv::dnn::readNetFromCaffe(lenet_txt_file, lenet_model_file);
    char key = 0;
    while (key != 'q')
    {
        /********************************* 二值化 ************************************/

        // 灰度阈值二值
        Mat binary, src, gray, gray_binary;
        video >> src;

        cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        threshold(gray, gray_binary, 50, 255, cv::THRESH_BINARY);

		cv::Mat imgHSV;

		cvtColor(src, imgHSV, cv::COLOR_BGR2HSV);

        cv::Mat temp, tempBinary;

        inRange(imgHSV, Scalar(0, 60, 80), Scalar(25, 255, 255), temp);
		inRange(imgHSV, Scalar(156, 60, 80), Scalar(181, 255, 255), tempBinary);

        tempBinary = temp | tempBinary;
		dilate(tempBinary, tempBinary, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
        binary = tempBinary & gray_binary;
        dilate(binary, binary, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
        // imshow("bin", binary);
        

        /******************************* 检测扇叶 *************************************/
        vector<vector<Point> > armorContours;
	    vector<Vec4i> armorHierarchy;
	    findContours(binary, armorContours, armorHierarchy, RETR_TREE, CHAIN_APPROX_NONE);
        size_t armorContours_size = armorContours.size();

        // drawContours(src, armorContours, -1, Scalar(255, 0, 0), 2);

        // 统计每个轮廓的子轮廓的个数
        //动态分配，并且初始化为0
        int *findCount = new int[armorContours_size]();        
        // findCount是个指针，sizeof测量的是指针的长度，一直都为8,正确为armorContours_size*size(int)
        // int *findCount = new int[armorContours_size]; 
        // memset(findCount, 0, sizeof(findCount));
        //对于数组来说，可以用sizeof(a)测量数组的大小,例如:
        //a[10],sizeof(a)=4*10
        for (size_t i = 0; i < armorContours_size; ++i) {
            // 选择有父轮廓的
            if (armorHierarchy[i][3] != -1) {// 可以尝试加入0个的
                // 去掉噪点(内嵌小轮廓)
                if (contourArea(armorContours[i]) > 100) {
                    findCount[armorHierarchy[i][3]]++;
                }
            }
        }

        // 选择只有一个子轮廓（装甲板）的箭头(新激活的扇叶)
        vector<int> conIndexs;
        for (size_t i = 0; i < armorContours_size; ++i) {
            if (findCount[i] == 1) {
                RotatedRect rrect = minAreaRect(armorContours[i]);

                // condition1: 长宽比
                float width = MAX(rrect.size.width, rrect.size.height);
                float height = MIN(rrect.size.width, rrect.size.height);
                float whrio = width / height;
                if (whrio > 2.7 || whrio < 1.5) 
                    continue;

                // condition2: 面积筛选
                float area = contourArea(armorContours[i]);
                if (area < 2000) continue;

				conIndexs.push_back(i);
                // drawContours(src, armorContours, i, Scalar(255, 0, 0), 2);
            }
        }


        //分类：噪声/激活扇叶
        int index = -1;// 最后的轮廓编号
        float min_score = 921600;
        for (size_t i = 0; i < conIndexs.size(); ++i) {
            //由于扇叶旋转，所以外界矩形框的大小和长宽比例都在变化
            Rect rect = boundingRect(armorContours[conIndexs[i]]);
            Mat roi = src(rect);
            float rot_angle = 0;
            RotatedRect rrect = minAreaRect(armorContours[conIndexs[i]]);
            //调整使得高>宽
			if (rrect.size.width > rrect.size.height) {
				rot_angle = 90 + rrect.angle;
				swap(rrect.size.width, rrect.size.height);
			}
			else {
				rot_angle = rrect.angle;
			}
			Point2f roi_center = Point2f(roi.cols / 2, roi.rows / 2);
            // imshow("roi", roi);
            //一波操作截取出包含扇叶的区域，其中扇叶得到了旋转矫正（竖直，上下有翻转）
            Mat rot = getRotationMatrix2D(roi_center, rot_angle, 1);
			Rect bbox = RotatedRect(roi_center, roi.size(), rot_angle).boundingRect2f();
            rot.at<double>(0, 2) += bbox.width / 2.0 - roi.cols / 2.0;
			rot.at<double>(1, 2) += bbox.height / 2.0 - roi.rows / 2.0;
			Mat rot_roi;
			warpAffine(roi, rot_roi, rot, bbox.size());
            // imshow("rot_roi", rot_roi);

            // 扇叶中心旋转后的点
			Mat rrect_center_mat = (Mat_<double>(3, 1) << rrect.center.x - rect.tl().x,
				rrect.center.y - rect.tl().y,
				1);
            //中心点放射变换
			Mat rot_mat = rot * rrect_center_mat;
			Point2f rot_center = Point2f(rot_mat.at<double>(0, 0), rot_mat.at<double>(1, 0));
            // circle(rot_roi, rot_center, 2, Scalar(255, 0, 0), 2);
            // imshow("rot_roi", rot_roi);

			// 截取矫正的图形
			Mat dst;
			getRectSubPix(rot_roi, rrect.size, rot_center, dst);
            imshow("dst", dst);

  

            // lenet forward
			Mat gray_input;
			cvtColor(dst, gray_input, COLOR_BGR2GRAY);
			Mat inputBlob = dnn::blobFromImage(gray_input, 0.00390625f, Size(28, 28), Scalar(), false);
			lenet.setInput(inputBlob, "data");
			Mat prob = lenet.forward("prob");
			Mat probMat = prob.reshape(1, 1);
			int classId;
			double classProb;
			Point classNumber;
			minMaxLoc(probMat, nullptr, &classProb, nullptr, &classNumber);
			classId = classNumber.x;    // 类别：0是noise，1是true
			if (classId == 0) {
				cout << "noise" << endl;
				// imshow("noise", gray_input);
                continue;
			}
			else {
				// imshow("true", gray_input);
			}


            // 根据面积和长度计算分数
            float final_length = arcLength(armorContours[conIndexs[i]], true);
            float fianl_area = contourArea(armorContours[conIndexs[i]]);
            float score = fianl_area + final_length * 10;

            if (score < min_score) {
                min_score = score;
                index = conIndexs[i];
            }

		}

        if (index == -1) {
            cout << "装甲板检测失败: 找不到箭头轮廓 " << endl;
            return false;
	    }

        // imshow("src", src);
        // key = waitKey(0);
        delete [] findCount;
    }
    
}