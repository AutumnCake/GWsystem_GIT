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
//���캯��
ClassClusterInfo::ClassClusterInfo(){}
//�趨����
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
//�趨����ȡCentromereLocationVector
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
//��ȡMat
Mat ClassClusterInfo::GetClusterImage()
{
	return ClassClusterInfo::image;
}
//��ȡλ����Ϣ
Point ClassClusterInfo::GetRelativeOrigin()
{
	return ClassClusterInfo::UprightRect.tl();
}
Rect ClassClusterInfo::GetUprightRect()
{
	return ClassClusterInfo::UprightRect;
}
//��ȡ��С��Ӿ�����Ϣ
void ClassClusterInfo::SetminRect_ClusterArea()
{
	Mat Img=ClassClusterInfo::image.clone();
	if(Img.empty()) return;
	//�趨��С��Ӿ���
	Mat gray;
	cvtColor( Img, gray, CV_BGR2GRAY );
	Mat Bi;
	threshold(gray,Bi,0,255,THRESH_BINARY);
	vector<vector<Point> > contours_out;  
	vector<Vec4i> hierarchy_out;  
	findContours(Bi, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE); 
	//cout<<contours_out.size()<<"\t"<<IM<<"\n";
	//Find minAreaRect ����˳��洢
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
//��ȡ��С���γ��̱߲���
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
//��ȡ�����
double ClassClusterInfo::GetAreaRatio()
{
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	if(ClassClusterInfo::minRect.size.area()>0)
		return ClassClusterInfo::ClusterArea/(double)ClassClusterInfo::minRect.size.area();
	else
		return 0;
}
////������С�����ĸ�������ΪP0�����ϣ�P1�����£�P2�����£�P3 //x=0,1,2,3
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
	//	cout<<"error:ClassClusterInfo::GetminRectPoints(int x)��xֻ����0,1,2,3!!!!!!!!\n";
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
//����С���γ��ߺͶ̱��ϵ����ĵ� x=0,1.
vector<Point2f> ClassClusterInfo::GetminRectCenterPointxOnShortside()
{
	vector<Point2f> vPS;
	if(ClassClusterInfo::minRect.size==Size2f(0,0))
		ClassClusterInfo::SetminRect_ClusterArea();
	vector<Point2f> vP=ClassClusterInfo::GetminRectPoints();
	double IfShort01=DistancePoints(vP[0],vP[1]);
	if(abs(IfShort01-ClassClusterInfo::GetminRectShort())<2.0)//01Ϊ��̱�
	{
		//cout<<"ɵ��\n";
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
	if(abs(IfLong01-ClassClusterInfo::GetminRectLong())<2.0)//01Ϊ����
	{
		//cout<<"ɵ��\n";
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
//��minRectCenterPointΪ����,�������ֱ����ƫת�Ƕ�(Pi)
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

//��ʱ�趨�����־��1Ϊ���䣬0Ϊ����
void ClassClusterInfo::SetAberrationFlage(int AbF)
{
	ClassClusterInfo::AberrationFlage = AbF;
}
int ClassClusterInfo::GetAberrationFlage()
{
	return ClassClusterInfo::AberrationFlage;
}

//��������
ClassClusterInfo::~ClassClusterInfo(){}