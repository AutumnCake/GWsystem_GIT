#pragma once
#include "ADOAccess.h"


// CAlgorithmParaSet �Ի���

class CAlgorithmParaSet : public CDialogEx
{
	DECLARE_DYNAMIC(CAlgorithmParaSet)

public:
	CAlgorithmParaSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlgorithmParaSet();

// �Ի�������
	enum { IDD = IDD_ALGORITHMPARASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//���ݿ�
	ADOAccess m_Database_AdminScanSet;
	_bstr_t sql;
	_variant_t var;

	void ReadFromDatabase();//�����ݿ��ȡ����
	void WriteToDatabase();//������д�����ݿ�
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDefauteal();
	afx_msg void OnBnClickedBtnUpdateal();
};
