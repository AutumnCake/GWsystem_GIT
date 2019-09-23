#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CUserBuild �Ի���

class CUserBuild : public CDialogEx
{
	DECLARE_DYNAMIC(CUserBuild)

public:
	CUserBuild(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserBuild();

// �Ի�������
	enum { IDD = IDD_USERBUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
