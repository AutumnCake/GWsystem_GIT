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

#include"Point2Vector.h"

void AddBlankFrame(Mat& srcimage,int N)
{
	if(srcimage.channels()==3)
	{
		Mat Temp=Mat::zeros(srcimage.size()+Size(2*N,2*N),CV_8UC3);
		Mat gray;
		cvtColor( srcimage, gray, CV_BGR2GRAY );
		Mat mask=gray.clone();
		Mat Location = Temp(Rect(N-1,N-1,srcimage.size().width,srcimage.size().height));
		srcimage.copyTo(Location,mask);
		srcimage=Temp.clone();
	}
	else
	{
		Mat Temp=Mat::zeros(srcimage.size()+Size(2*N,2*N),CV_8UC1);
		//Mat gray;
		//cvtColor( srcimage, gray, CV_BGR2GRAY );
		Mat mask=srcimage.clone();
		Mat Location = Temp(Rect(N-1,N-1,srcimage.size().width,srcimage.size().height));
		srcimage.copyTo(Location,mask);
		srcimage=Temp.clone();
	}

}
void SubBlankFrame(Mat& srcimage,int N)
{
	Mat Temp=srcimage(Rect(N-1,N-1,srcimage.size().width-2*N,srcimage.size().height-2*N));
	srcimage=Temp.clone();

}