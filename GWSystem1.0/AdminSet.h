#pragma once
#include"AdminScanSet.h"
#include"AdminMotorSet.h"
#include"AdminFocusSet.h"
#include "afxcmn.h"


// CAdminSet �Ի���

class CAdminSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminSet)

public:
	CAdminSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminSet();

// �Ի�������
	enum { IDD = IDD_ADMINSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CAdminScanSet ScanSet;
	CAdminMotorSet MotorSet;
	CAdminFocusSet FoucsSet;

private:
	CTabCtrl m_tab;

public:
	afx_msg void OnTcnSelchangeTabAdminset(NMHDR *pNMHDR, LRESULT *pResult);
};
