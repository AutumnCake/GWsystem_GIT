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

//²»´ø¼ìÑé
double MixDistanceGraylevel(Point PTemp,Mat GrayImage,int Max,double kx,double ky,double(*DistanceWeightGrayFunction)(double,int),vector<double>& Kt);
vector<double> MixDistanceGraylevel(vector<Point> ThinPoints,Mat GrayImage,int Max,double(*DistanceWeightGrayFunction)(double,int),vector<vector<double>>& Kv);
vector<double> MixDistanceGraylevel(vector<Point> ThinPoints,Mat GrayImage,int Max);
double MixDistanceGraylevel(Point PTemp,Mat GrayImage,int Max,double kx,double ky);