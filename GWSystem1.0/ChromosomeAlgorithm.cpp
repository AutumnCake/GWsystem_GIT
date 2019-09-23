#include "stdafx.h"
#include "ChromosomeAlgorithm.h"
#include"ReadAndWriteForAccess.h"
//Ⱦɫ��ʶ��
#include"Ⱦɫ���㷨�ļ�\Int_to_String.h"
#include"Ⱦɫ���㷨�ļ�\BoolSort.h"
#include"Ⱦɫ���㷨�ļ�\MyFunction.h"
#include"Ⱦɫ���㷨�ļ�\ClassClusterInfo.h"
#include"Ⱦɫ���㷨�ļ�\InterestRegion.h"
#include"Ⱦɫ���㷨�ļ�\CutFirst.h"
#include"Ⱦɫ���㷨�ļ�\DrawClassChromInfoVector.h"
#include"Ⱦɫ���㷨�ļ�\ClassifyChromo.h"
#include"Ⱦɫ���㷨�ļ�\CutSecond.h"
#include"Ⱦɫ���㷨�ļ�\res.h"
//ʶ����˿����λ��
#include"��˿����ʶ��\LocateCentromere.h"
//ȥ����
#include"Ⱦɫ���㷨�ļ�\HSVRemoveImpurity.h"
//������
#include"Ⱦɫ���㷨�ļ�\DrawClassChromInfoVector.h"
//����
#include"Ⱦɫ���㷨�ļ�\Count.h"

//CNN
#include"Ⱦɫ���㷨�ļ�\normalizeImgSizeCNN.h"
#include"Ⱦɫ���㷨�ļ�\CNNpredict.h"

#include <io.h>
#include "direct.h"
#define PI 3.1415926

////CNNʶ��ͼ��ߴ�
int CNN_IMG_SIZE = 40;

vector<long>time1;
CReadAndWriteForAccess ReadCHRO;

CChromosomeAlgorithm::CChromosomeAlgorithm()
{
}


CChromosomeAlgorithm::~CChromosomeAlgorithm()
{
	
}


