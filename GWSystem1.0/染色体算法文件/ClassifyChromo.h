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


void  ClassifyChromo(vector<ClassClusterInfo> &ChromInfo, vector<ClassClusterInfo>& LumpInfo, vector<ClassClusterInfo>& SingleInfo, vector<ClassClusterInfo>& TinyInfo);