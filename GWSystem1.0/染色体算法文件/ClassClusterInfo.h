#pragma once

#include"iostream"
#include"iterator"
#include"algorithm"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
using namespace std;
using namespace cv;


class ClassClusterInfo
{
public:
	//���캯��
	ClassClusterInfo();
	//�趨����
	void SetClassClusterInfo(Mat img);
	void SetClassClusterInfo(Mat img,Rect UrRect);
	void SetClassClusterInfo(Mat img,Rect UrRect,Point RelativeOrigin);
	//�趨����ȡCentromereLocationVector
	void SetCentromereLocationVector(vector<Point> CLV);
	void SetCentromereLocationVector(Point CL);
	vector<Point> GetCentromereLocationVector();
	//��ȡMat
	Mat GetClusterImage();
	//��ȡλ����Ϣ
	Point GetRelativeOrigin();
	Rect GetUprightRect();
	//��ȡ��С��Ӿ�����Ϣ
	void SetminRect_ClusterArea();
	RotatedRect GetminRect();
	double GetClusterArea();
	//��ȡ��С���γ��̱߲���
	double GetminRectShort();
	double GetminRectLong();
	//��ȡ�����
	double GetAreaRatio();
	//������С�����ĸ�������ΪP0�����ϣ�P1�����£�P2�����£�P3
	vector<Point2f> GetminRectPoints();//x=0,1,2,3
	//����С���γ��ߺͶ̱��ϵ����ĵ� x=0,1.
	vector<Point2f> GetminRectCenterPointxOnShortside();
	vector<Point2f> GetminRectCenterPointxOnLongside();
	Point GetminRectCenterPoint();
	//��minRectCenterPointΪ����,�������ֱ����ƫת�Ƕ�(Pi)
	double GetAngleBaseUpright();

	//��ʱ�趨�����־��1Ϊ���䣬0Ϊ����
	void SetAberrationFlage(int AbF);
	int GetAberrationFlage();


	//��������
	~ClassClusterInfo();


private:
	//image
	Mat image;
	//Location λ����Ϣ
	Rect UprightRect;//��Ӿ���
	//Ĭ��Ϊ0����Ҫ����
	RotatedRect minRect;//��С��Ӿ���
	double ClusterArea;//Ⱦɫ�������
	//��˿�����λ��
	vector<Point> CentromereLocationVector; //�Ƿ���Ҫ��copy����������
	//�����־��1Ϊ���䣬0Ϊ������Ĭ��Ϊ����
	int AberrationFlage=0;
	
};