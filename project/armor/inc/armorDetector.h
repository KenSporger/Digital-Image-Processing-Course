#ifndef _ARMORDETECTOR_H
#define _ARMORDETECTOR_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include "opencv_extended.h"

// #define DEBUG_PRETREATMENT
// #define DEBUG_DETECTION
#define SHOW_RESULT
#define TIME_COUNT

//RM空间包裹
namespace rm
{

enum DetectSatus
{
    ARMOR_NONE = 0,
    ARMOR_LOST = 1,
    ARMOR_LOCAL = 2,
};

enum ArmorColor
{
    ARMOR_BLUE = 0,
    ARMOR_RED = 1
};

enum ObjectType
{
    UNKNOWN_ARMOR = 0,
    SMALL_ARMOR = 1,
    BIG_ARMOR = 2
};

//参数
struct ArmorParam
{
public:
    //预处理
    int brightness_threshold;
    float light_rect_extend_ratio;
    float light_max_height_diff_ratio;

    
    //light
    float light_min_area;
    float light_min_ratio;
    float light_min_rectangularity;

    //pair
    float light_max_angle_diff;
    float light_max_y_diff_ratio;
    float light_min_x_diff_ratio;

    //armor
    float armor_max_dist_ratio;
    float armor_min_dist_ratio;
    float armor_big_armor_ratio;
    float armor_small_armor_ratio;

    //score
    float area_normalized_base;
    float sight_offset_normalized_base;


    ArmorParam()
    {
        brightness_threshold = 100;
        light_min_area = 10;
        light_min_ratio = 2.0;
        light_min_rectangularity = 0.5;
        light_rect_extend_ratio = 1.1;
        light_max_angle_diff = 7;
        light_max_height_diff_ratio = 0.2;
        light_max_y_diff_ratio = 2.0;
        light_min_x_diff_ratio = 0.5;
        armor_max_dist_ratio = 5.0;
        armor_min_dist_ratio = 1.0;
        armor_big_armor_ratio = 3.2;
        armor_small_armor_ratio = 2;
        area_normalized_base = 1000;
        sight_offset_normalized_base = 200;
    }
};

//灯条特征：宽、高、角度、面积、中心
class LightDescriptor
{
public:
    //默认初始化
    LightDescriptor(){};

    //利用RotatedRect进行初始化(特征取自最小包围矩形)
    LightDescriptor(const cv::RotatedRect &rec)
    {
        width = rec.size.width;
        length = rec.size.height;
        angle = rec.angle;
        area = rec.size.area();
        center = rec.center;
    }
    //灯条的旋转矩阵
    //const是因为在调用时对象是const
    cv::RotatedRect rec() const
    {
        return cv::RotatedRect(center, cv::Size2f(width, length), angle);
    }

public:
    float width;
    float length;
    float angle;
    float area;
    cv::Point2f center;
};

//装甲信息
class ArmorDescriptor
{
public:
    ArmorDescriptor();
    ArmorDescriptor(const LightDescriptor &lLight, const LightDescriptor &rLight, const int armorType, const float rotaScore, cv::Mat &grayImg, ArmorParam param);
    void getFrontImg(const cv::Mat& grayImg);
    void clear()
	{
		rotationScore = 0;
		sizeScore = 0;
		distScore = 0;
		finalScore = 0;
		for(int i = 0; i < 4; i++)
		{
			vertex[i] = cv::Point2f(0, 0);
		}
		type = UNKNOWN_ARMOR;
	}
public:
    std::array<cv::RotatedRect, 2> lightPairs;
    //装甲射击策略评估(分数高优先攻击)
    float sizeScore;
    float distScore;
    float rotationScore;
    float finalScore;
    //装甲类型
    int type;
    //装甲的四个顶点(旋转矩阵的四个顶点)
    std::vector<cv::Point2f> vertex;
    //不知道干嘛
    cv::Mat frontImg;


};

class ArmorDetector
{

#if defined(TIME_COUNT)
public:
    cv::cvex::TimeSummary preProcessTS;
    cv::cvex::TimeSummary findContourTS;
    cv::cvex::TimeSummary contourFilterTS;
    cv::cvex::TimeSummary armorFilterTS;
#endif

public:
    //无参数初始化
    ArmorDetector();
    //使用检测参数初始化
    ArmorDetector(const ArmorParam &armorParam);
    //装甲颜色设置
    void setEnemyColor(int enemy_color);
    //读取帧
    void loadImg(const cv::Mat &srcImg);

    //装甲检测
    int detect();

    //返回攻击装甲的角点
    const std::vector<cv::Point2f> getArmorVertex() const;

#if defined(TIME_COUNT)
    void timeInit()
    {
        preProcessTS.text = "预处理";
        findContourTS.text = "查找轮廓";
        contourFilterTS.text = "灯条筛选";
        armorFilterTS.text = "装甲筛选";
    }
    
    void printEchoTimeSummary()
    {
       std::cout << "-------------echo Time Summary------------" << std::endl;
       preProcessTS.printTime();
       findContourTS.printTime();
       contourFilterTS.printTime();
       armorFilterTS.printTime();
    }

    void printHistroySummary()
    {
       std::cout << "-------------Histroy Time Summary------------" << std::endl;
       preProcessTS.printSum();
       findContourTS.printSum();
       contourFilterTS.printSum();
       armorFilterTS.printSum();        
    }
#endif

 
#if defined(SHOW_RESULT)
    //显示识别结果
    void showDetectedArmor()
    {
	    imshow(_debugWindowName, _debugImg);
    }

#endif 


private:
    //检测参数
    ArmorParam _param;
    //敌方颜色
    int _enemy_color;
    //己方颜色
    int _self_color;

    //原图
    cv::Mat _srcImg;
    cv::Mat _roiImg;
    cv::Mat _grayImg;

    cv::Rect _roi;

    //视野范围内的所有敌人装甲
    std::vector<ArmorDescriptor> _armors;
    //需要打击的敌方装甲
    ArmorDescriptor _targetArmor;

    //追踪状态
    int _status;
    int _isTracking;

#if defined(DEBUG_DETECTION) || defined(SHOW_RESULT)
    std::string _debugWindowName;
    cv::Mat _debugImg;
#endif


};



cv::RotatedRect &adjustRec(cv::RotatedRect &rec);

}

#endif 


