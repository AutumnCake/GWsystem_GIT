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

	// ɾ��xx�������е�ͼƬ
	bool DeleteImgCHRO(CString sourcefile, CString targetfile);
	//ɾ���ϸ��ļ��е�ͼƬ
	bool DeleteImgInQuaCHRO(CString sourcefile, CString targetfile);

public:
	// �����ݿ�����Ƭ��Ϣ���е���ɾ��
	bool MoveToDelInAccessCHRO(CString picturepath);
	// ȡ��ɾ��
	bool CancelDeleteCHRO();
	// ���ݿ�����ɾ������Ϣ��ԭ
	bool MoveToIntrinsicAccessCHRO(CString picturepath);
	// ���Ϊ�ϸ�
	bool QuaImgCHRO(CString sourcefile, CString targetfile);
	// ȡ�����Ϊ�ϸ�
	bool CancelQuaCHRO();
	// ���Ϊ���ϸ�
	bool UnqImgCHRO(CString sourcefile, CString targetfile);

};

