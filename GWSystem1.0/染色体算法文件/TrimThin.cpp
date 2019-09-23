/*mtf 2018.4.19修改p1n>=3*255条件为p1n>=3*255&&至少三点不相邻*/

#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

#include"D8ConnectedComponent.h"
#include"CR.h"
#include"BoolSort.h"
//#include"ThinImage.h"
#include"MyFunction.h"
#include"SmoothThin.h"
#include"DeleteInterferencePointONThin.h"


void TrimThin(Mat& srcimage)
{
	int Zonenum=0;
	Mat Label=Mat::zeros(srcimage.size(),CV_8UC1);
	D8ConnectedComponent(srcimage,Label,Zonenum);
	//cout<<Zonenum;
	//修补骨架断裂处
	if(Zonenum>1)
	{
		for (int i=0;i<srcimage.rows;i++) //第i行
		{
			for(int j=0;j<srcimage.cols;j++)//第j列
			{
				if(srcimage.at<uchar>(i,j)>0)
				{
					vector<Point> p1=CR(srcimage,Point(j,i),1);
					vector<Point> p2=CR(srcimage,Point(j,i),2);
					
					
					if(p1.size()==1)
					{
						if(p2.size()>=2)
						{
							bool NConnected=false;
							for(int P2N=0;P2N<p2.size();P2N++)
							{
								//cout<<NConnected<<"\t"<<(int)Label.at<uchar>(i,j)<<"\t"<<(int)Label.at<uchar>(p2[P2N].y,p2[P2N].x)<<"\n";
								NConnected=(NConnected||Label.at<uchar>(i,j)!=Label.at<uchar>(p2[P2N].y,p2[P2N].x));
								//cout<<NConnected<<"\n";
								if(NConnected)
								{
									 line(srcimage,Point(j,i),p2[P2N],Scalar(255),1,8,0);
									 break;
								}
							}
						}
					}
				}
			}
		}
	}
	
	//找分叉点,并打散分枝
	vector<Point> terminalPoint;//端点
	vector<Point> crunodePoint;//叉点
	for (int i=0;i<srcimage.rows;i++) //第i行
	{
		for(int j=0;j<srcimage.cols;j++)//第j列
		{
			if(srcimage.at<uchar>(i,j)>0)
			{
				vector<Point> p1=CR(srcimage,Point(j,i),1);		
				if(p1.size()==1)
				{
					terminalPoint.push_back(Point(j,i));
				}
				if(p1.size()>=3)
				{
					crunodePoint.push_back(Point(j,i));
				}

			}

		}
	}
	
	//打散
	for(int i=0;i<crunodePoint.size();i++)
	{
		srcimage.at<uchar>(crunodePoint[i].y,crunodePoint[i].x)=0;
		int x=crunodePoint[i].x;
		int y=crunodePoint[i].y;
		int r=1;
		for(int i=-r;i<r;i++)
		{
			if(x+i>=0&&y-r>=0&&x+i<srcimage.size().width&&y-r<srcimage.size().height) srcimage.at<uchar>(y-r,x+i)=0;
			if(x+r>=0&&y+i>=0&&x+r<srcimage.size().width&&y+i<srcimage.size().height) srcimage.at<uchar>(y+i,x+r)=0;
			if(x-i>=0&&y+r>=0&&x-i<srcimage.size().width&&y+r<srcimage.size().height) srcimage.at<uchar>(y+r,x-i)=0;
			if(x-r>=0&&y-i>=0&&x-r<srcimage.size().width&&y-i<srcimage.size().height) srcimage.at<uchar>(y-i,x-r)=0;
		}
	}
	
	//cout<<crunodePoint.size()<<"\t"<<terminalPoint.size()<<"\n";
	//cout<<"a\n";
	//if(crunodePoint.size()==0) return;//没有枝丫，直接返回。//不能返回，需要去除多余的点
	if(terminalPoint.size()==4)
	{
		
		for(int i=0;i<terminalPoint.size();i++)
		{
			//circle(image,terminalPoint[i],0.5,Scalar(0,0,255),-1,8);
			Rect ccomp;  
			floodFill(srcimage, terminalPoint[i], Scalar(0), &ccomp, Scalar(0), Scalar(0), 8); 
			//srcimage.at<uchar>(p1v[i].y,p1v[i].x)=255;
		}
		bool Nempty=false;
		for (int i=0;i<srcimage.rows;i++) //第i行
		{
			for(int j=0;j<srcimage.cols;j++)//第j列
			{
				if(srcimage.at<uchar>(i,j)>0)
				{
					Nempty=true;
					break;
				}
			}
		}
		if(!Nempty&&crunodePoint.size()==2) line(srcimage,crunodePoint[0],crunodePoint[1],Scalar(255),1,8);
		else if(!Nempty&&crunodePoint.size()==1) 
		{
			crunodePoint.push_back(crunodePoint[0]);
			srcimage.at<uchar>(crunodePoint[0].y,crunodePoint[0].x)=255;
		}
		else 
		{
			crunodePoint.clear();
			for (int i=0;i<srcimage.rows;i++) //第i行
			{
				for(int j=0;j<srcimage.cols;j++)//第j列
				{
					if(srcimage.at<uchar>(i,j)>0)
					{
						vector<Point> p1=CR(srcimage,Point(j,i),1);	
						if(p1.size()==1)
						{
							crunodePoint.push_back(Point(j,i));
						}
				
					}

				}
			}
		}
		if(crunodePoint.size()==1) crunodePoint.push_back(crunodePoint[0]);
		
		sort(terminalPoint.begin(),terminalPoint.end(),lessY);
		sort(terminalPoint.begin(),terminalPoint.begin()+2,lessX);
		sort(terminalPoint.begin()+2,terminalPoint.end(),greaterX);
		//cout<<"大傻逼\n";
		if(crunodePoint.size()==2)
		{
			if(DistancePoints(terminalPoint[0],terminalPoint[1])+DistancePoints(terminalPoint[2],terminalPoint[3])<DistancePoints(terminalPoint[1],terminalPoint[2])+DistancePoints(terminalPoint[0],terminalPoint[3]))
			{
				Point pt1,pt2;
				pt1.x=(terminalPoint[0].x+terminalPoint[1].x)/2;pt1.y=(terminalPoint[0].y+terminalPoint[1].y)/2;
				pt2.x=(terminalPoint[2].x+terminalPoint[3].x)/2;pt2.y=(terminalPoint[2].y+terminalPoint[3].y)/2;
				if(DistancePoints(crunodePoint[0],pt1)<DistancePoints(crunodePoint[1],pt1))
				{
					line(srcimage,crunodePoint[0],pt1,Scalar(255),1,8);
					line(srcimage,crunodePoint[1],pt2,Scalar(255),1,8);
				}
				else
				{
					line(srcimage,crunodePoint[1],pt1,Scalar(255),1,8);
					line(srcimage,crunodePoint[0],pt2,Scalar(255),1,8);
				}
			}
			else
			{
				Point pt1,pt2;
				pt1.x=(terminalPoint[1].x+terminalPoint[2].x)/2;pt1.y=(terminalPoint[1].y+terminalPoint[2].y)/2;
				pt2.x=(terminalPoint[0].x+terminalPoint[3].x)/2;pt2.y=(terminalPoint[0].y+terminalPoint[3].y)/2;
				if(DistancePoints(crunodePoint[0],pt1)<DistancePoints(crunodePoint[1],pt1))
				{
					line(srcimage,crunodePoint[0],pt1,Scalar(255),1,8);
					line(srcimage,crunodePoint[1],pt2,Scalar(255),1,8);
				}
				else
				{
					line(srcimage,crunodePoint[1],pt1,Scalar(255),1,8);
					line(srcimage,crunodePoint[0],pt2,Scalar(255),1,8);
				}
			}
		}
		else
		{
			if(DistancePoints(terminalPoint[0],terminalPoint[1])+DistancePoints(terminalPoint[2],terminalPoint[3])<DistancePoints(terminalPoint[1],terminalPoint[2])+DistancePoints(terminalPoint[0],terminalPoint[3]))
			{
				Point pt1,pt2;
				pt1.x=(terminalPoint[0].x+terminalPoint[1].x)/2;pt1.y=(terminalPoint[0].y+terminalPoint[1].y)/2;
				pt2.x=(terminalPoint[2].x+terminalPoint[3].x)/2;pt2.y=(terminalPoint[2].y+terminalPoint[3].y)/2;
				line(srcimage,pt1,pt2,Scalar(255),1,8);
			}
			else
			{
				Point pt1,pt2;
				pt1.x=(terminalPoint[1].x+terminalPoint[2].x)/2;pt1.y=(terminalPoint[1].y+terminalPoint[2].y)/2;
				pt2.x=(terminalPoint[0].x+terminalPoint[3].x)/2;pt2.y=(terminalPoint[0].y+terminalPoint[3].y)/2;
				line(srcimage,pt1,pt2,Scalar(255),1,8);
			}
		}
	}
	//cout<<"b\n";
	//cout<<p1v.size();
	if(terminalPoint.size()==3)
	{
		//sort(p1v.begin(),p1v.end(),lessY);
		//sort(p1v.begin(),p1v.begin()+2,lessX);
		Point pt;
		int MinD=0;
		int flage=0;
		vector<Point> trimpoint(2);
		if(DistancePoints(terminalPoint[0],terminalPoint[1])<DistancePoints(terminalPoint[1],terminalPoint[2]))
		{
			MinD=DistancePoints(terminalPoint[0],terminalPoint[1]);
			pt.x=(terminalPoint[0].x+terminalPoint[1].x)/2;pt.y=(terminalPoint[0].y+terminalPoint[1].y)/2;
			trimpoint[0]=terminalPoint[0];
			trimpoint[1]=terminalPoint[1];
		}
		else
		{
			MinD=DistancePoints(terminalPoint[1],terminalPoint[2]);
			pt.x=(terminalPoint[1].x+terminalPoint[2].x)/2;pt.y=(terminalPoint[1].y+terminalPoint[2].y)/2;
			trimpoint[0]=terminalPoint[1];
			trimpoint[1]=terminalPoint[2];
		}
		if(MinD>DistancePoints(terminalPoint[0],terminalPoint[2]))
		{
			pt.x=(terminalPoint[0].x+terminalPoint[2].x)/2;pt.y=(terminalPoint[0].y+terminalPoint[2].y)/2;
			trimpoint[0]=terminalPoint[0];
			trimpoint[1]=terminalPoint[2];
		}
		
		for(int i=0;i<trimpoint.size();i++)
		{
			Rect ccomp;  
			floodFill(srcimage, trimpoint[i], Scalar(0), &ccomp, Scalar(0), Scalar(0), 8); 
			//srcimage.at<uchar>(trimpoint[i].y,trimpoint[i].x)=255;
		}
		crunodePoint.clear();
		for (int i=0;i<srcimage.rows;i++) //第i行
		{
			for(int j=0;j<srcimage.cols;j++)//第j列
			{
				if(srcimage.at<uchar>(i,j)>0)
				{
					vector<Point> p1=CR(srcimage,Point(j,i),1);	
					bool IFterminalPoint=false;
					for(int IF=0;IF<terminalPoint.size();IF++)
					{
						IFterminalPoint=(IFterminalPoint||(Point(j,i)==terminalPoint[IF]));
					}
					if(p1.size()==1&&!IFterminalPoint)
					{
						crunodePoint.push_back(Point(j,i));
					}
				
				}

			}
		}

	   line(srcimage,crunodePoint[0],pt,Scalar(255),1,8);

	}

	if(terminalPoint.size()>4)//待验证
	{
		D8ConnectedComponent(srcimage,Label,Zonenum);
		vector<int> interest(Zonenum);
		for (int i=0;i<Label.rows;i++) //第i行
		{
			for(int j=0;j<Label.cols;j++)//第j列
			{
				if(Label.at<uchar>(i,j)>0)
				interest[Label.at<uchar>(i,j)-1]++;
			}
		}
		int max=0;
		int interestlabel=0;
		for(int i=0;i<Zonenum;i++)
		{
			if(max<interest[i])
			{
				max=interest[i];
				interestlabel=i;
			}
		}
		for (int i=0;i<Label.rows;i++) //第i行
		{
			for(int j=0;j<Label.cols;j++)//第j列
			{
				if(Label.at<uchar>(i,j)!=interestlabel+1)
				srcimage.at<uchar>(i,j)=0;
			}
		}

	}

	//Mat img=Mat::zeros(srcimage.size(),CV_8UC3);
	//int r=10;
	//Point pre(-1,-1);
	//Point c=be[0];
	////int num=0;
	//while(1)
	//{ 
	//	vector<Point> pr=CR(srcimage,c,r);
	//	if(pr[0]!=pre&&img.at<Vec3b>(pr[0].y,pr[0].x)!=Vec3b(255,0,0)) { line(img,c,pr[0],Scalar(255,0,0),1,8);pre=c;c=pr[0];}
	//	else if(pr.size()>=2&&img.at<Vec3b>(pr[1].y,pr[1].x)!=Vec3b(255,0,0)){ line(img,c,pr[1],Scalar(255,0,0),1,8);pre=c;c=pr[1];}
	//	else 
	//	{
	//		line(img,c,be[1],Scalar(255,0,0),1,8);
	//		break;
	//	}
	//	//num++;
	//	//cout<<c<<"\n";

	//}
	//Mat Gray;
	//cvtColor(img,Gray,CV_RGB2GRAY);
	//threshold( Gray,srcimage,0,255,THRESH_BINARY);
	
	//圆滑曲线
	
	int LD=10;
	int LD2=15;
	SmoothThin(srcimage,LD,Point(0,0),Point(0,0));
	//SmoothThin(srcimage,LD2,Point(0,0),Point(0,0),false);
	

	Mat image=Mat::zeros(srcimage.size(),CV_8UC3);//便于观察
	for (int i=0;i<srcimage.rows;i++) //第i行
	{
		for(int j=0;j<srcimage.cols;j++)//第j列
		{
			if(srcimage.at<uchar>(i,j)>0)
				image.at<Vec3b>(i,j)=Vec3b(0,0,255);

		}
	}
	//imshow("srcimage",image);


	 
}