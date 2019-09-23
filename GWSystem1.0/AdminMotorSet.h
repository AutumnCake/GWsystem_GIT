#pragma once
#include "ADOAccess.h"

// CAdminMotorSet �Ի���

class CAdminMotorSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminMotorSet)

public:
	CAdminMotorSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdminMotorSet();

// �Ի�������
	enum { IDD = IDD_ADMIN_MOTORSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefaultmotorad();
	afx_msg void OnBnClickedBtnUpdatemotorad();
	void ReadFromDatabase();//�����ݿ��ȡ����
	void WriteToDatabase();//������д�����ݿ�

	//���ݿ�
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;
};
