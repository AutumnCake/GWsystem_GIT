//生成随机颜色填充
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

vector<Vec3b> colors;

Vec3b RandomColor(int sq)
{
	  
	for (size_t i = 0; i <100 /*Zonenum*/; i++)  
	{  
		int b = theRNG().uniform(0, 255);  
		int g = theRNG().uniform(0, 255);  
		int r = theRNG().uniform(0, 255);  
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));  
	}  
	return colors[sq];
}