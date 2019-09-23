#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"


// CCHROPrintReport �Ի���

class CCHROPrintReport : public CDialogEx
{
	DECLARE_DYNAMIC(CCHROPrintReport)

public:
	CCHROPrintReport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCHROPrintReport();

// �Ի�������
	enum { IDD = IDD_PRINTREPORTCHRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_reportchro;
	CListCtrl m_listchro;
	vector<WaitToPrint>* p;
	vector<WaitToPrint>pList;
	ADOAccess m_Conn;
	vector<CString>PatientInList;

public:
	// ����һ�����˵ı���
	bool MakeOneReporte(CString patientname, string path);
	// ˢ����ʾ����
	void FreshPatient();

public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//���ݿɴ�ӡ����Ĳ���
	afx_msg void OnBnClickedBtnAddchro();
	afx_msg void OnBnClickedBtnPaintallchro();
	afx_msg void OnBnClickedBtnChroprint();
	afx_msg void OnBnClickedBtnDelchro();
	afx_msg void OnBnClickedBtnChrocancle();
};
