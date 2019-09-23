#pragma once


// CMainDlgImgShow 对话框

class CMainDlgImgShow : public CDialog
{
	DECLARE_DYNAMIC(CMainDlgImgShow)

public:
	CMainDlgImgShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlgImgShow();

// 对话框数据
	enum { IDD = IDD_MAINDLGIMGSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
