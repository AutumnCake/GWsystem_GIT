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


// ��������Ҫ���ļ���
MyFiles CScanAndSaveImg::MakeNecessaryFiles(CString savepath)
{
	/*m_savepath = MySlide->path;*/
	string sz2 = CT2A(savepath.GetBuffer());
	string writeroad;
	writeroad = sz2 + "\\";
	CString strs, m_readpath;

	CTime time = CTime::GetCurrentTime(); ///����CTime����
	int m_nYear = time.GetYear(); ///��
	int m_nMonth = time.GetMonth(); ///��
	int m_nDay = time.GetDay(); ///��
	int m_nHour = time.GetHour(); ///Сʱ
	int m_nMinute = time.GetMinute(); ///����
	int m_nSecond = time.GetSecond(); ///��
	CString strDate;
	strDate.Format(_T("%d��%d��%d��"), m_nYear, m_nMonth, m_nDay); //�������ڸ�ʽ
	CString strTime = time.Format(_T("%H-%M-%S")); //����ʱ���ʽ
	CString index(_T("�Ĵ�����"));
	strs = strDate + strTime + index;
	string sz3 = CT2A(strs.GetBuffer());
	allfiles.filesolve = writeroad + sz3 + "\\";//���ɽ�����ļ��� 
	cout << "�ļ�������" << allfiles.filesolve << endl;
	const char* c_filesolve = allfiles.filesolve.c_str();//��·��ת���� const char
	_mkdir(c_filesolve);

	//ץͼ�ļ���
	CString picture(_T("ץͼ���"));
	string  s_picture = CT2A(picture.GetBuffer());
	allfiles.picsolve = allfiles.filesolve + s_picture + "\\";
	const char* c_picsolve = allfiles.picsolve.c_str();//��·��ת���� const char
	_mkdir(c_picsolve);

	//�����ļ���
	CString end(_T("������"));
	string  s_end = CT2A(end.GetBuffer());
	allfiles.endsolve = allfiles.filesolve + s_end + "\\";
	const char* c_endsolve = allfiles.endsolve.c_str();//��·��ת���� const char
	_mkdir(c_endsolve);

	//�ϸ����Ƭ
	CString qua(_T("�ϸ��ͼƬ"));
	string  s_qua = CT2A(qua.GetBuffer());
	allfiles.quasolve = allfiles.filesolve + s_qua + "\\";
	const char* c_quasolve = allfiles.quasolve.c_str();//��·��ת���� const char
	_mkdir(c_quasolve);

	//ɾ����ͼƬ
	CString dele(_T("ɾ����ͼƬ"));
	string  s_dele = CT2A(dele.GetBuffer());
	allfiles.delesolve = allfiles.filesolve + s_dele + "\\";
	const char* c_delesolve = allfiles.delesolve.c_str();//��·��ת���� const char
	_mkdir(c_delesolve);

	//���ϸ��ͼƬ
	CString unq(_T("���ϸ��ͼƬ"));
	string  s_unq = CT2A(unq.GetBuffer());
	allfiles.unqsolve = allfiles.filesolve + s_unq + "\\";
	const char* c_unqsolve = allfiles.unqsolve.c_str();//��·��ת���� const char
	_mkdir(c_unqsolve);

	//�۽�·��
	CString foc(_T("�۽��ļ�"));
	string  s_foc = CT2A(foc.GetBuffer());
	allfiles.focussolve = allfiles.filesolve + s_foc + "\\";
	const char* c_focussolve = allfiles.focussolve.c_str();//��·��ת���� const char
	_mkdir(c_focussolve);

	//10��ɨ���ļ���
	CString scan(_T("10��ɨ��"));
	string  s_scan = CT2A(scan.GetBuffer());
	allfiles.scan10Xsolve = allfiles.filesolve + s_scan + "\\";
	const char* c_scansolve = allfiles.scan10Xsolve.c_str();//��·��ת���� const char
	_mkdir(c_scansolve);

	//�۽�·��
	CString pos(_T("CAλ��"));
	string  s_pos = CT2A(foc.GetBuffer());
	allfiles.CApossolve = allfiles.filesolve + s_pos + "\\";
	const char* c_possolve = allfiles.CApossolve.c_str();//��·��ת���� const char
	_mkdir(c_possolve);

	return allfiles;
}


// ִ��ɨ��ʱ���̺߳���
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

	char *fileName = "C:\\Users\\xibao\\Pictures\\bppp\\";    //�˴�ӦΪ����ץͼ������ļ���
	sprintf_s(tempFilePath, "%s\*.bmp", fileName);
	const char *desfile = allfiles.picsolve.c_str();
	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// ���Ҹ��ļ��������ļ���������Զ�ȡ��WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	int find_pic = 0;//��ͼƬ�����к�

	int picnum = 0;

	if (handle != INVALID_HANDLE_VALUE)
	{
		// ѭ�������õ��ļ��е������ļ���    
		do
		{
			sprintf_s(tempFileName, "%s", fileName);
			imgNames.push_back(app->WChar2Ansi(file.cFileName));
			imgNames[find_pic].insert(0, tempFileName);
			WholePath = imgNames[find_pic].c_str();
			MyFileName = app->GetFileTitleFromFileName(WholePath, TRUE);
			const char * charFileName;
			// ת�������ļ���
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




