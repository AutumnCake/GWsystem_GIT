#pragma once
#include "GWSystem1.0Dlg.h"
#include"GWSystem1.0.h"


class CScanAndSaveImg :
	public CGWSystem10Dlg
{
public:
	CScanAndSaveImg();
	~CScanAndSaveImg();

	MyFiles allfiles; //�����ɵ��ļ���

	// ��������Ҫ���ļ���
	MyFiles MakeNecessaryFiles(CString savepath);
	
	// ִ��ɨ��ʱ���̺߳���
	void ScanningThread();
};


