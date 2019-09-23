#pragma once
#include "ADOAccess.h"


// CFocusSet �Ի���

class CFocusSet : public CDialogEx
{
	DECLARE_DYNAMIC(CFocusSet)

public:
	CFocusSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFocusSet();

// �Ի�������
	enum { IDD = IDD_FOCUSSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	//���ݿ�
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;
	//static ALL_PAREMETER *para = new ALL_PAREMETER;
	CEdit m_slidelength;
	CEdit m_slidewidth;
	CEdit m_slidedistance;
	CEdit m_savefile;
	CString m_savepath;//��ͼ·���ļ���·��

	void ReadFromDatabase();//�����ݿ��ȡ����
	void WriteToDatabase();//������д�����ݿ�
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnUpdatefocus();
	afx_msg void OnBnClickedBtnRefreshfocus();
};
