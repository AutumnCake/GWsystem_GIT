#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;


vector<Point> CRA(Mat srcimage,Point c,int r)
{
	vector<Point> wp;
	//wp.clear();
	//��⣨c,r����Χ0,1���
	vector<int> v(r*2*4);
	int x=c.x;
	int y=c.y;
	for(int i=-r;i<r;i++)
	{
		v[r+i]    =(x+i>=0&&y-r>=0&&x+i<srcimage.size().width&&y-r<srcimage.size().height)?srcimage.at<uchar>(y-r,x+i):0;
		v[2*r+r+i]=(x+r>=0&&y+i>=0&&x+r<srcimage.size().width&&y+i<srcimage.size().height)?srcimage.at<uchar>(y+i,x+r):0;
		v[4*r+r+i]=(x-i>=0&&y+r>=0&&x-i<srcimage.size().width&&y+r<srcimage.size().height)?srcimage.at<uchar>(y+r,x-i):0;
		v[6*r+r+i]=(x-r>=0&&y-i>=0&&x-r<srcimage.size().width&&y-i<srcimage.size().height)?srcimage.at<uchar>(y-i,x-r):0;
	}
	//��wp
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==1*255)
		{
			Point P(0,0);
			int d=2*r;
			switch (i/d)
			{
			case 0:
				P=Point(x-r+i%d,y-r);
				break;
			case 1:
				P=Point(x+r,y-r+i%d);
				break;
			case 2:
				P=Point(x+r-i%d,y+r);
				break;
			case 3:
				P=Point(x-r,y+r-i%d);
				break;
			}
			wp.push_back(P);
		}
	}
	return wp;
}
//srcimageΪ�����ͼ��cΪ������ĵ㣬rΪ���뾶��wp.size()�ڣ�c,r����Χ�����ڰ׵�����wp��c,r����Χ�����ڰ׵��λ������
vector<Point> CR(Mat srcimage,Point c,int r)
{
	vector<Point> wp;
	//wp.clear();
	//��⣨c,r����Χ0,1���
	vector<int> v(r*2*4);
	int x=c.x;
	int y=c.y;
	for(int i=-r;i<r;i++)
	{
		v[r+i]    =(x+i>=0&&y-r>=0&&x+i<srcimage.size().width&&y-r<srcimage.size().height)?srcimage.at<uchar>(y-r,x+i):0;
		v[2*r+r+i]=(x+r>=0&&y+i>=0&&x+r<srcimage.size().width&&y+i<srcimage.size().height)?srcimage.at<uchar>(y+i,x+r):0;
		v[4*r+r+i]=(x-i>=0&&y+r>=0&&x-i<srcimage.size().width&&y+r<srcimage.size().height)?srcimage.at<uchar>(y+r,x-i):0;
		v[6*r+r+i]=(x-r>=0&&y-i>=0&&x-r<srcimage.size().width&&y-i<srcimage.size().height)?srcimage.at<uchar>(y-i,x-r):0;
	}
	//��wp
	for(int i=0;i<v.size();i++)
	{
		if(v[i]==1*255)
		{
			int n=i+1;
			if(n==v.size()) n=0;
			if(v[i]*v[n]==0)
			{
				Point P(0,0);
				int d=2*r;
				switch (i/d)
				{
				case 0:
					P=Point(x-r+i%d,y-r);
					break;
				case 1:
					P=Point(x+r,y-r+i%d);
					break;
				case 2:
					P=Point(x+r-i%d,y+r);
					break;
				case 3:
					P=Point(x-r,y+r-i%d);
					break;
				}
				wp.push_back(P);
			}
		}
	}
	return wp;
}