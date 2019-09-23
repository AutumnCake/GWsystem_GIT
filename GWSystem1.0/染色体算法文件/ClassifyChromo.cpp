#include "stdafx.h"
#include"iostream"
#include "queue"  
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
#include"res.h"
#include"ClassClusterInfo.h"

using namespace std;
using namespace cv;

void  ClassifyChromo(vector<ClassClusterInfo> &ChromInfo, vector<ClassClusterInfo>& LumpInfo, vector<ClassClusterInfo>& SingleInfo, vector<ClassClusterInfo>& TinyInfo)
{
	for(int CI=0;CI<ChromInfo.size();CI++)
	{
		int sT=ChromInfo[CI].GetminRectShort();
		if(sT<ExternWidthChromatidMin) continue;
		if(sT<ExternChromosomeWidthMin) 
		{
			Mat imgT=ChromInfo[CI].GetClusterImage();
			Mat gray;
			cvtColor( imgT, gray, CV_BGR2GRAY );
			Mat Bi;
			threshold(gray,Bi,0,255,THRESH_BINARY);
			//内孔太多的对象视为杂质
			Mat BiTemp=Bi.clone();
			vector<vector<Point> > cT;  
			vector<Vec4i> hierarchyTemp;  
			findContours(BiTemp, cT, hierarchyTemp,CV_RETR_LIST, CHAIN_APPROX_NONE); 
			if(ChromInfo[CI].GetAreaRatio()<0.5||cT.size()>3) continue;
		}
			
			//减少较大杂质
		if(sT>ExternChromosomeWidthMax&&ChromInfo[CI].GetAreaRatio()<0.2) continue;
			//减少细长杂质
		if(sT<=ExternChromosomeWidthMax&&sT>=ExternChromosomeWidthMin&&ChromInfo[CI].GetAreaRatio()<0.1) continue;
		if(ChromInfo[CI].GetminRectShort()<ExternChromosomeWidthMin) TinyInfo.push_back(ChromInfo[CI]);
		else if(ChromInfo[CI].GetminRectShort()>=ExternChromosomeWidthMin && ChromInfo[CI].GetminRectShort()<=ExternChromosomeWidthMax) SingleInfo.push_back(ChromInfo[CI]);
		else LumpInfo.push_back(ChromInfo[CI]);
	}
	ChromInfo.clear();
}