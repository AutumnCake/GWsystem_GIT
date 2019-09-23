// UserMangement.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserMangement.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include"GWSystem1.0Dlg.h"

// CUserMangement 对话框

IMPLEMENT_DYNAMIC(CUserMangement, CDialogEx)

CUserMangement::CUserMangement(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserMangement::IDD, pParent)
{

}

CUserMangement::~CUserMangement()
{
}

void CUserMangement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERMANAGE, m_datalist);

}


BEGIN_MESSAGE_MAP(CUserMangement, CDialogEx)
	ON_MESSAGE(WM_USER_UPDATE, &CUserMangement::InitialList1) //自定义消息处理函数
	ON_BN_CLICKED(IDC_BTN_BUILDUSER, &CUserMangement::OnBnClickedBtnBuilduser)
	ON_BN_CLICKED(IDC_BTN_AMENDUSER, &CUserMangement::OnBnClickedBtnAmenduser)
	ON_BN_CLICKED(IDC_BTN_DELETEUSER, &CUserMangement::OnBnClickedBtnDeleteuser)
	ON_BN_CLICKED(IDC_BTN_REFRESHUSER, &CUserMangement::OnBnClickedBtnRefreshuser)
	ON_BN_CLICKED(IDC_BTN_CANCLEUSER, &CUserMangement::OnBnClickedBtnCancleuser)
END_MESSAGE_MAP()


// CUserMangement 消息处理程序
BOOL CUserMangement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from 用户管理");
	m_Conn.GetRecordSet(sql);
	//列表控件的外观设置
	m_datalist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_datalist.InsertColumn(0, _T("编号"), LVCFMT_LEFT, 60, 0);
	m_datalist.InsertColumn(1, _T("用户名"), LVCFMT_LEFT, 120, 1);
	m_datalist.InsertColumn(2, _T("密码"), LVCFMT_LEFT, 60, 2);
	m_datalist.InsertColumn(3, _T("级别"), LVCFMT_LEFT, 100, 3);
	m_datalist.InsertColumn(4, _T("创建者"), LVCFMT_LEFT, 120, 4);
	m_datalist.InsertColumn(5, _T("创建日期"), LVCFMT_LEFT, 100, 5);
	m_datalist.InsertColumn(6, _T("修改者"), LVCFMT_LEFT, 120, 7);
	m_datalist.InsertColumn(7, _T("修改日期"), LVCFMT_LEFT, 100, 8);
	m_datalist.InsertColumn(8, _T("修改内容"), LVCFMT_LEFT, 120, 9);
	m_datalist.InsertColumn(9, _T("修改原因"), LVCFMT_LEFT, 120, 10);

	//列表控件与数据库的实时连接
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("编号"));
			//if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("用户名"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("密码"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("级别"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改内容"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改原因"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 9, (_bstr_t)var);

			i += 1;
			m_Conn.m_pRecordset->MoveNext();


		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	m_Conn.ExitConnect();
	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();

	CString usertype;

	usertype = CApp->convert2;
	usertype.TrimRight();
	// TODO:  Add extra initialization here
	if (usertype != "超级管理员")
	{
		GetDlgItem(IDC_BTN_DELETEUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BUILDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_AMENDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REFRESHUSER)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//新建用户
void CUserMangement::OnBnClickedBtnBuilduser()
{
	// TODO:  在此添加控件通知处理程序代码
	userbuild = new CUserBuild;
	userbuild->Create(IDD_USERBUILD, NULL);
	userbuild->ShowWindow(SW_SHOW);
}

//用户修改
void CUserMangement::OnBnClickedBtnAmenduser()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_datalist.GetSelectionMark();
	CString dataid = m_datalist.GetItemText(sel, 1);
	dataid.TrimRight();
	CString mm = m_datalist.GetItemText(sel, 2);
	CString bh = m_datalist.GetItemText(sel, 0);
	CString type = m_datalist.GetItemText(sel, 3);
	type.TrimRight();
	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();

	CString usertype;

	usertype = CApp->convert2;
	usertype.TrimRight();
	if (sel<0)
	{
		AfxMessageBox(_T("没有选中的项目！"));
	}
	else
	{
		if (dataid == "Admin")
		{
			MessageBox(_T("超级管理员不允许修改！"));
		}
		else
		{

			CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)GetParent(); //取得父窗口指针，重要！！
			dlg = new CUserAmend();
			CString amender;
			amender = pParent->users;
			dlg->bh = dataid;
			dlg->Create(IDD_USERAMEND, NULL);
			dlg->ShowWindow(SW_SHOW);
			dlg->m_yhm.SetWindowText(dataid);
			dlg->m_mm.SetWindowText(mm);
			dlg->m_amender.SetWindowText(amender);
			dlg->m_yhm.EnableWindow(FALSE);
			dlg->m_mm.EnableWindow(FALSE);

		}
	}


}


