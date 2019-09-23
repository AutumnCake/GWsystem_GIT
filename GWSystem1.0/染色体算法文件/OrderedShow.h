#pragma once
#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
using namespace std;
using namespace cv;


#include"ClassClusterInfo.h"
void OrderedShow(vector<ClassClusterInfo> CCIv,Mat& img,int & DicNum,Point StartShowPoint=Point(10,10),int ColSpace=100,int RowSpace=200,int minCentromereNum=0,int maxCentromereNum=10,double Sscale=0.5);
void OrderedShow(vector<ClassClusterInfo> CCIv,Mat& img,Point StartShowPoint,int ColSpace,int RowSpace,double Sscale);