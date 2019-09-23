#include "stdafx.h"
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

#include"ClassClusterInfo.h"
#include"MyFunction.h"
#include"BoolSort.h"

void Count(vector<ClassClusterInfo> CCIv,int & DicNum,int minCentromereNum,int maxCentromereNum)
{
	
	DicNum=0;
	for(int i=0;i<CCIv.size();i++)
	{
		 int CLVS=CCIv[i].GetCentromereLocationVector().size();
		 if(CLVS>=minCentromereNum&&CLVS<=maxCentromereNum)
		 {
			DicNum++;
			//cout<<DicNum<<"\n";
		}
	}
}