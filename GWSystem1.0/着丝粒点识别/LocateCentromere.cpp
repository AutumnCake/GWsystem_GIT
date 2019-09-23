// 着丝粒点识别.cpp : 定义控制台应用程序的入口点。
//

#include "..\stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"



using namespace std;
using namespace cv;

#define PI 3.1415926

#include"ThinImage.h"
#include"..\染色体算法文件\NonezeroMeanGrayValue.h"
#include"Point2Vector.h"
#include"..\染色体算法文件\MyFunction.h"
#include"LineFitLeastSquares.h"
#include"ExtendThinImage.h"
#include"MixDistanceGraylevel.h"
#include"BlankFrame.h"
#include"TrimThin.h"
#include"FilteringMethod.h"
#include"SmoothThin.h"

#include"..\染色体算法文件\ClassClusterInfo.h"
#include"DeleteInterferencePointONThin.h"



void LocateCentromere(vector<ClassClusterInfo>& SCCI)
{
	if(SCCI.size()==0) return;
	for(int SCCIn=0;SCCIn<SCCI.size();SCCIn++)
	//for(int SCCIn=27;SCCIn<28;SCCIn++)
	//int SCCIn=27;
	{
		//cout<<"SCCIn\t"<<SCCIn<<"\n";
		Mat image=SCCI[SCCIn].GetClusterImage();
		if(image.empty()) continue;
		int BlankFrameWidth=20;
		AddBlankFrame(image,BlankFrameWidth);
		//SubBlankFrame(image,BlankFrameWidth);
		//转化为灰度图
		Mat gray,Bi;
		cvtColor(image,gray,CV_RGB2GRAY);
		threshold(gray,Bi,0,255,THRESH_BINARY);
		//imshow("二值图",Bi);
		Mat BiTemp=Bi.clone();
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//核2 结构元素
		dilate(BiTemp,BiTemp,element);
		//填孔，并提取外轮廓，此处填孔是为了防止提取骨架时候出现孔状部分
		vector<vector<Point> > contours_out;  
		vector<Vec4i> hierarchy_out; 
		Mat BiContours=BiTemp.clone();
		findContours(BiContours, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE); 
		drawContours(BiTemp,contours_out,0,Scalar(255),-1,8,hierarchy_out);

		//最小矩形中线做骨架
		vector<Point> ThinPoints;//用于依次存储骨架点
		double Kx=0,Ky=0;//用于存储骨架斜率
		bool K_Rect=false;//用于标示是否使用最小矩形求骨架
		RotatedRect minRect;
		if(contours_out.size()==0) continue;
		else minRect=minAreaRect(contours_out[0]);
		
		Point2f rect_points[4];
		double d1=0; //最小外接矩形的两个边长分别为d1 d2
		double d2=0;
		double w=0;//外接矩形的宽度，即d1 d2中较小者
		double l=0;
		bool wd01=true;//点01所在的边是宽度
		double areaRatio=0;//染色体与外接最小矩形的面积比
		minRect.points(rect_points);
		d1=DistancePoints(rect_points[0],rect_points[1]);
		d2=DistancePoints(rect_points[1],rect_points[2]);
		w=(d1<d2? d1:d2);
		l=(d1>d2? d1:d2);
		wd01=(d1<d2? true:false);
	
		areaRatio=double(contourArea(contours_out[0]))/double(d1*d2);
		Mat thinImage=Mat::zeros(image.size(),CV_8U);
		double EmpiricalAreaRatio=0.8;//该经验面积比值取决于弯曲程度
		Point PBeginofPoint2Vector(0,0);
		Point PEndofPoint2Vector(0,0);
		double lwRatio=l/w;
		int  Inward=15;//推断点向内侧内移Inward
		if(areaRatio>EmpiricalAreaRatio||lwRatio<1.5)//
		{
		

			Point P1,P2;
			if(wd01==true)
			{
				P1=Point((rect_points[0].x+rect_points[1].x)/2.0,(rect_points[0].y+rect_points[1].y)/2.0);
				P2=Point((rect_points[2].x+rect_points[3].x)/2.0,(rect_points[2].y+rect_points[3].y)/2.0);
			 
			}
			else
			{
				P1=Point((rect_points[0].x+rect_points[3].x)/2.0,(rect_points[0].y+rect_points[3].y)/2.0);
				P2=Point((rect_points[2].x+rect_points[1].x)/2.0,(rect_points[2].y+rect_points[1].y)/2.0);
			}
			line(thinImage, P1,P2,Scalar(255), 1,8);
			//circle(thinImage, P1, 3, Scalar(255, 255, 255), -1, 8); //画圆，在图上显示点击的点 
			//记录斜率，并标示K_Rect
			K_Rect=true;
			Kx=(double)(P2.x-P1.x);
			Ky=(double)(P2.y-P1.y);
			//cout<<K_Thin<<":"<<(double)(P2.y-P1.y)<<"\t"<<(double)(P2.x-P1.x)<<"\n";
			//将直线上的点由P1到P2依次存储在ThinPoints中
			//cout<<"d0\n";
			Point2Vector(thinImage,ThinPoints,P1);
		
			PBeginofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[Inward-1]:P1);//P1端
			PEndofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[ThinPoints.size()-Inward]:P2);
			//cout<<"d1\n";
		}
		else
		{
	
			thinImage=ThinImage(BiTemp);
			////imshow("骨架",thinImage);
			TrimThin(thinImage);
			//
			//延长骨架并重新存储，改善骨架变短的情况
			vector<Point> ThinPointsTemp;
		   //将骨架上的点依次存储在ThinPoints中
			Point2Vector(thinImage,ThinPointsTemp);
		
			//imshow("原始骨架",thinImage);
			if(ThinPointsTemp.size()>=15)
			{
				ExtendThinImage(thinImage,10,20,0);
				ExtendThinImage(thinImage,10,20,1);
			}
			if(ThinPointsTemp.size()<15&&ThinPointsTemp.size()>=8)
			{
				ExtendThinImage(thinImage,5,10,0);
				ExtendThinImage(thinImage,5,10,1);
			}
			SmoothThin(thinImage,20);//圆滑延长后的曲线
			if(ThinPointsTemp.size()<8)
			{
				//待完善
				cout<<SCCIn<<":考虑是环或者微小断片\n";
				continue;
			}
			//imshow("延长骨架",thinImage);
			Point2Vector(thinImage,ThinPoints);
		
			PBeginofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[Inward-1]:ThinPoints[0]);
			PEndofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[ThinPoints.size()-Inward]:ThinPoints[ThinPoints.size()-1]);

		}
		//imshow("骨架",thinImage);
		
		vector<double> ResultofChrom;
		vector<Point> PV;
		vector<vector<double>> Kv;
		if(K_Rect)//此时骨架上每一点的斜率都已知，且可以用Ky、Kx表示
		{
			for(int i=0;i<ThinPoints.size();i++)
			{
				vector<double> Kt;
				double GDCTP= MixDistanceGraylevel(ThinPoints[i],gray,w/2,Kx,Ky,DistanceWeightGrayFunction,Kt);
				//double GDCTP= MixDistanceGraylevel(ThinPoints[i],gray,w/2,Kx,Ky);
				//double GDCTP= GraylevelDistributionofChromOnThinPoints(ThinPoints[i],gray,w/2,Kx,Ky);
				ResultofChrom.push_back(GDCTP);
				Kv.push_back(Kt);
			}	
		}
		else//当没有用最小矩形求骨架时，即腐蚀出的骨架情况
		{
		
			ResultofChrom=MixDistanceGraylevel(ThinPoints,gray,w/2,DistanceWeightGrayFunction,Kv);
			//ResultofChrom=MixDistanceGraylevel(ThinPoints,gray,w/2);
			//GraylevelDistributionofChrom= GraylevelDistributionofChromOnThinPoints(ThinPoints,gray,w/2);
		}
		//适应窗口，便于显示，归一化到某个高度进行显示
		double MaxResult=0;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			if(ResultofChrom[i]>MaxResult)
				MaxResult=ResultofChrom[i];
		}
		double WinHeight=700;//窗口高度
		double ratio2show=MaxResult/WinHeight;
		vector<double> Result2show;
		//for(int i=0;i<ResultofChrom.size();i++)
		//{
		//	Result2show.push_back(ResultofChrom[i]/ratio2show);
		//	//cout<<Result2show[i]<<"\n";
		//}
		//Mat ResultCurve=Mat::zeros(1000,2000,CV_8UC3);
		//Mat RC=Mat::zeros(1000,2000,CV_8UC3);
		//circle(ResultCurve, Point(0,Result2show[0]), 3, Scalar(255, 0, 255), -1, 8);
		//for(int i=1;i<Result2show.size();i++)
		//{
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(255, 0, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,255,255), 1,8);
		//}
		//imshow("灰度曲线图",RC);
		////平滑灰度曲线图
	
		// int T=1;//拟合曲线，拟合近似次数。次数越多，越光滑
		// for(int j=0;j<T;j++)
		// {
		//	 ResultofChrom[1]=0;
		//	 ResultofChrom[0]=0;

		//	  for( int i = 2; i < ResultofChrom.size()-2; i++ )  
		//		{  
		//				ResultofChrom[i]=(ResultofChrom[i-2]+ResultofChrom[i-1]+ResultofChrom[i]+ResultofChrom[i+1]+ResultofChrom[i+2])/5.0;
		//
		//		}
		//	 ResultofChrom[ ResultofChrom.size()-2]=0;
		//	 ResultofChrom[ ResultofChrom.size()-1]=0;
		// }

		// for(int i=0;i<ResultofChrom.size();i++)
		//{
		//	Result2show[i]=ResultofChrom[i]/ratio2show;
		//}
		// for(int i=1;i<Result2show.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(ResultCurve, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(ResultCurve, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//}
		//imshow("圆滑曲线图",RC);
		//imshow("圆滑曲线图",ResultCurve);
		//一维低通滤波
		
		 int T=1;//滤波次数。次数越多，越光滑
		 double FilterCoefficient=0.6;//滤波系数，系数越大越光滑
		 for(int j=0;j<T;j++)
		 {
			 double prevalue=ResultofChrom[0];
			 for( int i = 1; i < ResultofChrom.size(); i++ )  
				{  
						prevalue=filter(prevalue,ResultofChrom[i],FilterCoefficient);
						ResultofChrom[i]=prevalue;
		
				}
			  prevalue=ResultofChrom[ResultofChrom.size()-1];
			  for( int i = ResultofChrom.size()-2; i >=0; i-- )  
				{  
						prevalue=filter(prevalue,ResultofChrom[i],FilterCoefficient);
						ResultofChrom[i]=prevalue;
		
				}
		 }
		//归一化到某个高度进行显示
		MaxResult=0;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			if(ResultofChrom[i]>MaxResult)
				MaxResult=ResultofChrom[i];
		}
		ratio2show=MaxResult/WinHeight;
	    //截取有效区域，即去除两端为0的区域
		int DB=0;
		int DE=ResultofChrom.size()-1;
		for( int i = 0; i < ResultofChrom.size(); i++ )  
		{  
			if(ResultofChrom[i]/ratio2show>WinHeight/50.0)
			{
				DB=i;
				break;
			}
			else
			{
				ResultofChrom[i]=0;
			}
	
		}
		for( int i = ResultofChrom.size()-1; i >=0; i-- )  
		{  
			if(ResultofChrom[i]/ratio2show>WinHeight/50.0)
			{
				DE=i;
				break;
			}
			else
			{
				ResultofChrom[i]=0;
			}
		}

		vector<double> ResultofChromTemp;
		for(int i=DB;i<=DE;i++)
		{
			ResultofChromTemp.push_back(ResultofChrom[i]);
		}
		ResultofChrom.clear();
		ResultofChrom=ResultofChromTemp;
		//推断首尾是否有着丝粒点
		vector<Point> BEPoint;
		double DT=20;
		if(ResultofChrom.size()>2*DT)
		{
			//首
			vector<Point> GDB;
			double SGDB=0;//首总和
			for(int i=0;i<=DT;i++)
			{
				GDB.push_back(Point(i*10,ResultofChrom[i]/ratio2show));	
				if(SGDB<=ResultofChrom[i]/ratio2show)
				SGDB=ResultofChrom[i]/ratio2show;
			}

			vector<double> lineResultB;
			LineFitLeastSquares(GDB,lineResultB);
			//cout<<"rr"<<lineResultB[2]<<"\n";
			//cout<<"beg"<<lineResultB[1]<<"\n";
			//cout<<"斜率"<<(lineResultB[0]/**ResultofChrom.size()*SparseLx+lineResult[1]*/)<<"\n";
			//cout<<"ratio2show"<<ratio2show<<"\n";
			//line(ResultCurve, Point(0,lineResultB[1]),Point(ResultofChrom.size()*10,(lineResultB[0]*ResultofChrom.size()*10+lineResultB[1])),Scalar(0,0,255), 1,8);
			double CdPointIndexB=-1;
			if(lineResultB[0]<1.5 && SGDB<350)//有着丝粒点，以最低点为着丝粒点
			{
				vector<double> WidthLine_B;
				vector<double> GradientofResultB;
				WidthLine_B.push_back(ResultofChrom[0]/ratio2show-(lineResultB[0]*(0)*10+lineResultB[1]));
				//circle(ResultCurve, Point(0*10,ResultofChrom[0]/ratio2show-(lineResultB[0]*(0)*10+lineResultB[1])*3+400), 3, Scalar(0, 0, 255), -1, 8);
				for(int i=1;i<=DT;i++)
				{
					//存储ResultofChrom与拟合直线之间的距离
				
					double Temp=ResultofChrom[i]/ratio2show-(lineResultB[0]*(i)*10+lineResultB[1]);
					WidthLine_B.push_back(Temp);
					//circle(ResultCurve, Point(i*10,Temp*3+400), 3, Scalar(0, 0, 255), -1, 8);
					//if(Temp<=MinDSGDB)
					//{
					//	MinDSGDB=Temp;
					//	CdPointIndexB=i+DB;
					//}
					//求导
					double GRTemp=WidthLine_B[i]-WidthLine_B[i-1];
					GradientofResultB.push_back(GRTemp);
					//circle(ResultCurve, Point(i*10,GradientofResultB[i-1]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				}
				GradientofResultB.push_back(WidthLine_B[DT]-WidthLine_B[DT-1]);
				//circle(ResultCurve, Point(DT*10,GradientofResultB[DT]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				double FilterCoefficient=0.7;//滤波系数，系数越大越光滑
				double prevalue=GradientofResultB[0];
				for( int i = 1; i < GradientofResultB.size(); i++ )  
				{  
						prevalue=filter(prevalue,GradientofResultB[i],FilterCoefficient);
						GradientofResultB[i]=prevalue;
		
				}
				prevalue=GradientofResultB[GradientofResultB.size()-1];
				for( int i = GradientofResultB.size()-2; i >=0; i-- )  
				{  
					prevalue=filter(prevalue,GradientofResultB[i],FilterCoefficient);
					GradientofResultB[i]=prevalue;
		
				}
				//for(int i=0;i<DT;i++)
				//{
				//	//circle(ResultCurve, Point(i*10,GradientofResultB[i]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				//	line(ResultCurve, Point(i*10,GradientofResultB[i]*5+600),Point((i+1)*10,GradientofResultB[i+1]*5+600),Scalar(0,0,255), 1,8);

				//}
				for(int i=0;i<=DT;i++)
				{
					GradientofResultB[i]=((GradientofResultB[i]>0)?1:0);
				}
				//for(int i=0;i<DT;i++)
				//{
				//	//circle(ResultCurve, Point(i*10,GradientofResultB[i]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				//	line(ResultCurve, Point(i*10,GradientofResultB[i]*50+600),Point((i+1)*10,GradientofResultB[i+1]*5+600),Scalar(0,0,255), 1,8);

				//}
				for(int i=0;i<DT;i++)
				{
					if(GradientofResultB[i]==0&&GradientofResultB[i+1]==1)
					{
						CdPointIndexB=i+DB;

					}
				}
				if(CdPointIndexB<0)
				{
					CdPointIndexB=DB+DT;
				}

			}
			if(CdPointIndexB!=-1)
			{
				Point TempPointB(ThinPoints[CdPointIndexB].x-BlankFrameWidth,ThinPoints[CdPointIndexB].y-BlankFrameWidth);
				BEPoint.push_back(TempPointB);
				//circle(image,TempPointB, 3, Scalar(255, 255, 0), -1, 8); //画圆，在图上显示点击的点
			}

			//尾
			int Ezero=ResultofChrom.size()-1-DT;
			vector<Point> GDE;
			double SGDE=0;//首总和
			for(int i=ResultofChrom.size()-DT;i<ResultofChrom.size();i++)
			{
				GDE.push_back(Point(i*10,ResultofChrom[i]/ratio2show));	
				if(SGDE<=ResultofChrom[i]/ratio2show)
				SGDE=ResultofChrom[i]/ratio2show;
			}
			vector<double> lineResultE;
			LineFitLeastSquares(GDE,lineResultE);
			//cout<<"rr"<<lineResultE[2]<<"\n";
			//cout<<"beg"<<lineResultE[1]<<"\n";
			//cout<<"斜率"<<(lineResultE[0]/**ResultofChrom.size()*SparseLx+lineResult[1]*/)<<"\n";
			//cout<<"ratio2show"<<ratio2show<<"\n";
			//line(ResultCurve, Point(0,lineResultE[1]),Point(ResultofChrom.size()*10,(lineResultE[0]*ResultofChrom.size()*10+lineResultE[1])),Scalar(0,0,255), 1,8);
		
			//cout<<"均值：\t"<<MeanSGDB<<"\t\t"<<MeanSGDE<<"\n";
			//cout<<"斜率:\t"<<lineResultB[0]<<"\t\t"<<lineResultE[0]<<"\n";
			double CdPointIndexE=-1;
			if(lineResultE[0]>-1.5 && SGDE<350)//有着丝粒点，以最低点为着丝粒点
			{
				vector<double> WidthLine_E;
				vector<double> GradientofResultE;
				WidthLine_E.push_back(ResultofChrom[Ezero]/ratio2show-(lineResultE[0]*(Ezero)*10+lineResultE[1]));
				//circle(ResultCurve, Point(Ezero*10,(ResultofChrom[Ezero]/ratio2show-(lineResultE[0]*(Ezero)*10+lineResultE[1]))*5+400), 3, Scalar(0, 0, 255), -1, 8);
				for(int i=1;i<=DT;i++)
				{
					//存储ResultofChrom与拟合直线之间的距离
				
					double Temp=ResultofChrom[Ezero+i]/ratio2show-(lineResultE[0]*(Ezero+i)*10+lineResultE[1]);
					WidthLine_E.push_back(Temp);
					//circle(ResultCurve, Point((Ezero+i)*10,Temp*5+400), 3, Scalar(0, 0, 255), -1, 8);
					//if(Temp<=MinDSGDB)
					//{
					//	MinDSGDB=Temp;
					//	CdPointIndexB=i+DB;
					//}
					//求导
					double GRTemp=WidthLine_E[i]-WidthLine_E[i-1];
					GradientofResultE.push_back(GRTemp);
					//circle(ResultCurve, Point((Ezero+i)*10,GradientofResultE[i-1]*5+400), 3, Scalar(0, 255, 255), -1, 8);
				}
				GradientofResultE.push_back(WidthLine_E[DT]-WidthLine_E[DT-1]);
				//line(ResultCurve, Point(Ezero*10,400),Point((Ezero+DT)*10,400),Scalar(0,0,255), 1,8);
				//circle(ResultCurve, Point((Ezero+DT)*10,GradientofResultE[DT]*5+400), 3, Scalar(0, 255, 255), -1, 8);
				double FilterCoefficient=0.7;//滤波系数，系数越大越光滑
				double prevalue=GradientofResultE[0];
				for( int i = 1; i < GradientofResultE.size(); i++ )  
				{  
					prevalue=filter(prevalue,GradientofResultE[i],FilterCoefficient);
					GradientofResultE[i]=prevalue;
		
				}
				prevalue=GradientofResultE[GradientofResultE.size()-1];
				for( int i = GradientofResultE.size()-2; i >=0; i-- )  
				{  
					prevalue=filter(prevalue,GradientofResultE[i],FilterCoefficient);
					GradientofResultE[i]=prevalue;
				}
				for(int i=0;i<=DT;i++)
				{
					GradientofResultE[i]=((GradientofResultE[i]>0)?1:0);
				}
				for(int i=0;i<DT;i++)
				{
					if(GradientofResultE[i]==0&&GradientofResultE[i+1]==1)
					{
						CdPointIndexE=DB+Ezero+i;
						break;

					}
				}
				if(CdPointIndexE<0)
				{
					CdPointIndexE=DB+Ezero;
				}

			}
			if(CdPointIndexE!=-1)
			{
				Point TempPointE(ThinPoints[CdPointIndexE].x-BlankFrameWidth,ThinPoints[CdPointIndexE].y-BlankFrameWidth);
				//circle(image,TempPointE, 3, Scalar(255, 255, 0), -1, 8); //画圆，在图上显示点击的点
				BEPoint.push_back(TempPointE);
				//cout<<SCCIn<<"大傻逼\n";
			}

			//cout<<"\t"<<CdPointIndexE<<"\n";
			//imshow("线性拟合平均距离趋势BE",ResultCurve);

		}
		// //绘制曲线
		//for(int i=0;i<ResultofChrom.size();i++)
		//{
		//	Result2show[i]=ResultofChrom[i]/ratio2show;
		//}
		//circle(ResultCurve, Point((0)*10,Result2show[0]), 3, Scalar(0, 255, 255), -1, 8);
		//circle(RC, Point((0)*10,Result2show[0]), 3, Scalar(0, 255, 255), -1, 8);
		//for(int i=1;i<ResultofChrom.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(ResultCurve, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(ResultCurve, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(0,0,255), 1,8);
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//}
		//imshow("圆滑曲线图",RC);
		 
		//绘制平均距离线
		//double SparseLx=pow(100.0,1.0);
		vector<Point> GDPoint;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			GDPoint.push_back(Point(i*10,ResultofChrom[i]/ratio2show));	
		}
		vector<double> lineResult;
		LineFitLeastSquares(GDPoint,lineResult);
		//cout<<"rr"<<lineResult[2]<<"\n";
		//cout<<"beg"<<lineResult[1]<<"\n";
		//cout<<"end"<<(lineResult[0]/**ResultofChrom.size()*SparseLx+lineResult[1]*/)<<"\n";
		//cout<<"ratio2show"<<ratio2show<<"\n";
		//line(ResultCurve, Point(0,lineResult[1]),Point(ResultofChrom.size()*10,(lineResult[0]*ResultofChrom.size()*10+lineResult[1])),Scalar(0,0,255), 1,8);
		//imshow("线性拟合平均距离趋势",ResultCurve);
		//绘制距离曲线图与平均距离线之间的差值
		//int bias=400;
		//int bias_x=0;
		vector<double> WidthLine_Point;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			WidthLine_Point.push_back(ResultofChrom[i]-(lineResult[0]*(i)*10+lineResult[1])*ratio2show);	
		}
		//circle(ResultCurve, Point(bias_x+0,WidthLine_Point[0]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8);
		//for(int i=1;i<WidthLine_Point.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(ResultCurve, Point((i)*10+bias_x,WidthLine_Point[i]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(ResultCurve, Point((i-1)*10+bias_x,WidthLine_Point[i-1]/ratio2show+bias),Point(i*10+bias_x,WidthLine_Point[i]/ratio2show+bias),Scalar(0,255,0), 1,8);

		//}
		//line(ResultCurve, Point(0+bias_x,bias),Point(WidthLine_Point.size()*10+bias_x,bias),Scalar(0,0,255), 1,8);
		//imshow("距离曲线图与平均距离趋势线之间的差值图",ResultCurve);
		//求导并绘制
		vector<double> GradientofResult;
		GradientofResult.push_back(ResultofChrom[1]-ResultofChrom[0]);
		double GR=15.0;
		for(int i=1;i<ResultofChrom.size()-1;i++)
		{
			GradientofResult.push_back(((ResultofChrom[i]-ResultofChrom[i-1])+(ResultofChrom[i+1]-ResultofChrom[i]))/2.0);
		}
		GradientofResult.push_back(ResultofChrom[ResultofChrom.size()-1]-ResultofChrom[ResultofChrom.size()-2]);
		//Mat GRC=Mat::zeros(1000,2000,CV_8UC3);
		//circle(GRC, Point(0*10,GR*GradientofResult[0]/ratio2show+bias), 3, Scalar(255, 0, 255), -1, 8);
		//for(int i=1;i<ResultofChrom.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(GRC, Point((i)*10,GR*GradientofResult[i]/ratio2show+bias), 3, Scalar(255, 0, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(GRC, Point((i-1)*10,GR*GradientofResult[i-1]/ratio2show+bias),Point(i*10,GR*GradientofResult[i]/ratio2show+bias),Scalar(255,255,0), 1,8);

		//}
		//line(GRC, Point(0,bias),Point(GR*GradientofResult.size()*10,bias),Scalar(255,255,255), 1,8);

			//一维低通滤波
		 int GT=1;//滤波次数。次数越多，越光滑
		 double GFilterCoefficient=0.6;//滤波系数，系数越大越光滑
		 for(int j=0;j<GT;j++)
		 {
			 double prevalue=GradientofResult[0];
			 for( int i = 1; i < GradientofResult.size(); i++ )  
				{  
						prevalue=filter(prevalue,GradientofResult[i],GFilterCoefficient);
						GradientofResult[i]=prevalue;
		
				}
			  prevalue=GradientofResult[GradientofResult.size()-1];
			  for( int i = GradientofResult.size()-2; i >=0; i-- )  
				{  
						prevalue=filter(prevalue,GradientofResult[i],GFilterCoefficient);
						GradientofResult[i]=prevalue;
		
				}
		 }

		//circle(GRC, Point(0*10,GR*GradientofResult[0]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8);
		//for(int i=1;i<ResultofChrom.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(GRC, Point((i)*10,GR*GradientofResult[i]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(GRC, Point((i-1)*10,GR*GradientofResult[i-1]/ratio2show+bias),Point(i*10,GR*GradientofResult[i]/ratio2show+bias),Scalar(0,255,255), 1,8);

		//}
		//line(GRC, Point(0,bias),Point(GR*GradientofResult.size()*10,bias),Scalar(255,255,255), 1,8);
		//imshow("一阶导数",GRC);
		//扫描，一阶导数符号化，并找到最低点
		vector<double> 	InterestPart;
		for(int i=0;i<GradientofResult.size();i++)
		{
			if(GradientofResult[i]<=0)
			{
				InterestPart.push_back(-1);
			}
			else
			{
				InterestPart.push_back(1);
			}
		}

		////绘制出符号化后的图像
		//circle(GRC, Point(bias_x+0,InterestPart[0]*30+bias), 3, Scalar(255, 255, 255), -1, 8);
		//for(int i=1;i<InterestPart.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(GRC, Point((i)*10+bias_x,InterestPart[i]*30+bias), 3, Scalar(255, 255, 255), -1, 8); //画圆，在图上显示点击的点 
		//	line(GRC, Point((i-1)*10+bias_x,InterestPart[i-1]*30+bias),Point(i*10+bias_x,InterestPart[i]*30+bias),Scalar(125,255,0), 2,8);

		//}

		//imshow("一阶导数",GRC);
	
	
		//找到山谷，并计算下降幅度为参考
		vector<Point> CdPoint;//着丝粒点的位置
		double WLThreshold=(150.0)*ratio2show;
		vector<int> minKv;
		vector<int> peakvalleyIndex;
		for(int i=0;i<InterestPart.size()-1;i++)
		{
			if((InterestPart[i]>0&&InterestPart[i+1]<0)||(InterestPart[i]<0&&InterestPart[i+1]>0))
			{
				peakvalleyIndex.push_back(i);
			}
		}
		for(int i=1;i<peakvalleyIndex.size()-1;i++)
		{
			if (peakvalleyIndex.size()<3) break;
			int pvB=peakvalleyIndex[i-1];
			int pvC=peakvalleyIndex[i];
			int pvD=peakvalleyIndex[i+1];
			if(i%2)
			{
				double BpeakvalleyDrop=WidthLine_Point[pvB]-WidthLine_Point[pvC];
				double DpeakvalleyDrop=WidthLine_Point[pvD]-WidthLine_Point[pvC];
				double peakvalleyDrop=((BpeakvalleyDrop>DpeakvalleyDrop)?BpeakvalleyDrop:DpeakvalleyDrop);
				
				if(peakvalleyDrop>WLThreshold /*&& BpeakvalleyDrop>WLThreshold/2.0 && DpeakvalleyDrop>WLThreshold/2.0*/)
				{
					//cout<<SCCIn<<"\t(峰-谷)：\t"<<peakvalleyDrop/ratio2show<<"\t\t"<<BpeakvalleyDrop/ratio2show<<"\t\t"<<DpeakvalleyDrop/ratio2show<<"\n";
					Point TempPoint(ThinPoints[pvC+DB].x-BlankFrameWidth,ThinPoints[pvC+DB].y-BlankFrameWidth);
					CdPoint.push_back(TempPoint);
					minKv.push_back(pvC+DB);

				}
			}
		

		}
		//imshow("距离斜率曲线图",ResultCurve);
		////利用横截面信息筛选
		//
		////cout<<CdPN<<"\n";
		//if(CdPoint.size()>=2)
		//{
		//	int CdPN=CdPoint.size();
		//	vector<Point> CdPointTemp;
		//	SubBlankFrame(gray,BlankFrameWidth);
		//	for(int Cdn=0;Cdn<CdPoint.size();Cdn++)
		//	{
		//		//cout<<"CdPN\t"<<CdPN<<"\n";
		//		
		//		if(CdPN<2) 
		//		{
		//			CdPointTemp.push_back(CdPoint[Cdn]);
		//			break;
		//		}
		//		//cout<<"CdPN2\t"<<CdPN<<"\n";
		//		vector<double> ResultofCross;
		//		for(int i=-(w/2);i<=w/2;i++)
		//		{
		//			Point Pt(CdPoint[Cdn].x+Kv[minKv[Cdn]][0]*(i),CdPoint[Cdn].y+Kv[minKv[Cdn]][1]*(i));
		//			if(Pt.x>=0&&Pt.x<gray.cols&&Pt.y>=0&&Pt.y<gray.rows) 
		//			{
		//				ResultofCross.push_back(int(gray.at<uchar>(Pt.y,Pt.x)));
		//			}
		//			//cout<<Pt<<"\t\t"<<(int)gray.at<uchar>(Pt)<<"\n";
		//		}
		//		//筛选有效信息，即去除两端的0
		//		int RoCB=0;
		//		int RoCE=0;
		//		for(int i=0;i<ResultofCross.size();i++)
		//		{
		//			if(ResultofCross[i])
		//			{
		//				RoCB=i;
		//				break;
		//			}
		//		}
		//		for(int i=ResultofCross.size()-1;i>=0;i--)
		//		{
		//			if(ResultofCross[i])
		//			{
		//				RoCE=i;
		//				break;
		//			}
		//		}
		//		if(RoCB>=RoCE) continue;
		//		else
		//		{
		//			vector<double> ResultofCrossTemp;
		//			ResultofCrossTemp.insert(ResultofCrossTemp.begin(),ResultofCross.begin()+RoCB,ResultofCross.begin()+RoCE+1);
		//			ResultofCross.clear();
		//			ResultofCross.insert(ResultofCross.begin(),ResultofCrossTemp.begin(),ResultofCrossTemp.end());
		//		}
		//		
		//		//平滑灰度曲线图
		//		if(ResultofCross.size()>=4)
		//		{
		//			int TCC=1;//拟合曲线，拟合近似次数。次数越多，越光滑
		//			double CrossFilterCoefficient=0.3;//滤波系数，系数越大越光滑
		//			for(int j=0;j<TCC;j++)
		//			{
		//				 double prevalue=ResultofCross[0];
		//				 for( int i = 1; i < ResultofCross.size(); i++ )  
		//					{  
		//							prevalue=filter(prevalue,ResultofCross[i],CrossFilterCoefficient);
		//							ResultofCross[i]=prevalue;
		//
		//					}
		//				  prevalue=ResultofCross[ResultofCross.size()-1];
		//				  for( int i = ResultofCross.size()-2; i >=0; i-- )  
		//					{  
		//							prevalue=filter(prevalue,ResultofCross[i],CrossFilterCoefficient);
		//							ResultofCross[i]=prevalue;
		//
		//					}
		//		
		//			}
		//			//求导
		//			vector<double> DerivativeResultofCross;
		//			for( int i = 0; i < ResultofCross.size()-1; i++ )  
		//			{  
		//				DerivativeResultofCross.push_back((ResultofCross[i+1]-ResultofCross[i]>=0)?1:0);
		//			}
		//			DerivativeResultofCross.push_back((ResultofCross[ResultofCross.size()-1]-ResultofCross[ResultofCross.size()-2]>=0)?1:0);
		//			for( int i = 0; i < DerivativeResultofCross.size(); i++ ) 
		//				cout<<DerivativeResultofCross[i];
		//			cout<<"\n";
		//			int DRoC01N=0;
		//			for( int i = 0; i < DerivativeResultofCross.size()-2; i++ )
		//			{
		//				if(/*DerivativeResultofCross[i-1]==0&&*/DerivativeResultofCross[i]==0&&DerivativeResultofCross[i+1]==1/*&&DerivativeResultofCross[i+2]==1*/) DRoC01N++;
		//			}
		//			if(DRoC01N>=2) CdPN--;
		//			else
		//			{
		//				CdPointTemp.push_back(CdPoint[Cdn]);
		//				//CdPoint.erase(CdPoint.begin()+Cdn+1);
		//				//Cdn--;
		//			}

		//		}
		//	}
		//	//cout<<CdPointTemp.size()<<"\t"<<CdPoint.size()<<"\n";
		//	CdPoint.clear();
		//	CdPoint.insert(CdPoint.begin(),CdPointTemp.begin(),CdPointTemp.end());
		//	//cout<<CdPointTemp.size()<<"\t"<<CdPoint.size()<<"\n";
		//}

		//距离过近两个点合并
		CdPoint.insert(CdPoint.end(),BEPoint.begin(),BEPoint.end());
		vector<Point> SCLV;
		if(CdPoint.size()>=2)
		{

			for(int i=0;i<CdPoint.size()-1;i++)
			{
				bool CdPY=true;
				for(int j=i+1;j<CdPoint.size();j++)
				{
					double CdPD=DistancePoints(CdPoint[i],CdPoint[j]);
					if(CdPD<10.0) 
					{
						CdPY=false;
						continue;
					}
				}
				if(CdPY)
				{
					SCLV.push_back(CdPoint[i]);
				}
			}
			SCLV.push_back(CdPoint[CdPoint.size()-1]);
		}
		if(CdPoint.size()==1) SCLV.push_back(CdPoint[0]);
		SCCI[SCCIn].SetCentromereLocationVector(SCLV);
		////int BlankFrameWidth=20;
		////AddBlankFrame(image,BlankFrameWidth);
		//SubBlankFrame(image,BlankFrameWidth);
		double Slope=0.2;
		//对没有着丝粒点的情况，根据斜率推断某端有着丝粒点
		if(CdPoint.size()==0)
		{
			Point TempPEndofPoint2Vector(PEndofPoint2Vector.x-BlankFrameWidth,PEndofPoint2Vector.y-BlankFrameWidth);
			Point TempPBeginofPoint2Vector(PBeginofPoint2Vector.x-BlankFrameWidth,PBeginofPoint2Vector.y-BlankFrameWidth);
			//cout<<PBeginofPoint2Vector;
			if(lineResult[0]<-Slope) 
			{
				SCCI[SCCIn].SetCentromereLocationVector(TempPEndofPoint2Vector);
				//circle(image,TempPEndofPoint2Vector, 3, Scalar(255, 255, 0), -1, 8); //画圆，在图上显示点击的点 
			}
			else if(lineResult[0]>Slope) 
			{
				SCCI[SCCIn].SetCentromereLocationVector(TempPBeginofPoint2Vector);
				//circle(image,TempPBeginofPoint2Vector, 3, Scalar(255, 255, 0), -1, 8); //画圆，在图上显示点击的点 
			}
			else 
			{
				cout<<"无法推测该染色体的着丝粒点位置！！！考虑是断片："<<SCCIn<<"\t斜率为："<<lineResult[0]<<"\n";
			}
			//if(lineResult[0]==0) cout<<"无法推测该染色体的着丝粒点位置！！！\n";
		}
	

		////在染色体图上绘制出找到的着丝粒点
		//for(int i=0;i<CdPoint.size();i++)
		//{
		//	circle(image,CdPoint[i], 3, Scalar(255, 255, 255), -1, 8); //画圆，在图上显示点击的点 

		//}
		//imshow("着丝粒点位置图",image);


	}
	
}
