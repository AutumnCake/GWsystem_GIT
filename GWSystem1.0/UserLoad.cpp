// UserLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserLoad.h"
#include"GWSystem1.0Dlg.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include"ADOAccess.h"
#include "Diary.h"


// CUserLoad 对话框

CDiary diary_system;

IMPLEMENT_DYNAMIC(CUserLoad, CDialogEx)

CUserLoad::CUserLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserLoad::IDD, pParent)
{
	m_bCancle = false;
	m_SuccessLogin = false;
}

CUserLoad::~CUserLoad()
{
}

void CUserLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERNAME, m_user);
	DDX_Control(pDX, IDC_PASSWORD, m_password);
}


BEGIN_MESSAGE_MAP(CUserLoad, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUserLoad::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUserLoad::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CUserLoad 消息处理程序
BOOL CUserLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	font1.CreatePointFont(120, TEXT("黑体"));
	font2.CreatePointFont(100, TEXT("Consolas"));
	font3.CreatePointFont(140, TEXT("Consolas"));

	m_user.SetFont(&font3);
	m_password.SetFont(&font1);

	CRect rcEdit;
	m_user.GetClientRect(&rcEdit);
	TEXTMETRIC tm;
	m_user.GetDC()->GetTextMetrics(&tm);
	int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	int nOffY = (rcEdit.Height() - nFontHeight) / 2;
	::OffsetRect(&rcEdit, 0, nOffY);
	::SendMessage(m_user.m_hWnd, EM_SETRECT, 0, (LPARAM)&rcEdit);

	m_password.GetClientRect(&rcEdit);
	m_password.GetDC()->GetTextMetrics(&tm);
	nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	nOffY = (rcEdit.Height() - nFontHeight) / 2;
	::OffsetRect(&rcEdit, 0, nOffY);
	::SendMessage(m_password.m_hWnd, EM_SETRECT, 0, (LPARAM)&rcEdit);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CUserLoad::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	BeginWaitCursor();
	CString c_user, c_password;
	UpdateData();
	m_user.GetWindowText(c_user);
	m_password.GetWindowText(c_password);


	if ((c_user != "Admin") && (c_user.IsEmpty() || c_password.IsEmpty()))
	{
		MessageBox(_T("用户名或密码不能为空！"), _T("用户登录信息"));
	}
	else if ((c_user == "Admin") && c_password.IsEmpty())
	{
		ADOConn m_Connection1;
		CString sqlstr1, sqlstr2;
		sqlstr1 = "select * from 用户管理 where 用户名= 'Admin'";
		m_Connection1.GetRecordSet(_bstr_t(sqlstr1));
		sqlstr2 = m_Connection1.m_pRecordset->GetCollect(_T("密码"));
		if (sqlstr2.IsEmpty())
		{
			CGWSystem10App *CApp1;

			CApp1 = (CGWSystem10App*)::AfxGetApp();//获得主框架指针，重要！！

			CApp1->convert1 = "Admin";

			CApp1->convert2 = "超级管理员";


			JudgeType(_T("Admin"));
			CGWSystem10Dlg *pParent1 = (CGWSystem10Dlg *)GetParent(); //取得父窗口指针，重要！！
			CMenu*   mmenu1 = pParent1->GetMenu();
			CMenu*   submenu1 = mmenu1->GetSubMenu(1);
			//若系统登陆成功，则“系统登录”按钮被屏蔽，“退出系统”按钮可用
			submenu1->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			submenu1->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);


			pParent1->users = "Admin";
			UpdateData();
			pParent1->usertype = "超级管理员";
			UpdateData(false);
			AfxGetMainWnd()->SetWindowText(_T("遗传学工作站（Admin已登录）"));//修改主窗口标题
			EndWaitCursor();
			diary_system.LogRecord(_T("用户登录成功"));
			AfxMessageBox(_T("初次登陆，密码为空！请尽快修改密码！"));
			OnOK();
			m_SuccessLogin = true;
		}
		else
			AfxMessageBox(_T("请输入密码！"));


	}

	else
	{

		CString sql, str1, str2, str3, sq1;
		//sql="select * from 用户管理 where 用户名='"+c_user+"'and 密码='"+c_password+"'";
		str1 = "select * from 用户管理 where 用户名='";
		str2 = "'and 密码='";
		str3 = "'";
		sql = str1 + c_user + str2 + c_password + str3;
		ADOConn m_Conn;
		_RecordsetPtr m_pRs;
		m_Conn.OnInitADOConn();
		m_pRs = m_Conn.GetRecordSet((_bstr_t)sql);
		if (m_pRs->adoEOF)               //用户名或密码错误
		{
			sq1 = str1 + c_user + str3;
			m_pRs = m_Conn.GetRecordSet((_bstr_t)sq1);
			if (!m_pRs->adoEOF)
			{
				static int p = 0;   //此处必须声明为静态变量，重要！
				AfxMessageBox(_T("密码错误！请重新输入！"));
				EndWaitCursor();
				UpdateData(false);
				m_password.SetWindowText(_T(""));
			}
			else
			{
				AfxMessageBox(_T("用户名不存在!"));
				EndWaitCursor();
			}

		}
		else
		{

			user = m_pRs->GetCollect(_T("用户名"));
			user.TrimRight();
			CGWSystem10App *CApp;

			CApp = (CGWSystem10App*)::AfxGetApp();//获得主框架指针，重要！！
			CApp->convert1 = user;
			CApp->convert2 = m_pRs->GetCollect(_T("级别"));

			diary_system.LogRecord(_T("用户登录成功"));
			EndWaitCursor();
			OnOK();
			m_SuccessLogin = true;
		}

	}


	CDialogEx::OnOK();
}

