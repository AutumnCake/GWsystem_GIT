// UserAmend.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserAmend.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "Diary.h"

// CUserAmend 对话框

IMPLEMENT_DYNAMIC(CUserAmend, CDialogEx)
CDiary diary_u2;

CUserAmend::CUserAmend(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserAmend::IDD, pParent)
{

}

CUserAmend::~CUserAmend()
{
}

void CUserAmend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_yhm);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_mm);
	DDX_Control(pDX, IDC_COMBO_REASON, m_reason);
	DDX_Control(pDX, IDC_DATETIMEPICKER_AMEND, m_time);
	DDX_Control(pDX, IDC_EDIT_AMENDER, m_amender);
	DDX_Control(pDX, IDC_EDIT_CONTEXT, m_context);
}


BEGIN_MESSAGE_MAP(CUserAmend, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserAmend::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RESERCODE, &CUserAmend::OnBnClickedResercode)
	ON_BN_CLICKED(IDCANCEL, &CUserAmend::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_PT, &CUserAmend::OnBnClickedRadioPt)
	ON_BN_CLICKED(IDC_RADIO_ZJ, &CUserAmend::OnBnClickedRadioZj)
END_MESSAGE_MAP()


// CUserAmend 消息处理程序

BOOL CUserAmend::OnInitDialog()
{

	CDialog::OnInitDialog();

	usertype = theApp.convert2;
	usertype.TrimRight();
	((CComboBox*)GetDlgItem(IDC_COMBO_REASON))->AddString(_T("用户权限不符"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REASON))->AddString(_T("密码需要重置"));

	GetUserInformation(bh);
	if (userinfo_type == "普通用户")
	{
		((CButton *)GetDlgItem(IDC_RADIO_PT))->SetCheck(TRUE);
	}
	else if (userinfo_type == "专家")
	{
		((CButton *)GetDlgItem(IDC_RADIO_ZJ))->SetCheck(TRUE);
	}

	tempstr1 = false;
	tempstr2 = false;


	return TRUE;

}

void CUserAmend::GetUserInformation(CString str)
{
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString name, str1, str2, str3, str4;
	//sql="select * from 用户管理 where 用户名='"+str1+"'";
	str1 = "select * from 用户管理 where 用户名='";
	str2 = "'";
	str3 = str1 + str + str2;
	try
	{
		m_Conn.GetRecordSet(_bstr_t(str3));
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		userinfo_type = m_Conn.m_pRecordset->GetCollect(_T("级别"));
		userinfo_type.TrimRight();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}


void CUserAmend::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strlb, strzt, stryy, strsj, strnr, strbz, strxg, strname;
	CString str1, str2, str3, str4;
	m_reason.GetWindowText(stryy);//yy=原因
	m_time.GetWindowText(strsj);//sj=时间
	m_context.GetWindowText(strnr);//nr=内容
	m_amender.GetWindowText(strxg);//xg=修改者
	m_yhm.GetWindowText(strname);//yhm=用户名
	CButton  *p_button1 = (CButton *)GetDlgItem(IDC_RADIO_PT);
	if (p_button1->GetCheck() == 1)
	{
		strlb = _T("普通用户");
	}
	CButton  *p_button2 = (CButton *)GetDlgItem(IDC_RADIO_ZJ);
	if (p_button2->GetCheck() == 1)
	{
		strlb = _T("专家");
	}


	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString string1, string2, string3;
	//删除记录的SQL语句
	//str1="select * from 用户管理 where 用户名 ='"+bh+"'";
	string1 = "select * from 用户管理 where 用户名 ='";
	string2 = "'";
	string3 = string1 + bh + string2;
	try
	{
		m_Conn.GetRecordSet((_bstr_t)string3);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	if (!stryy.IsEmpty())
	{
		try
		{
			if (!strlb.IsEmpty())//如果不修改“级别”，则保持原有值
			{
				//str="update zhbgl set 级别 ='"+strlb+"' where 用户名='"+bh+"'";
				str1 = "update 用户管理 set 级别 ='";
				str2 = "' where 用户名='";
				str3 = "'";
				str4 = str1 + strlb + str2 + bh + str3;
				m_Conn.ExecuteSQL((_bstr_t)str4);

			}
			//写入
			m_Conn.m_pRecordset->PutCollect(_T("修改者"), _variant_t(strxg));
			m_Conn.m_pRecordset->PutCollect(_T("修改原因"), _variant_t(stryy));
			m_Conn.m_pRecordset->PutCollect(_T("修改日期"), _variant_t(strsj));
			m_Conn.m_pRecordset->PutCollect(_T("修改内容"), _variant_t(user_amendcontext));
			m_Conn.m_pRecordset->Update();
			CString tempstr1, tempstr2, tempstr3;
			tempstr1 = "修改用户“";
			tempstr3 = "”";
			tempstr2 = tempstr1 + strname + tempstr3;
			diary_u2.LogRecord(tempstr2);

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.ErrorMessage());
		}
		m_Conn.ExitConnect();

		HWND   hnd = ::FindWindow(NULL, _T("用户管理"));//调用消息处理函数刷新页面

		::SendMessage(hnd, WM_USER_UPDATE, NULL, NULL);

		OnOK();
	}
	else if (stryy.IsEmpty())
		AfxMessageBox(_T("修改原因不能为空！"));

}


void CUserAmend::OnBnClickedResercode()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	m_yhm.GetWindowText(username);
	codereset = new CCodeReset(this);
	codereset->Create(IDD_CODERESET, NULL);
	codereset->ShowWindow(SW_SHOW);
	codereset->user_name = username;

}


void CUserAmend::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CUserAmend::OnBnClickedRadioPt()
{
	// TODO:  在此添加控件通知处理程序代码
	if (userinfo_type != "普通用户"&&tempstr1 == false)
	{
		user_amendcontext += "修改用户类别，";
		m_context.SetWindowText(user_amendcontext);
		tempstr1 = true;

	}
}


void CUserAmend::OnBnClickedRadioZj()
{
	// TODO:  在此添加控件通知处理程序代码
	if (userinfo_type != "专家"&&tempstr1 == false)
	{
		user_amendcontext += "修改用户类别，";
		m_context.SetWindowText(user_amendcontext);
		tempstr1 = true;

	}
}
