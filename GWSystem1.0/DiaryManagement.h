#pragma once
#include "afxcmn.h"


// CDiaryManagement 对话框

class CDiaryManagement : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaryManagement)

public:
	CDiaryManagement(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaryManagement();

// 对话框数据
	enum { IDD = IDD_DIARYMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 列表添加的变量
	CListCtrl m_diary;
public:
	afx_msg void OnBnClickedBtnDeletediary();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCleardiary();
	afx_msg void OnBnClickedBtnCanclediary();
	void OninitialList(void);
};
