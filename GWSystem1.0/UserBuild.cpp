// UserBuild.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserBuild.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "UserMangement.h"
#include "ADOAccess.h"
#include "Diary.h"

// CUserBuild 对话框

IMPLEMENT_DYNAMIC(CUserBuild, CDialogEx)

CDiary diary_u1;

CUserBuild::CUserBuild(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserBuild::IDD, pParent)
{

}

CUserBuild::~CUserBuild()
{
}

void CUserBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_USERNAME, m_name);
	DDX_Control(pDX, IDC_EDIT_USERCODE, m_code);
	DDX_Control(pDX, IDC_EDIT_BUILDER, m_builder);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
}


BEGIN_MESSAGE_MAP(CUserBuild, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserBuild::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserBuild::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserBuild 消息处理程序


BOOL CUserBuild::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//窗口初始化时将已有用户名添加到控件中
	_bstr_t sql;
	UpdateData(false);

	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();
	CString user_type;

	builder = CApp->convert1;

	user_type = CApp->convert2;
	user_type.TrimRight();
	if (user_type == "超级管理员")
	{
		GetDlgItem(IDC_RADIO_ADMINISTER)->EnableWindow(FALSE);
	}
	m_builder.SetWindowTextW(builder);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CUserBuild::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strlb;
	CString strname, date;

	CString sql, code;
	m_name.GetWindowText(strname);
	m_code.GetWindowText(code);
	m_date.GetWindowText(date);


	bool temp, temp1;
	temp = UserTypeSelect(strlb);
	if (temp == true && !strname.IsEmpty() && !code.IsEmpty())
	{
		temp1 = IsNameExist(strname);
		if (temp1 == false)

		{
			if (strlb == _T("普通用户"))

			{
				ADOAccess m_Conn;
				m_Conn.OnInitADOAccess();
				_bstr_t sql;
				_variant_t var;
				UpdateData(false);
				sql = _T("select * from 用户管理 ");
				try
				{
					m_Conn.GetRecordSet(sql);
				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());

				}
				try
				{
					m_Conn.m_pRecordset->AddNew();
					m_Conn.m_pRecordset->PutCollect(_T("用户名"), _variant_t(strname));
					m_Conn.m_pRecordset->PutCollect(_T("密码"), _variant_t(code));
					m_Conn.m_pRecordset->PutCollect(_T("级别"), _variant_t(strlb));
					m_Conn.m_pRecordset->PutCollect(_T("创建者"), _variant_t(builder));
					m_Conn.m_pRecordset->PutCollect(_T("创建日期"), _variant_t(date));
					m_Conn.m_pRecordset->PutCollect(_T("修改者"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改日期"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改内容"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改原因"), _variant_t("无"));

					m_Conn.m_pRecordset->Update();
					AfxMessageBox(_T("添加纪录成功！"));
					CString str1, str2, str3;
					str1 = "创建用户“";
					str3 = "”";
					str2 = str1 + strname + str3;
					diary_u1.LogRecord(str2);

				}
				catch (_com_error e)
				{
					AfxMessageBox(e.Description());
				}

			}

			else
			{
				ADOAccess m_Conn;
				m_Conn.OnInitADOAccess();
				_bstr_t sql;
				_variant_t var;
				UpdateData(false);
				sql = _T("select * from 用户管理 ");
				try
				{
					m_Conn.GetRecordSet(sql);
				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());

				}
				try
				{
					m_Conn.m_pRecordset->AddNew();
					m_Conn.m_pRecordset->PutCollect(_T("用户名"), _variant_t(strname));
					m_Conn.m_pRecordset->PutCollect(_T("密码"), _variant_t(code));
					m_Conn.m_pRecordset->PutCollect(_T("级别"), _variant_t(strlb));
					m_Conn.m_pRecordset->PutCollect(_T("创建者"), _variant_t(builder));
					m_Conn.m_pRecordset->PutCollect(_T("创建日期"), _variant_t(date));
					m_Conn.m_pRecordset->PutCollect(_T("修改者"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改日期"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改内容"), _variant_t("无"));
					m_Conn.m_pRecordset->PutCollect(_T("修改原因"), _variant_t("无"));
					m_Conn.m_pRecordset->Update();
					AfxMessageBox(_T("添加纪录成功！"));
					m_Conn.ExitConnect();
					CString str1, str2, str3;
					str1 = "创建用户“";
					str3 = "”";
					str2 = str1 + strname + str3;
					diary_u1.LogRecord(str2);

				}
				catch (_com_error e)
				{
					AfxMessageBox(e.Description());
				}


			}
			HWND   hnd = ::FindWindow(NULL, _T("用户管理"));

			::SendMessage(hnd, WM_USER_UPDATE, NULL, NULL);

			OnOK();

		}
		else

			AfxMessageBox(_T("该用户名已经存在！"));
	}
	else if (strname.IsEmpty())
		AfxMessageBox(_T("用户名不能为空！"));
	else
		AfxMessageBox(_T("密码不能为空！"));

}


void CUserBuild::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

//判断用户是否选择了用户类别
bool CUserBuild::UserTypeSelect(CString &str)//判断用户是否选择了用户类别
{

	CButton  *p_button1 = (CButton *)GetDlgItem(IDC_RADIO_NORMAL);
	CButton  *p_button2 = (CButton *)GetDlgItem(IDC_RADIO_SPECIALIST);
	CButton  *p_button3 = (CButton *)GetDlgItem(IDC_RADIO_ADMINISTER);
	if (p_button1->GetCheck() == 1)
	{
		str = _T("普通用户");

		return true;
	}

	else if (p_button2->GetCheck() == 1)
	{
		str = _T("专家");
		return true;
	}

	else if (p_button3->GetCheck() == 1)
	{
		str = _T("超级管理员");
		return true;
	}
	else
	{
		AfxMessageBox(_T("请选择用户类别！"));
		return false;
	}
}

//判断用户名是否存在
bool CUserBuild::IsNameExist(CString str)
{
	///加年份判断


	CString strlb;
	UserTypeSelect(strlb);
	CString sql;
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	sql = _T("select * from 用户管理");
	try
	{
		m_Conn.GetRecordSet((_bstr_t)sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			if (strlb == _T("普通用户"))
			{

				if (m_Conn.m_pRecordset->GetCollect(_T("用户名")) == str)
				{
					return true;
				}
				m_Conn.m_pRecordset->MoveNext();

			}
			else
			{

				if (m_Conn.m_pRecordset->GetCollect(_T("用户名")) == str)
				{
					return true;
				}
				m_Conn.m_pRecordset->MoveNext();
			}


		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	m_Conn.ExitConnect();
	return false;


}
