#pragma once
#include "CodeReset.h"

// CUserAmend 对话框

class CUserAmend : public CDialogEx
{
	DECLARE_DYNAMIC(CUserAmend)

public:
	CUserAmend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserAmend();

// 对话框数据
	enum { IDD = IDD_USERAMEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString userinfo_type, user_amendcontext;
	CString bh;
	CEdit m_context;
	CEdit m_yhm;
	CEdit m_mm;
	CComboBox m_reason;
	CEdit m_amender;
	CDateTimeCtrl m_time;
	CString usertype;
	CString username;
	CCodeReset *codereset;
	void GetUserInformation(CString str);
	bool tempstr1;
	bool tempstr2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedResercode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioPt();
	afx_msg void OnBnClickedRadioZj();
};
