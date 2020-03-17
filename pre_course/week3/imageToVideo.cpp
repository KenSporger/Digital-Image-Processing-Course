#include <iostream>
#include <io.h>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//从文件目录inPath下匹配文件，将文件名存入results
void searchImages(string inPath, vector<string> &results)
{ 
	//用于查找的句柄
	long handle;
	struct _finddata_t fileinfo;
	//第一次查找
	handle = _findfirst(inPath.c_str(), &fileinfo);
	if (handle == -1)
		return;
	do
	{
		//加入找到的文件的文件名(不含路径)
		results.push_back(fileinfo.name);
 
	} while (!_findnext(handle, &fileinfo));
 
	_findclose(handle);
}

//将一系列图片做成视频
void imageToVideo(string imagePath, vector<string> imageFiles, string videoPath, int frameFps, int frameWidth, int frameHeight)
{
	string imageName;
	VideoWriter writer;

	writer.open(videoPath, -1, frameFps, Size(frameWidth,frameHeight), true);
	
	for (int i = 0; i < imageFiles.size(); i++)
	{
        cout <<"No." << i << " added:"<< imageFiles[i] << endl;
		imageName = imageFiles[i];
		Mat img = imread(imagePath + imageName, IMREAD_GRAYSCALE);//读入图片
		resize(img, img, Size(frameWidth, frameHeight));
		if (!img.data)//判断图片调入是否成功
		{
			cout << "Could not load image file...\n" << endl;
		}
		
		//写入
		writer << img;
		waitKey(1);
	}
}

int main()
{
    vector<string> filesname;
	//图片文件夹路径
    string path = "D:\\programe\\C++\\opencv\\papers\\";
	//输出视频路径
    string video = "video/papers.avi";
	//匹配JPG文件
    searchImages(path + "*.jpg", filesname);
	//制作视频
    imageToVideo(path, filesname, video, 30, 800, 1000);
}