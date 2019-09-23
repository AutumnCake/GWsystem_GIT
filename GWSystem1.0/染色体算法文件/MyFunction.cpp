#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

double DistancePoints(Point a,Point b)
{
	return (double)sqrt(pow(double(a.x-b.x),2.0)+pow(double(a.y-b.y),2.0));
}
double DistanceWeightGrayFunction(double D,int G)
{
	double Dn=1.0;
	double Gn=1.0;
	double a=6.0;    //a、b调节数值大小
	double b=Dn+Gn-4;
	if (G==0) return 0;
	else return pow(D/4.0,Dn)*pow((double)((255-G)/25.5),Gn)/(a*pow(10.0,b));
}