#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"



// CMNPrintReport �Ի���

class CMNPrintReport : public CDialogEx
{
	DECLARE_DYNAMIC(CMNPrintReport)

public:
	CMNPrintReport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMNPrintReport();

// �Ի�������
	enum { IDD = IDD_PRINTREPORTMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	vector<WaitToPrint>* p;
	vector<WaitToPrint>pList;
	vector<CString>PatientInList;

public:

	CComboBox m_comboxreportmn;
	CListCtrl m_listmn;
	ADOAccess m_Conn;


	// ����һ�����˵ı���
	bool MakeOneReporte(CString patientname, string path);
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//���ݿɴ�ӡ����Ĳ���
	afx_msg void OnBnClickedBtnAddmn();
	afx_msg void OnBnClickedBtnMnprint();
	afx_msg void OnBnClickedBtnDelmn();
	// ˢ����ʾ����
	void FreshPatient();
	afx_msg void OnBnClickedBtnMncancle();
	afx_msg void OnBnClickedBtnPaintallmn();
};
