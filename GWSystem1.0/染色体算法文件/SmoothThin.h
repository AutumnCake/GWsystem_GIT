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


void SmoothThin(Mat& thinImage,int LD=10,Point& BP=Point(0,0),Point& EP=Point(0,0),bool BeginfromB=true);