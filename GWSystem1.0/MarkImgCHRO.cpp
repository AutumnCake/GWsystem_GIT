#include "stdafx.h"
#include "MarkImgCHRO.h"
#include"GWSystem1.0Dlg.h"
#include"GWSystem1.0.h"

//ȫ�ֱ���
extern CGWSystem10Dlg* pDlg;

CMarkImgCHRO::CMarkImgCHRO()
{
}


CMarkImgCHRO::~CMarkImgCHRO()
{
}

// ɾ��ͼƬ
bool CMarkImgCHRO::DeleteImgCHRO(CString sourcefile, CString targetfile)
{
	//1.���ļ��ƶ���ɾ���ļ�����
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = MoveFile(wstr, wstr_des);
	DeletedCHRO.push_back(mytargetpath);
	SourcedelCHRO.push_back(sourcefile);

	//�����ݿ��ڵ���Ƭ��Ϣ���е���ɾ��
	MoveToDelInAccessCHRO(sourcefile);

	return false;
}


// �����ݿ�����Ƭ��Ϣ���е���ɾ��
bool CMarkImgCHRO::MoveToDelInAccessCHRO(CString picturepath)
{
	sql = _T("insert into Ⱦɫ��ͼ�����������ݱ���ɾ���� select * from Ⱦɫ��ͼ�����������ݱ�\
			 			 			  where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ·��]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete from Ⱦɫ��ͼ�����������ݱ�\
			 			 			 	 where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ·��]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	return false;
}


// ȡ��ɾ��
bool CMarkImgCHRO::CancelDeleteCHRO()
{
	CString sourcefile, targetfile;
	if (!DeletedCHRO.empty())
	{
		sourcefile = DeletedCHRO.back();
		targetfile = SourcedelCHRO.back();
	}
	else
	{
		MessageBox(_T("δ�ҵ���ɾ����ͼƬ"));
		return true;
	}
	//1.���ļ��ƶ���ɾ���ļ�����
	const char* sourcefilepath;
	const char* targetfilepath;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(targetfile);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = MoveFile(wstr, wstr_des);
	DeletedCHRO.pop_back();
	SourcedelCHRO.pop_back();
	//�����ݿ��ڵ���Ƭ��Ϣ���е���ɾ��
	MoveToIntrinsicAccessCHRO(targetfile);

	return false;
}


// ���ݿ�����ɾ������Ϣ��ԭ
bool CMarkImgCHRO::MoveToIntrinsicAccessCHRO(CString picturepath)
{
	sql = _T("insert into Ⱦɫ��ͼ�����������ݱ� select * from Ⱦɫ��ͼ�����������ݱ���ɾ����\
			 			 			 			 			 where Ⱦɫ��ͼ�����������ݱ���ɾ����.[��Ƭ·��]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete from Ⱦɫ��ͼ�����������ݱ���ɾ����\
			 			 		 where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ·��]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	return false;
}


// ���Ϊ�ϸ�
bool CMarkImgCHRO::QuaImgCHRO(CString sourcefile, CString targetfile)
{
	//1.���ļ��ƶ����ϸ��ļ�����
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;

	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = CopyFile(wstr, wstr_des, TRUE);
	QuaPicCHRO.push_back(mytargetpath);
	return false;
}


// ȡ�����Ϊ�ϸ�
bool CMarkImgCHRO::CancelQuaCHRO()
{
	CString sourcefile, targetfile;
	if (!QuaPicCHRO.empty())
	{
		sourcefile = QuaPicCHRO.back();
	}
	else
	{
		MessageBox(_T("δ�ұ��Ϊ�ϸ��ͼƬ"));
		return true;
	}
	//1.���ļ��ƶ���ɾ���ļ�����
	const char* sourcefilepath;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	int a = DeleteFile(wstr);
	QuaPicCHRO.pop_back();

	return false;
}


// ���Ϊ���ϸ�
bool CMarkImgCHRO::UnqImgCHRO(CString sourcefile, CString targetfile)
{
	//1.���ļ��ƶ����ϸ��ļ�����
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;

	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = CopyFile(wstr, wstr_des, TRUE);
	return false;
}

//ɾ���ϸ��ļ��е�ͼƬ
bool CMarkImgCHRO::DeleteImgInQuaCHRO(CString sourcefile, CString targetfile)
{
	//1.���ļ��ƶ���ɾ���ļ�����
	const char* sourcefilepath;

	CString myfilename;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);

	// ���ֽ�ת��
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	int a = DeleteFile(wstr);

	return false;
}