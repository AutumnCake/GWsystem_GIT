
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
#include"res.h"
//minWidthChromatid�������ƹ���ϸС�����ʣ���ֻ������ͳ�������Ⱦɫ�����ȵ�����

void CutFirst(vector<ClassClusterInfo>& CCI)
{
	vector<ClassClusterInfo> CCIVTemp;
	//CCITemp.insert(CCITemp.begin(),CCI.begin(),CCI.end());
	//imshow("ԭͼ",image);

	for(int Iv=0;Iv<CCI.size();Iv++)
	//int Iv=33;
	{
		
		//cout<<Iv<<"\n";
		Mat img=CCI[Iv].GetClusterImage().clone();
		int clusterCount=3;
		Mat clusterResult=KmeansCluster(img,clusterCount);
		
		 //�ûҶȱ�ʾ������ 
		float step=255/(clusterCount - 1);  
		Mat showclusterResult=Mat::zeros(clusterResult.size(),CV_8UC1);
		for(int i=0; i < clusterResult.rows; i++)   
		{    
			for(int j=0; j< clusterResult.cols; j++)   
			{     
				showclusterResult.at<uchar>(i,j) =clusterResult.at<uchar>(i,j)*step;
			}   
	   }  

		//namedWindow("K-Means�ָ�Ч��",0);   
		//imshow("K-Means�ָ�Ч��", showclusterResult);  
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
		threshold(gray, ChromBi, GrayMean-0, 255, THRESH_TOZERO_INV);
		threshold(ChromBi, ChromBi, 0, 255, THRESH_BINARY);
		//imshow("ChromBi",ChromBi);
		//add(ChromBi,showclusterResult,ChromBi);
		//imshow("ChromBi+",ChromBi);
		int Num2=0;
		int Num1=0;//���ڼ�¼clusterResult�ж�Ӧ��ChromBi��255�������ص��������߽ϴ��߼�Ϊ����Ȥ����
		int Num0=0;

		for(int i=0;i<ChromBi.rows;i++)
		{
			for(int j=0;j<ChromBi.cols;j++)
			{
				if(ChromBi.at<uchar>(i,j)==255)
				{
					if(clusterResult.at<uchar>(i,j)==0) Num0++;
					if(clusterResult.at<uchar>(i,j)==1) Num1++;
					if(clusterResult.at<uchar>(i,j)==2) Num2++;
				}
			}	
		} 
		int ChromFlag=0;
		int Max=Num0;
		if(Num1>Num0)
		{
			ChromFlag=1;
			Max=Num1;
		}
		if(Num2>Max)
		{
			ChromFlag=2;
			Max=Num2;
		}
		//cout<<ChromFlag<<"\t"<<Max<<"\n";
		//cout<<Num0<<"\t"<<Num1<<"\t"<<Num2<<"\n";
		
		
		//����ȷ���ı�Ǻţ��ָ��Ⱦɫ��
		Mat chrom=Mat::zeros(ChromBi.size(),CV_8UC1);
		for(int i=0;i<ChromBi.rows;i++)
		{
			for(int j=0;j<ChromBi.cols;j++)
			{
				if(clusterResult.at<uchar>(i,j)==ChromFlag)
				{
					//chrom.at<Vec3b>(i,j)=img.at<Vec3b>(i,j);
					chrom.at<uchar>(i,j)=255;
				}
			}
		}
		//imshow("chrom",chrom);
		
		//���ٳ��ĸ���Ȥ����ָ�ɸ�����ͨ����
		//���ٳ��ĸ���Ȥ����ָ�ɸ�����ͨ����
		Mat Labelimage=Mat::zeros(CCI[Iv].GetClusterImage().size(),CV_8UC1);
		int Zonenum=0;
		//���ƹ��ָ� ��С��Ӿ���<60 ֻ��һ����ͨ����Zonenum=1����chrom��Ӧ��LabelimageȫΪ1  ע�⣺Ӧ��ʹ����ͨ�������������ɸѡ������ȡ����
		//cout<<Iv<<"\t["<<CCI[Iv].GetminRectShort()<<","<<CCI[Iv].GetminRectLong()<<"]\n";
		int mS=CCI[Iv].GetminRectShort();
		//cout<<Iv<<"\t"<<mS<<"\n";
		if(mS<ExternChromosomeWidthMin+10) D8ConnectedComponent(chrom, Labelimage,Zonenum);
		if(mS>=ExternChromosomeWidthMin+10 && mS<=ExternChromosomeWidthMax+10)
		{
			//cout<<Iv<<"\t"<<mS<<"\n";
			D8ConnectedComponent(chrom, Labelimage,Zonenum);
			//cout<<Zonenum<<"\n";
			if(Zonenum>=2)
			{
				Mat element = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//��2 �ṹԪ��
	            dilate(chrom,chrom,element);
				D8ConnectedComponent(chrom, Labelimage,Zonenum);
			//	Zonenum=1;
			//	for(int i=0;i<chrom.rows;i++)
			//	{
			//		for(int j=0;j<chrom.cols;j++)
			//		{
			//			if(chrom.at<uchar>(i,j)!=0)
			//			{
			//				Labelimage.at<uchar>(i,j)=1;
			//			}
			//		}
			//	}
			}
		}
		if(mS>ExternChromosomeWidthMax+10)
		{
			D8ConnectedComponent(chrom, Labelimage,Zonenum);
			//cout<<Zonenum<<"\n";
			//if(Zonenum>50) continue;
			if(Zonenum>=2)
			{
					vector<Mat> imageVector;
					vector<Mat> InterestVector;
					Label2Vector_RGB(Labelimage,Zonenum,img,imageVector,0);
					Label2Vector_RGB(Labelimage,Zonenum,chrom,InterestVector,0);
					int mN=0;
					for(int i=0;i<imageVector.size();i++)
					{
						vector<vector<Point> > contours_out;  
						vector<Vec4i> hierarchy_out;  
						Mat contoursTemp=InterestVector[i].clone();//findContours��ı�ԭͼ�񣬹���һ����Ϊ�˷�ֹ�ı�ChromosomeGroup��Ԫ��
						findContours(contoursTemp, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE);
						if(contours_out.size()==0) continue;		
						else
						{	
							RotatedRect mRTemp=minAreaRect(contours_out[0]);
							if(mRTemp.size.width==0||mRTemp.size.height==0) continue;
							Rect boundRect;
							boundRect = boundingRect(contours_out[0]);
							if(mRTemp.size.width<ExternChromosomeWidthMin||mRTemp.size.height<ExternChromosomeWidthMin)
							{
								Mat element = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//��2 �ṹԪ��
								dilate(InterestVector[i],InterestVector[i],element);
								int Blank=10;
								int x=(int)boundRect.x-Blank;  x=(x>0?x:0);
								int y=(int)boundRect.y-Blank;  y=(y>0?y:0); 
								int w=boundRect.width+2*Blank;   w=(w<imageVector[i].size().width-x?w:imageVector[i].size().width-x);
								int h=boundRect.height+2*Blank;  h=(h<imageVector[i].size().height-y?h:imageVector[i].size().height-y);
								Mat IVTemp = InterestVector[i](Rect(x,y,w,h));  //Rect��������ROI,ע����ǣ�cols������rows
								Mat mask=IVTemp.clone();
								threshold(mask, mask, 0, 255, THRESH_BINARY);//��mask���ж�ֵ������mask��һ������  
								Mat chromLocation = chrom(Rect(x,y,w,h));  //Rect��������ROI,ע����ǣ�cols������rows
								IVTemp.copyTo(chromLocation,mask);
								mN++;
							}
						}
					}
					if(mN) D8ConnectedComponent(chrom, Labelimage,Zonenum);
			}
		}
		//cout<<Zonenum<<"\n";
		//D8ConnectedComponent(chrom, Labelimage,Zonenum);
		vector<Mat> imageVector;
		vector<Mat> InterestVector;
		Label2Vector_RGB(Labelimage,Zonenum,img,imageVector,0);
		Label2Vector_RGB(Labelimage,Zonenum,chrom,InterestVector,0);
		for(int i=0;i<imageVector.size();i++)
		{
			vector<vector<Point> > contours_out;  
			vector<Vec4i> hierarchy_out;  
			Mat contoursTemp=InterestVector[i].clone();//findContours��ı�ԭͼ�񣬹���һ����Ϊ�˷�ֹ�ı�ChromosomeGroup��Ԫ��
			findContours(contoursTemp, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE);
			//cout<<contours_out.size();
			if(contours_out.size()==0) continue;
			else
			{
				//for(int ci=0;ci<contours_out.size();ci++)
				//{
					ClassClusterInfo CCITemp;
					Rect boundRect;
					boundRect = boundingRect(contours_out[0]);
					if(boundRect.width<ExternWidthChromatidMin||boundRect.height<ExternWidthChromatidMin) continue;
					//rectangle(imageVector[i], Rect((int)boundRect.x, (int)boundRect.y,boundRect.width, boundRect.height), Scalar(0, 255, 0),2); //������Ȥ��������
					//ClassChromLocation CCLTemp(boundRect,Point(0,0));
					//int Blank=1;
					//int x=(int)boundRect.x-Blank;  x=(x>0?x:0);
					//int y=(int)boundRect.y-Blank;  y=(y>0?y:0); 
					//int w=boundRect.width+2*Blank;   w=(w<imageVector[i].size().width?w:imageVector[i].size().width);
					//int h=boundRect.height+2*Blank;  h=(h<imageVector[i].size().height?h:imageVector[i].size().height);
					//boundRect=Rect(x,y,w,h);
					Mat ICTemp = imageVector[i](boundRect);  //Rect��������ROI,ע����ǣ�cols������rows��
					CCITemp.SetClassClusterInfo(ICTemp,boundRect,CCI[Iv].GetRelativeOrigin());
					CCIVTemp.push_back(CCITemp);
				//}
				
			}
		}
		//cout<<Iv<<"\n";
	}
	
	CCI.clear();
	CCI.insert(CCI.begin(),CCIVTemp.begin(),CCIVTemp.end());
}