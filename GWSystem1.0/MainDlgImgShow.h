#pragma once


// CMainDlgImgShow �Ի���

class CMainDlgImgShow : public CDialog
{
	DECLARE_DYNAMIC(CMainDlgImgShow)

public:
	CMainDlgImgShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlgImgShow();

// �Ի�������
	enum { IDD = IDD_MAINDLGIMGSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
