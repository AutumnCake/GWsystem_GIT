#pragma once
#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"..\染色体算法文件\ClassClusterInfo.h"

using namespace std;
using namespace cv;


void LocateCentromere(vector<ClassClusterInfo>& SCCI);