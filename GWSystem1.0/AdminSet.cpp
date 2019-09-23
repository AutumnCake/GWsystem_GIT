// AdminSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminSet.h"
#include "afxdialogex.h"


// CAdminSet �Ի���

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


// CAdminSet ��Ϣ�������


BOOL CAdminSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//((CButton*)GetDlgItem(IDC_TAB_ADMINSET))->SetCheck(TRUE);//����Radio�ؼ�Ĭ��

	m_tab.InsertItem(0, _T("ɨ������"));
	m_tab.InsertItem(1, _T("�������"));
	m_tab.InsertItem(2, _T("�Զ��Խ�����"));

	ScanSet.Create(IDD_ADMIN_SCANSET, &m_tab);
	MotorSet.Create(IDD_ADMIN_MOTORSET, &m_tab);
	FoucsSet.Create(IDD_ADMIN_FOCUSSET, &m_tab);

	CRect rc;
	m_tab.GetClientRect(&rc);//���m_tab�Ի�����гߴ�

	rc.top += 22;
	rc.bottom -= 4;
	rc.left += 4;
	rc.right -= 4;

	ScanSet.SetWindowPos(NULL, rc.left,  rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);
	MotorSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
	FoucsSet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_HIDEWINDOW);

	m_tab.SetCurSel(0);//Ĭ������ҳ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAdminSet::OnTcnSelchangeTabAdminset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int CurSel = m_tab.GetCurSel();
	CRect rc;
	m_tab.GetClientRect(&rc);//���m_tab�Ի�����гߴ�

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
