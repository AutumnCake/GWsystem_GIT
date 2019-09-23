// AdminSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminSet.h"
#include "afxdialogex.h"


// CAdminSet 对话框

IMPLEMENT_DYNAMIC(CAdminSet, CDialogEx)

CAdminSet::CAdminSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdminSet::IDD, pParent)
{

}

CAdminSet::~CAdminSet()
{
}

void CAdminSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_ADMINSET, m_tab);
}


BEGIN_MESSAGE_MAP(CAdminSet, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ADMINSET, &CAdminSet::OnTcnSelchangeTabAdminset)
END_MESSAGE_MAP()


// CAdminSet 消息处理程序


BOOL CAdminSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//((CButton*)GetDlgItem(IDC_TAB_ADMINSET))->SetCheck(TRUE);//设置Radio控件默认

	m_tab.InsertItem(0, _T("扫描设置"));
	m_tab.InsertItem(1, _T("电机设置"));
	m_tab.InsertItem(2, _T("自动对焦设置"));

	ScanSet.Create(IDD_ADMIN_SCANSET, &m_tab);
	MotorSet.Create(IDD_ADMIN_MOTORSET, &m_tab);
	FoucsSet.Create(IDD_ADMIN_FOCUSSET, &m_tab);

	CRect rc;
	m_tab.GetClientRect(&rc);//获得m_tab对话框举行尺寸

	rc.top += 22;
	rc.bottom -= 4;
	rc.left += 4;
	rc.right -= 4;

	ScanSet.SetWindowPos(NULL, rc.left,  rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);
	MotorSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
	FoucsSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);

	m_tab.SetCurSel(0);//默认设置页面

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAdminSet::OnTcnSelchangeTabAdminset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int CurSel = m_tab.GetCurSel();
	CRect rc;
	m_tab.GetClientRect(&rc);//获得m_tab对话框举行尺寸

	rc.top += 22;
	rc.bottom -= 4;
	rc.left += 4;
	rc.right -= 4;
	switch (CurSel)

	{
	case 0:
		ScanSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		MotorSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		FoucsSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		break;

	case 1:

		ScanSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		MotorSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		FoucsSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		break;

	case 2:
		ScanSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		MotorSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
		FoucsSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		break;

	default:
		break;

	}

	*pResult = 0;
}
