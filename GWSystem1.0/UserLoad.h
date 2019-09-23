#pragma once
#include "afxwin.h"

// CUserLoad 对话框

class CUserLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CUserLoad)

public:
	CUserLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserLoad();

// 对话框数据
	enum { IDD = IDD_USERLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_user;
	CEdit m_password;
	CString user;
	CFont font1;
	CFont font2;
	CFont font3;

	bool m_bCancle;
	bool m_SuccessLogin;

	void JudgeType(CString str);

	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
