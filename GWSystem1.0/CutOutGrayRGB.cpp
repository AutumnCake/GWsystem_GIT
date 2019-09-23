#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

void CutOutGrayRGB(Mat InBi,Mat InGray,Mat InRGB,Mat& OutGray,Mat& OutRGB,int flag)
{
	OutRGB=Mat::zeros(InBi.size(),CV_8UC3);
	OutGray=Mat::zeros(InBi.size(),CV_8U);
	for(int i=0;i<InBi.rows-1;i++)
	{
		for(int j=0;j<InBi.cols-1;j++)
		{
			if(flag==0)
			{
				if(InBi.at<uchar>(i,j)==0)
				{
					OutGray.at<uchar>(i,j)=InGray.at<uchar>(i,j);
					OutRGB.at<Vec3b>(i,j)=InRGB.at<Vec3b>(i,j);
				}
			}
			else if(flag>0)
			{
				if(InBi.at<uchar>(i,j)!=0)
				{
					OutGray.at<uchar>(i,j)=InGray.at<uchar>(i,j);
					OutRGB.at<Vec3b>(i,j)=InRGB.at<Vec3b>(i,j);
				}
			}
			else
			{
				cout<<"错误：标志位flag<0.（二值图上没有负像素点！！！！）\n";
			}
		}	
	}
	return;
}

void CutOutRGB(Mat InBi,Mat InRGB,Mat& OutRGB,int flag)
{
	OutRGB=Mat::zeros(InBi.size(),CV_8UC3);
	for(int i=0;i<InBi.rows-1;i++)
	{
		for(int j=0;j<InBi.cols-1;j++)
		{
			if(flag==0)
			{
				if(InBi.at<uchar>(i,j)==0)
				{
					OutRGB.at<Vec3b>(i,j)=InRGB.at<Vec3b>(i,j);
				}
			}
			else if(flag>0)
			{
				if(InBi.at<uchar>(i,j)!=0)
				{
					OutRGB.at<Vec3b>(i,j)=InRGB.at<Vec3b>(i,j);
				}
			}
			else
			{
				cout<<"错误：标志位flag<0.（二值图上没有负像素点！！！！）\n";
			}
		}	
	}
	return;
}

void CutOutGray(Mat InBi,Mat InGray,Mat& OutGray,int flag)
{
	OutGray=Mat::zeros(InBi.size(),CV_8U);
	for(int i=0;i<InBi.rows-1;i++)
	{
		for(int j=0;j<InBi.cols-1;j++)
		{
			if(flag==0)
			{
				if(InBi.at<uchar>(i,j)==0)
				{
					OutGray.at<uchar>(i,j)=InGray.at<uchar>(i,j);
				}
			}
			else if(flag>0)
			{
				if(InBi.at<uchar>(i,j)!=0)
				{
					OutGray.at<uchar>(i,j)=InGray.at<uchar>(i,j);
				}
			}
			else
			{
				cout<<"错误：标志位flag<0.（二值图上没有负像素点！！！！）\n";
			}
		}	
	}
	return;
}