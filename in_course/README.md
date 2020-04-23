# Week1-C++练习

### printPrimeNumber.cpp
两种方法实现素数查找

# Week2-Mat的使用

### pixelsAverage.cpp
比较OpenCV内置灰度化方法和自实现均值灰度化

### matCopy.cpp
比较Mat的深复制和浅复制的区别

# Week3-多媒体、通道、直方图

### splitBGR.cpp
分割RGB通道

### video.cpp
展示如何使用的自己的摄像头和读取视频流文件

### paint.cpp
Opencv绘图基础，包括点、线、矩形、圆、椭圆的绘制方法

### hist.cpp
直方图统计和归一化，使用绘图技术可视化直方图


# Week4-阈值化

### rangHSV.cpp
利用HSV模型阈值化，并进行简单的肤色检测

### threshold.cpp
比较了大津全局阈值和局部自适应阈值MEAN-C法的二值化效果

### trackBar.cpp
使用OPENCV内置的滑动条进行交互


# Week5-形态学和连通域标记

### morphology.cpp
形态学操作：腐蚀、膨胀、开运算、闭运算

### coinCount.cpp
利用connectedComponentsWithStats进行连通域标记，利用其返回值绘制包围矩形框，进行硬币计数

### circuitCount.cpp
连通域标记+形态学操作进行电路板焊盘计数

### clipCount.cpp
连通域标记+面积特征进行回形针计数


# Week6-滤波与边缘检测

### blur.cpp
摄像头实时中值滤波、均值滤波、高斯滤波

### sobel.cpp
利用Sobel算子提取图像边缘（水平、垂直）

### buffing.cpp
结合HSV和中值滤波进行简单地磨皮美白，利用了掩码技术

### canny.cpp
canny边缘检测两种方法：通过Sobel获取梯度进行canny边缘检测;通过重载函数直接对灰度图进行检测


# Week7-几何变换

### rotateAndScale.cpp
用 warpAffine进行旋转和缩放

### warpAffine.cpp
通过指定三点的映射确定变换矩阵进行仿射变换

### warpPerspect.cpp
通过指定四点的映射确定变换矩阵，利用warpPerspective进行透视变换

### rotateCorrect.cpp
搜索边界三点，将旋转后的矩阵校正回来，为方便搜点，编写了边界查点函数

### wholeRotate.cpp
利用几何变换前后的数学关系，调整旋转矩阵，进行非剪裁旋转

### wholeRotate2.cpp
通过扩大显示范围来进行非剪裁旋转，利用了RotatedRect和boundingRect来获取旋转后的最小包围矩形


# Week8-霍夫直线变换

### houghLine.cpp
霍夫直线变换，获取工件棱线。利用Canny获取边缘，将霍夫变换结果从极坐标转换到笛卡尔坐标系

### houghLinep.cpp
概率霍夫直线变换获取图上所有直线，返回结果为笛卡尔系的直线端点坐标。相对于霍夫直线变换，对直线的长度和点间距作为额外限制


# Week9-图像颜色和形状特征

### chipDetect.cpp
图像特征（宽长比）提取芯片中心，利用开运算作为辅助去噪，findContours查找轮廓，minAreaRect获取最小包围矩形

### circleDetect.cpp
图像特征（宽长比+面积）提取圆孔，findContours查找轮廓，minAreaRect获取最小包围矩形

### cupDectect.cpp
结合HSV进行二值化，图像特征（面积）提取红色杯盖，findContours查找轮廓，minAreaRect获取最小包围矩形

