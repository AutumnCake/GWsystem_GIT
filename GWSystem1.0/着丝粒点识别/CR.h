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

vector<Point> CRA(Mat srcimage,Point c,int r);
vector<Point> CR(Mat srcimage,Point c,int r);