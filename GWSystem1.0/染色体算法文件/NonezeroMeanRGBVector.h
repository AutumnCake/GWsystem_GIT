//NonezeroMeanRGBVector

#pragma once

#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
using namespace std;
using namespace cv;

vector<int> NonezeroMeanRGBVector(Mat RGB);
vector<int> NonezeroDeviationRGBVector(Mat RGB);
	vector<int> NonezeroDeviationRGBVector(Mat RGB,vector<int> RGBMeanVector);