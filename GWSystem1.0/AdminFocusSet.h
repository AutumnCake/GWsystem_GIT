#pragma once
#include "ADOAccess.h"


// CAdminFocusSet �Ի���

class CAdminFocusSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminFocusSet)

public:
	CAdminFocusSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminFocusSet();

// �Ի�������
	enum { IDD = IDD_ADMIN_FOCUSSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	void ReadFromDatabase();//�����ݿ��ȡ����
	void WriteToDatabase();//������д�����ݿ�
	afx_msg void OnBnClickedBtnDefaultmocusad();
	afx_msg void OnBnClickedBtnUpdatefocusad();

public:
	//���ݿ�
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;


};
