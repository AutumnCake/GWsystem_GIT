// DiaryManagement.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "DiaryManagement.h"
#include "afxdialogex.h"
#include "ADOConn.h"


// CDiaryManagement 对话框

IMPLEMENT_DYNAMIC(CDiaryManagement, CDialogEx)

CDiaryManagement::CDiaryManagement(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaryManagement::IDD, pParent)
{

}

CDiaryManagement::~CDiaryManagement()
{
}

void CDiaryManagement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDIARY, m_diary);
}


BEGIN_MESSAGE_MAP(CDiaryManagement, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DELETEDIARY, &CDiaryManagement::OnBnClickedBtnDeletediary)
	ON_BN_CLICKED(IDC_BTN_CLEARDIARY, &CDiaryManagement::OnBnClickedBtnCleardiary)
	ON_BN_CLICKED(IDC_BTN_CANCLEDIARY, &CDiaryManagement::OnBnClickedBtnCanclediary)
END_MESSAGE_MAP()


// CDiaryManagement 消息处理程序

BOOL CDiaryManagement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString usertype;
	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();

	usertype = CApp->convert2;
	CString  str("超级管理员");
	//usertype.Replace("","");
	usertype.TrimRight();
	// TODO:  Add extra initialization here
	if (usertype.Compare(str) != 0)
	{
		//禁用按钮的方法
		GetDlgItem(IDC_BTN_DELETEDIARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLEARDIARY)->EnableWindow(FALSE);
	}
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from 日志管理");
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	m_Conn.GetRecordSet(sql);
	m_diary.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_diary.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60, 0);
	m_diary.InsertColumn(1, _T("流水号"), LVCFMT_LEFT, 160, 0);
	m_diary.InsertColumn(2, _T("用户"), LVCFMT_LEFT, 140, 1);
	m_diary.InsertColumn(3, _T("级别"), LVCFMT_LEFT, 140, 2);
	m_diary.InsertColumn(4, _T("日期"), LVCFMT_LEFT, 100, 3);
	m_diary.InsertColumn(5, _T("时间"), LVCFMT_LEFT, 80, 4);
	m_diary.InsertColumn(6, _T("操作内容"), LVCFMT_LEFT, 160, 5);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_diary.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("序号"));
			//if(var.vt!=VT_NULL)
			m_diary.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("流水号"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("用户"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 2, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("级别"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("日期"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("时间"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("操作内容"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 6, (_bstr_t)var);

			i += 1;
			m_Conn.m_pRecordset->MoveNext();


		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	m_Conn.ExitConnect();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//删除记录
void CDiaryManagement::OnBnClickedBtnDeletediary()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_diary.GetSelectionMark();
	ADOConn m_Conn;
	if (sel >= 0 && AfxMessageBox(_T("是否删除？"), MB_YESNO) == IDYES)
	{
		//根据用户在List控件中的选择获得配送编号
		CString dataid = m_diary.GetItemText(sel, 1);
		HRESULT hr;
		CString str1, str2, str3;

		//删除记录的SQL语句
		//"delete from yhgl where 用户名 ='"+nametype+"'";
		str1 = "delete from 日志管理 where 流水号 ='";
		str2 = "'";
		str3 = str1 + dataid + str2;
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
		//m_Conn.m_pRecordset->Update();
		//List控件中也删除该条记录
		m_diary.DeleteItem(sel);
		OninitialList();
	}
	else if (sel<0)
		MessageBox(_T("列表中无选中记录！"));

}


//清空
void CDiaryManagement::OnBnClickedBtnCleardiary()
{
	// TODO:  在此添加控件通知处理程序代码
	ADOConn m_Conn;

	if (AfxMessageBox(_T("确定清空？"), MB_YESNO) == IDYES)

	{
		CString xm;

		xm = "delete from 日志管理"; // 清空数据表语句
		_variant_t RecordsAffected;
		try
		{

			m_Conn.ExecuteSQL((_bstr_t)xm);//清空数据表

			AfxMessageBox(_T("清空成功！"));

		}
		catch (_com_error *e)

		{

			AfxMessageBox(e->ErrorMessage());

			return;

		}
		OninitialList();//刷新列表
	}


}

//取消
void CDiaryManagement::OnBnClickedBtnCanclediary()
{
	// TODO:  在此添加控件通知处理程序代码
	OnOK();
}

//列表刷新
void CDiaryManagement::OninitialList(void)//列表刷新
{
	m_diary.DeleteAllItems();//清空列表
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from 日志管理");
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_diary.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("序号"));
			//if(var.vt!=VT_NULL)
			m_diary.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("流水号"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("用户"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 2, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("级别"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("日期"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("时间"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("操作内容"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 6, (_bstr_t)var);

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
