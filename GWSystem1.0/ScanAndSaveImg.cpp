#include "stdafx.h"
#include "ScanAndSaveImg.h"

#include<string>
#include<direct.h>

using namespace std;
using std::string;

extern CGWSystem10Dlg* pDlg;

CScanAndSaveImg::CScanAndSaveImg()
{
}


CScanAndSaveImg::~CScanAndSaveImg()
{
}


// 生成所需要的文件夹
MyFiles CScanAndSaveImg::MakeNecessaryFiles(CString savepath)
{
	/*m_savepath = MySlide->path;*/
	string sz2 = CT2A(savepath.GetBuffer());
	string writeroad;
	writeroad = sz2 + "\\";
	CString strs, m_readpath;

	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	int m_nYear = time.GetYear(); ///年
	int m_nMonth = time.GetMonth(); ///月
	int m_nDay = time.GetDay(); ///日
	int m_nHour = time.GetHour(); ///小时
	int m_nMinute = time.GetMinute(); ///分钟
	int m_nSecond = time.GetSecond(); ///秒
	CString strDate;
	strDate.Format(_T("%d年%d月%d日"), m_nYear, m_nMonth, m_nDay); //定义日期格式
	CString strTime = time.Format(_T("%H-%M-%S")); //定义时间格式
	CString index(_T("的处理结果"));
	strs = strDate + strTime + index;
	string sz3 = CT2A(strs.GetBuffer());
	allfiles.filesolve = writeroad + sz3 + "\\";//生成结果的文件名 
	cout << "文件夹名：" << allfiles.filesolve << endl;
	const char* c_filesolve = allfiles.filesolve.c_str();//将路径转换成 const char
	_mkdir(c_filesolve);

	//抓图文件夹
	CString picture(_T("抓图结果"));
	string  s_picture = CT2A(picture.GetBuffer());
	allfiles.picsolve = allfiles.filesolve + s_picture + "\\";
	const char* c_picsolve = allfiles.picsolve.c_str();//将路径转换成 const char
	_mkdir(c_picsolve);

	//处理文件夹
	CString end(_T("处理结果"));
	string  s_end = CT2A(end.GetBuffer());
	allfiles.endsolve = allfiles.filesolve + s_end + "\\";
	const char* c_endsolve = allfiles.endsolve.c_str();//将路径转换成 const char
	_mkdir(c_endsolve);

	//合格的照片
	CString qua(_T("合格的图片"));
	string  s_qua = CT2A(qua.GetBuffer());
	allfiles.quasolve = allfiles.filesolve + s_qua + "\\";
	const char* c_quasolve = allfiles.quasolve.c_str();//将路径转换成 const char
	_mkdir(c_quasolve);

	//删除的图片
	CString dele(_T("删除的图片"));
	string  s_dele = CT2A(dele.GetBuffer());
	allfiles.delesolve = allfiles.filesolve + s_dele + "\\";
	const char* c_delesolve = allfiles.delesolve.c_str();//将路径转换成 const char
	_mkdir(c_delesolve);

	//不合格的图片
	CString unq(_T("不合格的图片"));
	string  s_unq = CT2A(unq.GetBuffer());
	allfiles.unqsolve = allfiles.filesolve + s_unq + "\\";
	const char* c_unqsolve = allfiles.unqsolve.c_str();//将路径转换成 const char
	_mkdir(c_unqsolve);

	//聚焦路径
	CString foc(_T("聚焦文件"));
	string  s_foc = CT2A(foc.GetBuffer());
	allfiles.focussolve = allfiles.filesolve + s_foc + "\\";
	const char* c_focussolve = allfiles.focussolve.c_str();//将路径转换成 const char
	_mkdir(c_focussolve);

	//10倍扫描文件夹
	CString scan(_T("10倍扫描"));
	string  s_scan = CT2A(scan.GetBuffer());
	allfiles.scan10Xsolve = allfiles.filesolve + s_scan + "\\";
	const char* c_scansolve = allfiles.scan10Xsolve.c_str();//将路径转换成 const char
	_mkdir(c_scansolve);

	//聚焦路径
	CString pos(_T("CA位置"));
	string  s_pos = CT2A(foc.GetBuffer());
	allfiles.CApossolve = allfiles.filesolve + s_pos + "\\";
	const char* c_possolve = allfiles.CApossolve.c_str();//将路径转换成 const char
	_mkdir(c_possolve);

	return allfiles;
}


// 执行扫描时的线程函数
void CScanAndSaveImg::ScanningThread()
{
	char tempFilePath[MAX_PATH + 1];
	WCHAR   wstr[MAX_PATH];
	WCHAR   wstr_des[MAX_PATH];
	char tempFileName[10000];
	WIN32_FIND_DATA file;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	vector <string>  imgNames;
	CString WholePath, MyFileName;
	/*const char* DstFile;*/
	char DesTempFilePath[MAX_PATH + 1];

	char *fileName = "C:\\Users\\xibao\\Pictures\\bppp\\";    //此处应为本次抓图所存的文件夹
	sprintf_s(tempFilePath, "%s\*.bmp", fileName);
	const char *desfile = allfiles.picsolve.c_str();
	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// 查找该文件待操作文件的相关属性读取到WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	int find_pic = 0;//的图片的序列号

	int picnum = 0;

	if (handle != INVALID_HANDLE_VALUE)
	{
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf_s(tempFileName, "%s", fileName);
			imgNames.push_back(app->WChar2Ansi(file.cFileName));
			imgNames[find_pic].insert(0, tempFileName);
			WholePath = imgNames[find_pic].c_str();
			MyFileName = app->GetFileTitleFromFileName(WholePath, TRUE);
			const char * charFileName;
			// 转换输入文件名
			CStringA strA(MyFileName.GetBuffer(0));
			MyFileName.ReleaseBuffer();
			string s;
			if (picnum < 10)
				s = "000" + to_string(picnum) + ".bmp";
			else if (picnum<100)
				s = "00" + to_string(picnum) + ".bmp";
			else if (picnum<1000)
				s = "0" + to_string(picnum) + ".bmp";
			else
				s = to_string(picnum) + ".bmp";

			charFileName = s.c_str();
			picnum++;

			sprintf_s(DesTempFilePath, "%s%s", desfile, charFileName);
			MultiByteToWideChar(CP_ACP, 0, imgNames[find_pic].c_str(), -1, wstr, sizeof(wstr));
			MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
			int a = CopyFile(wstr, wstr_des, TRUE);
			Sleep(333);
			find_pic++;

		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);
}




