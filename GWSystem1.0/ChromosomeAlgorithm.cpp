#include "stdafx.h"
#include "ChromosomeAlgorithm.h"
#include"ReadAndWriteForAccess.h"
//染色体识别
#include"染色体算法文件\Int_to_String.h"
#include"染色体算法文件\BoolSort.h"
#include"染色体算法文件\MyFunction.h"
#include"染色体算法文件\ClassClusterInfo.h"
#include"染色体算法文件\InterestRegion.h"
#include"染色体算法文件\CutFirst.h"
#include"染色体算法文件\DrawClassChromInfoVector.h"
#include"染色体算法文件\ClassifyChromo.h"
#include"染色体算法文件\CutSecond.h"
#include"染色体算法文件\res.h"
//识别着丝粒点位置
#include"着丝粒点识别\LocateCentromere.h"
//去杂质
#include"染色体算法文件\HSVRemoveImpurity.h"
//检验用
#include"染色体算法文件\DrawClassChromInfoVector.h"
//排序
#include"染色体算法文件\Count.h"

//CNN
#include"染色体算法文件\normalizeImgSizeCNN.h"
#include"染色体算法文件\CNNpredict.h"

#include <io.h>
#include "direct.h"
#define PI 3.1415926

////CNN识别图像尺寸
int CNN_IMG_SIZE = 40;

vector<long>time1;
CReadAndWriteForAccess ReadCHRO;

CChromosomeAlgorithm::CChromosomeAlgorithm()
{
}


CChromosomeAlgorithm::~CChromosomeAlgorithm()
{
	
}


// //染色体分析主函数
CHRO_HandleResult* CChromosomeAlgorithm::DicMain(string road1name1, string file1name1, string writename, CString patientname, CHRO_HandleResult* pB)
{
	//将该照片的分析结果初始化
	OnePicResult.dic = 0;
	OnePicResult.ace = 0;//
	OnePicResult.allcell = 0;//
	OnePicResult.inv = 0;//
	OnePicResult.round = 0;//
	OnePicResult.t = 0;//
	OnePicResult.picpath = (writename + file1name1).c_str();//
	OnePicResult.patientname = patientname;

	OnePicResult.tri = 0;//
	OnePicResult.ten = 0;//
	OnePicResult.del = 0;
	OnePicResult.ctg = 0;
	OnePicResult.csg = 0;
	OnePicResult.ctb = 0;
	OnePicResult.cte = 0;

	OnePicResult.abnormal = 0;
	OnePicResult.normalcell = 0;
	OnePicResult.CheckPath = "0";

	string picpath = road1name1 +"\\"+ file1name1;

	Mat image = imread(picpath);
	//读取图像为空时，直接跳过，即返回；
	if (image.empty())
	{
		return pB;
	}

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

	//cout << "开始二次分割……\n";
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
	/*cout << "分割完成……\n";*/

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

	HSVRemoveImpurity(SingleInfo, LumpInfo, TinyInfo); // LumpInfo :染色体团的相关信息
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
		/*cout << "单个染色体数目太少，跳过！\n";*/
		return pB;
	}
	OnePicResult.chromosome_num = SingleInfo.size() + LumpInfo.size() * 2; //近似后的染色体条数
	//调整单个染色体图像尺寸，并存进vector
	vector<Mat> SingleVec;
	for (int i = 0; i<SingleInfo.size(); i++)
	{
		SingleVec.push_back(normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
		//imwrite("C://Users//MaTengfei//Desktop//tt//" + Int_to_String(i) + ".bmp", normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
	}
	vector<int> DicFlage;
	//cout << "开始识别\n";
	long start = clock();  //开始时间
	CNNpredict(SingleVec, DicFlage);
	long finish = clock();
	long t = finish - start;
	time1.push_back(t);
	//cout << "识别结束\n";
	//将SingleInfo对应标志位进行标记
	for (int i = 0; i < SingleInfo.size(); i++)
	{
		SingleInfo[i].SetAberrationFlage(DicFlage[i]);
		//cout << i << "\t" << SingleInfo[i].GetAberrationFlage() << "\n";
	}

	int dicNum=0;
	//显示含双着的图片
	for (int i = 0; i < SingleInfo.size(); i++)
	{
		if (SingleInfo[i].GetAberrationFlage() == 1)
		{
			dicNum++;
		}	
	}

	OnePicResult.dic = dicNum;
	if (dicNum > 0 && abs(OnePicResult.chromosome_num - 46) < 5)
	{
		//不正常细胞数+1
		OnePicResult.abnormal = 1;

		/*Mat abeShow = imread(picpath).clone();
		for (int i = 0; i < SingleInfo.size(); i++)
		{
			if (SingleInfo[i].GetAberrationFlage() == 1)
			{
				Rect rect = SingleInfo[i].GetUprightRect();
				rectangle(abeShow, rect, Scalar(255, 255, 0), 2);
			}
		}


		imwrite(writename + file1name1, abeShow);
*/


		//char DesTempFilePath[MAX_PATH + 1];
		//char sucTempFilePath[MAX_PATH + 1];
		//WCHAR   wstr[MAX_PATH];
		//WCHAR   wstr_des[MAX_PATH];
		//sprintf_s(sucTempFilePath, "%s", (road1name1 + "\\" + file1name1).c_str());
		//sprintf_s(DesTempFilePath, "%s", (writename + file1name1).c_str());

		//MultiByteToWideChar(CP_ACP, 0, sucTempFilePath, -1, wstr, sizeof(wstr));
		//MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
		//int abc = CopyFile(wstr, wstr_des, TRUE);

		//总细胞数
		OnePicResult.allcell = OnePicResult.normalcell + OnePicResult.abnormal;
		OnePicResult.picpath = (writename + file1name1).c_str();

		//将该张照片的分析结果存到数据库
		ReadCHRO.SaveOnePicResult(OnePicResult);
	}
	else{
		//正常细胞数+1
		OnePicResult.normalcell = 1;
	}
	//显示畸变识别结果

	//imshow("abeShow", abeShow);
	//waitKey(0);


	pB->dic += dicNum;
	pB->abnormal += OnePicResult.abnormal;
	pB->normalcell += OnePicResult.normalcell;
	pB->allcell = pB->abnormal + pB->normalcell;
	pB->Y = (double)pB->abnormal / (double)pB->allcell;
	return pB;
}


