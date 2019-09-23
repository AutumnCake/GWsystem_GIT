#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

#include"MyFunction.h"


//��������
double MixDistanceGraylevel(Point PTemp,Mat GrayImage,int Max,double kx,double ky,double(*DistanceWeightGrayFunction)(double,int),vector<double>& Kt)
{
	Mat BiImage;
	threshold(GrayImage,BiImage,1,255,THRESH_BINARY);
	double MDG=0;//���ڷ��ؽ��
	//��������Nx Ny��ʾ
	double Nx=ky;
	double Ny=-kx;
	vector<double> kt(2,0);
	//��¼�Ǽ���ÿ���Ӧ��Ⱦɫ���ȣ��������С�����ȵ�һ��Max���������õ�
	if(Nx==0)
	{
		kt[0]=0;
		kt[1]=1;
		int Y1=PTemp.y-Max;
		int Y2=PTemp.y+Max;
		//bool OutPointofChrom1=false;
		//bool OutPointofChrom2=false;
		double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
		double DeP2=0;
		double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
		double MDG2=0;
		MDG=0;

		for(int i=0;i<=Max;i++)
			{
				//�㣨PTemp.x,Y1+i��
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(Y1+i,PTemp.x)!=0)
					MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(PTemp.x,Y1+i)),GrayImage.at<uchar>(Y1+i,PTemp.x));
			}
			for(int i=0;i<Max;i++)
			{
				//�㣨PTemp.x,Y2-i��
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(Y2-i,PTemp.x)!=0)
					MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(PTemp.x,Y2-i)),GrayImage.at<uchar>(Y2-i,PTemp.x));	
			}
			MDG=MDG1+MDG2;
		}

		if(Ny==0)
		{
			kt[0]=1;
			kt[1]=0;
			int X1=PTemp.x-Max;
			int X2=PTemp.x+Max;
			double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
			double DeP2=0;
			double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
			double MDG2=0;
			MDG=0;
			for(int i=0;i<=Max;i++)
			{
				//�㣨X1+i,PTemp.y��
				if(PTemp.y<0||PTemp.y>=GrayImage.size().height||X1+i<0||X1+i>=GrayImage.size().width) continue;
				if(BiImage.at<uchar>(PTemp.y,X1+i)!=0)
					MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X1+i,PTemp.y)),GrayImage.at<uchar>(PTemp.y,X1+i));
			}
			for(int i=0;i<Max;i++)
			{
				//�㣨X2-i,PTemp.y��
				if(PTemp.y<0||PTemp.y>=GrayImage.size().height||X2-i<0||X2-i>=GrayImage.size().width) continue;
				if(BiImage.at<uchar>(PTemp.y,X2-i)!=0)
					MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X2-i,PTemp.y)),GrayImage.at<uchar>(PTemp.y,X2-i));
			}
			MDG=MDG1+MDG2;
		}

		if(Nx!=0 && Ny!=0)
		{
			if(fabs(Ny/Nx)>1)
			{
				kt[0]=Nx/Ny;
				kt[1]=1;
				int Y1=PTemp.y-Max;
				int Y2=PTemp.y+Max;
				double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
				double DeP2=0;
				double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
				double MDG2=0;
				MDG=0;
				for(int i=0;i<=Max;i++)
				{
					int XTemp=((Y1+i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					//�㣨XTemp,Y1+i��
					if(XTemp<0||XTemp>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;
					if(BiImage.at<uchar>(Y1+i,XTemp)!=0)
						MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(XTemp,Y1+i)),GrayImage.at<uchar>(Y1+i,XTemp));
				}
				for(int i=0; i<Max;i++)
				{
					int XTemp=((Y2-i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					//�㣨XTemp,Y2-i��
					if(XTemp<0||XTemp>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
					if(BiImage.at<uchar>(Y2-i,XTemp)!=0)
						MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(XTemp,Y2-i)),GrayImage.at<uchar>(Y2-i,XTemp));
				}
				MDG=MDG1+MDG2;
			}
			else
			{
				//cout<<"a\n";
				kt[0]=1;
				kt[1]=Ny/Nx;
				int X1=PTemp.x-Max;
				int X2=PTemp.x+Max;
				double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
				double DeP2=0;
				double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
				double MDG2=0;
				MDG=0;
				
				for(int i=0;i<=Max;i++)
				{
					int YTemp=((X1+i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					//�㣨X1+i,YTemp��
					if(YTemp<0||YTemp>=GrayImage.size().height||X1+i<0||X1+i>=GrayImage.size().width) continue;
					if(BiImage.at<uchar>(YTemp,X1+i)!=0)
						MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X1+i,YTemp)),GrayImage.at<uchar>(YTemp,X1+i));
				}
				
				for(int i=0;i<Max;i++)
				{
					int YTemp=((X2-i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					//�㣨X2-i,YTemp��
					if(YTemp<0||YTemp>=GrayImage.size().height||X2-i<0||X2-i>=GrayImage.size().width) continue;
					if(BiImage.at<uchar>(YTemp,X2-i)!=0)
						MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X2-i,YTemp)),GrayImage.at<uchar>(YTemp,X2-i));
				}
				MDG=MDG1+MDG2;
			}
		}
		Kt.clear();
		Kt.insert(Kt.begin(),kt.begin(),kt.end());
	return MDG;
}

