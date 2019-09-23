#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"


// CCHROPrintReport 对话框

class CCHROPrintReport : public CDialogEx
{
	DECLARE_DYNAMIC(CCHROPrintReport)

public:
	CCHROPrintReport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCHROPrintReport();

// 对话框数据
	enum { IDD = IDD_PRINTREPORTCHRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_reportchro;
	CListCtrl m_listchro;
	vector<WaitToPrint>* p;
	vector<WaitToPrint>pList;
	ADOAccess m_Conn;
	vector<CString>PatientInList;

public:
	// 生成一个病人的报表
	bool MakeOneReporte(CString patientname, string path);
	// 刷新显示病人
	void FreshPatient();

public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//传递可打印报表的病人
	afx_msg void OnBnClickedBtnAddchro();
	afx_msg void OnBnClickedBtnPaintallchro();
	afx_msg void OnBnClickedBtnChroprint();
	afx_msg void OnBnClickedBtnDelchro();
	afx_msg void OnBnClickedBtnChrocancle();
};
