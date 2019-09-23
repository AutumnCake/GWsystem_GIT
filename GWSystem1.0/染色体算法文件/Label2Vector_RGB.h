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


void Label2Vector_RGB(Mat label,int Zonenum, Mat RGB,vector<Mat>& Vector,int VectorBeginIterator);