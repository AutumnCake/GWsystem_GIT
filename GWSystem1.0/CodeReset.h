#pragma once
#include "afxwin.h"


// CCodeReset �Ի���

class CCodeReset : public CDialogEx
{
	DECLARE_DYNAMIC(CCodeReset)

public:
	CCodeReset(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCodeReset();

// �Ի�������
	enum { IDD = IDD_CODERESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
