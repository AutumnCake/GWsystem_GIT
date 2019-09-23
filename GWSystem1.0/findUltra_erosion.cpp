#include "StdAfx.h"
#include"iostream"
#include "queue"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Mat findUltra_erosion(Mat previous,Mat current)
{
	threshold(previous,previous,1,255,THRESH_BINARY);
	threshold(current,current,1,255,THRESH_BINARY);
	Mat result=previous.clone();
	Mat c=current.clone();
	for(int i=0;i<c.rows-1;i++)
	{
		for(int j=0;j<c.cols-1;j++)
		{
			if(c.at<uchar>(i,j)==255)
			{
				Rect ccomp;  
				floodFill(c,Point(j,i), Scalar(0), &ccomp,Scalar(5),Scalar(5),8);
				floodFill(result,Point(j,i), Scalar(0), &ccomp,Scalar(5),Scalar(5),8);
			}

		}

	}
	return result;

}