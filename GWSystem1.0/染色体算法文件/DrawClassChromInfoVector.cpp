#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
#include"res.h"
#include"ClassClusterInfo.h"
#include"RandomColor.h"
using namespace std;
using namespace cv;

void DrawClassChromInfoVector(vector<ClassClusterInfo> CCI,Mat Draw)//画图的目标Draw必须大于等于原图，否则会出错
{
	for(int CIN=0;CIN<CCI.size();CIN++)
	{
		Mat img=CCI[CIN].GetClusterImage().clone();
		if(img.empty()) continue;
		Mat gray;
		cvtColor( img, gray, CV_BGR2GRAY );
		Mat RandomImage=Mat::zeros(img.size(),CV_8UC3);
		for(int i=0; i < gray.rows; i++)   
		{   
			//uchar* p = result.ptr<uchar>(i);   
			for(int j=0; j< gray.cols; j++)   
			{
				if(gray.at<uchar>(i,j)!=0)
				{
					RandomImage.at<Vec3b>(i,j)=RandomColor(CIN);
				}
			}   
		}
		Mat mask = gray.clone();
		Mat imageROI = Draw(CCI[CIN].GetUprightRect());
		RandomImage.copyTo(imageROI,mask);
	}
}