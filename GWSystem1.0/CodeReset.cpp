// CodeReset.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CodeReset.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "UserAmend.h"
#include "Diary.h"

// CCodeReset 对话框

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


// CCodeReset 消息处理程序


void CCodeReset::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
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
			//sql="update 用户管理 set 密码 = '"+code1+"' where 用户名 = '"+str+"'";
			str1 = "update 用户管理 set 密码 = '";
			str2 = "' where 用户名 = '";
			str3 = "'";
			sql = str1 + code1 + str2 + user_name + str3;
			try
			{
				m_Conn.ExecuteSQL((_bstr_t)sql);
				parent->user_amendcontext += "重置密码，";
				parent->m_context.SetWindowText(parent->user_amendcontext);
				AfxMessageBox(_T("密码重置成功！"));
				diary_code.LogRecord(_T("密码重置"));
				OnOK();
			}
			catch (_com_error e)
			{
				AfxMessageBox(e.Description());

			}
		}
		else
		{
			AfxMessageBox(_T("两次输入的密码不一致，请重新输入！"));
			UpdateData(false);
			m_resetcode1.SetWindowText(_T(""));
			m_resetcode2.SetWindowText(_T(""));

		}
	}
	else
		AfxMessageBox(_T("请输入要重置的密码！"));

}


void CCodeReset::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
