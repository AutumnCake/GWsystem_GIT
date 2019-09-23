#include "stdafx.h"
#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

Mat normalizeImgSizeCNN(Mat image,int IMG_SIZE)
{
	int SIZE200 = 200;
	Mat Squareimg;//���������
	Mat Identicalimg;//��һ��ͼ
	if (image.empty())
	{
		return Mat::zeros(SIZE200, SIZE200, CV_8UC3);
	}
	//��ɷ���ͼ
	int l = image.cols;
	int w = image.rows;
	if (l == w)
	{
		Squareimg = image;
		//continue;
	}
	else if (l > w)
	{
		Mat out = Mat::zeros(l, l, CV_8UC3);
		int dw = (l - w) / 2;
		for (int i = 0; i < l; i++)
		{
			for (int j = 0; j < w; j++)
			{
				out.at<Vec3b>(j + dw, i) = image.at<Vec3b>(j, i);
			}
		}
		Squareimg = out;
		//imshow("��ͼ", out);
		//waitKey('0');
		//wait();
	}
	else
	{
		Mat out = Mat::zeros(w, w, CV_8UC3);
		int dl = (w - l) / 2;
		for (int i = 0; i < l; i++)
		{
			for (int j = 0; j < w; j++)
			{
				out.at<Vec3b>(j, i + dl) = image.at<Vec3b>(j, i);
			}
		}
		l = w;
		Squareimg = out;
		//imshow("��ͼ", out);
		//waitKey('0');
		//wait();
	}
	//ͼ��ߴ��һ��
	if (l >= SIZE200)   //��С��ͼ
	{
		resize(Squareimg, Identicalimg, Size(SIZE200, SIZE200));
	}
	else  //��հ�
	{
		Identicalimg = Mat::zeros(SIZE200, SIZE200, CV_8UC3);
		int dl = (SIZE200 - l) / 2;
		for (int i = 0; i < l; i++)
		{
			for (int j = 0; j < l; j++)
			{
				Identicalimg.at<Vec3b>(j + dl, i + dl) = Squareimg.at<Vec3b>(j, i);
			}
		}
	}
	resize(Identicalimg, Identicalimg, Size(IMG_SIZE, IMG_SIZE));
	return Identicalimg;
}