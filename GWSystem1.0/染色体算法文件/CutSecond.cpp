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
	//将Lump进一步分割
	//int LI=1;
	//cout<<ChromCluster.size();
	for(int LI=0;LI<ChromCluster.size();LI++)
	{
		//cout << "正在处理第LI个染色体团：LI=" << LI << "\n";
		Mat img=ChromCluster[LI].GetClusterImage().clone();
		//imshow("img",img);
		//阈值确定染色体位置
		//转化为灰度图
		Mat gray;
		cvtColor( img, gray, CV_BGR2GRAY );
		//imshow("灰度图",gray);
		//0阈值二值图
		Mat Bi;
		threshold(gray, Bi, 0, 255, THRESH_BINARY);
		int GrayMean=NonezeroMeanGrayValue(gray);
		Mat ChromBi;		
		threshold(gray, ChromBi, GrayMean+10, 255, THRESH_TOZERO_INV);
		threshold(ChromBi, ChromBi, 0, 255, THRESH_BINARY);
		//imshow("ChromBi",ChromBi);
		//分别计算ChromBi与Bi-ChromBi的灰度均值
		Mat Biout=Bi-ChromBi;
		//imshow("Bi",Bi);
		//cout<<"大傻逼\n";
		int ChromBiMean=NonezeroMeanGrayValue(gray,ChromBi);
		
		int BiMean=NonezeroMeanGrayValue(gray,Biout);

		//cout<<ChromBiMean<<"\t"<<BiMean<<"\n";
		int MiddleThreshold=ChromBiMean+(BiMean-ChromBiMean)*0.9;
		Mat MiddleBi;
		threshold(gray, MiddleBi, MiddleThreshold, 0, THRESH_TOZERO_INV);
		//imshow("MiddleBi",MiddleBi);
		//cout<<GrayMean+10<<"\t"<<MiddleThreshold<<"\n";


		//根据灰度图二值化结果,并结合原图，转化出高度灰度图用于灌水法分割
		Mat GrayHeight=Mat::zeros(img.size(),CV_8UC1);
		Mat seed=Mat::zeros(GrayHeight.size(),CV_8UC1);
		int min=40;//此处可为1~120间任意值，不能取0
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
		//灰度高度图分水岭
		//imshow("GrayHeight",GrayHeight);
		////种子图填洞 
		////mtf 20190529 初步怀疑是种子中的空洞导致莫名其妙崩溃。暂时认为可能并不是这个原因
		//imshow("seed", seed);
		//Mat seedHole = seed.clone();
		//Rect ccomp;
		//floodFill(seedHole, Point(0, 0), Scalar(255), &ccomp, Scalar(1), Scalar(1), 8);//灰度区间为经验值
		//bitwise_not(seedHole, seedHole);
		//Mat seednohole;
		//add(seedHole, seed, seednohole);
		//threshold(seednohole, seednohole, 0, 255, THRESH_BINARY);
		//imshow("seednohole", seednohole);
		//分水岭分割
		int Zonenum=0;
		Mat labelimg=Mat::zeros(img.size(),CV_8U);
		//cout << "开始分水岭……\n";
		myWatershed(GrayHeight,seed,labelimg,Zonenum);
		//cout << "结束分水岭……\n";
	  //  //对每一个区域进行颜色填充  
	  // //为了清晰显示分割后的图，做如下处理
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
	  //  imshow("颜色填充后的分割图",PerspectiveImage); 

		//根据labelimg进行分割，将分割结果显示到不同图片上
		vector<Mat> imageVector;
		vector<Mat> InterestVector;
		Label2Vector_RGB(labelimg,Zonenum,img,imageVector,0);
		Label2Vector_RGB(labelimg,Zonenum,Bi,InterestVector,0);
		//imshow("Bi",Bi);
		//int LN=FirstCutLocation.size();//用于标记对应vector<Mat>标号
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
				//cout<<"a";
				ClassClusterInfo CCITemp;
				Rect boundRect;
				boundRect = boundingRect(contours_out[0]);
				//if(boundRect.width<ExternWidthChromatidMin||boundRect.height<ExternWidthChromatidMin) continue;
				//cout<<"d";
				//rectangle(imageVector[i], Rect((int)boundRect.x, (int)boundRect.y,boundRect.width, boundRect.height), Scalar(0, 255, 0),2); //将感兴趣区域框出来
				//ClassChromLocation CCLTemp(boundRect,Point(0,0));
				//ICTemp = imageVector[i](Rect((int)boundRect.x, (int)boundRect.y,boundRect.width, boundRect.height));  //Rect方法定义ROI,注意宽是：cols；高是rows。
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
