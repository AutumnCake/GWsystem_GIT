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
	 
	//生成一维采样点,包括所有图像像素点,注意采样点格式为32bit浮点数。   
    Mat samples(img.cols*img.rows, 1, CV_32FC3);   
    //标记矩阵，32位整形   
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
    //我们已知有3个聚类，用不同的灰度层表示。   
    Mat result( img.size(), CV_8UC1);   
    //float step=255/(clusterCount - 1);   
    k=0;   
    for(int i=0; i < result.rows; i++)   
    {   
        //uchar* p = result.ptr<uchar>(i);   
        for(int j=0; j< result.cols; j++)   
        {   
           result.at<uchar>(i,j) = labels.at<int>(k, 0);  //注意转换为Mat时，与前面Mat转换为一维矩阵相对应，否则会位置出错。    
			k++;
        }   
    }  
	return result;
  

}