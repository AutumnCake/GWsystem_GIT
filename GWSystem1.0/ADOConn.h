#pragma once

#import "c:\Program Files\Common Files\System\ado\msado15.dll"no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")

#include "ADOView.h"

class ADOConn
{
public:
	_ConnectionPtr m_pConnection;//���ݿ����Ӷ���

	_RecordsetPtr m_pRecordset;//���ݿ��¼������

public:
	
	ADOConn(void);

	~ADOConn(void);

	void OnInitADOConn(void);

	void ExitConnect(void);

	BOOL ExecuteSQL(_bstr_t bstrSQL);

	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

	BOOL ShowADOView(CString fields[], int len, CADOView* view);
	bool DatabaseState(void);
	int DataNumber(_bstr_t bstrSQL);
};
