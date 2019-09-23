#pragma once
//#include "stdafx.h"
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


bool lessX(Point a,Point b);
bool lessY(Point a,Point b);
bool greaterX(Point a,Point b);

bool SortClassClusterInfoLess_S( ClassClusterInfo p1,  ClassClusterInfo  p2) ;
bool SortClassClusterInfoGreater_S( ClassClusterInfo p1,  ClassClusterInfo  p2)   ;
bool SortClassClusterInfoLess_L( ClassClusterInfo p1,  ClassClusterInfo  p2); 
bool SortClassClusterInfoGreater_L( ClassClusterInfo p1,  ClassClusterInfo  p2); 
bool SortClassClusterInfoLess_SL( ClassClusterInfo p1,  ClassClusterInfo  p2) ;
bool SortClassClusterInfoGreater_SL( ClassClusterInfo p1,  ClassClusterInfo  p2)   ;