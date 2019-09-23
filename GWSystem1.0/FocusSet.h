#pragma once
#include "ADOAccess.h"


// CFocusSet 对话框

class CFocusSet : public CDialogEx
{
	DECLARE_DYNAMIC(CFocusSet)

public:
	CFocusSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFocusSet();

// 对话框数据
	enum { IDD = IDD_FOCUSSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	//数据库
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;
	//static ALL_PAREMETER *para = new ALL_PAREMETER;
	CEdit m_slidelength;
	CEdit m_slidewidth;
	CEdit m_slidedistance;
	CEdit m_savefile;
	CString m_savepath;//存图路径文件夹路径

	void ReadFromDatabase();//从数据库读取数据
	void WriteToDatabase();//将数据写入数据库
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnUpdatefocus();
	afx_msg void OnBnClickedBtnRefreshfocus();
};
