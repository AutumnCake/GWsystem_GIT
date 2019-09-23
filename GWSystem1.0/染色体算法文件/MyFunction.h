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

double DistancePoints(Point a,Point b);
double DistanceWeightGrayFunction(double D,int G);
double GrayFunction(double D,int G);