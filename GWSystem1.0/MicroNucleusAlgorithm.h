#pragma once
#include "GWSystem1.0.h"
#include "ADOAccess.h"

//����OpenCVAPI
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

	//���������ɫ����
	Vec3b RandomColorMN(int value);

	bool Screenshot(IplImage* src, IplImage* dst, CvRect rect);

	// ��ȫ���þ��δ�С  
	void SafeResetSizeOfRect(IplImage* src, CvRect& rect);

	//ȥ��������������������ʵ��Ӻ���
	void  cacBounding(string pathName1, IplImage *src1);

	//pathName1��Ϊ��ͼƬ��·����fileName2��Ϊ�ϸ�ͼ��洢��·����ϸ���ָ������ٴ��ж���д�뵽�ϸ��ϸ���ļ��е��Ӻ���
	void   distinguish(string pathName1, string fileName2, string  pathName2, string fileName3);

	//����������
	MN_HandleResult* handlemicronucleus(string road1name1, string file1name1, string writename, CString patientname, MN_HandleResult *pB, string processfile);

	void readImgNamefromFile(char* fileName, vector <string> &imgNames);

	void readImgNamefromFileFG(char* fileName, vector <string> &imgNames);

	string WChar2Ansi(LPCWSTR pwszSrc);

	

};

