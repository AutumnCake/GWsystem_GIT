// UserMangement.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserMangement.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include"GWSystem1.0Dlg.h"

// CUserMangement �Ի���

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
	ON_MESSAGE(WM_USER_UPDATE, &CUserMangement::InitialList1) //�Զ�����Ϣ������
	ON_BN_CLICKED(IDC_BTN_BUILDUSER, &CUserMangement::OnBnClickedBtnBuilduser)
	ON_BN_CLICKED(IDC_BTN_AMENDUSER, &CUserMangement::OnBnClickedBtnAmenduser)
	ON_BN_CLICKED(IDC_BTN_DELETEUSER, &CUserMangement::OnBnClickedBtnDeleteuser)
	ON_BN_CLICKED(IDC_BTN_REFRESHUSER, &CUserMangement::OnBnClickedBtnRefreshuser)
	ON_BN_CLICKED(IDC_BTN_CANCLEUSER, &CUserMangement::OnBnClickedBtnCancleuser)
END_MESSAGE_MAP()


// CUserMangement ��Ϣ�������
BOOL CUserMangement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from �û�����");
	m_Conn.GetRecordSet(sql);
	//�б�ؼ����������
	m_datalist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_datalist.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60, 0);
	m_datalist.InsertColumn(1, _T("�û���"), LVCFMT_LEFT, 120, 1);
	m_datalist.InsertColumn(2, _T("����"), LVCFMT_LEFT, 60, 2);
	m_datalist.InsertColumn(3, _T("����"), LVCFMT_LEFT, 100, 3);
	m_datalist.InsertColumn(4, _T("������"), LVCFMT_LEFT, 120, 4);
	m_datalist.InsertColumn(5, _T("��������"), LVCFMT_LEFT, 100, 5);
	m_datalist.InsertColumn(6, _T("�޸���"), LVCFMT_LEFT, 120, 7);
	m_datalist.InsertColumn(7, _T("�޸�����"), LVCFMT_LEFT, 100, 8);
	m_datalist.InsertColumn(8, _T("�޸�����"), LVCFMT_LEFT, 120, 9);
	m_datalist.InsertColumn(9, _T("�޸�ԭ��"), LVCFMT_LEFT, 120, 10);

	//�б�ؼ������ݿ��ʵʱ����
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("���"));
			//if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("�û���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�ԭ��"));
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
	if (usertype != "��������Ա")
	{
		GetDlgItem(IDC_BTN_DELETEUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BUILDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_AMENDUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REFRESHUSER)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�½��û�
void CUserMangement::OnBnClickedBtnBuilduser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	userbuild = new CUserBuild;
	userbuild->Create(IDD_USERBUILD, NULL);
	userbuild->ShowWindow(SW_SHOW);
}

//�û��޸�
void CUserMangement::OnBnClickedBtnAmenduser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("û��ѡ�е���Ŀ��"));
	}
	else
	{
		if (dataid == "Admin")
		{
			MessageBox(_T("��������Ա�������޸ģ�"));
		}
		else
		{

			CGWSystem10Dlg *pParent = (CGWSystem10Dlg *)GetParent(); //ȡ�ø�����ָ�룬��Ҫ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int sel = m_datalist.GetSelectionMark();
	CString nametype = m_datalist.GetItemText(sel, 1);
	CString type = m_datalist.GetItemText(sel, 3);
	CString beizhu = m_datalist.GetItemText(sel, 11);

	if (nametype == "Admin")
	{
		MessageBox(_T("��������Ա����ɾ����"));
	}
	else
	{
		ADOConn m_Conn;
		if (sel >= 0 && AfxMessageBox(_T("�Ƿ�ɾ����"), MB_YESNO) == IDYES)
		{

			if (type == _T("��ͨ�û�"))
			{
				HRESULT hr;
				CString str1, str2, str3, str4;

				//ɾ����¼��SQL���
				//"delete from �û����� where �û��� ='"+nametype+"' ";
				str1 = "delete from �û����� where �û��� ='";
				str2 = "'";
				str3 = str1 + nametype + str2;
				_variant_t RecordsAffected;
				try
				{
					//ִ��ɾ��SQL���
					hr = m_Conn.ExecuteSQL((_bstr_t)str3);
					if (SUCCEEDED(hr))
					{
						MessageBox(_T("ɾ����¼�ɹ���"));
					}
					else
						MessageBox(_T("ɾ����¼ʧ�ܣ�"));

				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
					return;
				}
				//List�ؼ���Ҳɾ��������¼
				m_datalist.DeleteItem(sel);
				OnBnClickedBtnRefreshuser();

			}
			else
			{
				//�����û���List�ؼ��е�ѡ�������ͱ��
				//CString dataid=m_datalist.GetItemText(sel,0);
				HRESULT hr;
				CString str1, str2, str3;

				//ɾ����¼��SQL���
				//"delete from �û����� where �û��� ='"+nametype+"'";
				str1 = "delete from �û����� where �û��� ='";
				str2 = "'";
				str3 = str1 + nametype + str2;
				_variant_t RecordsAffected;
				try
				{
					//ִ��ɾ��SQL���
					hr = m_Conn.ExecuteSQL((_bstr_t)str3);
					if (SUCCEEDED(hr))
					{
						MessageBox(_T("ɾ����¼�ɹ���"));
					}
					else
						MessageBox(_T("ɾ����¼ʧ�ܣ�"));

				}
				catch (_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
					return;
				}
				//List�ؼ���Ҳɾ��������¼
				m_datalist.DeleteItem(sel);
				//InitialList();
				OnBnClickedBtnRefreshuser();
			}
		}
		else if (sel<0)
			MessageBox(_T("�б�����ѡ�м�¼��"));
	}

}


void CUserMangement::OnBnClickedBtnRefreshuser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_datalist.DeleteAllItems();
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from �û�����");
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("���"));
			//if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("�û���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�ԭ��"));
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}

//�Զ�����Ϣ����
LRESULT CUserMangement::InitialList1(WPARAM wParam, LPARAM lParam)
{
	m_datalist.DeleteAllItems();
	ADOConn  m_Conn;
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from �û�����");
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_datalist.InsertItem(i, 0);
			//	var=m_Conn.m_pRecordset->GetCollect(_T("���"));
			//	if(var.vt!=VT_NULL)
			m_datalist.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("�û���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 2, (_bstr_t)_T("****"));
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸���"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 6, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 7, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�����"));
			if (var.vt != VT_NULL)
				m_datalist.SetItemText(i, 8, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�޸�ԭ��"));
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
