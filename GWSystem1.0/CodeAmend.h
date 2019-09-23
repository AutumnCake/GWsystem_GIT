#pragma once
#include "afxwin.h"


// CCodeAmend �Ի���

class CCodeAmend : public CDialogEx
{
	DECLARE_DYNAMIC(CCodeAmend)

public:
	CCodeAmend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCodeAmend();

// �Ի�������
	enum { IDD = IDD_CODEAMEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CEdit m_formercode;
	CEdit m_code1;
	CEdit m_code2;
	BOOL IsCodeRight(CString str);
	BOOL ResetCode(CString str);
};
