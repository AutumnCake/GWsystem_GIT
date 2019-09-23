//#include "stdafx.h"
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


#include"D8ConnectedComponent.h"
#include"KmeansCluster.h"
#include"Label2Vector_RGB.h"
#include"NonezeroMeanGrayValue.h"

#include"ClassClusterInfo.h"
#include"Invert0_1.h"
#include"mywatershed.h"
#include"RandomColor.h"
#include"Label2Vector_RGB.h"
#include"res.h"
#include"ClassifyChromo.h"

void CutSecond(vector<ClassClusterInfo>& ChromCluster)
{
	vector<ClassClusterInfo> CCVTemp;
	//��Lump��һ���ָ�
	//int LI=1;
	//cout<<ChromCluster.size();
	for(int LI=0;LI<ChromCluster.size();LI++)
	{
		//cout << "���ڴ����LI��Ⱦɫ���ţ�LI=" << LI << "\n";
		Mat img=ChromCluster[LI].GetClusterImage().clone();
		//imshow("img",img);
		//��ֵȷ��Ⱦɫ��λ��
		//ת��Ϊ�Ҷ�ͼ
		Mat gray;
		cvtColor( img, gray, CV_BGR2GRAY );
		//imshow("�Ҷ�ͼ",gray);
		//0��ֵ��ֵͼ
		Mat Bi;
		threshold(gray, Bi, 0, 255, THRESH_BINARY);
		int GrayMean=NonezeroMeanGrayValue(gray);
		Mat ChromBi;		
		threshold(gray, ChromBi, GrayMean+10, 255, THRESH_TOZERO_INV);
		threshold(ChromBi, ChromBi, 0, 255, THRESH_BINARY);
		//imshow("ChromBi",ChromBi);
		//�ֱ����ChromBi��Bi-ChromBi�ĻҶȾ�ֵ
		Mat Biout=Bi-ChromBi;
		//imshow("Bi",Bi);
		//cout<<"��ɵ��\n";
		int ChromBiMean=NonezeroMeanGrayValue(gray,ChromBi);
		
		int BiMean=NonezeroMeanGrayValue(gray,Biout);

		//cout<<ChromBiMean<<"\t"<<BiMean<<"\n";
		int MiddleThreshold=ChromBiMean+(BiMean-ChromBiMean)*0.9;
		Mat MiddleBi;
		threshold(gray, MiddleBi, MiddleThreshold, 0, THRESH_TOZERO_INV);
		//imshow("MiddleBi",MiddleBi);
		//cout<<GrayMean+10<<"\t"<<MiddleThreshold<<"\n";


		//���ݻҶ�ͼ��ֵ�����,�����ԭͼ��ת�����߶ȻҶ�ͼ���ڹ�ˮ���ָ�
		Mat GrayHeight=Mat::zeros(img.size(),CV_8UC1);
		Mat seed=Mat::zeros(GrayHeight.size(),CV_8UC1);
		int min=40;//�˴���Ϊ1~120������ֵ������ȡ0
		int medium=120;
		int max=255;
		for(int i=0;i<MiddleBi.rows;i++)
		{
			for(int j=0;j<MiddleBi.cols;j++)
			{
				if(MiddleBi.at<uchar>(i,j)>0)
				{
					GrayHeight.at<uchar>(i,j)=min;
				    seed.at<uchar>(i,j)=255;
				}	
				if(gray.at<uchar>(i,j)>0&&MiddleBi.at<uchar>(i,j)==0)
					GrayHeight.at<uchar>(i,j)=medium;
				if(gray.at<uchar>(i,j)==0)
					GrayHeight.at<uchar>(i,j)=max;

			}
		}
		//�Ҷȸ߶�ͼ��ˮ��
		//imshow("GrayHeight",GrayHeight);
		////����ͼ� 
		////mtf 20190529 ���������������еĿն�����Ī�������������ʱ��Ϊ���ܲ��������ԭ��
		//imshow("seed", seed);
		//Mat seedHole = seed.clone();
		//Rect ccomp;
		//floodFill(seedHole, Point(0, 0), Scalar(255), &ccomp, Scalar(1), Scalar(1), 8);//�Ҷ�����Ϊ����ֵ
		//bitwise_not(seedHole, seedHole);
		//Mat seednohole;
		//add(seedHole, seed, seednohole);
		//threshold(seednohole, seednohole, 0, 255, THRESH_BINARY);
		//imshow("seednohole", seednohole);
		//��ˮ��ָ�
		int Zonenum=0;
		Mat labelimg=Mat::zeros(img.size(),CV_8U);
		//cout << "��ʼ��ˮ�롭��\n";
		myWatershed(GrayHeight,seed,labelimg,Zonenum);
		//cout << "������ˮ�롭��\n";
	  //  //��ÿһ�����������ɫ���  
	  // //Ϊ��������ʾ�ָ���ͼ�������´���
		 //Mat PerspectiveImage=Mat::zeros(img.size(),CV_8UC3);  
	  //  for(int i=0;i<labelimg.rows;i++)  
	  //  {  
	  //      for(int j=0;j<labelimg.cols;j++)  
	  //      {  
	  //          int index=(int)labelimg.at<uchar>(i,j);  
	  //          if(labelimg.at<uchar>(i,j)==0)  
	  //          { 
	  //              PerspectiveImage.at<Vec3b>(i,j)=Vec3b(255,255,255);  
	  //          }              
	  //          else  
	  //          {  
	  //              PerspectiveImage.at<Vec3b>(i,j) =RandomColor(index);  
	  //          }  
	  //      }  
	  //  }
	  //  imshow("��ɫ����ķָ�ͼ",PerspectiveImage); 

		//����labelimg���зָ���ָ�����ʾ����ͬͼƬ��
		vector<Mat> imageVector;
		vector<Mat> InterestVector;
		Label2Vector_RGB(labelimg,Zonenum,img,imageVector,0);
		Label2Vector_RGB(labelimg,Zonenum,Bi,InterestVector,0);
		//imshow("Bi",Bi);
		//int LN=FirstCutLocation.size();//���ڱ�Ƕ�Ӧvector<Mat>���
		for(int i=0;i<imageVector.size();i++)
		{
			vector<vector<Point> > contours_out;  
			vector<Vec4i> hierarchy_out;  
			Mat contoursTemp=InterestVector[i].clone();//findContours��ı�ԭͼ�񣬹���һ����Ϊ�˷�ֹ�ı�ChromosomeGroup��Ԫ��
			findContours(contoursTemp, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE);
			if(contours_out.size()==0)
			{
				continue;
				//Mat emptyMat;
				//imageVector[i]=emptyMat;//  �� img.empty() �ж��Ƿ�Ϊ��
			
			}
			else
			{
				//cout<<"a";
				ClassClusterInfo CCITemp;
				Rect boundRect;
				boundRect = boundingRect(contours_out[0]);
				//if(boundRect.width<ExternWidthChromatidMin||boundRect.height<ExternWidthChromatidMin) continue;
				//cout<<"d";
				//rectangle(imageVector[i], Rect((int)boundRect.x, (int)boundRect.y,boundRect.width, boundRect.height), Scalar(0, 255, 0),2); //������Ȥ��������
				//ClassChromLocation CCLTemp(boundRect,Point(0,0));
				//ICTemp = imageVector[i](Rect((int)boundRect.x, (int)boundRect.y,boundRect.width, boundRect.height));  //Rect��������ROI,ע����ǣ�cols������rows��
				//int Blank=1;
				//int x=(int)boundRect.x-Blank;  x=(x>0?x:0);
				//int y=(int)boundRect.y-Blank;  y=(y>0?y:0); 
				//int w=boundRect.width+2*Blank;   w=(w<imageVector[i].size().width?w:imageVector[i].size().width);
				//int h=boundRect.height+2*Blank;  h=(h<imageVector[i].size().height?h:imageVector[i].size().height);
				//boundRect=Rect(x,y,w,h);
				Mat ICTemp = imageVector[i](boundRect);
				CCITemp.SetClassClusterInfo(ICTemp,boundRect,ChromCluster[LI].GetRelativeOrigin());
				CCVTemp.push_back(CCITemp);
				
			}
		}
		//cout<<CCVTemp.size();
	}
	//cout<<CCVTemp.size();
	ChromCluster.clear();
	ChromCluster.insert(ChromCluster.begin(),CCVTemp.begin(),CCVTemp.end());	
}
