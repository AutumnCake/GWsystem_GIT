// UserLoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserLoad.h"
#include"GWSystem1.0Dlg.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include"ADOAccess.h"
#include "Diary.h"


// CUserLoad �Ի���

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


// CUserLoad ��Ϣ�������
BOOL CUserLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	font1.CreatePointFont(120, TEXT("����"));
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CUserLoad::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BeginWaitCursor();
	CString c_user, c_password;
	UpdateData();
	m_user.GetWindowText(c_user);
	m_password.GetWindowText(c_password);


	if ((c_user != "Admin") && (c_user.IsEmpty() || c_password.IsEmpty()))
	{
		MessageBox(_T("�û��������벻��Ϊ�գ�"), _T("�û���¼��Ϣ"));
	}
	else if ((c_user == "Admin") && c_password.IsEmpty())
	{
		ADOConn m_Connection1;
		CString sqlstr1, sqlstr2;
		sqlstr1 = "select * from �û����� where �û���= 'Admin'";
		m_Connection1.GetRecordSet(_bstr_t(sqlstr1));
		sqlstr2 = m_Connection1.m_pRecordset->GetCollect(_T("����"));
		if (sqlstr2.IsEmpty())
		{
			CGWSystem10App *CApp1;

			CApp1 = (CGWSystem10App*)::AfxGetApp();//��������ָ�룬��Ҫ����

			CApp1->convert1 = "Admin";

			CApp1->convert2 = "��������Ա";


			JudgeType(_T("Admin"));
			CGWSystem10Dlg *pParent1 = (CGWSystem10Dlg *)GetParent(); //ȡ�ø�����ָ�룬��Ҫ����
			CMenu*   mmenu1 = pParent1->GetMenu();
			CMenu*   submenu1 = mmenu1->GetSubMenu(1);
			//��ϵͳ��½�ɹ�����ϵͳ��¼����ť�����Σ����˳�ϵͳ����ť����
			submenu1->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			submenu1->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
			submenu1->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);


			pParent1->users = "Admin";
			UpdateData();
			pParent1->usertype = "��������Ա";
			UpdateData(false);
			AfxGetMainWnd()->SetWindowText(_T("�Ŵ�ѧ����վ��Admin�ѵ�¼��"));//�޸������ڱ���
			EndWaitCursor();
			diary_system.LogRecord(_T("�û���¼�ɹ�"));
			AfxMessageBox(_T("���ε�½������Ϊ�գ��뾡���޸����룡"));
			OnOK();
			m_SuccessLogin = true;
		}
		else
			AfxMessageBox(_T("���������룡"));


	}

	else
	{

		CString sql, str1, str2, str3, sq1;
		//sql="select * from �û����� where �û���='"+c_user+"'and ����='"+c_password+"'";
		str1 = "select * from �û����� where �û���='";
		str2 = "'and ����='";
		str3 = "'";
		sql = str1 + c_user + str2 + c_password + str3;
		ADOConn m_Conn;
		_RecordsetPtr m_pRs;
		m_Conn.OnInitADOConn();
		m_pRs = m_Conn.GetRecordSet((_bstr_t)sql);
		if (m_pRs->adoEOF)               //�û������������
		{
			sq1 = str1 + c_user + str3;
			m_pRs = m_Conn.GetRecordSet((_bstr_t)sq1);
			if (!m_pRs->adoEOF)
			{
				static int p = 0;   //�˴���������Ϊ��̬��������Ҫ��
				AfxMessageBox(_T("����������������룡"));
				EndWaitCursor();
				UpdateData(false);
				m_password.SetWindowText(_T(""));
			}
			else
			{
				AfxMessageBox(_T("�û���������!"));
				EndWaitCursor();
			}

		}
		else
		{

			user = m_pRs->GetCollect(_T("�û���"));
			user.TrimRight();
			CGWSystem10App *CApp;

			CApp = (CGWSystem10App*)::AfxGetApp();//��������ָ�룬��Ҫ����
			CApp->convert1 = user;
			CApp->convert2 = m_pRs->GetCollect(_T("����"));

			diary_system.LogRecord(_T("�û���¼�ɹ�"));
			EndWaitCursor();
			OnOK();
			m_SuccessLogin = true;
		}

	}


	CDialogEx::OnOK();
}

//�Ե�¼�û������м�����ж�
void CUserLoad::JudgeType(CString str)
{
	CGWSystem10Dlg sysdlg;
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString str1, str2, str3, sql, user_type;
	//sql="select * from �û����� where �û���='"+str+"'";
	str1 = "select * from �û����� where �û���='";
	str2 = "'";
	sql = str1 + str + str2;
	m_Conn.GetRecordSet((_bstr_t)sql);
	user_type = m_Conn.m_pRecordset->GetCollect(_T("����"));
	user_type.TrimRight();
	if (user_type == "��ͨ�û�")
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //ȡ�ø�����ָ�룬��Ҫ����
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
		//��Ϣ��������������
		//submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//�˵�����������
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//pParent->m_Toolbar.SetButtonStyle(pParent->m_Toolbar.CommandToIndex(),TBBS_BUTTON);


	}
	else if (user_type == "ר��")
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //ȡ�ø�����ָ�룬��Ҫ����
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
		//��Ϣ��������������
		//submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(4, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		//�˵�����������
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		//pParent->m_Toolbar.SetButtonStyle(pParent->m_Toolbar.CommandToIndex(),TBBS_BUTTON);


	}
	else
	{
		CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)AfxGetApp()->m_pMainWnd; //ȡ�ø�����ָ�룬��Ҫ����

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
		//��Ϣ��������������
		submenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		//ϵͳ����������
		suamenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		suamenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		suamenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
	}


}

//ȡ��
void CUserLoad::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
	m_bCancle = true;
}


//���öԻ�����ؼ�����ɫ
HBRUSH CUserLoad::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
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

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
