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


void Invert0_1(Mat src,Mat& result)
{
	 for(int i=0;i<src.rows;i++)
	{
		for(int j=0;j<src.cols;j++)
		{
			if(src.at<uchar>(i,j)==0) result.at<uchar>(i,j)=1;
			else  result.at<uchar>(i,j)=0;
			
		}	
	}
}