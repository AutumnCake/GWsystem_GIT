#pragma once
#include "..\stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

void AddBlankFrame(Mat& srcimage,int N);
void SubBlankFrame(Mat& srcimage,int N);