//��ʴ�����и���ϸ����ճ����С΢��
/*�����ֵͼ�������ֵͼ*/
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

void ErodeDilateSeparateMN(Mat &bimg,Mat &result)
{
	
	Mat bimg1 = bimg.clone();
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(30, 30));//�� �ṹԪ��
	erode(bimg,bimg,element1);
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(31, 31));//�� �ṹԪ��
	dilate(bimg,bimg,element2);
	/*erode(bimg,bimg,element1);
	dilate(bimg,bimg,element2);*/
	
	Mat abimg;
    absdiff(bimg1,bimg,abimg);
    vector<vector<Point>>contours_out;
    findContours(abimg, contours_out, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	int thresholdConArea=10;
	vector<int> thresholdCon;
	for(int i=0;i<contours_out.size();i++)
	{
		double ConArea = fabs( contourArea(contours_out[i], true));
		/*cout<<i<<"���������"<<ConArea<< endl; */
		if(ConArea>thresholdConArea) 
		{
			thresholdCon.push_back(i);
			/*maxConArea=ConArea;*/
		}
	}
	result=Mat::zeros(bimg.size(),CV_8U);
	for(int i=0;i<thresholdCon.size();i++)
	{
		drawContours(result, contours_out, thresholdCon[i], Scalar(255,255,255), -1);//�ڰ�����
	}


}