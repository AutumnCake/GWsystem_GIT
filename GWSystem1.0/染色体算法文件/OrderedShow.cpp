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

#include"ClassClusterInfo.h"
#include"MyFunction.h"
#include"BoolSort.h"

void OrderedShow(vector<ClassClusterInfo> CCIv,Mat& img,int & DicNum,Point StartShowPoint,int ColSpace,int RowSpace,int minCentromereNum,int maxCentromereNum,double Sscale)
{
	if(img.empty())
		img =  Mat::zeros(1000,2000,CV_8UC3);
	DicNum=0;
	for(int i=0;i<CCIv.size();i++)
	{
		 int CLVS=CCIv[i].GetCentromereLocationVector().size();
		 if(CLVS>=minCentromereNum&&CLVS<=maxCentromereNum)
		 {
			Mat imgGS=CCIv[i].GetClusterImage().clone();
			Size wsize,tsize;
			wsize.width=CCIv[i].GetminRectShort();
			wsize.height=CCIv[i].GetminRectLong();
			int imgGSwidth=imgGS.size().width;
			int imgGSheight=imgGS.size().height;
			tsize.width=(wsize.width+imgGSwidth);
			tsize.height=(wsize.height+imgGSheight);
			 //旋转
			////以OnWidthPoint1为中心，外接矩形长轴相对于竖直方向的旋转角度为角度
			//int i=minAreaRect_VectorOrderNum[j].OrderNum;
			//minAreaRectInfoStrcut Rot_minAreaRectInfo;
			//minAreaRectOFchromosomeWidthHeighthAreaRatio(SingleVector[i][2],Rot_minAreaRectInfo);
			 //平移
			 Mat t_mat =cv::Mat::zeros(2, 3, CV_32FC1);  
  
			t_mat.at<float>(0, 0) = 1;  
			t_mat.at<float>(0, 2) = (wsize.width)/2; //水平平移量  
			t_mat.at<float>(1, 1) = 1;  
			t_mat.at<float>(1, 2) = (wsize.height)/2; //竖直平移量  
			////获取旋转矩阵（2x3矩阵）
			//Point CentPoint=(CCIv[i].GetminRectCenterPointxOnShortside(0)+CCIv[i].GetminRectCenterPointxOnShortside(1));
			double angle=0;
			vector<Point2f> vPL=CCIv[i].GetminRectCenterPointxOnLongside();
			double minDCv0=10000;
			double minDCv1=10000;
			for(int CLVSI=0;CLVSI<CLVS;CLVSI++)
			{
				double DCv0=DistancePoints(CCIv[i].GetCentromereLocationVector()[CLVSI],vPL[0]);
				double DCv1=DistancePoints(CCIv[i].GetCentromereLocationVector()[CLVSI],vPL[1]);
				if(DCv0<minDCv0)
					minDCv0=DCv0;
				if(DCv1<minDCv1)
					minDCv1=DCv1;
			}
			if(minDCv0<minDCv1)
				angle=90-(CCIv[i].GetAngleBaseUpright() *180/(PI));
			else
				angle=270-(CCIv[i].GetAngleBaseUpright() *180/(PI));
			Point CentPoint=Point(tsize.width/2,tsize.height/2);
			Mat rot_mat = cv::getRotationMatrix2D(CentPoint, angle, 1.0);
			//cout<<CCIv[i].GetAngleBaseUpright() *180/(PI)<<"\n";
			//cout<< minAreaRectInfo.AngleBaseUpright;
			//根据旋转矩阵进行仿射变换
			Mat rot_image, t_image;
			warpAffine(imgGS, t_image, t_mat, tsize); 
			warpAffine(t_image, rot_image, rot_mat, tsize); 
			Size Ssize;
			Ssize.width=rot_image.cols*Sscale;//压缩图片，减少数据量，加快运算
			Ssize.height=rot_image.rows*Sscale;
			resize(rot_image,rot_image,Ssize,0,0,INTER_LINEAR);


			Mat mask;
			cvtColor(rot_image,mask,CV_RGB2GRAY);
			Mat imgLocation;
			int Maxofline=(img.cols-StartShowPoint.x*2)/ColSpace;
			imgLocation=img(Rect(StartShowPoint.x+ColSpace*(DicNum%Maxofline), StartShowPoint.y+RowSpace*(DicNum/Maxofline),rot_image.size().width,rot_image.size().height));
			rot_image.copyTo(imgLocation,mask);
			DicNum++;
			//cout<<DicNum<<"\n";
		}
	}
}

void OrderedShow(vector<ClassClusterInfo> CCIv,Mat& img,Point StartShowPoint,int ColSpace,int RowSpace,double Sscale)
{
	if(img.empty())
	img =  Mat::zeros(1000,2000,CV_8UC3);
	int DicNum=0;
	for(int i=0;i<CCIv.size();i++)
	{
		 {
			Mat imgGS=CCIv[i].GetClusterImage().clone();
			Size Ssize;
			Ssize.width=imgGS.cols*Sscale;//压缩图片，减少数据量，加快运算
			Ssize.height=imgGS.rows*Sscale;
			resize(imgGS,imgGS,Ssize,0,0,INTER_LINEAR);
			Mat mask;
			cvtColor(imgGS,mask,CV_RGB2GRAY);
			Mat imgLocation;
			int Maxofline=(img.cols-StartShowPoint.x*2)/ColSpace;
			imgLocation=img(Rect(StartShowPoint.x+ColSpace*(DicNum%Maxofline), StartShowPoint.y+RowSpace*(DicNum/Maxofline),imgGS.size().width,imgGS.size().height));
			imgGS.copyTo(imgLocation,mask);
			DicNum++;
			//cout<<DicNum<<"\n";
		}
	}
}