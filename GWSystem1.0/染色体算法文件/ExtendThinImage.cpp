/*
将腐蚀法腐蚀出来的骨架两端分别延长，
flag==ExP时，延长头部；flag>ExP时，延长尾部
Exy为延长的大致长度。
Dis为取斜率的距离

*/
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

#include"Point2Vector.h"

void ExtendThinImage(Mat &Thinimage,int Dis,int Exy,bool flag)
{
	
	vector<Point> ThinPoints;
	//将骨架上的点依次存储在ThinPoints中
	Point2Vector(Thinimage,ThinPoints);
	int ExP=0;
	int ExDisP=0;
	if(!flag)
	{
		ExP=0;
		ExDisP=ExP+Dis-1;
	}
	else 
	{
		ExP=ThinPoints.size()-1;
		ExDisP=ExP-(Dis-1);
	}

	double Dx=ThinPoints[ExP].x-ThinPoints[ExDisP].x;
	double Dy=ThinPoints[ExP].y-ThinPoints[ExDisP].y;
	
	if(Dx==0)
	{
		int xTemp=ThinPoints[ExP].x;
		int yTemp=ThinPoints[ExP].y+(Dy/fabs(Dy))*Exy;
		line(Thinimage, ThinPoints[ExP],Point(xTemp,yTemp),Scalar(255), 1,8);

	}
	if(Dy==0)
	{
		
		int xTemp=ThinPoints[ExP].x+(Dx/fabs(Dx))*Exy;
		int yTemp=ThinPoints[ExP].y;
		line(Thinimage, ThinPoints[ExP],Point(xTemp,yTemp),Scalar(255), 1,8);

	}
	if(Dx!=0 && Dy!=0)
	{
		if(fabs(Dy/Dx)>1)
		{
			
			int yTemp=ThinPoints[ExP].y+(Dy/fabs(Dy))*Exy;
			int xTemp=(Dy/fabs(Dy))*Exy*Dx/Dy+ThinPoints[ExP].x;
			line(Thinimage, ThinPoints[ExP],Point(xTemp,yTemp),Scalar(255), 1,8);


		}
		else
		{
			
			int xTemp=ThinPoints[ExP].x+(Dx/fabs(Dx))*Exy;
			int yTemp=(Dx/fabs(Dx))*Exy*Dy/Dx+ThinPoints[ExP].y;
			line(Thinimage, ThinPoints[ExP],Point(xTemp,yTemp),Scalar(255), 1,8);

		}
	}
		

}