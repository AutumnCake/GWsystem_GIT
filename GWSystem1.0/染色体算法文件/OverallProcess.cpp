// Kmeans总进度2.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include"windows.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>


#include"Int_to_String.h"
#include"BoolSort.h"
#include"MyFunction.h"


#include"ClassClusterInfo.h"
#include"InterestRegion.h"
#include"CutFirst.h"
#include"DrawClassChromInfoVector.h"
#include"ClassifyChromo.h"
#include"CutSecond.h"
#include"res.h"
//识别着丝粒点位置
#include"LocateCentromere.h"
//去杂质
#include"HSVRemoveImpurity.h"
////检验用
//#include"DrawClassChromInfoVector.h"
//排序
#include"OrderedShow.h"

//CNN
#include"normalizeImgSizeCNN.h"
#include"CNNpredict.h"

using namespace std;
using namespace cv;

#define PI 3.1415926



//int TQN = 0;
////CNN识别图像尺寸
int CNN_IMG_SIZE = 40;

int OverallProcess(string imgpath)
{
	//*******************************************************************************************************
	//图像读取
	//*******************************************************************************************************
	////string filename=E:\\tmp\\ChromImage\\100Xoil\\染色体20\\";
	//string filename = "E:\\CA_PIC\\2019年4月23日17-04-52的处理结果\\抓图结果\\";//双
	//																//string imgname="CA-100X-06";//12 02 03 04 25 26
	//string imgname = "玻片4_染色体_005000162";//RGB或HSV向量距离去杂质
	//string imgformat = ".bmp";
	//Mat image = imread(filename + imgname + imgformat);
	////cout<<image.size()<<"\n";
	////namedWindow("原图",WINDOW_NORMAL);
	////imshow("原图",image);
	Mat image = imread(imgpath);
	//读取图像为空时，直接跳过，即返回；
	if (image.empty())
	{
		return 0;
	}
	//*******************************************************************************************************
	//分割出感兴趣区域
	//*******************************************************************************************************

	double Sscale = 0.1;//缩放倍数
	vector<ClassClusterInfo> InterestCluster;
	InterestRegion(image, Sscale, InterestCluster);
	//sort(InterestCluster,SortClassClusterInfoLess_S);
	//for(int i=0;i<InterestCluster.size();i++)
	//{
	//	imwrite(filename+"rr//"+imgname+"."+Int_to_String(i)+imgformat,InterestCluster[i].GetClusterImage());
	//	//cout<<"感兴趣："<<Location[i].GetLabel()<<"\t"<<i<<"\n";
	//}

	//*******************************************************************************************************
	//对每个区域进行初步切割FirstCut
	//*******************************************************************************************************
	CutFirst(InterestCluster);

	//Mat ICimg=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(InterestCluster,ICimg);
	//imwrite(filename+"rr//"+imgname+"_"+"ICimg"+imgformat,ICimg);
	//cout<<CutFirstLocation.size()<<"\n";
	//for(int i=0;i<CutFirstLocation.size();i++)
	//cout<<CutFirstLocation[i].GetRelativeOrigin()<<"\n";
	//for(int i=0;i<InterestCluster.size();i++)
	//{
	//	imwrite(filename+"rr//ri//ra//"+imgname+"."+Int_to_String(i)+imgformat,InterestCluster[i].GetClusterImage());
	////	cout<<CutFirstLocation[i].GetLabel()<<"\t"<<i<<"\n";
	//}
	//
	//
	////获取染色体信息,并去除部分杂质
	//vector<ClassChromInfo> CutFirstClusterInfo=GetClassChromInfo(CutFirstCluster,CutFirstLocation);
	//cout<<CutFirstClusterInfo.size()<<"\t"<<CutFirstCluster.size()<<"\n";
	////for(int i=0;i<CutFirstClusterInfo.size();i++)
	////{
	////	cout<<CutFirstClusterInfo[i].GetWidth()<<"\t"<<CutFirstClusterInfo[i].GetAreaRatio()<<"\t"<<CutFirstClusterInfo[i].GetLabel()<<"\n";
	////}

	//*******************************************************************************************************
	//分类，分出疑似断片及单着丝粒染色环，粘连染色体及较大杂质，单个染色体,并去除部分杂质
	//*******************************************************************************************************
	vector<ClassClusterInfo> LumpInfo, SingleInfo, TinyInfo;
	ClassifyChromo(InterestCluster, LumpInfo, SingleInfo, TinyInfo);
	//////检验分类结果，画彩图
	//Mat SFCS=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(SingleInfo,SFCS);
	//imwrite(filename+"rr//"+imgname+"."+"SFCS"+imgformat,SFCS);
	//Mat SFCL=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(LumpInfo,SFCL);
	//imwrite(filename+"rr//"+imgname+"."+"SFCL"+imgformat,SFCL);
	//Mat SFCT=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(TinyInfo,SFCT);
	//imwrite(filename+"rr//"+imgname+"."+"SFCT"+imgformat,SFCT);

	////进一步精确染色体宽度范围
	//sort(SingleInfo,SortClassClusterInfoLess_S);
	//for(int i=0;i<SingleInfo.size();i++)
	//	cout<<i<<"\t"<<SingleInfo[i].GetminRectShort()<<"\n";
	//将筛选出来的较大染色体进行二次分割,此处改变CutFirstCluster，LumpInfo, SingleInfo, TinyInfo等，而不是另建一个CutSecondCluster等，为了节省内存？
	//注意vector.clear以节省内存
	//cout<<LumpInfo.size()<<"\n";

	//*******************************************************************************************************
	//针对第一次分割后，筛选出的染色体团LumpInfo进行二次分割---------------------------------------------------------------------------(目前在CutSecond，Mywatershed里有bug。暂时跳过这一步，后续debug或者改算法)
	//*******************************************************************************************************
	//输出Lump，debug
	//for (int i = 0;i<LumpInfo.size();i++)
	//{
	//	imwrite("C://Users//MaTengfei//Desktop//tt//lump//" + Int_to_String(i) + imgformat, LumpInfo[i].GetClusterImage());
	//}

	cout << "开始二次分割……\n";
	CutSecond(LumpInfo);
	//cout << LumpInfo.size() << "\n";
	vector<ClassClusterInfo> LITemp, SITemp, TITemp;
	ClassifyChromo(LumpInfo, LITemp, SITemp, TITemp);
	//cout<<LumpInfo.size()<<"\t"<<SITemp.size()<<"\n";
	LumpInfo.insert(LumpInfo.begin(), LITemp.begin(), LITemp.end());
	LITemp.clear();
	//cout<<SingleInfo.size()<<"\n";
	SingleInfo.insert(SingleInfo.end(), SITemp.begin(), SITemp.end());
	//cout<<SingleInfo.size()<<"\t"<<SITemp.size()<<"\n";
	SITemp.clear();
	TinyInfo.insert(TinyInfo.end(), TITemp.begin(), TITemp.end());
	TITemp.clear();
	cout << "分割完成……\n";

	//////检验分类结果，画彩图
	//Mat SSCS=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(SingleInfo,SSCS);
	//imwrite(filename+"rr//"+imgname+"."+"SSCS"+imgformat,SSCS);
	//Mat SSCL=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(LumpInfo,SSCL);
	//imwrite(filename+"rr//"+imgname+"."+"SSCL"+imgformat,SSCL);
	//Mat SSCT=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(TinyInfo,SSCT);
	//imwrite(filename+"rr//"+imgname+"."+"SSCT"+imgformat,SSCT);

	//*******************************************************************************************************
	//利用HSV向量，筛选杂质
	//*******************************************************************************************************

	HSVRemoveImpurity(SingleInfo, LumpInfo, TinyInfo);
	////输出Single便于单独研究
	//for(int i=0;i<SingleInfo.size();i++)
	//{
	//	imwrite(filename+"rr//ri//"+imgname+"."+Int_to_String(i)+imgformat,SingleInfo[i].GetClusterImage());
	////	cout<<CutFirstLocation[i].GetLabel()<<"\t"<<i<<"\n";
	//}
	//给田琦制图

	//for(int i=0;i<SingleInfo.size();i++)
	//{
	//	TQN++;
	//	Mat TQ=Mat::zeros(256,256,CV_8UC3);
	//	Size Ssize;
	//	Mat TQImg=SingleInfo[i].GetClusterImage();
	//	double S=((TQImg.cols>TQImg.rows)?250.0/TQImg.cols:250.0/TQImg.rows);
	//	Ssize.width=TQImg.cols*S;//压缩图片，减少数据量，加快运算
	//	Ssize.height=TQImg.rows*S;
	//	Mat img;
	//	resize(TQImg,img,Ssize,0,0,INTER_LINEAR);
	//	Mat imgROI = img.clone()/*(Rect(0, 0,SingleInfo[i].GetUprightRect().width*S, SingleInfo[i].GetUprightRect().height*S))*/;  //Rect方法定义ROI,注意宽是：cols；高是rows。  
	//	//imshow("imgROI",imgROI);
	//	//移动
	//	//Mat logo=imgROI;
	//	Mat mask=imgROI.clone();
	//	threshold(mask, mask, 0, 255, THRESH_BINARY);//对mask进行二值化，将mask进一步处理  
	//	//bitwise_not(mask, mask);//对mask图像取反，0变255
	//	Mat imgLocation;
	//	imgLocation=TQ(Rect(256/2-SingleInfo[i].GetUprightRect().width*S/2, 256/2-SingleInfo[i].GetUprightRect().height*S/2,SingleInfo[i].GetUprightRect().width*S, SingleInfo[i].GetUprightRect().height*S));  //Rect方法定义ROI,注意宽是：cols；高是rows。
	//	imgROI.copyTo(imgLocation,mask);
	//	imwrite(filename+"rr//"+Int_to_String(TQN)+imgformat,TQ);
	//}

	////*******************************************************************************************************
	////识别Single染色体中的着丝粒点位置
	////*******************************************************************************************************
	//cout << "开始识别……\n";
	//LocateCentromere(SingleInfo);
	////输出识别结果
	//Mat LCimage = imread(filename + imgname + imgformat);
	//int nk = 0;
	//int Num0 = 0;
	//putText(LCimage, "Suspected Single:", Point(50, 200), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, Int_to_String(SingleInfo.size()), Point(500 + 50, 200), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, "Suspected Cluster:", Point(50, 250), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, Int_to_String(LumpInfo.size()), Point(500 + 50, 250), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, "Suspected Tiny:", Point(50, 300), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, Int_to_String(TinyInfo.size()), Point(500 + 50, 300), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//for (int i = 0;i<SingleInfo.size();i++)
	//{
	//	putText(LCimage, Int_to_String(i), SingleInfo[i].GetRelativeOrigin(), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//	int CLVS = SingleInfo[i].GetCentromereLocationVector().size();

	//	if (CLVS >= 2)
	//	{
	//		nk++;
	//		//cout<<i<<"\t"<<CLVS<<"\n";
	//		putText(LCimage, "Suspected Dicentric:", Point(50, 300 + nk * 50), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//		putText(LCimage, "(" + Int_to_String(nk) + ")", Point(500 + 50, 300 + nk * 50), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//		putText(LCimage, Int_to_String(i), Point(600 + 50, 300 + nk * 50), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//		putText(LCimage, Int_to_String(CLVS), Point(700 + 50, 300 + nk * 50), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//	}
	//	if (CLVS == 0)
	//	{

	//		Num0++;
	//	}
	//	for (int j = 0;j<CLVS;j++)
	//	{
	//		circle(LCimage, SingleInfo[i].GetCentromereLocationVector()[j], 5, Scalar(0, 255, 255), -1, 8);
	//	}
	//}
	//putText(LCimage, "No find Centromere:", Point(50, 150), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);
	//putText(LCimage, Int_to_String(Num0), Point(500 + 50, 150), FONT_HERSHEY_COMPLEX, 1.0, Scalar(0, 0, 255), 1, 8, 0);

	//for (int i = 0;i<LumpInfo.size();i++)
	//{
	//	putText(LCimage, Int_to_String(i), LumpInfo[i].GetRelativeOrigin(), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255, 255, 255), 1, 8, 0);
	//}
	//for (int i = 0;i<TinyInfo.size();i++)
	//{
	//	putText(LCimage, Int_to_String(i), TinyInfo[i].GetRelativeOrigin(), FONT_HERSHEY_COMPLEX, 1.0, Scalar(200, 200, 0), 1, 8, 0);
	//	rectangle(LCimage, Point(TinyInfo[i].GetRelativeOrigin().x, TinyInfo[i].GetRelativeOrigin().y), cvPoint(TinyInfo[i].GetRelativeOrigin().x + TinyInfo[i].GetUprightRect().width, TinyInfo[i].GetRelativeOrigin().y + TinyInfo[i].GetUprightRect().height), cvScalar(200, 200, 0), 1, 4, 0); ;
	//}


	//imwrite(filename + "rr//" + imgname + "_" + "1" + imgformat, LCimage);


	////To祁亚峰
	//	 
	//Mat LCimage=imread(filename+imgname+imgformat);
	//int nk=0;
	//int Num0=0;
	//for(int i=0;i<SingleInfo.size();i++)
	//{
	// int CLVS=SingleInfo[i].GetCentromereLocationVector().size();
	// if(CLVS==2)
	// {
	//	 for(int j=0;j<CLVS;j++)
	//	 {
	//		 circle(LCimage,SingleInfo[i].GetCentromereLocationVector()[j], 5, Scalar(0, 255, 255), -1, 8);
	//	 }
	// }
	//}

	//imwrite(filename+"rr//Q//"+imgname+"_"+"1"+imgformat,LCimage);

	//*******************************************************************************************************
	////排列显示
	//*******************************************************************************************************

	//Mat SSimage= Mat::zeros(1500,2000,CV_8UC3);
	//sort(SingleInfo,SortClassClusterInfoGreater_L);
	//int dicNum=0;
	//int normalNum=0;
	//int TinyNum=0;
	//OrderedShow(SingleInfo,SSimage,dicNum,Point(10,10),100,150,2,10,0.5);
	//OrderedShow(SingleInfo,SSimage,normalNum,Point(10,160),50,150,1,1,0.5);
	//OrderedShow(LumpInfo,SSimage,Point(10,610),200,200,0.5);
	//OrderedShow(TinyInfo,SSimage,Point(10,850),50,50,1.0);
	//OrderedShow(SingleInfo,SSimage,TinyNum,Point(10,900),50,150,0,0,0.5);
	//int imgw=SSimage.size().width;
	//int imgh=SSimage.size().height;
	//putText(SSimage,"dic:",Point(imgw-300,imgh-200),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);
	//putText(SSimage,Int_to_String(dicNum),Point(imgw-100,imgh-200),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);

	//putText(SSimage,"normal:",Point(imgw-300,imgh-150),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);
	//putText(SSimage,Int_to_String(normalNum),Point(imgw-100,imgh-150),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);

	//putText(SSimage,"cluster:",Point(imgw-300,imgh-100),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);
	//putText(SSimage,Int_to_String(LumpInfo.size()),Point(imgw-100,imgh-100),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);	 //int DicNum=0;

	//putText(SSimage,"tiny:",Point(imgw-300,imgh-50),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);
	//putText(SSimage,Int_to_String(TinyInfo.size()+TinyNum),Point(imgw-100,imgh-50),FONT_HERSHEY_COMPLEX,1.0,Scalar(255, 255, 255),1,8,0);	 //int DicNum=0;



	//imwrite(filename+"rr//"+imgname+"_"+"2"+imgformat,SSimage);
	////imshow("结果",SSimage);
	//*******************************************************************************************************
	////CNN预测
	//*******************************************************************************************************
	if (SingleInfo.size() < 10)//单个染色体太少，就没必要识别了，直接跳过
	{
		cout << "单个染色体数目太少，跳过！\n";
		return 0;
	}
	//调整单个染色体图像尺寸，并存进vector
	vector<Mat> SingleVec;
	for (int i = 0;i<SingleInfo.size();i++)
	{
		SingleVec.push_back(normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
		//imwrite("C://Users//MaTengfei//Desktop//tt//" + Int_to_String(i) + ".bmp", normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
	}
	vector<int> DicFlage;
	cout << "开始识别\n";
	CNNpredict(SingleVec, DicFlage);
	cout << "识别结束\n";
	//将SingleInfo对应标志位进行标记
	for (int i = 0;i < SingleInfo.size();i++)
	{
		SingleInfo[i].SetAberrationFlage(DicFlage[i]);
		//cout << i << "\t" << SingleInfo[i].GetAberrationFlage() << "\n";
	}
	//显示畸变识别结果
	Mat abeShow = imread(imgpath).clone();
	for (int i = 0;i < SingleInfo.size();i++)
	{
		if (SingleInfo[i].GetAberrationFlage() == 1)
		{
			Rect rect = SingleInfo[i].GetUprightRect();
			rectangle(abeShow, rect, Scalar(255, 255, 0), 2);
		}
	}
	double abSscale = 0.5;
	Size abSsize, abBsize;
	abSsize.width = image.cols*abSscale;//压缩图片，减少数据量，加快运算
	abSsize.height = image.rows*abSscale;
	abBsize.width = image.cols;//用于还原图片尺寸
	abBsize.height = image.rows;
	resize(abeShow, abeShow, abSsize, 0, 0, INTER_LINEAR);

	//写入路径
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(imgpath.c_str(), drive, dir, fname, ext);
	string strfname = fname;
	string strext = ext;

	imwrite("..//..//识别结果//" + strfname + ext, abeShow);
	//imshow("abeShow", abeShow);
	//waitKey(0);

	return 0;
}