#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>

using namespace std;
using namespace cv;

#define PI 3.1415926

#include"ClassClusterInfo.h"
#include"MyFunction.h"
#include"BoolSort.h"
//构造函数
ClassClusterInfo::ClassClusterInfo(){}
//设定函数
void ClassClusterInfo::SetClassClusterInfo(Mat img)
{
	ClassClusterInfo::image=img.clone();
	ClassClusterInfo::UprightRect=Rect(0,0,0,0);
	ClassClusterInfo::minRect=RotatedRect(Point2f(0,0), Size2f(0,0), 0);
	ClassClusterInfo::ClusterArea=0;

}
void ClassClusterInfo::SetClassClusterInfo(Mat img,Rect UrRect)
{
	ClassClusterInfo::image=img.clone();
	ClassClusterInfo::UprightRect=UrRect;
	ClassClusterInfo::minRect=RotatedRect(Point2f(0,0), Size2f(0,0), 0);
	ClassClusterInfo::ClusterArea=0;

}
void ClassClusterInfo::SetClassClusterInfo(Mat img,Rect UrRect,Point RelativeOrigin)
{
	ClassClusterInfo::image=img.clone();
	ClassClusterInfo::UprightRect=UrRect+RelativeOrigin;
	ClassClusterInfo::minRect=RotatedRect(Point2f(0,0), Size2f(0,0), 0);
	ClassClusterInfo::ClusterArea=0;

}
//设定并获取CentromereLocationVector
void ClassClusterInfo::SetCentromereLocationVector(vector<Point> CLV)
{
	for(int i=0;i<CLV.size();i++)
	{

		ClassClusterInfo::CentromereLocationVector.push_back(CLV[i]+ClassClusterInfo::GetRelativeOrigin());
	}
	
}
void ClassClusterInfo::SetCentromereLocationVector(Point CL)
{
	ClassClusterInfo::CentromereLocationVector.push_back(CL+ClassClusterInfo::GetRelativeOrigin());
}
vector<Point> ClassClusterInfo::GetCentromereLocationVector()
{
	return ClassClusterInfo::CentromereLocationVector;
}
//获取Mat
Mat ClassClusterInfo::GetClusterImage()
{
	return ClassClusterInfo::image;
}
//获取位置信息
Point ClassClusterInfo::GetRelativeOrigin()
{
	return ClassClusterInfo::UprightRect.tl();
}
Rect ClassClusterInfo::GetUprightRect()
{
	return ClassClusterInfo::UprightRect;
}
//获取最小外接矩形信息
void ClassClusterInfo::SetminRect_ClusterArea()
{
	Mat Img=ClassClusterInfo::image.clone();
	if(Img.empty()) return;
	//设定最小外接矩形
	Mat gray;
	cvtColor( Img, gray, CV_BGR2GRAY );
	Mat Bi;
	threshold(gray,Bi,0,255,THRESH_BINARY);
	vector<vector<Point> > contours_out;  
	vector<Vec4i> hierarchy_out;  
	findContours(Bi, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE); 
	//cout<<contours_out.size()<<"\t"<<IM<<"\n";
	//Find minAreaRect 并按顺序存储
	if(contours_out.size()>0)
	{
		RotatedRect mRTemp=minAreaRect(contours_out[0]);
		double CATemp=contourArea(contours_out[0]);
		if(mRTemp.size==Size2f(0,0)||CATemp==0) return;
		ClassClusterInfo::minRect=mRTemp;
		ClassClusterInfo::ClusterArea=CATemp;
	}
	return;
}
RotatedRect ClassClusterInfo::GetminRect()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	return ClassClusterInfo::minRect;
}
double ClassClusterInfo::GetClusterArea()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	return ClassClusterInfo::ClusterArea;
}
//获取最小矩形长短边参数
double ClassClusterInfo::GetminRectShort()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	int a=ClassClusterInfo::minRect.size.height;
	int b=ClassClusterInfo::minRect.size.width;
	return (a<b?a:b);
}
double ClassClusterInfo::GetminRectLong()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	int a=ClassClusterInfo::minRect.size.height;
	int b=ClassClusterInfo::minRect.size.width;
	return (a>b?a:b);
}
//获取面积比
double ClassClusterInfo::GetAreaRatio()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	if(ClassClusterInfo::minRect.size.area()>0)
		return ClassClusterInfo::ClusterArea/(double)ClassClusterInfo::minRect.size.area();
	else
		return 0;
}
////分配最小矩形四个点左上为P0，右上：P1，左下：P2，右下：P3 //x=0,1,2,3
//bool ClassClusterInfoless_y(const Point2f p1, const Point2f  p2)   
//{  
//    return p1.y < p2.y;  
//} 
//bool ClassClusterInfoless_x(const Point2f p1, const Point2f  p2)   
//{  
//    return p1.x < p2.x;  
//} 
vector<Point2f> ClassClusterInfo::GetminRectPoints()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	//if(x<0||x>4)
	//{
	//	cout<<"error:ClassClusterInfo::GetminRectPoints(int x)中x只能是0,1,2,3!!!!!!!!\n";
	//	Point p=Point(-1,-1);
	//	return p;
	//}
	//else
	{
		Point2f rect_points[4];
		ClassClusterInfo::minRect.points(rect_points);
		vector<Point2f> vP;
		for(int i=0;i<4;i++)
		{
			vP.push_back(rect_points[i]);
		}
		sort(vP.begin(),vP.end(),lessY);
		sort(vP.begin(),vP.begin()+2,lessX);
		sort(vP.begin()+2,vP.end(),lessX);
		return vP;
	}
}
//在最小矩形长边和短边上的中心点 x=0,1.
vector<Point2f> ClassClusterInfo::GetminRectCenterPointxOnShortside()
{
	vector<Point2f> vPS;
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	vector<Point2f> vP=ClassClusterInfo::GetminRectPoints();
	double IfShort01=DistancePoints(vP[0],vP[1]);
	if(abs(IfShort01-ClassClusterInfo::GetminRectShort())<2.0)//01为最短边
	{
		//cout<<"傻逼\n";
		vPS.push_back(Point((vP[0].x+vP[1].x)/2.0,(vP[0].y+vP[1].y)/2.0));
		vPS.push_back(Point((vP[2].x+vP[3].x)/2.0,(vP[2].y+vP[3].y)/2.0));
	}
	else
	{
		vPS.push_back(Point((vP[0].x+vP[2].x)/2.0,(vP[0].y+vP[2].y)/2.0));
		vPS.push_back(Point((vP[1].x+vP[3].x)/2.0,(vP[1].y+vP[3].y)/2.0));
	}
	return vPS;
}
vector<Point2f> ClassClusterInfo::GetminRectCenterPointxOnLongside()
{
	vector<Point2f> vPL;
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	vector<Point2f> vP=ClassClusterInfo::GetminRectPoints();
	double IfLong01=DistancePoints(vP[0],vP[1]);
	//cout<<IfLong01<<"\t"<<ClassClusterInfo::GetminRectLong()<<"\t"<<abs(IfLong01-ClassClusterInfo::GetminRectLong())<<"\n";
	if(abs(IfLong01-ClassClusterInfo::GetminRectLong())<2.0)//01为长边
	{
		//cout<<"傻逼\n";
		vPL.push_back(Point((vP[0].x+vP[1].x)/2.0,(vP[0].y+vP[1].y)/2.0));
		vPL.push_back( Point((vP[2].x+vP[3].x)/2.0,(vP[2].y+vP[3].y)/2.0));
	}
	else
	{
		vPL.push_back( Point((vP[0].x+vP[2].x)/2,(vP[0].y+vP[2].y)/2));
		vPL.push_back( Point((vP[1].x+vP[3].x)/2,(vP[1].y+vP[3].y)/2));
	}
	return vPL;

}
Point ClassClusterInfo::GetminRectCenterPoint()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	return ClassClusterInfo::minRect.center;
}
//以minRectCenterPoint为中心,相对于竖直方向偏转角度(Pi)
double ClassClusterInfo::GetAngleBaseUpright()
{
	vector<Point2f> vPL=ClassClusterInfo::GetminRectCenterPointxOnLongside();
	//cout<<ClassClusterInfo::minRect.size<<"\n";
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
	{
		ClassClusterInfo::SetminRect_ClusterArea();
		//cout<<"set"<<"\n";
	}
	if(vPL[1].y-vPL[0].y==0)
	{
		//cout<<"90"<<"\n";
		//cout<<vPL[1].y<<"\t"<<vPL[0].y<<"\n";
		return PI/2;
	}
		
	else
	{
		double Tan=(double)(vPL[1].x-vPL[0].x)/(double)(vPL[1].y-vPL[0].y);
		return atan(Tan);
	}
}

//暂时设定畸变标志，1为畸变，0为正常
void ClassClusterInfo::SetAberrationFlage(int AbF)
{
	ClassClusterInfo::AberrationFlage = AbF;
}
int ClassClusterInfo::GetAberrationFlage()
{
	return ClassClusterInfo::AberrationFlage;
}

//析构函数
ClassClusterInfo::~ClassClusterInfo(){}