// //Ⱦɫ�����������
CHRO_HandleResult* CChromosomeAlgorithm::DicMain(string road1name1, string file1name1, string writename, CString patientname, CHRO_HandleResult* pB)
{
	//������Ƭ�ķ��������ʼ��
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
	//��ȡͼ��Ϊ��ʱ��ֱ�������������أ�
	if (image.empty())
	{
		return pB;
	}

	//�ָ������Ȥ����
	//*******************************************************************************************************

	double Sscale = 0.1;//���ű���
	vector<ClassClusterInfo> InterestCluster;
	InterestRegion(image, Sscale, InterestCluster);
	//sort(InterestCluster,SortClassClusterInfoLess_S);
	//for(int i=0;i<InterestCluster.size();i++)
	//{
	//	imwrite(filename+"rr//"+imgname+"."+Int_to_String(i)+imgformat,InterestCluster[i].GetClusterImage());
	//	//cout<<"����Ȥ��"<<Location[i].GetLabel()<<"\t"<<i<<"\n";
	//}

	//*******************************************************************************************************
	//��ÿ��������г����и�FirstCut
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
	////��ȡȾɫ����Ϣ,��ȥ����������
	//vector<ClassChromInfo> CutFirstClusterInfo=GetClassChromInfo(CutFirstCluster,CutFirstLocation);
	//cout<<CutFirstClusterInfo.size()<<"\t"<<CutFirstCluster.size()<<"\n";
	////for(int i=0;i<CutFirstClusterInfo.size();i++)
	////{
	////	cout<<CutFirstClusterInfo[i].GetWidth()<<"\t"<<CutFirstClusterInfo[i].GetAreaRatio()<<"\t"<<CutFirstClusterInfo[i].GetLabel()<<"\n";
	////}

	//*******************************************************************************************************
	//���࣬�ֳ����ƶ�Ƭ������˿��Ⱦɫ����ճ��Ⱦɫ�弰�ϴ����ʣ�����Ⱦɫ��,��ȥ����������
	//*******************************************************************************************************
	vector<ClassClusterInfo> LumpInfo, SingleInfo, TinyInfo;
	ClassifyChromo(InterestCluster, LumpInfo, SingleInfo, TinyInfo);
	//////���������������ͼ
	//Mat SFCS=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(SingleInfo,SFCS);
	//imwrite(filename+"rr//"+imgname+"."+"SFCS"+imgformat,SFCS);
	//Mat SFCL=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(LumpInfo,SFCL);
	//imwrite(filename+"rr//"+imgname+"."+"SFCL"+imgformat,SFCL);
	//Mat SFCT=Mat::zeros(image.size(),CV_8UC3);
	//DrawClassChromInfoVector(TinyInfo,SFCT);
	//imwrite(filename+"rr//"+imgname+"."+"SFCT"+imgformat,SFCT);

	////��һ����ȷȾɫ���ȷ�Χ
	//sort(SingleInfo,SortClassClusterInfoLess_S);
	//for(int i=0;i<SingleInfo.size();i++)
	//	cout<<i<<"\t"<<SingleInfo[i].GetminRectShort()<<"\n";
	//��ɸѡ�����Ľϴ�Ⱦɫ����ж��ηָ�,�˴��ı�CutFirstCluster��LumpInfo, SingleInfo, TinyInfo�ȣ���������һ��CutSecondCluster�ȣ�Ϊ�˽�ʡ�ڴ棿
	//ע��vector.clear�Խ�ʡ�ڴ�
	//cout<<LumpInfo.size()<<"\n";

	//*******************************************************************************************************
	//��Ե�һ�ηָ��ɸѡ����Ⱦɫ����LumpInfo���ж��ηָ�---------------------------------------------------------------------------(Ŀǰ��CutSecond��Mywatershed����bug����ʱ������һ��������debug���߸��㷨)
	//*******************************************************************************************************
	//���Lump��debug
	//for (int i = 0;i<LumpInfo.size();i++)
	//{
	//	imwrite("C://Users//MaTengfei//Desktop//tt//lump//" + Int_to_String(i) + imgformat, LumpInfo[i].GetClusterImage());
	//}

	//cout << "��ʼ���ηָ��\n";
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
	/*cout << "�ָ���ɡ���\n";*/

	//////���������������ͼ
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
	//����HSV������ɸѡ����
	//*******************************************************************************************************

	HSVRemoveImpurity(SingleInfo, LumpInfo, TinyInfo); // LumpInfo :Ⱦɫ���ŵ������Ϣ
	////���Single���ڵ����о�
	//for(int i=0;i<SingleInfo.size();i++)
	//{
	//	imwrite(filename+"rr//ri//"+imgname+"."+Int_to_String(i)+imgformat,SingleInfo[i].GetClusterImage());
	////	cout<<CutFirstLocation[i].GetLabel()<<"\t"<<i<<"\n";
	//}
	//��������ͼ

	//for(int i=0;i<SingleInfo.size();i++)
	//{
	//	TQN++;
	//	Mat TQ=Mat::zeros(256,256,CV_8UC3);
	//	Size Ssize;
	//	Mat TQImg=SingleInfo[i].GetClusterImage();
	//	double S=((TQImg.cols>TQImg.rows)?250.0/TQImg.cols:250.0/TQImg.rows);
	//	Ssize.width=TQImg.cols*S;//ѹ��ͼƬ���������������ӿ�����
	//	Ssize.height=TQImg.rows*S;
	//	Mat img;
	//	resize(TQImg,img,Ssize,0,0,INTER_LINEAR);
	//	Mat imgROI = img.clone()/*(Rect(0, 0,SingleInfo[i].GetUprightRect().width*S, SingleInfo[i].GetUprightRect().height*S))*/;  //Rect��������ROI,ע����ǣ�cols������rows��  
	//	//imshow("imgROI",imgROI);
	//	//�ƶ�
	//	//Mat logo=imgROI;
	//	Mat mask=imgROI.clone();
	//	threshold(mask, mask, 0, 255, THRESH_BINARY);//��mask���ж�ֵ������mask��һ������  
	//	//bitwise_not(mask, mask);//��maskͼ��ȡ����0��255
	//	Mat imgLocation;
	//	imgLocation=TQ(Rect(256/2-SingleInfo[i].GetUprightRect().width*S/2, 256/2-SingleInfo[i].GetUprightRect().height*S/2,SingleInfo[i].GetUprightRect().width*S, SingleInfo[i].GetUprightRect().height*S));  //Rect��������ROI,ע����ǣ�cols������rows��
	//	imgROI.copyTo(imgLocation,mask);
	//	imwrite(filename+"rr//"+Int_to_String(TQN)+imgformat,TQ);
	//}

	////*******************************************************************************************************
	////ʶ��SingleȾɫ���е���˿����λ��
	////*******************************************************************************************************
	//cout << "��ʼʶ�𡭡�\n";
	//LocateCentromere(SingleInfo);
	////���ʶ����
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


	////To���Ƿ�
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
	////������ʾ
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
	////imshow("���",SSimage);
	//*******************************************************************************************************
	////CNNԤ��
	//*******************************************************************************************************
	if (SingleInfo.size() < 10)//����Ⱦɫ��̫�٣���û��Ҫʶ���ˣ�ֱ������
	{
		/*cout << "����Ⱦɫ����Ŀ̫�٣�������\n";*/
		return pB;
	}
	OnePicResult.chromosome_num = SingleInfo.size() + LumpInfo.size() * 2; //���ƺ��Ⱦɫ������
	//��������Ⱦɫ��ͼ��ߴ磬�����vector
	vector<Mat> SingleVec;
	for (int i = 0; i<SingleInfo.size(); i++)
	{
		SingleVec.push_back(normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
		//imwrite("C://Users//MaTengfei//Desktop//tt//" + Int_to_String(i) + ".bmp", normalizeImgSizeCNN(SingleInfo[i].GetClusterImage(), CNN_IMG_SIZE));
	}
	vector<int> DicFlage;
	//cout << "��ʼʶ��\n";
	long start = clock();  //��ʼʱ��
	CNNpredict(SingleVec, DicFlage);
	long finish = clock();
	long t = finish - start;
	time1.push_back(t);
	//cout << "ʶ�����\n";
	//��SingleInfo��Ӧ��־λ���б��
	for (int i = 0; i < SingleInfo.size(); i++)
	{
		SingleInfo[i].SetAberrationFlage(DicFlage[i]);
		//cout << i << "\t" << SingleInfo[i].GetAberrationFlage() << "\n";
	}

	int dicNum=0;
	//��ʾ��˫�ŵ�ͼƬ
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
		//������ϸ����+1
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

		//��ϸ����
		OnePicResult.allcell = OnePicResult.normalcell + OnePicResult.abnormal;
		OnePicResult.picpath = (writename + file1name1).c_str();

		//��������Ƭ�ķ�������浽���ݿ�
		ReadCHRO.SaveOnePicResult(OnePicResult);
	}
	else{
		//����ϸ����+1
		OnePicResult.normalcell = 1;
	}
	//��ʾ����ʶ����

	//imshow("abeShow", abeShow);
	//waitKey(0);


	pB->dic += dicNum;
	pB->abnormal += OnePicResult.abnormal;
	pB->normalcell += OnePicResult.normalcell;
	pB->allcell = pB->abnormal + pB->normalcell;
	pB->Y = (double)pB->abnormal / (double)pB->allcell;
	return pB;
}


