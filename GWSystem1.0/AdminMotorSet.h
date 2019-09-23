#pragma once
#include "ADOAccess.h"

// CAdminMotorSet 对话框

class CAdminMotorSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminMotorSet)

public:
	CAdminMotorSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminMotorSet();

// 对话框数据
	enum { IDD = IDD_ADMIN_MOTORSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefaultmotorad();
	afx_msg void OnBnClickedBtnUpdatemotorad();
	void ReadFromDatabase();//从数据库读取数据
	void WriteToDatabase();//将数据写入数据库

	//数据库
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;
};
