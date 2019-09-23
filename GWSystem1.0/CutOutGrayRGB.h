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

void CutOutGrayRGB(Mat InBi,Mat InGray,Mat InRGB,Mat& OutGray,Mat& OutRGB,int flag);
void CutOutRGB(Mat InBi,Mat InRGB,Mat& OutRGB,int flag);
void CutOutGray(Mat InBi,Mat InGray,Mat& OutGray,int flag);