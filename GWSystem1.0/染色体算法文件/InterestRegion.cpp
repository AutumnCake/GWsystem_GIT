#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"


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


void InterestRegion(Mat image,double Sscale,vector<ClassClusterInfo>& InterestCluster)
{
	//double Sscale=0.1;
	Size Ssize,Bsize;
	Ssize.width=image.cols*Sscale;//ѹ��ͼƬ���������������ӿ�����
	Ssize.height=image.rows*Sscale;
	Bsize.width=image.cols;//���ڻ�ԭͼƬ�ߴ�
	Bsize.height=image.rows;
	Mat img;
	resize(image,img,Ssize,0,0,INTER_LINEAR);
	//imshow("���ź�ԭͼ",img);
	//ת��Ϊ�Ҷ�ͼ
	Mat gray;
	cvtColor( img, gray, CV_BGR2GRAY );
	//imshow("�Ҷ�ͼ",gray);
	int GraymeanALL=NonezeroMeanGrayValue(gray);
	Mat Bi;
	threshold(gray,Bi,GraymeanALL-30,255,THRESH_BINARY_INV);
	//imshow("Bi",Bi);
	//����Kmeans����
	
	int clusterCount=2;
	Mat clusterResult=KmeansCluster(img,clusterCount);
	////���ö�ֵͼBi��255��Ӧ����Ȥ����
	//float step=255/(clusterCount - 1);  
	//Mat showclusterResult=Mat::zeros(clusterResult.size(),CV_8UC1);
 //   for(int i=0; i < showclusterResult.rows; i++)   
 //   {    
 //       for(int j=0; j< showclusterResult.cols; j++)   
 //       {     
 //           showclusterResult.at<uchar>(j,i) =clusterResult.at<uchar>(j,i)*step;
 //       }   
 //   }  
 // 
 //   imshow("K-Means�ָ�Ч��", showclusterResult);  

	//���ö�ֵͼBi��255��Ӧ����Ȥ����
	int Num1=0;//���ڼ�¼clusterResult�ж�Ӧ��Bi��255�������ص��������߽ϴ��߼�Ϊ����Ȥ����
	int Num0=0;
	for(int i=0;i<Bi.rows;i++)
	{
		for(int j=0;j<Bi.cols;j++)
		{
			if(Bi.at<uchar>(i,j)==255)
			{
				if(clusterResult.at<uchar>(i,j)==0) Num0++;
				if(clusterResult.at<uchar>(i,j)==1) Num1++;
			}
		}	
	}
	 //cout<<Num0<<"-"<<Num1<<"\n";
	 if(Num0>Num1) Invert0_1(clusterResult,clusterResult);
	 
  //�ûҶȱ�ʾ������ 
 //   float step=255/(clusterCount - 1);  
	//Mat showclusterResult=Mat::zeros(clusterResult.size(),CV_8UC1);
 //   for(int i=0; i < showclusterResult.rows; i++)   
 //   {    
 //       for(int j=0; j< showclusterResult.cols; j++)   
 //       {     
 //           showclusterResult.at<uchar>(j,i) =clusterResult.at<uchar>(j,i)*step;
 //       }   
 //   }  
 // 
 //   imshow("K-Means�ָ�Ч��", showclusterResult);  
	//��ʴ����ʣ��Ⱦɫ��
	Mat BigNoInterest;//������Ȥ�Ĵ�����
	threshold(clusterResult, BigNoInterest, 0, 255, THRESH_BINARY);//������ֵ����ȡ���ֵ��255Ϊ��ɫ��
	Mat srcBNI=BigNoInterest.clone();
	Mat elementBNI_e = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//��2 �ṹԪ��
	erode(BigNoInterest,BigNoInterest,elementBNI_e);
	Mat elementBNI_d = getStructuringElement(MORPH_ELLIPSE, Size(20,20));//��2 �ṹԪ��
	dilate(BigNoInterest,BigNoInterest,elementBNI_d);
	//imshow("�ϴ�����λ��",BigNoInterest);
	Mat Interest;//����Ȥ����
	subtract(srcBNI,BigNoInterest,Interest);
	//imshow("����Ȥ����",Interest);
	
	// for(int i=0;i<BigNoInterest.rows-1;i++)
	//{
	//	for(int j=0;j<BigNoInterest.cols-1;j++)
	//	{
	//		if(BigNoInterest.at<uchar>(j,i)==255)
	//		{
	//			Rect ccomp;  
	//			floodFill(BigNoInterest,Point(i,j), Scalar(0), &ccomp,Scalar(1),Scalar(1),8);
	//			floodFill(srcBNI,Point(i,j), Scalar(0), &ccomp,Scalar(1),Scalar(1),8);
	//		}

	//	}
	//	
	//}
 //  imshow("ȥ���ϴ����ʺ�",srcBNI);
	//Mat InterestCluster=clusterResult.clone();
   resize(Interest,Interest,Bsize,0,0,INTER_LINEAR);//�ָ�ԭͼ��С
   //imshow("ȥ���ϴ����ʺ�",InterestCluster);
   //cout<<"��ɵ��\n";
   //Mat image1=image.clone();
    for(int i=0;i<Interest.rows;i++)
	{
		for(int j=0;j<Interest.cols;j++)
		{
			if(Interest.at<uchar>(i,j)==0)
				image.at<Vec3b>(i,j)=Vec3b(0,0,0);
			if(i==Interest.rows-1||i==0) image.at<Vec3b>(i,j)=Vec3b(0,0,0);//�б�
			if(j==Interest.cols-1||j==0) image.at<Vec3b>(i,j)=Vec3b(0,0,0);
			//else
			//	image1.at<Vec3b>(j,i)=Vec3b(0,0,0);

		}
	}
	//cout<<"��ɵ��\n";
	//namedWindow("��ɵ��", CV_WINDOW_NORMAL);
	//imshow("��ɵ��",image);
	//imwrite(filename+"rr//"+imgname+".1"+imgformat,image1);
	//imwrite(filename+"rr//"+imgname+".2"+imgformat,image);
	//���ٳ��ĸ���Ȥ����ָ�ɸ�����ͨ����
	Mat Labelimage=Mat::zeros(image.size(),CV_8UC1);
	int Zonenum=0;
	D8ConnectedComponent(Interest, Labelimage,Zonenum);
	vector<Mat> imageVector;
	vector<Mat> InterestVector;
	Label2Vector_RGB(Labelimage,Zonenum,image,imageVector,0);
	Label2Vector_RGB(Labelimage,Zonenum,Interest,InterestVector,0);
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
			Mat ICTemp;
			ClassClusterInfo CCITemp;
			Rect boundRect;
			boundRect = boundingRect(contours_out[0]);
			if(boundRect.width<ExternWidthChromatidMin||boundRect.height<ExternWidthChromatidMin) continue;
			//���ٽϳ�����
		    if(boundRect.height>1000) continue;//1000ԼΪԭͼ��ȵ�0.5

			//int Blank=1;
			//int x=(int)boundRect.x-Blank;  x=(x>0?x:0);
			//int y=(int)boundRect.y-Blank;  y=(y>0?y:0); 
			//int w=boundRect.width+2*Blank;   w=(w<imageVector[i].size().width?w:imageVector[i].size().width);
			//int h=boundRect.height+2*Blank;  h=(h<imageVector[i].size().height?h:imageVector[i].size().height);
			//boundRect=Rect(x,y,w,h);
			ICTemp = imageVector[i](boundRect);  //Rect��������ROI,ע����ǣ�cols������rows��
			CCITemp.SetClassClusterInfo(ICTemp,boundRect);
			InterestCluster.push_back(CCITemp);
		}
	}
}