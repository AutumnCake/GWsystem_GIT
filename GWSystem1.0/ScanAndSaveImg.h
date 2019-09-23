#pragma once
#include "GWSystem1.0Dlg.h"
#include"GWSystem1.0.h"


class CScanAndSaveImg :
	public CGWSystem10Dlg
{
public:
	CScanAndSaveImg();
	~CScanAndSaveImg();

	MyFiles allfiles; //所生成的文件夹

	// 生成所需要的文件夹
	MyFiles MakeNecessaryFiles(CString savepath);
	
	// 执行扫描时的线程函数
	void ScanningThread();
};


