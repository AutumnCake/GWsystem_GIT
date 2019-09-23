// ��˿����ʶ��.cpp : �������̨Ӧ�ó������ڵ㡣
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
#include"..\Ⱦɫ���㷨�ļ�\NonezeroMeanGrayValue.h"
#include"Point2Vector.h"
#include"..\Ⱦɫ���㷨�ļ�\MyFunction.h"
#include"LineFitLeastSquares.h"
#include"ExtendThinImage.h"
#include"MixDistanceGraylevel.h"
#include"BlankFrame.h"
#include"TrimThin.h"
#include"FilteringMethod.h"
#include"SmoothThin.h"

#include"..\Ⱦɫ���㷨�ļ�\ClassClusterInfo.h"
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
		//ת��Ϊ�Ҷ�ͼ
		Mat gray,Bi;
		cvtColor(image,gray,CV_RGB2GRAY);
		threshold(gray,Bi,0,255,THRESH_BINARY);
		//imshow("��ֵͼ",Bi);
		Mat BiTemp=Bi.clone();
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//��2 �ṹԪ��
		dilate(BiTemp,BiTemp,element);
		//��ף�����ȡ���������˴������Ϊ�˷�ֹ��ȡ�Ǽ�ʱ����ֿ�״����
		vector<vector<Point> > contours_out;  
		vector<Vec4i> hierarchy_out; 
		Mat BiContours=BiTemp.clone();
		findContours(BiContours, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE); 
		drawContours(BiTemp,contours_out,0,Scalar(255),-1,8,hierarchy_out);

		//��С�����������Ǽ�
		vector<Point> ThinPoints;//�������δ洢�Ǽܵ�
		double Kx=0,Ky=0;//���ڴ洢�Ǽ�б��
		bool K_Rect=false;//���ڱ�ʾ�Ƿ�ʹ����С������Ǽ�
		RotatedRect minRect;
		if(contours_out.size()==0) continue;
		else minRect=minAreaRect(contours_out[0]);
		
		Point2f rect_points[4];
		double d1=0; //��С��Ӿ��ε������߳��ֱ�Ϊd1 d2
		double d2=0;
		double w=0;//��Ӿ��εĿ�ȣ���d1 d2�н�С��
		double l=0;
		bool wd01=true;//��01���ڵı��ǿ��
		double areaRatio=0;//Ⱦɫ���������С���ε������
		minRect.points(rect_points);
		d1=DistancePoints(rect_points[0],rect_points[1]);
		d2=DistancePoints(rect_points[1],rect_points[2]);
		w=(d1<d2? d1:d2);
		l=(d1>d2? d1:d2);
		wd01=(d1<d2? true:false);
	
		areaRatio=double(contourArea(contours_out[0]))/double(d1*d2);
		Mat thinImage=Mat::zeros(image.size(),CV_8U);
		double EmpiricalAreaRatio=0.8;//�þ��������ֵȡ���������̶�
		Point PBeginofPoint2Vector(0,0);
		Point PEndofPoint2Vector(0,0);
		double lwRatio=l/w;
		int  Inward=15;//�ƶϵ����ڲ�����Inward
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
			//circle(thinImage, P1, 3, Scalar(255, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
			//��¼б�ʣ�����ʾK_Rect
			K_Rect=true;
			Kx=(double)(P2.x-P1.x);
			Ky=(double)(P2.y-P1.y);
			//cout<<K_Thin<<":"<<(double)(P2.y-P1.y)<<"\t"<<(double)(P2.x-P1.x)<<"\n";
			//��ֱ���ϵĵ���P1��P2���δ洢��ThinPoints��
			//cout<<"d0\n";
			Point2Vector(thinImage,ThinPoints,P1);
		
			PBeginofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[Inward-1]:P1);//P1��
			PEndofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[ThinPoints.size()-Inward]:P2);
			//cout<<"d1\n";
		}
		else
		{
	
			thinImage=ThinImage(BiTemp);
			////imshow("�Ǽ�",thinImage);
			TrimThin(thinImage);
			//
			//�ӳ��Ǽܲ����´洢�����ƹǼܱ�̵����
			vector<Point> ThinPointsTemp;
		   //���Ǽ��ϵĵ����δ洢��ThinPoints��
			Point2Vector(thinImage,ThinPointsTemp);
		
			//imshow("ԭʼ�Ǽ�",thinImage);
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
			SmoothThin(thinImage,20);//Բ���ӳ��������
			if(ThinPointsTemp.size()<8)
			{
				//������
				cout<<SCCIn<<":�����ǻ�����΢С��Ƭ\n";
				continue;
			}
			//imshow("�ӳ��Ǽ�",thinImage);
			Point2Vector(thinImage,ThinPoints);
		
			PBeginofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[Inward-1]:ThinPoints[0]);
			PEndofPoint2Vector=(ThinPoints.size()>2*Inward? ThinPoints[ThinPoints.size()-Inward]:ThinPoints[ThinPoints.size()-1]);

		}
		//imshow("�Ǽ�",thinImage);
		
		vector<double> ResultofChrom;
		vector<Point> PV;
		vector<vector<double>> Kv;
		if(K_Rect)//��ʱ�Ǽ���ÿһ���б�ʶ���֪���ҿ�����Ky��Kx��ʾ
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
		else//��û������С������Ǽ�ʱ������ʴ���ĹǼ����
		{
		
			ResultofChrom=MixDistanceGraylevel(ThinPoints,gray,w/2,DistanceWeightGrayFunction,Kv);
			//ResultofChrom=MixDistanceGraylevel(ThinPoints,gray,w/2);
			//GraylevelDistributionofChrom= GraylevelDistributionofChromOnThinPoints(ThinPoints,gray,w/2);
		}
		//��Ӧ���ڣ�������ʾ����һ����ĳ���߶Ƚ�����ʾ
		double MaxResult=0;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			if(ResultofChrom[i]>MaxResult)
				MaxResult=ResultofChrom[i];
		}
		double WinHeight=700;//���ڸ߶�
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
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(255, 0, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,255,255), 1,8);
		//}
		//imshow("�Ҷ�����ͼ",RC);
		////ƽ���Ҷ�����ͼ
	
		// int T=1;//������ߣ���Ͻ��ƴ���������Խ�࣬Խ�⻬
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
		//	circle(ResultCurve, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(ResultCurve, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//}
		//imshow("Բ������ͼ",RC);
		//imshow("Բ������ͼ",ResultCurve);
		//һά��ͨ�˲�
		
		 int T=1;//�˲�����������Խ�࣬Խ�⻬
		 double FilterCoefficient=0.6;//�˲�ϵ����ϵ��Խ��Խ�⻬
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
		//��һ����ĳ���߶Ƚ�����ʾ
		MaxResult=0;
		for(int i=0;i<ResultofChrom.size();i++)
		{
			if(ResultofChrom[i]>MaxResult)
				MaxResult=ResultofChrom[i];
		}
		ratio2show=MaxResult/WinHeight;
	    //��ȡ��Ч���򣬼�ȥ������Ϊ0������
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
		//�ƶ���β�Ƿ�����˿����
		vector<Point> BEPoint;
		double DT=20;
		if(ResultofChrom.size()>2*DT)
		{
			//��
			vector<Point> GDB;
			double SGDB=0;//���ܺ�
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
			//cout<<"б��"<<(lineResultB[0]/**ResultofChrom.size()*SparseLx+lineResult[1]*/)<<"\n";
			//cout<<"ratio2show"<<ratio2show<<"\n";
			//line(ResultCurve, Point(0,lineResultB[1]),Point(ResultofChrom.size()*10,(lineResultB[0]*ResultofChrom.size()*10+lineResultB[1])),Scalar(0,0,255), 1,8);
			double CdPointIndexB=-1;
			if(lineResultB[0]<1.5 && SGDB<350)//����˿���㣬����͵�Ϊ��˿����
			{
				vector<double> WidthLine_B;
				vector<double> GradientofResultB;
				WidthLine_B.push_back(ResultofChrom[0]/ratio2show-(lineResultB[0]*(0)*10+lineResultB[1]));
				//circle(ResultCurve, Point(0*10,ResultofChrom[0]/ratio2show-(lineResultB[0]*(0)*10+lineResultB[1])*3+400), 3, Scalar(0, 0, 255), -1, 8);
				for(int i=1;i<=DT;i++)
				{
					//�洢ResultofChrom�����ֱ��֮��ľ���
				
					double Temp=ResultofChrom[i]/ratio2show-(lineResultB[0]*(i)*10+lineResultB[1]);
					WidthLine_B.push_back(Temp);
					//circle(ResultCurve, Point(i*10,Temp*3+400), 3, Scalar(0, 0, 255), -1, 8);
					//if(Temp<=MinDSGDB)
					//{
					//	MinDSGDB=Temp;
					//	CdPointIndexB=i+DB;
					//}
					//��
					double GRTemp=WidthLine_B[i]-WidthLine_B[i-1];
					GradientofResultB.push_back(GRTemp);
					//circle(ResultCurve, Point(i*10,GradientofResultB[i-1]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				}
				GradientofResultB.push_back(WidthLine_B[DT]-WidthLine_B[DT-1]);
				//circle(ResultCurve, Point(DT*10,GradientofResultB[DT]*5+600), 3, Scalar(0, 255, 255), -1, 8);
				double FilterCoefficient=0.7;//�˲�ϵ����ϵ��Խ��Խ�⻬
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
				//circle(image,TempPointB, 3, Scalar(255, 255, 0), -1, 8); //��Բ����ͼ����ʾ����ĵ�
			}

			//β
			int Ezero=ResultofChrom.size()-1-DT;
			vector<Point> GDE;
			double SGDE=0;//���ܺ�
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
			//cout<<"б��"<<(lineResultE[0]/**ResultofChrom.size()*SparseLx+lineResult[1]*/)<<"\n";
			//cout<<"ratio2show"<<ratio2show<<"\n";
			//line(ResultCurve, Point(0,lineResultE[1]),Point(ResultofChrom.size()*10,(lineResultE[0]*ResultofChrom.size()*10+lineResultE[1])),Scalar(0,0,255), 1,8);
		
			//cout<<"��ֵ��\t"<<MeanSGDB<<"\t\t"<<MeanSGDE<<"\n";
			//cout<<"б��:\t"<<lineResultB[0]<<"\t\t"<<lineResultE[0]<<"\n";
			double CdPointIndexE=-1;
			if(lineResultE[0]>-1.5 && SGDE<350)//����˿���㣬����͵�Ϊ��˿����
			{
				vector<double> WidthLine_E;
				vector<double> GradientofResultE;
				WidthLine_E.push_back(ResultofChrom[Ezero]/ratio2show-(lineResultE[0]*(Ezero)*10+lineResultE[1]));
				//circle(ResultCurve, Point(Ezero*10,(ResultofChrom[Ezero]/ratio2show-(lineResultE[0]*(Ezero)*10+lineResultE[1]))*5+400), 3, Scalar(0, 0, 255), -1, 8);
				for(int i=1;i<=DT;i++)
				{
					//�洢ResultofChrom�����ֱ��֮��ľ���
				
					double Temp=ResultofChrom[Ezero+i]/ratio2show-(lineResultE[0]*(Ezero+i)*10+lineResultE[1]);
					WidthLine_E.push_back(Temp);
					//circle(ResultCurve, Point((Ezero+i)*10,Temp*5+400), 3, Scalar(0, 0, 255), -1, 8);
					//if(Temp<=MinDSGDB)
					//{
					//	MinDSGDB=Temp;
					//	CdPointIndexB=i+DB;
					//}
					//��
					double GRTemp=WidthLine_E[i]-WidthLine_E[i-1];
					GradientofResultE.push_back(GRTemp);
					//circle(ResultCurve, Point((Ezero+i)*10,GradientofResultE[i-1]*5+400), 3, Scalar(0, 255, 255), -1, 8);
				}
				GradientofResultE.push_back(WidthLine_E[DT]-WidthLine_E[DT-1]);
				//line(ResultCurve, Point(Ezero*10,400),Point((Ezero+DT)*10,400),Scalar(0,0,255), 1,8);
				//circle(ResultCurve, Point((Ezero+DT)*10,GradientofResultE[DT]*5+400), 3, Scalar(0, 255, 255), -1, 8);
				double FilterCoefficient=0.7;//�˲�ϵ����ϵ��Խ��Խ�⻬
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
				//circle(image,TempPointE, 3, Scalar(255, 255, 0), -1, 8); //��Բ����ͼ����ʾ����ĵ�
				BEPoint.push_back(TempPointE);
				//cout<<SCCIn<<"��ɵ��\n";
			}

			//cout<<"\t"<<CdPointIndexE<<"\n";
			//imshow("�������ƽ����������BE",ResultCurve);

		}
		// //��������
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
		//	circle(ResultCurve, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(ResultCurve, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(0,0,255), 1,8);
		//	circle(RC, Point((i)*10,Result2show[i]), 3, Scalar(0, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(RC, Point((i-1)*10,Result2show[i-1]),Point(i*10,Result2show[i]),Scalar(255,0,0), 1,8);
		//}
		//imshow("Բ������ͼ",RC);
		 
		//����ƽ��������
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
		//imshow("�������ƽ����������",ResultCurve);
		//���ƾ�������ͼ��ƽ��������֮��Ĳ�ֵ
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
		//	circle(ResultCurve, Point((i)*10+bias_x,WidthLine_Point[i]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(ResultCurve, Point((i-1)*10+bias_x,WidthLine_Point[i-1]/ratio2show+bias),Point(i*10+bias_x,WidthLine_Point[i]/ratio2show+bias),Scalar(0,255,0), 1,8);

		//}
		//line(ResultCurve, Point(0+bias_x,bias),Point(WidthLine_Point.size()*10+bias_x,bias),Scalar(0,0,255), 1,8);
		//imshow("��������ͼ��ƽ������������֮��Ĳ�ֵͼ",ResultCurve);
		//�󵼲�����
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
		//	circle(GRC, Point((i)*10,GR*GradientofResult[i]/ratio2show+bias), 3, Scalar(255, 0, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(GRC, Point((i-1)*10,GR*GradientofResult[i-1]/ratio2show+bias),Point(i*10,GR*GradientofResult[i]/ratio2show+bias),Scalar(255,255,0), 1,8);

		//}
		//line(GRC, Point(0,bias),Point(GR*GradientofResult.size()*10,bias),Scalar(255,255,255), 1,8);

			//һά��ͨ�˲�
		 int GT=1;//�˲�����������Խ�࣬Խ�⻬
		 double GFilterCoefficient=0.6;//�˲�ϵ����ϵ��Խ��Խ�⻬
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
		//	circle(GRC, Point((i)*10,GR*GradientofResult[i]/ratio2show+bias), 3, Scalar(0, 0, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(GRC, Point((i-1)*10,GR*GradientofResult[i-1]/ratio2show+bias),Point(i*10,GR*GradientofResult[i]/ratio2show+bias),Scalar(0,255,255), 1,8);

		//}
		//line(GRC, Point(0,bias),Point(GR*GradientofResult.size()*10,bias),Scalar(255,255,255), 1,8);
		//imshow("һ�׵���",GRC);
		//ɨ�裬һ�׵������Ż������ҵ���͵�
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

		////���Ƴ����Ż����ͼ��
		//circle(GRC, Point(bias_x+0,InterestPart[0]*30+bias), 3, Scalar(255, 255, 255), -1, 8);
		//for(int i=1;i<InterestPart.size();i++)
		//{
		//	//cout<<GraylevelDistributionofChrom[i]<<"\n";
		//	//ResultCurve.at<Vec3b>(GraylevelDistributionofChrom[i]*30,i*20)=Vec3b(255,0,255);
		//	circle(GRC, Point((i)*10+bias_x,InterestPart[i]*30+bias), 3, Scalar(255, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
		//	line(GRC, Point((i-1)*10+bias_x,InterestPart[i-1]*30+bias),Point(i*10+bias_x,InterestPart[i]*30+bias),Scalar(125,255,0), 2,8);

		//}

		//imshow("һ�׵���",GRC);
	
	
		//�ҵ�ɽ�ȣ��������½�����Ϊ�ο�
		vector<Point> CdPoint;//��˿�����λ��
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
					//cout<<SCCIn<<"\t(��-��)��\t"<<peakvalleyDrop/ratio2show<<"\t\t"<<BpeakvalleyDrop/ratio2show<<"\t\t"<<DpeakvalleyDrop/ratio2show<<"\n";
					Point TempPoint(ThinPoints[pvC+DB].x-BlankFrameWidth,ThinPoints[pvC+DB].y-BlankFrameWidth);
					CdPoint.push_back(TempPoint);
					minKv.push_back(pvC+DB);

				}
			}
		

		}
		//imshow("����б������ͼ",ResultCurve);
		////���ú������Ϣɸѡ
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
		//		//ɸѡ��Ч��Ϣ����ȥ�����˵�0
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
		//		//ƽ���Ҷ�����ͼ
		//		if(ResultofCross.size()>=4)
		//		{
		//			int TCC=1;//������ߣ���Ͻ��ƴ���������Խ�࣬Խ�⻬
		//			double CrossFilterCoefficient=0.3;//�˲�ϵ����ϵ��Խ��Խ�⻬
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
		//			//��
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

		//�������������ϲ�
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
		//��û����˿��������������б���ƶ�ĳ������˿����
		if(CdPoint.size()==0)
		{
			Point TempPEndofPoint2Vector(PEndofPoint2Vector.x-BlankFrameWidth,PEndofPoint2Vector.y-BlankFrameWidth);
			Point TempPBeginofPoint2Vector(PBeginofPoint2Vector.x-BlankFrameWidth,PBeginofPoint2Vector.y-BlankFrameWidth);
			//cout<<PBeginofPoint2Vector;
			if(lineResult[0]<-Slope) 
			{
				SCCI[SCCIn].SetCentromereLocationVector(TempPEndofPoint2Vector);
				//circle(image,TempPEndofPoint2Vector, 3, Scalar(255, 255, 0), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
			}
			else if(lineResult[0]>Slope) 
			{
				SCCI[SCCIn].SetCentromereLocationVector(TempPBeginofPoint2Vector);
				//circle(image,TempPBeginofPoint2Vector, 3, Scalar(255, 255, 0), -1, 8); //��Բ����ͼ����ʾ����ĵ� 
			}
			else 
			{
				cout<<"�޷��Ʋ��Ⱦɫ�����˿����λ�ã����������Ƕ�Ƭ��"<<SCCIn<<"\tб��Ϊ��"<<lineResult[0]<<"\n";
			}
			//if(lineResult[0]==0) cout<<"�޷��Ʋ��Ⱦɫ�����˿����λ�ã�����\n";
		}
	

		////��Ⱦɫ��ͼ�ϻ��Ƴ��ҵ�����˿����
		//for(int i=0;i<CdPoint.size();i++)
		//{
		//	circle(image,CdPoint[i], 3, Scalar(255, 255, 255), -1, 8); //��Բ����ͼ����ʾ����ĵ� 

		//}
		//imshow("��˿����λ��ͼ",image);


	}
	
}
