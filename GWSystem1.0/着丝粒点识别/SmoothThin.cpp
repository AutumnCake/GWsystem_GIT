/*

*/


#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

#include"CR.h"
#include"DeleteInterferencePointONThin.h"


//LD 表示线段长度
void SmoothThin(Mat& thinImage,int LD,Point& BP,Point& EP,bool BeginfromB)
{
	DeleteInterferencePointONThin(thinImage);
	vector<Point> BPv=CR(thinImage,BP,1);
	vector<Point> EPv=CR(thinImage,EP,1);
	//在thinImage上寻找端点P
	vector<Point> BEP;
	if(BPv.size()!=1||EPv.size()!=1)
	{
		for (int i=0;i<thinImage.rows;i++) //第i行
		{
			for(int j=0;j<thinImage.cols;j++)//第j列
			{
				if(thinImage.at<uchar>(i,j)>0)
				{
					vector<Point> Pv=CR(thinImage,Point(j,i),1);
					if(Pv.size()==1)
						BEP.push_back(Point(j,i));
				}
			}
		}
	}
	if(BEP.size()!=2) return;
	else
	{
		BP=BEP[0];
		EP=BEP[1];
	}
	Mat ThinTemp=Mat::zeros(thinImage.size(),CV_8UC1);
	Point LB;
	Point temp;
	Point LEP;

	if(BeginfromB)
	{
		LB=BP;//线段起点
		temp = BP;
		LEP=EP;
	}
	else
	{
		LB=EP;//线段起点
		temp = EP;
		LEP=BP;
	}
	int EX=0;
	int EY=0;
	int m=0,n=0;
	  
    m = temp.x; //row 
    n = temp.y;  //col
	int num=0;
	while(1)
	{
		
		if (m > 0)  
		{  
			if (thinImage.at<uchar>(n,m-1) == 255) 
			{
				if(!((m-1==EX)&&(n==EY)))
				{
					
					temp.x = m - 1;  
					temp.y = n;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					
					//continue;
					//cout<<"left\n";
				}
					
			}  
		} 
		
		if (m >0 && n > 0)  
		{  
			if (thinImage.at<uchar>(n-1,m-1) == 255)//  
			{ 
				if(!((m-1==EX)&&(n-1==EY)))
				{/*cout<<"大傻逼\n";*/
					temp.x = m - 1;  
					temp.y = n - 1;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"upleft\n";
				}
					
			}  
		}  
  
		if(m<thinImage.cols-1)  
		{  
			if(thinImage.at<uchar>(n,m+1)==255)//
			{ 
				if(!((m+1==EX)&&(n==EY)))
				{
					temp.x=m+1;  
					temp.y=n;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"right\n";
				}
			}  
		}  
  
		if (m < (thinImage.cols - 1) && n < (thinImage.rows - 1))  
		{  
			if(thinImage.at<uchar>(n+1,m+1)==255)//  
			{ 
				if(!((m+1==EX)&&(n+1==EY)))
				{
					temp.x=m+1;  
					temp.y=n+1;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"downright\n";
				}
			}  
		}  
  
		if(n<thinImage.rows-1)  
		{  
			if(thinImage.at<uchar>(n+1,m)==255)//
			{
				if(!((m==EX)&&(n+1==EY)))
				{
					temp.x=m;  
					temp.y=n+1;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"down\n";
				}
               
			}  
		}  
  
		if(m>0&&n<(thinImage.rows-1))  
		{  
			if(thinImage.at<uchar>(n+1,m-1)==255)//  
			{
				if(!((m-1==EX)&&(n+1==EY)))
				{
					
					temp.x=m-1;  
					temp.y=n+1;  
			        num++;
					//cout<<num<<"\t"<<!(num%LD)<<"\t"<<m<<n<<"\t"<<EX<<EY<<"\n";
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
						//cout<<"大傻逼\n";
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					
					//continue;
					//cout<<"downleft\n";
				}
			}  
		}  
  
		if(n>0)  
		{  
			if(thinImage.at<uchar>(n-1,m)==255)// 
			{ 
				if(!((m==EX)&&(n-1==EY)))
				{
					temp.x=m;  
					temp.y=n-1;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"up\n";
					////cout<<temp<<"\n";

				}
			}  
		}  
  
		if(m<(thinImage.cols-1)&&n>0)  
		{   
			if(thinImage.at<uchar>(n-1,m+1)==255)//  
			{  
				if(!((m+1==EX)&&(n-1==EY)))
				{
					temp.x=m+1;  
					temp.y=n-1;  
					num++;
					if(!(num%LD))
					{
						line(ThinTemp,LB,temp,Scalar(255),1,8);
						LB=temp;
					}
					if(temp==LEP)
					{
						line(ThinTemp,LB,LEP,Scalar(255),1,8);
						break;
					}
					EX=m;
		            EY=n;
				    m=temp.x;n=temp.y;
					//continue;
					//cout<<"upright\n";
				}
			}  
		}

	}
	//与void Point2Vector(Mat thinImage,vector<Point>& ThinPoints,Point P)相同
	//imshow("ThinTemp",ThinTemp);
	thinImage=ThinTemp.clone();
	DeleteInterferencePointONThin(thinImage);
	return ;
}