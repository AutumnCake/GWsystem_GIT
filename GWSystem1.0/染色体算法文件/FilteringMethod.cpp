/*************************************************************************
�˲���������ʱ��һ�׵�ͨ�˲�
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
	return Fpara*value + (1-Fpara)*new_value;  //һ�׵�ͨ�˲�
 }  