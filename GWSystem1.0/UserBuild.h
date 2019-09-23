#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CUserBuild 对话框

class CUserBuild : public CDialogEx
{
	DECLARE_DYNAMIC(CUserBuild)

public:
	CUserBuild(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserBuild();

// 对话框数据
	enum { IDD = IDD_USERBUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_name;
	CEdit m_code;
	CEdit m_builder;
	CDateTimeCtrl m_date;
	CString builder;

	bool IsNameExist(CString str);
	bool UserTypeSelect(CString &str);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
