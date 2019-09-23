#pragma once
#include"AdminScanSet.h"
#include"AdminMotorSet.h"
#include"AdminFocusSet.h"
#include "afxcmn.h"


// CAdminSet 对话框

class CAdminSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminSet)

public:
	CAdminSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdminSet();

// 对话框数据
	enum { IDD = IDD_ADMINSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
