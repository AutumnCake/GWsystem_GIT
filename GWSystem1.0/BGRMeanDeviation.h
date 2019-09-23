#pragma once
#include"iostream"
#include "queue"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int RGBmean(Mat& src,int BGR);
double RGBdeviation(Mat& src,int BGR);