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
	//构造函数
	ClassClusterInfo();
	//设定函数
	void SetClassClusterInfo(Mat img);
	void SetClassClusterInfo(Mat img,Rect UrRect);
	void SetClassClusterInfo(Mat img,Rect UrRect,Point RelativeOrigin);
	//设定并获取CentromereLocationVector
	void SetCentromereLocationVector(vector<Point> CLV);
	void SetCentromereLocationVector(Point CL);
	vector<Point> GetCentromereLocationVector();
	//获取Mat
	Mat GetClusterImage();
	//获取位置信息
	Point GetRelativeOrigin();
	Rect GetUprightRect();
	//获取最小外接矩形信息
	void SetminRect_ClusterArea();
	RotatedRect GetminRect();
	double GetClusterArea();
	//获取最小矩形长短边参数
	double GetminRectShort();
	double GetminRectLong();
	//获取面积比
	double GetAreaRatio();
	//分配最小矩形四个点左上为P0，右上：P1，左下：P2，右下：P3
	vector<Point2f> GetminRectPoints();//x=0,1,2,3
	//在最小矩形长边和短边上的中心点 x=0,1.
	vector<Point2f> GetminRectCenterPointxOnShortside();
	vector<Point2f> GetminRectCenterPointxOnLongside();
	Point GetminRectCenterPoint();
	//以minRectCenterPoint为中心,相对于竖直方向偏转角度(Pi)
	double GetAngleBaseUpright();

	//暂时设定畸变标志，1为畸变，0为正常
	void SetAberrationFlage(int AbF);
	int GetAberrationFlage();


	//析构函数
	~ClassClusterInfo();


private:
	//image
	Mat image;
	//Location 位置信息
	Rect UprightRect;//外接矩形
	//默认为0，需要设置
	RotatedRect minRect;//最小外接矩形
	double ClusterArea;//染色体团面积
	//着丝粒点的位置
	vector<Point> CentromereLocationVector; //是否需要深copy，后续考虑
	//畸变标志，1为畸变，0为正常。默认为正常
	int AberrationFlage=0;
	
};