#pragma once
#include "ADOAccess.h"


// CAdminScanSet 对话框

class CAdminScanSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminScanSet)

public:
	CAdminScanSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminScanSet();

// 对话框数据
	enum { IDD = IDD_ADMIN_SCANSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefaultscanad();
	afx_msg void OnBnClickedBtnUpdatescanad();

	//数据库
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;

	CComboBox m_comboccm;
	CComboBox m_combodm;
	CComboBox m_combofileextention;
	CEdit m_slidelength;
	CEdit m_slidewidth;
	CEdit m_slidedistance;

	void ReadFromDatabase();//从数据库读取数据
	void WriteToDatabase();//将数据写入数据库
};
