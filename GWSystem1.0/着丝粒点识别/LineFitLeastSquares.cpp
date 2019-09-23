/*************************************************************************
 最小二乘法拟合直线，y = a*x + b; n组数据; r-相关系数[-1,1],fabs(r)->1,说明x,y之间线性关系好，fabs(r)->0，x,y之间无线性关系，拟合无意义
 a = (n*C - B*D) / (n*A - B*B)
 b = (A*D - B*C) / (n*A - B*B)
 r = E / F
 其中：
 A = sum(Xi * Xi)
 B = sum(Xi)
 C = sum(Xi * Yi)
 D = sum(Yi)
 E = sum((Xi - Xmean)*(Yi - Ymean))
 F = sqrt(sum((Xi - Xmean)*(Xi - Xmean))) * sqrt(sum((Yi - Ymean)*(Yi - Ymean)))

**************************************************************************/

#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

void LineFitLeastSquares(vector<Point> data_Point,vector<double> &vResult)
{
	int data_n=data_Point.size();
	float A = 0.0;
	float B = 0.0;
	float C = 0.0;
	float D = 0.0;
	float E = 0.0;
	float F = 0.0;

	for (int i=0; i<data_n; i++)
	{
		A += data_Point[i].x * data_Point[i].x;
		B += data_Point[i].x;
		C += data_Point[i].x * data_Point[i].y;
		D += data_Point[i].y;
	}

	// 计算斜率a和截距b
	float a, b, temp = 0;
	if( temp = (data_n*A - B*B) )// 判断分母不为0
	{
		a = (data_n*C - B*D) / temp;
		b = (A*D - B*C) / temp;
	}
	else
	{
		a = 1;
		b = 0;
	}

	// 计算相关系数r
	float Xmean, Ymean;
	Xmean = B / data_n;
	Ymean = D / data_n;

	float tempSumXX = 0.0, tempSumYY = 0.0;
	for (int i=0; i<data_n; i++)
	{
		tempSumXX += (data_Point[i].x - Xmean) * (data_Point[i].x - Xmean);
		tempSumYY += (data_Point[i].y - Ymean) * (data_Point[i].y - Ymean);
		E += (data_Point[i].x - Xmean) * (data_Point[i].y - Ymean);
	}
	F = sqrt(tempSumXX) * sqrt(tempSumYY);

	double r;
	r = E / F;

	vResult.push_back(a);
	vResult.push_back(b);
	vResult.push_back(r*r);
}
