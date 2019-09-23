#pragma once
#include "GWSystem1.0.h"
#include "ADOAccess.h"

//����OpenCVAPI
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
	



	// Ⱦɫ�����������
	CHRO_HandleResult* DicMain(string road1name1, string file1name1, string writename, CString patientname, CHRO_HandleResult* pB);
};

