#pragma once
#include "afxcmn.h"


// CDiaryManagement �Ի���

class CDiaryManagement : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaryManagement)

public:
	CDiaryManagement(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaryManagement();

// �Ի�������
	enum { IDD = IDD_DIARYMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// �б���ӵı���
	CListCtrl m_diary;
public:
	afx_msg void OnBnClickedBtnDeletediary();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCleardiary();
	afx_msg void OnBnClickedBtnCanclediary();
	void OninitialList(void);
};
