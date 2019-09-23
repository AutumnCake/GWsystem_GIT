#pragma once
#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;


void Label2Vector_RGB(Mat label,int Zonenum, Mat RGB,vector<Mat>& Vector,int VectorBeginIterator)
{
	if(RGB.channels()==3)
	{
		for(int i=0;i<Zonenum;i++)
		{
			Mat  Lump;
			Lump=Mat::zeros(label.size(),CV_8UC3);
			/*Group[i]=Lump;*/
			Vector.push_back(Lump);	
		}
		for(int i=0;i<label.rows;i++)
		{
			for(int j=0;j<label.cols;j++)
			{
				if(label.at<uchar>(i,j)!=0)
					Vector[(int)label.at<uchar>(i,j)-1+VectorBeginIterator].at<Vec3b>(i,j)=RGB.at<Vec3b>(i,j);	
			}
		}
	}
	else
	{
		for(int i=0;i<Zonenum;i++)
		{
			Mat  Lump;
			Lump=Mat::zeros(label.size(),CV_8UC1);
			/*Group[i]=Lump;*/
			Vector.push_back(Lump);	
		}
		for(int i=0;i<label.rows-1;i++)
		{
			for(int j=0;j<label.cols-1;j++)
			{
				if(label.at<uchar>(i,j)!=0)
					Vector[(int)label.at<uchar>(i,j)-1+VectorBeginIterator].at<uchar>(i,j)=RGB.at<uchar>(i,j);	
			}
		}
	}
	

}
