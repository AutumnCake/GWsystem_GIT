#pragma once
#include "afxwin.h"

// CUserLoad �Ի���

class CUserLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CUserLoad)

public:
	CUserLoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserLoad();

// �Ի�������
	enum { IDD = IDD_USERLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
