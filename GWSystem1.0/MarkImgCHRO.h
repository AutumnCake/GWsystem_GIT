#pragma once
#include "ChromosomeResult.h"
class CMarkImgCHRO :
	public CChromosomeResult
{
public:
	CMarkImgCHRO();
	~CMarkImgCHRO();

private:
	WCHAR   wstr[MAX_PATH];
	WCHAR   wstr_des[MAX_PATH];
	vector<CString> DeletedCHRO;
	vector<CString> SourcedelCHRO;
	vector <CString>QuaPicCHRO;

public:
	ADOAccess m_Conn;
	_bstr_t sql;
	_variant_t var;

	// 删除xx处理结果中的图片
	bool DeleteImgCHRO(CString sourcefile, CString targetfile);
	//删除合格文件夹的图片
	bool DeleteImgInQuaCHRO(CString sourcefile, CString targetfile);

public:
	// 将数据库内照片信息剪切到已删除
	bool MoveToDelInAccessCHRO(CString picturepath);
	// 取消删除
	bool CancelDeleteCHRO();
	// 数据库中已删除的信息复原
	bool MoveToIntrinsicAccessCHRO(CString picturepath);
	// 标记为合格
	bool QuaImgCHRO(CString sourcefile, CString targetfile);
	// 取消标记为合格
	bool CancelQuaCHRO();
	// 标记为不合格
	bool UnqImgCHRO(CString sourcefile, CString targetfile);

};

