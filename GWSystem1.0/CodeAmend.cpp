// CodeAmend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CodeAmend.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "Diary.h"
#include"GWSystem1.0Dlg.h"

// CCodeAmend �Ի���

IMPLEMENT_DYNAMIC(CCodeAmend, CDialogEx)
CDiary diary_c;

CCodeAmend::CCodeAmend(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeAmend::IDD, pParent)
{

}

CCodeAmend::~CCodeAmend()
{
}

void CCodeAmend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OLDCODE, m_formercode);
	DDX_Control(pDX, IDC_EDIT_NEWCODE, m_code1);
	DDX_Control(pDX, IDC_EDIT_NEWCODE2, m_code2);
}


BEGIN_MESSAGE_MAP(CCodeAmend, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCodeAmend::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCodeAmend::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCodeAmend ��Ϣ�������


void CCodeAmend::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString excode, code1, code2;
	m_formercode.GetWindowText(excode);
	m_code1.GetWindowText(code1);
	m_code2.GetWindowText(code2);
	//if (!excode.IsEmpty())
	//{
	if (!code1.IsEmpty())
	{
		if (!code2.IsEmpty())
		{
			BOOL temp;
			CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)GetParent(); //ȡ�ø�����ָ�룬��Ҫ����
			CString yhm = pParent->users;
			temp = IsCodeRight(yhm);  //�ж�ԭ���������Ƿ���ȷ
			if (temp == TRUE)
			{
				BOOL temp_reset;
				temp_reset = ResetCode(yhm);
				if (temp_reset == TRUE)
				{
					diary_c.LogRecord(_T("�û��޸�����"));
					OnOK();
				}
			}
		}
		else
			AfxMessageBox(_T("���ٴ����������룡"));

	}
	else
		AfxMessageBox(_T("�����������룡"));

}


void CCodeAmend::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

BOOL CCodeAmend::IsCodeRight(CString str)//�ж������ԭ�����Ƿ����
{
	UpdateData();
	CString excode;
	m_formercode.GetWindowText(excode);
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString str1, str2, sql, code;
	str1 = "select * from �û����� where �û���='";
	str2 = "'";
	sql = str1 + str + str2;
	m_Conn.GetRecordSet((_bstr_t)sql);
	code = m_Conn.m_pRecordset->GetCollect(_T("����"));
	code.TrimRight();
	if (code.Compare(excode) != 0)
	{
		AfxMessageBox(_T("ԭ������������"));
		return FALSE;

	}
	else
		return TRUE;
	m_Conn.ExitConnect();
}

BOOL CCodeAmend::ResetCode(CString str)//������������
{
	UpdateData();
	CString code1, code2;
	m_code1.GetWindowText(code1);
	m_code2.GetWindowText(code2);
	if (code1 == code2)
	{
		ADOConn m_Conn;
		m_Conn.OnInitADOConn();
		CString str1, str2, str3, sql;
		//sql="update �û����� set ���� = '"+code1+"' where �û��� = '"+str+"'";
		str1 = "update �û����� set ���� = '";
		str2 = "' where �û��� = '";
		str3 = "'";
		sql = str1 + code1 + str2 + str + str3;
		try
		{
			m_Conn.ExecuteSQL((_bstr_t)sql);
			AfxMessageBox(_T("�����޸ĳɹ���"));
		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}
	else
	{
		AfxMessageBox(_T("������������벻һ��,���������룡"));
		UpdateData(false);
		m_code1.SetWindowText(_T(""));
		m_code2.SetWindowText(_T(""));
		return TRUE;

	}
	return FALSE;
}