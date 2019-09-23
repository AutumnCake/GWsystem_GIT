#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

int NonezeroMeanGrayValue(Mat gray)
{
	int pixSum=0;
	int pixNum=0;
	for(int i=0;i<gray.rows;i++)
	{
		for(int j=0;j<gray.cols;j++)
		{
			if(gray.at<uchar>(i,j)>0)
			{
				pixSum+=gray.at<uchar>(i,j);
				pixNum++;
			}
		}	
	}
	 int pixMean=pixSum/pixNum;
	 return pixMean;
}

int NonezeroMeanGrayValue(Mat gray,Mat mask)
{
	int pixSum=0;
	int pixNum=0;
	for(int i=0;i<gray.rows;i++)
	{
		for(int j=0;j<gray.cols;j++)
		{
			if(mask.at<uchar>(i,j)>0)
			{
				pixSum+=gray.at<uchar>(i,j);
				pixNum++;
			}
		}	
	}
	int pixMean=0;
	if(!pixNum)
		pixMean=0.1;
	else
	  pixMean=pixSum/pixNum;
	 return pixMean;
}