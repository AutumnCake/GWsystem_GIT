#pragma once
#include "afxcmn.h"
#include "UserBuild.h"
#include"UserAmend.h"

// CUserMangement 对话框

class CUserMangement : public CDialogEx
{
	DECLARE_DYNAMIC(CUserMangement)

public:
	CUserMangement(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserMangement();

// 对话框数据
	enum { IDD = IDD_USERMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 列表添加的变量
	CListCtrl m_datalist;
	CUserBuild * userbuild;
	CUserAmend * dlg;


public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam); //注：必须放在protected里面
	afx_msg void OnBnClickedBtnBuilduser();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAmenduser();
	afx_msg void OnBnClickedBtnDeleteuser();
	afx_msg void OnBnClickedBtnRefreshuser();
	afx_msg void OnBnClickedBtnCancleuser();
};