vector<double> MixDistanceGraylevel(vector<Point> ThinPoints,Mat GrayImage,int Max,double(*DistanceWeightGrayFunction)(double,int),vector<vector<double>>& Kv)
{
	Mat BiImage;
	threshold(GrayImage,BiImage,1,255,THRESH_BINARY);

	vector<double> MixDisGray;//���ڴ洢����õ��ĸ���Ļ����ֵ
	//����Ǽ���ÿ���б��
	vector<vector<double>> GradV;//���ڴ洢Dx��Dy
	vector<double> Grad(2,0);
	if(ThinPoints.size()<=5) 
	{
		cout<<" ��⵽����ϸС��Ƭ\n";
		return MixDisGray;//����һ��������
	}
	if(ThinPoints.size()<=2*5&&ThinPoints.size()>5) 
	{
		cout<<"�ӳ����ֳ�����ô���б����ֵ����Ӧ������\n";
		return MixDisGray;//����һ��������
	}
	if(ThinPoints.size()>5*2)
	{
		int GDis=5;
		for(int i=0;i<GDis;i++)
		{
			Grad[0]=ThinPoints[0+GDis-1].x-ThinPoints[0].x;
			Grad[1]=ThinPoints[0+GDis-1].y-ThinPoints[0].y;
			GradV.push_back(Grad);
		}

		for(int i=GDis;i<ThinPoints.size()-GDis;i++)
		{
			int Dx1Temp=ThinPoints[i+GDis-1].x-ThinPoints[i].x;
			int Dy1Temp=ThinPoints[i+GDis-1].y-ThinPoints[i].y;
			int Dx2Temp=ThinPoints[i-(GDis-1)].x-ThinPoints[i].x;
			int Dy2Temp=ThinPoints[i-(GDis-1)].y-ThinPoints[i].y;
			Grad[0]=2*Dx1Temp*Dx2Temp;
			Grad[1]=Dx1Temp*Dy2Temp+Dy1Temp*Dx2Temp;
			if(Grad[0]!=0||Grad[1]!=0) GradV.push_back(Grad);
			else
			{
				Grad[0]=ThinPoints[i+GDis-1].x-ThinPoints[i-(GDis-1)].x;
				Grad[1]=ThinPoints[i+GDis-1].y-ThinPoints[i-(GDis-1)].y;
				GradV.push_back(Grad);
			}
			//cout<<Grad[0]<<"\t"<<Grad[1]<<"\t"<<Grad[0]/Grad[1]<<"\n";

		}
		for(int i=ThinPoints.size()-1;i>ThinPoints.size()-GDis-1;i--)
		{
			Grad[0]=ThinPoints[ThinPoints.size()-GDis-1].x-ThinPoints[ThinPoints.size()-1].x;
			Grad[1]=ThinPoints[ThinPoints.size()-GDis-1].y-ThinPoints[ThinPoints.size()-1].y;
			GradV.push_back(Grad);
		}
	}
	//��ÿ���㴦�Ļ��ֵ
	
	for(int i=0;i<ThinPoints.size();i++)
	{
		double MDG=0;
		Point PTemp=ThinPoints[i];
		double kx=GradV[i][0];
		double ky=GradV[i][1];
		//��������Nx Ny��ʾ
		double Nx=ky;
		double Ny=-kx;
		vector<double> kt(2,0);
		//��¼�Ǽ���ÿ���Ӧ��Ⱦɫ���ȣ��������С�����ȵ�һ��Max���������õ�
		if(Nx==0)
		{
			kt[0]=0;
			kt[1]=1;
			//cout<<kt[0]<<kt[1]<<"\n";
			Kv.push_back(kt);
			int Y1=PTemp.y-Max;
			int Y2=PTemp.y+Max;
			//bool OutPointofChrom1=false;
			//bool OutPointofChrom2=false;
			double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
			double DeP2=0;
			double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
			double MDG2=0;
			MDG=0;

			for(int i=0;i<=Max;i++)
			{
				//�㣨PTemp.x,Y1+i��
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(Y1+i,PTemp.x)!=0)
					MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(PTemp.x,Y1+i)),GrayImage.at<uchar>(Y1+i,PTemp.x));
			}
			for(int i=0;i<Max;i++)
			{
				//�㣨PTemp.x,Y2-i��
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(Y2-i,PTemp.x)!=0)
					MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(PTemp.x,Y2-i)),GrayImage.at<uchar>(Y2-i,PTemp.x));	
			}
			MDG=MDG1+MDG2;
		}

		if(Ny==0)
		{
			kt[0]=1;
			kt[1]=0;
			Kv.push_back(kt);
			int X1=PTemp.x-Max;
			int X2=PTemp.x+Max;
			double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
			double DeP2=0;
			double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
			double MDG2=0;
			MDG=0;
			for(int i=0;i<=Max;i++)
			{
				//�㣨X1+i,PTemp.y��
				if(PTemp.y<0||PTemp.y>=GrayImage.size().height||X1+i<0||X1+i>=GrayImage.size().width) continue;
				if(BiImage.at<uchar>(PTemp.y,X1+i)!=0)
					MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X1+i,PTemp.y)),GrayImage.at<uchar>(PTemp.y,X1+i));
			}
			for(int i=0;i<Max;i++)
			{
				//�㣨X2-i,PTemp.y��
				if(PTemp.y<0||PTemp.y>=GrayImage.size().height||X2-i<0||X2-i>=GrayImage.size().width) continue;
				if(BiImage.at<uchar>(PTemp.y,X2-i)!=0)
					MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X2-i,PTemp.y)),GrayImage.at<uchar>(PTemp.y,X2-i));
			}
			MDG=MDG1+MDG2;
		}

		if(Nx!=0 && Ny!=0)
		{
			if(fabs(Ny/Nx)>1)
			{
				kt[0]=Nx/Ny;
				kt[1]=1;
				Kv.push_back(kt);
				int Y1=PTemp.y-Max;
				int Y2=PTemp.y+Max;
				double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
				double DeP2=0;
				double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
				double MDG2=0;
				MDG=0;
				for(int i=0;i<=Max;i++)
				{
					int XTemp=((Y1+i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					//�㣨XTemp,Y1+i��
					if(XTemp<0||XTemp>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;
					if(BiImage.at<uchar>(Y1+i,XTemp)!=0)
						MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(XTemp,Y1+i)),GrayImage.at<uchar>(Y1+i,XTemp));
				}
				for(int i=0; i<Max;i++)
				{
					int XTemp=((Y2-i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					//�㣨XTemp,Y2-i��
					if(XTemp<0||XTemp>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
					if(BiImage.at<uchar>(Y2-i,XTemp)!=0)
						MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(XTemp,Y2-i)),GrayImage.at<uchar>(Y2-i,XTemp));
				}
				MDG=MDG1+MDG2;
			}
			else
			{
				//cout<<"a\n";
				kt[0]=1;
				kt[1]=Ny/Nx;
				Kv.push_back(kt);
				int X1=PTemp.x-Max;
				int X2=PTemp.x+Max;
				double DeP1=0;//���ڼ�¼ÿ�㵽�����ߵľ��� distance every point
				double DeP2=0;
				double MDG1=0;//���ڼ�¼��������ÿ�㴦���߻Ҷ�����뺯���ܺ�
				double MDG2=0;
				MDG=0;
				
				for(int i=0;i<=Max;i++)
				{
					int YTemp=((X1+i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					//�㣨X1+i,YTemp��
					if(YTemp<0||YTemp>=GrayImage.size().height||X1+i<0||X1+i>=GrayImage.size().width) continue;
					if(BiImage.at<uchar>(YTemp,X1+i)!=0)
						MDG1+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X1+i,YTemp)),GrayImage.at<uchar>(YTemp,X1+i));
				}
				
				for(int i=0;i<Max;i++)
				{
					int YTemp=((X2-i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					//�㣨X2-i,YTemp��
					if(YTemp<0||YTemp>=GrayImage.size().height||X2-i<0||X2-i>=GrayImage.size().width) continue;
					if(BiImage.at<uchar>(YTemp,X2-i)!=0)
						MDG2+=DistanceWeightGrayFunction(DistancePoints(PTemp,Point(X2-i,YTemp)),GrayImage.at<uchar>(YTemp,X2-i));
				}
				MDG=MDG1+MDG2;
			}
		}
		MixDisGray.push_back(MDG);
	}
	return MixDisGray;
}

double MixDistanceGraylevel(Point PTemp,Mat GrayImage,int Max,double kx,double ky)
{
	Mat BiImage;
	threshold(GrayImage,BiImage,0,255,THRESH_BINARY);
	double DeP=0;
	//��������Nx Ny��ʾ
	double Nx=ky;
	double Ny=-kx;
	//��¼�Ǽ���ÿ���Ӧ��Ⱦɫ���ȣ��������С�����ȵ�һ��Max���������õ�
	if(Nx==0)
	{
		int Y1=PTemp.y-Max;
		int Y2=PTemp.y+Max;
		bool OutPointofChrom1=false;
		bool OutPointofChrom2=false;
		double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
		double DeP2=0;
		DeP=0;
		for(int i=0;!OutPointofChrom1&&i<=Max;i++)
		{
			//�㣨PTemp.x,Y1+i��
			if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;
			if(BiImage.at<uchar>(Y1+i,PTemp.x)!=0)
			{
				
				OutPointofChrom1=true;
				DeP1=DistancePoints(PTemp,Point(PTemp.x,Y1+i));
				break;
			}	
		}
		for(int i=0;!OutPointofChrom2&&i<=Max;i++)
		{
			if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
			//�㣨PTemp.x,Y2-i��
			if(BiImage.at<uchar>(Y2-i,PTemp.x)!=0)
			{
				
				OutPointofChrom2=true;
				DeP2=DistancePoints(PTemp,Point(PTemp.x,Y2-i));
				break;
			}	
		}
		DeP=DeP1+DeP2;
	}

	if(Ny==0)
	{
		
		int X1=PTemp.x-Max;
		int X2=PTemp.x+Max;
		bool OutPointofChrom1=false;
		bool OutPointofChrom2=false;
		double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
		double DeP2=0;
		DeP=0;
		for(int i=0;!OutPointofChrom1&&i<=Max;i++)
		{
			//�㣨X1+i,PTemp.y��
			if(X1+i<0||X1+i>=GrayImage.size().width||PTemp.y<0||PTemp.y>=GrayImage.size().height) continue;
			if(BiImage.at<uchar>(PTemp.y,X1+i)!=0)
			{
				OutPointofChrom1=true;
				DeP1=DistancePoints(PTemp,Point(X1+i,PTemp.y));
				break;
			}	
		}
		for(int i=0;!OutPointofChrom2&&i<=Max;i++)
		{
			if(X2-i<0||X2-i>=GrayImage.size().width||PTemp.y<0||PTemp.y>=GrayImage.size().height) continue;
			//�㣨X2-i,PTemp.y��
			if(BiImage.at<uchar>(PTemp.y,X2-i)!=0)
			{
				OutPointofChrom2=true;
				DeP2=DistancePoints(PTemp,Point(X2-i,PTemp.y));
				break;
			}	
		}
		DeP=DeP1+DeP2;
	}

	if(Nx!=0 && Ny!=0)
	{
		if(fabs(Ny/Nx)>1)
		{
			int Y1=PTemp.y-Max;
			int Y2=PTemp.y+Max;
			bool OutPointofChrom1=false;
			bool OutPointofChrom2=false;
			double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
			double DeP2=0;
			DeP=0;
			for(int i=0;!OutPointofChrom1&&i<=Max;i++)
			{
				int XTemp=((Y1+i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
				if(XTemp<0||XTemp>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;//�㣨XTemp,Y1+i��
				if(BiImage.at<uchar>(Y1+i,XTemp)!=0)
				{
					OutPointofChrom1=true;
					DeP1=DistancePoints(PTemp,Point(XTemp,Y1+i));
					break;
				}	
			}
			for(int i=0;!OutPointofChrom2&&i<=Max;i++)
			{
				int XTemp=((Y2-i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
				//�㣨XTemp,Y2-i��
				if(XTemp<0||XTemp>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(Y2-i,XTemp)!=0)
				{
					OutPointofChrom2=true;
					DeP2=DistancePoints(PTemp,Point(XTemp,Y2-i));
					break;
				}	
			}
			DeP=DeP1+DeP2;
		}
		else
		{
			//
			int X1=PTemp.x-Max;
			int X2=PTemp.x+Max;
			bool OutPointofChrom1=false;
			bool OutPointofChrom2=false;
			double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
			double DeP2=0;
			DeP=0;
			for(int i=0;!OutPointofChrom1&&i<=Max;i++)
			{
				int YTemp=((X1+i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
				//�㣨X1+i,YTemp��
				if(X1+i<0||X1+i>=GrayImage.size().width||YTemp<0||YTemp>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(YTemp,X1+i)!=0)
				{
					OutPointofChrom1=true;
					DeP1=DistancePoints(PTemp,Point(X1+i,YTemp));
					break;
				}	
			}
			for(int i=0;!OutPointofChrom2&&i<=Max;i++)
			{
				int YTemp=((X2-i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
				//�㣨X2-i,YTemp��
				if(X2-i<0||X2-i>=GrayImage.size().width||YTemp<0||YTemp>=GrayImage.size().height) continue;
				if(BiImage.at<uchar>(YTemp,X2-i)!=0)
				{
					OutPointofChrom2=true;
					DeP2=DistancePoints(PTemp,Point(X2-i,YTemp));
					break;
				}	
			}
			DeP=DeP1+DeP2;
		}
	}
	return DeP;
}

//��������
vector<double> MixDistanceGraylevel(vector<Point> ThinPoints,Mat GrayImage,int Max)
{
	Mat BiImage;
	threshold(GrayImage,BiImage,0,255,THRESH_BINARY);
	vector<double> WidthofChrom;//���ڴ洢����õ��ĸ���Ŀ��
	//����Ǽ���ÿ���б��
	vector<Point> GradPoint;//���ڴ洢Dx��Dy
	if(ThinPoints.size()<=5) 
	{
		cout<<" ��⵽����ϸС��Ƭ\n";
		return WidthofChrom;//����һ��������
	}
	if(ThinPoints.size()<=2*5&&ThinPoints.size()>5) 
	{
		cout<<"�ӳ����ֳ�����ô���б����ֵ����Ӧ������\n";
		return WidthofChrom;//����һ��������
	}
	if(ThinPoints.size()>5*2)
	{
		int GDis=5;
		for(int i=0;i<GDis;i++)
		{
			int DxTemp=ThinPoints[0+GDis-1].x-ThinPoints[0].x;
			int DyTemp=ThinPoints[0+GDis-1].y-ThinPoints[0].y;
			GradPoint.push_back(Point(DxTemp,DyTemp));
		}

		for(int i=GDis;i<ThinPoints.size()-GDis;i++)
		{
			int Dx1Temp=ThinPoints[i+GDis-1].x-ThinPoints[i].x;
			int Dy1Temp=ThinPoints[i+GDis-1].y-ThinPoints[i].y;
			int Dx2Temp=ThinPoints[i-(GDis-1)].x-ThinPoints[i].x;
			int Dy2Temp=ThinPoints[i-(GDis-1)].y-ThinPoints[i].y;
			int DxTemp=2*Dx1Temp*Dx2Temp;
			int DyTemp=Dx1Temp*Dy2Temp+Dy1Temp*Dx2Temp;
			GradPoint.push_back(Point(DxTemp,DyTemp));

		}
		for(int i=ThinPoints.size()-1;i>ThinPoints.size()-GDis-1;i--)
		{
			int DxTemp=ThinPoints[ThinPoints.size()-GDis-1].x-ThinPoints[ThinPoints.size()-1].x;
			int DyTemp=ThinPoints[ThinPoints.size()-GDis-1].y-ThinPoints[ThinPoints.size()-1].y;
			GradPoint.push_back(Point(DxTemp,DyTemp));
		}
	}
	//��ÿ���㴦�Ŀ��

	for(int i=0;i<ThinPoints.size();i++)
	{
		double DeP=0;
		Point PTemp=ThinPoints[i];
		double kx=GradPoint[i].x;
		double ky=GradPoint[i].y;
		//��������Nx Ny��ʾ
		double Nx=ky;
		double Ny=-kx;
		//��¼�Ǽ���ÿ���Ӧ��Ⱦɫ���ȣ��������С�����ȵ�һ��Max���������õ�
		if(Nx==0)
		{
			int Y1=PTemp.y-Max;
			int Y2=PTemp.y+Max;
			bool OutPointofChrom1=false;
			bool OutPointofChrom2=false;
			double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
			double DeP2=0;
			DeP=0;
			for(int i=0;!OutPointofChrom1&&i<=Max;i++)
			{
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;//�㣨PTemp.x,Y1+i��
				if(BiImage.at<uchar>(Y1+i,PTemp.x)!=0)
				{
					OutPointofChrom1=true;
					DeP1=DistancePoints(PTemp,Point(PTemp.x,Y1+i));
					break;
				}	
			}
			for(int i=0;!OutPointofChrom2&&i<=Max;i++)
			{
				if(PTemp.x<0||PTemp.x>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;//�㣨PTemp.x,Y2-i��
				if(BiImage.at<uchar>(Y2-i,PTemp.x)!=0)
				{
					OutPointofChrom2=true;
					DeP2=DistancePoints(PTemp,Point(PTemp.x,Y2-i));
					break;
				}	
			}
			DeP=DeP1+DeP2;
		}

		if(Ny==0)
		{
		
			int X1=PTemp.x-Max;
			int X2=PTemp.x+Max;
			bool OutPointofChrom1=false;
			bool OutPointofChrom2=false;
			double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
			double DeP2=0;
			DeP=0;
			for(int i=0;!OutPointofChrom1&&i<=Max;i++)
			{
				if(X1+i<0||X1+i>=GrayImage.size().width||PTemp.y<0||PTemp.y>=GrayImage.size().height) continue;//�㣨X1+i,PTemp.y��
				if(BiImage.at<uchar>(PTemp.y,X1+i)!=0)
				{
					OutPointofChrom1=true;
					DeP1=DistancePoints(PTemp,Point(X1+i,PTemp.y));
					break;
				}	
			}
			for(int i=0;!OutPointofChrom2&&i<=Max;i++)
			{
				if(X2-i<0||X2-i>=GrayImage.size().width||PTemp.y<0||PTemp.y>=GrayImage.size().height) continue;//�㣨X2-i,PTemp.y��
				if(BiImage.at<uchar>(PTemp.y,X2-i)!=0)
				{
					OutPointofChrom2=true;
					DeP2=DistancePoints(PTemp,Point(X2-i,PTemp.y));
					break;
				}	
			}
			DeP=DeP1+DeP2;
		}

		if(Nx!=0 && Ny!=0)
		{
			if(fabs(Ny/Nx)>1)
			{
				int Y1=PTemp.y-Max;
				int Y2=PTemp.y+Max;
				bool OutPointofChrom1=false;
				bool OutPointofChrom2=false;
				double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
				double DeP2=0;
				DeP=0;
				for(int i=0;!OutPointofChrom1&&i<=Max;i++)
				{
					int XTemp=((Y1+i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					if(XTemp<0||XTemp>=GrayImage.size().width||Y1+i<0||Y1+i>=GrayImage.size().height) continue;//�㣨XTemp,Y1+i��
					if(BiImage.at<uchar>(Y1+i,XTemp)!=0)
					{
						OutPointofChrom1=true;
						DeP1=DistancePoints(PTemp,Point(XTemp,Y1+i));
						break;
					}	
				}
				for(int i=0;!OutPointofChrom2&&i<=Max;i++)
				{
					int XTemp=((Y2-i)*Nx-PTemp.y*Nx+PTemp.x*Ny)/Ny;
					if(XTemp<0||XTemp>=GrayImage.size().width||Y2-i<0||Y2-i>=GrayImage.size().height) continue;//�㣨XTemp,Y2-i��
					if(BiImage.at<uchar>(Y2-i,XTemp)!=0)
					{
						OutPointofChrom2=true;
						DeP2=DistancePoints(PTemp,Point(XTemp,Y2-i));
						break;
					}	
				}
				DeP=DeP1+DeP2;
			}
			else
			{
				//
				int X1=PTemp.x-Max;
				int X2=PTemp.x+Max;
				bool OutPointofChrom1=false;
				bool OutPointofChrom2=false;
				double DeP1=0;//���ڼ�¼ÿ�㴦���߾���
				double DeP2=0;
				DeP=0;
				for(int i=0;!OutPointofChrom1&&i<=Max;i++)
				{
					int YTemp=((X1+i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					if(X1+i<0||X1+i>=GrayImage.size().width||YTemp<0||YTemp>=GrayImage.size().height) continue;//�㣨X1+i,YTemp��
					if(BiImage.at<uchar>(YTemp,X1+i)!=0)
					{
						OutPointofChrom1=true;
						DeP1=DistancePoints(PTemp,Point(X1+i,YTemp));
						break;
					}	
				}
				for(int i=0;!OutPointofChrom2&&i<=Max;i++)
				{
					int YTemp=((X2-i)*Ny-PTemp.x*Ny+PTemp.y*Nx)/Nx;
					if(X2-i<0||X2-i>=GrayImage.size().width||YTemp<0||YTemp>=GrayImage.size().height) continue;//�㣨X2-i,YTemp��
					if(BiImage.at<uchar>(YTemp,X2-i)!=0)
					{
						OutPointofChrom2=true;
						DeP2=DistancePoints(PTemp,Point(X2-i,YTemp));
						break;
					}	
				}
				DeP=DeP1+DeP2;
			}
		}
		WidthofChrom.push_back(DeP);
	}
	return WidthofChrom;
}
