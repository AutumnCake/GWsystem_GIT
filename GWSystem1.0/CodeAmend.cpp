// CodeAmend.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CodeAmend.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "Diary.h"
#include"GWSystem1.0Dlg.h"

// CCodeAmend 对话框

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


// CCodeAmend 消息处理程序


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
			CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)GetParent(); //取得父窗口指针，重要！！
			CString yhm = pParent->users;
			temp = IsCodeRight(yhm);  //判断原密码输入是否正确
			if (temp == TRUE)
			{
				BOOL temp_reset;
				temp_reset = ResetCode(yhm);
				if (temp_reset == TRUE)
				{
					diary_c.LogRecord(_T("用户修改密码"));
					OnOK();
				}
			}
		}
		else
			AfxMessageBox(_T("请再次输入新密码！"));

	}
	else
		AfxMessageBox(_T("请输入新密码！"));

}


void CCodeAmend::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL CCodeAmend::IsCodeRight(CString str)//判断输入的原密码是否错误
{
	UpdateData();
	CString excode;
	m_formercode.GetWindowText(excode);
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString str1, str2, sql, code;
	str1 = "select * from 用户管理 where 用户名='";
	str2 = "'";
	sql = str1 + str + str2;
	m_Conn.GetRecordSet((_bstr_t)sql);
	code = m_Conn.m_pRecordset->GetCollect(_T("密码"));
	code.TrimRight();
	if (code.Compare(excode) != 0)
	{
		AfxMessageBox(_T("原密码输入有误！"));
		return FALSE;

	}
	else
		return TRUE;
	m_Conn.ExitConnect();
}

BOOL CCodeAmend::ResetCode(CString str)//重新设置密码
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
		//sql="update 用户管理 set 密码 = '"+code1+"' where 用户名 = '"+str+"'";
		str1 = "update 用户管理 set 密码 = '";
		str2 = "' where 用户名 = '";
		str3 = "'";
		sql = str1 + code1 + str2 + str + str3;
		try
		{
			m_Conn.ExecuteSQL((_bstr_t)sql);
			AfxMessageBox(_T("密码修改成功！"));
		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}
	else
	{
		AfxMessageBox(_T("两次输入的密码不一致,请重新输入！"));
		UpdateData(false);
		m_code1.SetWindowText(_T(""));
		m_code2.SetWindowText(_T(""));
		return TRUE;

	}
	return FALSE;
}