#pragma once
#include "ADOAccess.h"


// CAlgorithmParaSet 对话框

class CAlgorithmParaSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAlgorithmParaSet)

public:
	CAlgorithmParaSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAlgorithmParaSet();

// 对话框数据
	enum { IDD = IDD_ALGORITHMPARASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	//数据库
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;

	void ReadFromDatabase();//从数据库读取数据
	void WriteToDatabase();//将数据写入数据库
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefauteal();
	afx_msg void OnBnClickedBtnUpdateal();
};
