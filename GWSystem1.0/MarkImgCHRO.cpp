#include "stdafx.h"
#include "MarkImgCHRO.h"
#include"GWSystem1.0Dlg.h"
#include"GWSystem1.0.h"

//全局变量
extern CGWSystem10Dlg* pDlg;

CMarkImgCHRO::CMarkImgCHRO()
{
}


CMarkImgCHRO::~CMarkImgCHRO()
{
}

// 删除图片
bool CMarkImgCHRO::DeleteImgCHRO(CString sourcefile, CString targetfile)
{
	//1.将文件移动到删除文件夹中
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = MoveFile(wstr, wstr_des);
	DeletedCHRO.push_back(mytargetpath);
	SourcedelCHRO.push_back(sourcefile);

	//将数据库内的照片信息剪切到已删除
	MoveToDelInAccessCHRO(sourcefile);

	return false;
}


// 将数据库内照片信息剪切到已删除
bool CMarkImgCHRO::MoveToDelInAccessCHRO(CString picturepath)
{
	sql = _T("insert into 染色体图像分析结果数据表（已删除） select * from 染色体图像分析结果数据表\
			 			 			  where 染色体图像分析结果数据表.[照片路径]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete from 染色体图像分析结果数据表\
			 			 			 	 where 染色体图像分析结果数据表.[照片路径]='" + picturepath + "' ");
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


// 取消删除
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
		MessageBox(_T("未找到已删除的图片"));
		return true;
	}
	//1.将文件移动到删除文件夹中
	const char* sourcefilepath;
	const char* targetfilepath;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(targetfile);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = MoveFile(wstr, wstr_des);
	DeletedCHRO.pop_back();
	SourcedelCHRO.pop_back();
	//将数据库内的照片信息剪切到已删除
	MoveToIntrinsicAccessCHRO(targetfile);

	return false;
}


// 数据库中已删除的信息复原
bool CMarkImgCHRO::MoveToIntrinsicAccessCHRO(CString picturepath)
{
	sql = _T("insert into 染色体图像分析结果数据表 select * from 染色体图像分析结果数据表（已删除）\
			 			 			 			 			 where 染色体图像分析结果数据表（已删除）.[照片路径]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete from 染色体图像分析结果数据表（已删除）\
			 			 		 where 染色体图像分析结果数据表.[照片路径]='" + picturepath + "' ");
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


// 标记为合格
bool CMarkImgCHRO::QuaImgCHRO(CString sourcefile, CString targetfile)
{
	//1.将文件移动到合格文件夹中
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;

	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = CopyFile(wstr, wstr_des, TRUE);
	QuaPicCHRO.push_back(mytargetpath);
	return false;
}


// 取消标记为合格
bool CMarkImgCHRO::CancelQuaCHRO()
{
	CString sourcefile, targetfile;
	if (!QuaPicCHRO.empty())
	{
		sourcefile = QuaPicCHRO.back();
	}
	else
	{
		MessageBox(_T("未找标记为合格的图片"));
		return true;
	}
	//1.将文件移动到删除文件夹中
	const char* sourcefilepath;
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	int a = DeleteFile(wstr);
	QuaPicCHRO.pop_back();

	return false;
}


// 标记为不合格
bool CMarkImgCHRO::UnqImgCHRO(CString sourcefile, CString targetfile)
{
	//1.将文件移动到合格文件夹中
	const char* sourcefilepath;
	const char* targetfilepath;

	CString myfilename, mytargetpath;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	mytargetpath = targetfile + myfilename;

	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);
	targetfilepath = T2A(mytargetpath);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	MultiByteToWideChar(CP_ACP, 0, targetfilepath, -1, wstr_des, sizeof(wstr_des));
	int a = CopyFile(wstr, wstr_des, TRUE);
	return false;
}

//删除合格文件夹的图片
bool CMarkImgCHRO::DeleteImgInQuaCHRO(CString sourcefile, CString targetfile)
{
	//1.将文件移动到删除文件夹中
	const char* sourcefilepath;

	CString myfilename;
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	myfilename = app->GetFileTitleFromFileName(sourcefile, TRUE);
	USES_CONVERSION;
	sourcefilepath = T2A(sourcefile);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, sourcefilepath, -1, wstr, sizeof(wstr));
	int a = DeleteFile(wstr);

	return false;
}