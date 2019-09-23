// CodeReset.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CodeReset.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "UserAmend.h"
#include "Diary.h"

// CCodeReset �Ի���

IMPLEMENT_DYNAMIC(CCodeReset, CDialogEx)
CUserAmend *parent;

CCodeReset::CCodeReset(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeReset::IDD, pParent)
{
	parent = (CUserAmend *)pParent;
}

CCodeReset::~CCodeReset()
{
}

void CCodeReset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RESETCODE1, m_resetcode1);
	DDX_Control(pDX, IDC_EDIT_RESETCODE2, m_resetcode2);
}


BEGIN_MESSAGE_MAP(CCodeReset, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCodeReset::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCodeReset::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCodeReset ��Ϣ�������


void CCodeReset::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDiary diary_code;
	UpdateData();
	CString code1, code2;
	m_resetcode1.GetWindowText(code1);
	m_resetcode2.GetWindowText(code2);
	if (!code1.IsEmpty() && !code2.IsEmpty())
	{
		if (code1 == code2)
		{
			ADOConn m_Conn;
			m_Conn.OnInitADOConn();
			CString str1, str2, str3, sql;
			//sql="update �û����� set ���� = '"+code1+"' where �û��� = '"+str+"'";
			str1 = "update �û����� set ���� = '";
			str2 = "' where �û��� = '";
			str3 = "'";
			sql = str1 + code1 + str2 + user_name + str3;
			try
			{
				m_Conn.ExecuteSQL((_bstr_t)sql);
				parent->user_amendcontext += "�������룬";
				parent->m_context.SetWindowText(parent->user_amendcontext);
				AfxMessageBox(_T("�������óɹ���"));
				diary_code.LogRecord(_T("��������"));
				OnOK();
			}
			catch (_com_error e)
			{
				AfxMessageBox(e.Description());

			}
		}
		else
		{
			AfxMessageBox(_T("������������벻һ�£����������룡"));
			UpdateData(false);
			m_resetcode1.SetWindowText(_T(""));
			m_resetcode2.SetWindowText(_T(""));

		}
	}
	else
		AfxMessageBox(_T("������Ҫ���õ����룡"));

}


void CCodeReset::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