void CUserMangement::OnBnClickedBtnDeleteuser()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_datalist.GetSelectionMark();
	CString nametype = m_datalist.GetItemText(sel, 1);
	CString type = m_datalist.GetItemText(sel, 3);
	CString beizhu = m_datalist.GetItemText(sel, 11);

	if (nametype == "Admin")
	{
		MessageBox(_T("超级管理员不能删除！"));
	}
	else
	{
		ADOConn m_Conn;
		if (sel >= 0 && AfxMessageBox(_T("是否删除？"), MB_YESNO) == IDYES)
		{

			if (type == _T("普通用户"))
			{
				HRESULT hr;
				CString str1, str2, str3, str4;

				//删除记录的SQL语句
				//"delete from 用户管理 where 用户名 ='"+nametype+"' ";
				str1 = "delete from 用户管理 where 用户名 ='";
				str2 = "'";
				str3 = str1 + nametype + str2;
				_variant_t RecordsAffected;
				try
				{
					//执行删除SQL语句
					hr = m_Conn.ExecuteSQL((_bstr_t)str3);
					if (SUCCEEDED(hr))
					{
						MessageBox(_T("删除记录成功！"));
					}
					else
						MessageBox(_T("删除记录失败！"));

				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
					return;
				}
				//List控件中也删除该条记录
				m_datalist.DeleteItem(sel);
				OnBnClickedBtnRefreshuser();

			}
			else
			{
				//根据用户在List控件中的选择获得配送编号
				//CString dataid=m_datalist.GetItemText(sel,0);
				HRESULT hr;
				CString str1, str2, str3;

				//删除记录的SQL语句
				//"delete from 用户管理 where 用户名 ='"+nametype+"'";
				str1 = "delete from 用户管理 where 用户名 ='";
				str2 = "'";
				str3 = str1 + nametype + str2;
				_variant_t RecordsAffected;
				try
				{
					//执行删除SQL语句
					hr = m_Conn.ExecuteSQL((_bstr_t)str3);
					if (SUCCEEDED(hr))
					{
						MessageBox(_T("删除记录成功！"));
					}
					else
						MessageBox(_T("删除记录失败！"));

				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
					return;
				}
				//List控件中也删除该条记录
				m_datalist.DeleteItem(sel);
				//InitialList();
				OnBnClickedBtnRefreshuser();
			}
		}
		else if (sel<0)
			MessageBox(_T("列表中无选中记录！"));
	}

}


void CUserMangement::OnBnClickedBtnRefreshuser()
{
	// TODO:  在此添加控件通知处理程序代码
	m_datalist.DeleteAllItems();
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from 用户管理");
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("编号"));
			//if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("用户名"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("密码"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("级别"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改内容"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改原因"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 9, (_bstr_t)var);
			i += 1;
			m_Conn.m_pRecordset->MoveNext();


		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	m_Conn.ExitConnect();

}


void CUserMangement::OnBnClickedBtnCancleuser()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

//自定义消息函数
LRESULT CUserMangement::InitialList1(WPARAM wParam, LPARAM lParam)
{
	m_datalist.DeleteAllItems();
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from 用户管理");
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//	var=m_Conn.m_pRecordset->GetCollect(_T("编号"));
			//	if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("用户名"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("密码"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("级别"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("创建日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改者"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改日期"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改内容"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("修改原因"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 9, (_bstr_t)var);
			i += 1;
			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	m_Conn.ExitConnect();
	return 0;
}
