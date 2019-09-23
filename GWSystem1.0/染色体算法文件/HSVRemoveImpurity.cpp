
#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>


using namespace std;
using namespace cv;

#define PI 3.1415926


#include"ClassClusterInfo.h"
#include"NonezeroMeanRGBVector.h"
#include"BoolSort.h"

void HSVRemoveImpurity(vector<ClassClusterInfo>& SingleInfo,vector<ClassClusterInfo>& LumpInfo,vector<ClassClusterInfo>& TinyInfo)
{
	vector<ClassClusterInfo> SI,LI,TI;
	vector<int> Bv,Gv,Rv;
	vector<int> BDv,GDv,RDv;
	vector<vector<int>> BGRVV,BGRDVV;
	if(SingleInfo.size()==0) return;
	for(int SIN=0;SIN<SingleInfo.size();SIN++)
	{
		Mat image=SingleInfo[SIN].GetClusterImage().clone();//37 38 40 43 44 45
		////转化为HSV
		Mat HSV;
		cvtColor(image,image,CV_RGB2HSV);
		//
		vector<int> BGRVector;
		BGRVector=NonezeroMeanRGBVector(image);
		BGRVV.push_back(BGRVector);
		Bv.push_back(BGRVector[0]);
		Gv.push_back(BGRVector[1]);
		Rv.push_back(BGRVector[2]);
		vector<int> BGRDVector;
		BGRDVector=NonezeroDeviationRGBVector(image,BGRVector);
		BGRDVV.push_back(BGRDVector);
		BDv.push_back(BGRDVector[0]);
		GDv.push_back(BGRDVector[1]);
		RDv.push_back(BGRDVector[2]);
	}
	int Bm,Gm,Rm;
	int BDm,GDm,RDm;
	sort(Bv.begin(),Bv.end());Bm=Bv[Bv.size()/2];
	sort(Gv.begin(),Gv.end());Gm=Gv[Gv.size()/2];
	sort(Rv.begin(),Rv.end());Rm=Rv[Rv.size()/2];
	sort(BDv.begin(),BDv.end());BDm=BDv[BDv.size()/2];
	sort(GDv.begin(),GDv.end());GDm=GDv[GDv.size()/2];
	sort(RDv.begin(),RDv.end());RDm=RDv[RDv.size()/2];
	//筛选SingleInfo	
	for(int SIN=0;SIN<SingleInfo.size();SIN++)
	{
		int B=BGRVV[SIN][0]-Bm;int G=BGRVV[SIN][1]-Gm;int R=BGRVV[SIN][2]-Rm;
		//暂时定sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0))>150 ||abs(abs(R)-G)>55为杂质
		double HSVdistance=sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0));
		double SVabs=abs(abs(R)-G);
		int BD=BGRDVV[SIN][0]-BDm;int GD=BGRDVV[SIN][1]-GDm;int RD=BGRDVV[SIN][2]-RDm;
		double HSVDdistance=sqrtf(powf(BD,2.0)+powf(GD,2.0)+powf(RD,2.0));
		//cout<<SIN<<"\t\t("<<B<<",\t"<<G<<",\t"<<R<<")\t"<< HSVdistance<<"\t\t"<<SVabs<<"\t\t"<<HSVDdistance<<"\n";
		if(HSVdistance>=150||SVabs>=60||HSVDdistance>3000) continue;
		else SI.push_back(SingleInfo[SIN]);
		
	}
	SingleInfo.clear();
	SingleInfo.insert(SingleInfo.begin(),SI.begin(),SI.end());
	//筛选LumpInfo
	if(LumpInfo.size()>0) 
	{
		for(int LIN=0;LIN<LumpInfo.size();LIN++)
		{
			Mat image=LumpInfo[LIN].GetClusterImage().clone();//37 38 40 43 44 45
			////转化为HSV
			Mat HSV;
			cvtColor(image,image,CV_RGB2HSV);
			//
			vector<int> BGRVector;
			BGRVector=NonezeroMeanRGBVector(image);
			vector<int> BGRDVector;
			BGRDVector=NonezeroDeviationRGBVector(image);

			int B=BGRVector[0]-Bm;int G=BGRVector[1]-Gm;int R=BGRVector[2]-Rm;
			//cout<<LIN<<"\tMean\t("<<B<<",\t"<<G<<",\t"<<R<<")\t"<<sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0))<<"\t\t"<<abs(abs(R)-G)<<"\n";
			//暂时定sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0))>150 ||abs(abs(R)-G)>55为杂质
			double HSVdistance=sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0));
			double SVabs=abs(abs(R)-G);
			int BD=BGRDVector[0]-BDm;int GD=BGRDVector[1]-GDm;int RD=BGRDVector[2]-RDm;
		    double HSVDdistance=sqrtf(powf(BD,2.0)+powf(GD,2.0)+powf(RD,2.0));

			if(HSVdistance>=80||SVabs>=45||HSVDdistance>2500) continue;
			else LI.push_back(LumpInfo[LIN]);
		}
		LumpInfo.clear();
		LumpInfo.insert(LumpInfo.begin(),LI.begin(),LI.end());
	}
	//cout<<"大傻逼\n";
	//筛选TinyInfo
	//cout<<TinyInfo.size()<<"\n";
	if(TinyInfo.size()>0)
	{
		for(int TIN=0;TIN<TinyInfo.size();TIN++)
		{
			Mat image=TinyInfo[TIN].GetClusterImage().clone();//37 38 40 43 44 45
			////转化为HSV
			Mat HSV;
			cvtColor(image,image,CV_RGB2HSV);
			//
			vector<int> BGRVector;
			BGRVector=NonezeroMeanRGBVector(image);
			vector<int> BGRDVector;
			BGRDVector=NonezeroDeviationRGBVector(image);

			int B=BGRVector[0]-Bm;int G=BGRVector[1]-Gm;int R=BGRVector[2]-Rm;
			//cout<<TIN<<"\tMean\t("<<B<<",\t"<<G<<",\t"<<R<<")\t"<<sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0))<<"\t\t"<<abs(abs(R)-G)<<"\n";
			//暂时定sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0))>150 ||abs(abs(R)-G)>55为杂质
			double HSVdistance=sqrtf(powf(B*25,2.0)+powf(G,2.0)+powf(R,2.0));
			double SVabs=abs(abs(R)-G);
			int BD=BGRDVector[0]-BDm;int GD=BGRDVector[1]-GDm;int RD=BGRDVector[2]-RDm;
		    double HSVDdistance=sqrtf(powf(BD,2.0)+powf(GD,2.0)+powf(RD,2.0));
			if(HSVdistance>=170||SVabs>=60||HSVDdistance>2500) continue;
			else TI.push_back(TinyInfo[TIN]);
		}
		TinyInfo.clear();
		TinyInfo.insert(TinyInfo.begin(),TI.begin(),TI.end());
	}
	//cout<<TinyInfo.size()<<"\n";

}