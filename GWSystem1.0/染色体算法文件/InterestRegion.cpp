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
	Ssize.width=image.cols*Sscale;//压缩图片，减少数据量，加快运算
	Ssize.height=image.rows*Sscale;
	Bsize.width=image.cols;//用于还原图片尺寸
	Bsize.height=image.rows;
	Mat img;
	resize(image,img,Ssize,0,0,INTER_LINEAR);
	//imshow("缩放后原图",img);
	//转化为灰度图
	Mat gray;
	cvtColor( img, gray, CV_BGR2GRAY );
	//imshow("灰度图",gray);
	int GraymeanALL=NonezeroMeanGrayValue(gray);
	Mat Bi;
	threshold(gray,Bi,GraymeanALL-30,255,THRESH_BINARY_INV);
	//imshow("Bi",Bi);
	//进行Kmeans聚类
	
	int clusterCount=2;
	Mat clusterResult=KmeansCluster(img,clusterCount);
	////利用二值图Bi中255对应感兴趣区域
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
 //   imshow("K-Means分割效果", showclusterResult);  

	//利用二值图Bi中255对应感兴趣区域
	int Num1=0;//用于记录clusterResult中对应于Bi（255）的像素点数，两者较大者即为感兴趣区域
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
	 
  //用灰度表示分类结果 
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
 //   imshow("K-Means分割效果", showclusterResult);  
	//腐蚀膨胀剩下染色体
	Mat BigNoInterest;//不感兴趣的大杂质
	threshold(clusterResult, BigNoInterest, 0, 255, THRESH_BINARY);//大于阈值，则取最大值（255为白色）
	Mat srcBNI=BigNoInterest.clone();
	Mat elementBNI_e = getStructuringElement(MORPH_ELLIPSE, Size(10,10));//核2 结构元素
	erode(BigNoInterest,BigNoInterest,elementBNI_e);
	Mat elementBNI_d = getStructuringElement(MORPH_ELLIPSE, Size(20,20));//核2 结构元素
	dilate(BigNoInterest,BigNoInterest,elementBNI_d);
	//imshow("较大杂质位置",BigNoInterest);
	Mat Interest;//感兴趣区域
	subtract(srcBNI,BigNoInterest,Interest);
	//imshow("感兴趣区域",Interest);
	
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
 //  imshow("去除较大杂质后",srcBNI);
	//Mat InterestCluster=clusterResult.clone();
   resize(Interest,Interest,Bsize,0,0,INTER_LINEAR);//恢复原图大小
   //imshow("去除较大杂质后",InterestCluster);
   //cout<<"大傻逼\n";
   //Mat image1=image.clone();
    for(int i=0;i<Interest.rows;i++)
	{
		for(int j=0;j<Interest.cols;j++)
		{
			if(Interest.at<uchar>(i,j)==0)
				image.at<Vec3b>(i,j)=Vec3b(0,0,0);
			if(i==Interest.rows-1||i==0) image.at<Vec3b>(i,j)=Vec3b(0,0,0);//切边
			if(j==Interest.cols-1||j==0) image.at<Vec3b>(i,j)=Vec3b(0,0,0);
			//else
			//	image1.at<Vec3b>(j,i)=Vec3b(0,0,0);

		}
	}
	//cout<<"大傻逼\n";
	//namedWindow("大傻逼", CV_WINDOW_NORMAL);
	//imshow("大傻逼",image);
	//imwrite(filename+"rr//"+imgname+".1"+imgformat,image1);
	//imwrite(filename+"rr//"+imgname+".2"+imgformat,image);
	//将抠出的感兴趣区域分割成各个连通区域
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
		Mat contoursTemp=InterestVector[i].clone();//findContours会改变原图像，故这一步是为了防止改变ChromosomeGroup中元素
		findContours(contoursTemp, contours_out, hierarchy_out,RETR_EXTERNAL, CHAIN_APPROX_NONE);
		if(contours_out.size()==0)
		{
			continue;
			//Mat emptyMat;
			//imageVector[i]=emptyMat;//  用 img.empty() 判断是否为空
			
		}
		else
		{
			Mat ICTemp;
			ClassClusterInfo CCITemp;
			Rect boundRect;
			boundRect = boundingRect(contours_out[0]);
			if(boundRect.width<ExternWidthChromatidMin||boundRect.height<ExternWidthChromatidMin) continue;
			//减少较长杂质
		    if(boundRect.height>1000) continue;//1000约为原图宽度的0.5

			//int Blank=1;
			//int x=(int)boundRect.x-Blank;  x=(x>0?x:0);
			//int y=(int)boundRect.y-Blank;  y=(y>0?y:0); 
			//int w=boundRect.width+2*Blank;   w=(w<imageVector[i].size().width?w:imageVector[i].size().width);
			//int h=boundRect.height+2*Blank;  h=(h<imageVector[i].size().height?h:imageVector[i].size().height);
			//boundRect=Rect(x,y,w,h);
			ICTemp = imageVector[i](boundRect);  //Rect方法定义ROI,注意宽是：cols；高是rows。
			CCITemp.SetClassClusterInfo(ICTemp,boundRect);
			InterestCluster.push_back(CCITemp);
		}
	}
}