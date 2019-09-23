/*************************************************************************
滤波方法：暂时用一阶低通滤波
***************************************************************************/

#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

double filter(double value,double new_value,double Fpara) 
{   
	return Fpara*value + (1-Fpara)*new_value;  //一阶低通滤波
 }  