//对登录用户名进行级别的判断
void CUserLoad::JudgeType(CString str)
{
	CGWSystem10Dlg sysdlg;
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString str1, str2, str3, sql, user_type;
	//sql="select * from 用户管理 where 用户名='"+str+"'";
	str1 = "select * from 用户管理 where 用户名='";
	str2 = "'";
	sql = str1 + str + str2;
	m_Conn.GetRecordSet((_bstr_t)sql);
	user_type = m_Conn.m_pRecordset->GetCollect(_T("级别"));
	user_type.TrimRight();
	if (user_type == "普通用户")
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //取得父窗口指针，重要！！
		CMenu*   mmenu = pParent->GetMenu();
		CMenu*   suamenu = mmenu->GetSubMenu(1);
		CMenu*   submenu = mmenu->GetSubMenu(2);
		CMenu*   sucmenu = mmenu->GetSubMenu(3);
		CMenu*   sudmenu = mmenu->GetSubMenu(4);

		//mmenu->EnableMenuItem(1, MF_BYPOSITION   |   MF_ENABLED ); 
		//mmenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//mmenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);
		//信息管理栏弹出设置
		//submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//菜单栏弹出设置
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//pParent->m_Toolbar.SetButtonStyle(pParent->m_Toolbar.CommandToIndex(),TBBS_BUTTON);


	}
	else if (user_type == "专家")
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //取得父窗口指针，重要！！
		CMenu*   mmenu = pParent->GetMenu();
		CMenu*   suamenu = mmenu->GetSubMenu(1);
		CMenu*   submenu = mmenu->GetSubMenu(2);
		CMenu*   sucmenu = mmenu->GetSubMenu(3);
		CMenu*   sudmenu = mmenu->GetSubMenu(4);

		//mmenu->EnableMenuItem(1, MF_BYPOSITION   |   MF_ENABLED ); 
		//mmenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//mmenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);
		//信息管理栏弹出设置
		//submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//菜单栏弹出设置
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//pParent->m_Toolbar.SetButtonStyle(pParent->m_Toolbar.CommandToIndex(),TBBS_BUTTON);


	}
	else
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //取得父窗口指针，重要！！

		CMenu*   mmenu = pParent->GetMenu();
		CMenu*   submenu = mmenu->GetSubMenu(2);
		CMenu*   suamenu = mmenu->GetSubMenu(1);
		mmenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		mmenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
		//mmenu->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);
		//信息管理栏弹出设置
		submenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		//系统栏弹出设置
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
	}


}

//取消
void CUserLoad::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	m_bCancle = true;
}


//设置对话框与控件的颜色
HBRUSH CUserLoad::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_STATIC1:
			pDC->SetTextColor(RGB(0, 0, 0));
			/*pDC->SetBkColor(RGB(255, 255, 255));*/
			/*pDC->SetBkMode(TRANSPARENT);*/
			pDC->SelectObject(&font1);
			/*return (HBRUSH)::GetStockObject(BLACK_BRUSH);*/
			break;
		case IDC_STATIC2:
			pDC->SetTextColor(RGB(0, 0, 0));
			/*pDC->SetBkColor(RGB(255, 255, 255));*/
			pDC->SelectObject(&font1);
			/*return (HBRUSH)::GetStockObject(BLACK_BRUSH);*/
			break;
		case IDC_STATIC3:
			pDC->SetTextColor(RGB(169, 169, 169));
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SelectObject(&font2);
			/*return (HBRUSH)::GetStockObject(BLACK_BRUSH);*/
			break;
		default:
			break;
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
