#pragma once
#include "ADOAccess.h"


// CAdminScanSet �Ի���

class CAdminScanSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminScanSet)

public:
	CAdminScanSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminScanSet();

// �Ի�������
	enum { IDD = IDD_ADMIN_SCANSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefaultscanad();
	afx_msg void OnBnClickedBtnUpdatescanad();

	//���ݿ�
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;

	CComboBox m_comboccm;
	CComboBox m_combodm;
	CComboBox m_combofileextention;
	CEdit m_slidelength;
	CEdit m_slidewidth;
	CEdit m_slidedistance;

	void ReadFromDatabase();//�����ݿ��ȡ����
	void WriteToDatabase();//������д�����ݿ�
};
