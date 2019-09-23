#pragma once
#include "afxwin.h"


// CCodeReset 对话框

class CCodeReset : public CDialogEx
{
	DECLARE_DYNAMIC(CCodeReset)

public:
	CCodeReset(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCodeReset();

// 对话框数据
	enum { IDD = IDD_CODERESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CEdit m_resetcode1;
	CEdit m_resetcode2;
public:
	CString  user_name;

};
