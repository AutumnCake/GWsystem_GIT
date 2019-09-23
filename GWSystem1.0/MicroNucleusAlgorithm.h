#pragma once
#include "GWSystem1.0.h"
#include "ADOAccess.h"

//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

using namespace cv;
class CMicroNucleusAlgorithm
{
public:
	CMicroNucleusAlgorithm();
	~CMicroNucleusAlgorithm();

	ADOAccess m_Conn;
	MN_HandleResultOnePic *OnePicResult = new MN_HandleResultOnePic;
	int SingleCellsWithMN[20];
	int DoubleCellsWithMN[20];
	int TripleCellsWithMN[20];
	int MultiCellsWithMN[20];
	int SaveNum;


	int  shape_judge(Mat img);

	//生成随机颜色函数
	Vec3b RandomColorMN(int value);

	bool Screenshot(IplImage* src, IplImage* dst, CvRect rect);

	// 安全重置矩形大小  
	void SafeResetSizeOfRect(IplImage* src, CvRect& rect);

	//去除非最大轮廓外其他杂质的子函数
	void  cacBounding(string pathName1, IplImage *src1);

	//pathName1作为到图片的路径，fileName2作为合格图像存储的路径，细胞分割后进行再次判定并写入到合格的细胞文件夹的子函数
	void   distinguish(string pathName1, string fileName2, string  pathName2, string fileName3);

	//处理主函数
	MN_HandleResult* handlemicronucleus(string road1name1, string file1name1, string writename, CString patientname, MN_HandleResult *pB, string processfile);

	void readImgNamefromFile(char* fileName, vector <string> &imgNames);

	void readImgNamefromFileFG(char* fileName, vector <string> &imgNames);

	string WChar2Ansi(LPCWSTR pwszSrc);

	

};

