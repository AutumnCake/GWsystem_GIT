#include "StdAfx.h"
#include"iostream"
#include "queue"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int RGBmean(Mat& src,int BGR)
{
	int pixNum=0;
	int BGRpixSum=0;
	int BGRpixMean=0;
	MatIterator_<Vec3b> it, end = src.end<Vec3b>();  
	for( it = src.begin<Vec3b>(); it != end; ++it)  
	{ 
		if((*it)[BGR]>0)
		{
			BGRpixSum+=(*it)[BGR];
			pixNum++;
		}   
	} 
	BGRpixMean=BGRpixSum/pixNum;
	return BGRpixMean;
}

double RGBdeviation(Mat& src,int BGR)
{
	int    pixNum=0;
	int BGRpixMean=RGBmean(src,BGR);
	double RGBdeviationSum=0;
	MatIterator_<Vec3b> it, end = src.end<Vec3b>();  
	for( it = src.begin<Vec3b>(); it != end; ++it)  
	{ 
		if((*it)[BGR]>0)
		{
			RGBdeviationSum +=(((*it)[BGR]-BGRpixMean)*((*it)[BGR]-BGRpixMean));
			pixNum++;
		}   
	} 
	double RGBdeviation=sqrt(RGBdeviationSum/pixNum);
	return RGBdeviation;
}