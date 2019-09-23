#pragma once
#include "GWSystem1.0.h"
#include "ADOAccess.h"

//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

using namespace cv;

class CChromosomeAlgorithm
{
public:
	CChromosomeAlgorithm();
	~CChromosomeAlgorithm();

public:
	CHRO_HandleResultOnePic OnePicResult;
	ADOAccess m_Conn;

	int normalNum = 0;
	



	// 染色体分析主函数
	CHRO_HandleResult* DicMain(string road1name1, string file1name1, string writename, CString patientname, CHRO_HandleResult* pB);
};

