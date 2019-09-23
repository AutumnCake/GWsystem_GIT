#pragma once
#include "afxcmn.h"
#include "UserBuild.h"
#include"UserAmend.h"

// CUserMangement �Ի���

class CUserMangement : public CDialogEx
{
	DECLARE_DYNAMIC(CUserMangement)

public:
	CUserMangement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserMangement();

// �Ի�������
	enum { IDD = IDD_USERMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// �б���ӵı���
	CListCtrl m_datalist;
	CUserBuild * userbuild;
	CUserAmend * dlg;


public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam); //ע���������protected����
	afx_msg void OnBnClickedBtnBuilduser();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAmenduser();
	afx_msg void OnBnClickedBtnDeleteuser();
	afx_msg void OnBnClickedBtnRefreshuser();
	afx_msg void OnBnClickedBtnCancleuser();
};
