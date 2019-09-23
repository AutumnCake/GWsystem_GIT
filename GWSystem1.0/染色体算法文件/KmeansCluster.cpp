#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv;

Mat  KmeansCluster(Mat img,int clusterCount)
{
	 
	//����һά������,��������ͼ�����ص�,ע��������ʽΪ32bit��������   
    Mat samples(img.cols*img.rows, 1, CV_32FC3);   
    //��Ǿ���32λ����   
    Mat labels(img.cols*img.rows, 1, CV_32SC1);    
    int k=0;   
    for(int i=0; i < img.rows; i++)   
    {   
       uchar*  p = img.ptr<uchar>(i);   
        for(int j=0; j< img.cols; j++)   
        {   
            samples.at<Vec3f>(k,0)[0] = float(p[j*3]);   
            samples.at<Vec3f>(k,0)[1] = float(p[j*3+1]);   
            samples.at<Vec3f>(k,0)[2] = float(p[j*3+2]);   
            k++;   
        }   
    }  
   //cout<<"e\n";
    Mat centers(clusterCount, 1, samples.type());   
    kmeans(samples, clusterCount, labels,   
        TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10,1.0),   
        3, KMEANS_PP_CENTERS, centers);  
    //������֪��3�����࣬�ò�ͬ�ĻҶȲ��ʾ��   
    Mat result( img.size(), CV_8UC1);   
    //float step=255/(clusterCount - 1);   
    k=0;   
    for(int i=0; i < result.rows; i++)   
    {   
        //uchar* p = result.ptr<uchar>(i);   
        for(int j=0; j< result.cols; j++)   
        {   
           result.at<uchar>(i,j) = labels.at<int>(k, 0);  //ע��ת��ΪMatʱ����ǰ��Matת��Ϊһά�������Ӧ�������λ�ó���    
			k++;
        }   
    }  
	return result;
  

}