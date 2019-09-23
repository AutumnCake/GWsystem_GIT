//NonezeroMeanRGBVector

#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

vector<int> NonezeroMeanRGBVector(Mat RGB)
{
	vector<int> RGBVector;
	vector<int> pixSum(3,0);
	int pixNum=0;
	for(int i=0;i<RGB.rows;i++)
	{
		for(int j=0;j<RGB.cols;j++)
		{
			if(RGB.at<Vec3b>(i,j)!=Vec3b(0,0,0))
			{
				for(int ch=0;ch<3;ch++)
				{
					pixSum[ch]+=RGB.at<Vec3b>(i,j)[ch];
				}
				pixNum++;
			}
		}	
	}
	for(int ch=0;ch<3;ch++)
	{
		RGBVector.push_back(pixSum[ch]/pixNum);
	}
	return RGBVector;
}
vector<int> NonezeroDeviationRGBVector(Mat RGB)
{
	vector<int> RGBMeanVector=NonezeroMeanRGBVector(RGB);
	vector<int> RGBDeviationVector;
	vector<int> pixDSum(3,0);
	int pixDNum=0;
	for(int i=0;i<RGB.rows;i++)
	{
		for(int j=0;j<RGB.cols;j++)
		{
			if(RGB.at<Vec3b>(i,j)!=Vec3b(0,0,0))
			{
				for(int ch=0;ch<3;ch++)
				{
					pixDSum[ch]+=pow(double(RGB.at<Vec3b>(i,j)[ch]-RGBMeanVector[ch]),2.0);
				}
				pixDNum++;
			}
		}	
	}
	for(int ch=0;ch<3;ch++)
	{
		RGBDeviationVector.push_back(pixDSum[ch]/pixDNum);
	}
	return RGBDeviationVector;
}

vector<int> NonezeroDeviationRGBVector(Mat RGB,vector<int> RGBMeanVector)
{
	vector<int> RGBDeviationVector;
	vector<int> pixDSum(3,0);
	int pixDNum=0;
	for(int i=0;i<RGB.rows;i++)
	{
		for(int j=0;j<RGB.cols;j++)
		{
			if(RGB.at<Vec3b>(i,j)!=Vec3b(0,0,0))
			{
				for(int ch=0;ch<3;ch++)
				{
					pixDSum[ch]+=pow(double(RGB.at<Vec3b>(i,j)[ch]-RGBMeanVector[ch]),2.0);
				}
				pixDNum++;
			}
		}	
	}
	for(int ch=0;ch<3;ch++)
	{
		RGBDeviationVector.push_back((pixDSum[ch]/(double)pixDNum));
	}
	return RGBDeviationVector;
}

