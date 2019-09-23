#pragma once

#import "c:\Program Files\Common Files\System\ado\msado15.dll"no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")

#include "ADOViewAccess.h"

class ADOAccess
{
public:
	_ConnectionPtr m_pConnection;//���ݿ����Ӷ���

	_RecordsetPtr m_pRecordset;//���ݿ��¼������

public:
	
	ADOAccess(void);

	~ADOAccess(void);

	void OnInitADOAccess(void);

	void ExitConnect(void);

	BOOL ExecuteAccess(_bstr_t bstrAccess);

	_RecordsetPtr& GetRecordSet(_bstr_t bstrAccess);

	BOOL ShowADOViewAccess(CString fields[], int len, CADOViewAccess* view);
	bool DatabaseState(void);
	int DataNumber(_bstr_t bstrAccess);
};
