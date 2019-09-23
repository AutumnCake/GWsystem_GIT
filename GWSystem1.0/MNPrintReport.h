#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"



// CMNPrintReport 对话框

class CMNPrintReport : public CDialogEx
{
	DECLARE_DYNAMIC(CMNPrintReport)

public:
	CMNPrintReport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMNPrintReport();

// 对话框数据
	enum { IDD = IDD_PRINTREPORTMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	vector<WaitToPrint>* p;
	vector<WaitToPrint>pList;
	vector<CString>PatientInList;

public:

	CComboBox m_comboxreportmn;
	CListCtrl m_listmn;
	ADOAccess m_Conn;


	// 生成一个病人的报表
	bool MakeOneReporte(CString patientname, string path);
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//传递可打印报表的病人
	afx_msg void OnBnClickedBtnAddmn();
	afx_msg void OnBnClickedBtnMnprint();
	afx_msg void OnBnClickedBtnDelmn();
	// 刷新显示病人
	void FreshPatient();
	afx_msg void OnBnClickedBtnMncancle();
	afx_msg void OnBnClickedBtnPaintallmn();
};
