#pragma once
#include "ADOAccess.h"


// CAdminFocusSet 对话框

class CAdminFocusSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminFocusSet)

public:
	CAdminFocusSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminFocusSet();

// 对话框数据
	enum { IDD = IDD_ADMIN_FOCUSSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	void ReadFromDatabase();//从数据库读取数据
	void WriteToDatabase();//将数据写入数据库
	afx_msg void OnBnClickedBtnDefaultmocusad();
	afx_msg void OnBnClickedBtnUpdatefocusad();

public:
	//数据库
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;


};
