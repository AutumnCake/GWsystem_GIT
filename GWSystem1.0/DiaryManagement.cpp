// DiaryManagement.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "DiaryManagement.h"
#include "afxdialogex.h"
#include "ADOConn.h"


// CDiaryManagement �Ի���

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


// CDiaryManagement ��Ϣ�������

BOOL CDiaryManagement::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString usertype;
	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();

	usertype = CApp->convert2;
	CString  str("��������Ա");
	//usertype.Replace("","");
	usertype.TrimRight();
	// TODO:  Add extra initialization here
	if (usertype.Compare(str) != 0)
	{
		//���ð�ť�ķ���
		GetDlgItem(IDC_BTN_DELETEDIARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLEARDIARY)->EnableWindow(FALSE);
	}
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from ��־����");
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	m_Conn.GetRecordSet(sql);
	m_diary.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_diary.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60, 0);
	m_diary.InsertColumn(1, _T("��ˮ��"), LVCFMT_LEFT, 160, 0);
	m_diary.InsertColumn(2, _T("�û�"), LVCFMT_LEFT, 140, 1);
	m_diary.InsertColumn(3, _T("����"), LVCFMT_LEFT, 140, 2);
	m_diary.InsertColumn(4, _T("����"), LVCFMT_LEFT, 100, 3);
	m_diary.InsertColumn(5, _T("ʱ��"), LVCFMT_LEFT, 80, 4);
	m_diary.InsertColumn(6, _T("��������"), LVCFMT_LEFT, 160, 5);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_diary.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("���"));
			//if(var.vt!=VT_NULL)
			m_diary.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("��ˮ��"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�û�"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 2, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("ʱ��"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("��������"));
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//ɾ����¼
void CDiaryManagement::OnBnClickedBtnDeletediary()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int sel = m_diary.GetSelectionMark();
	ADOConn m_Conn;
	if (sel >= 0 && AfxMessageBox(_T("�Ƿ�ɾ����"), MB_YESNO) == IDYES)
	{
		//�����û���List�ؼ��е�ѡ�������ͱ��
		CString dataid = m_diary.GetItemText(sel, 1);
		HRESULT hr;
		CString str1, str2, str3;

		//ɾ����¼��SQL���
		//"delete from yhgl where �û��� ='"+nametype+"'";
		str1 = "delete from ��־���� where ��ˮ�� ='";
		str2 = "'";
		str3 = str1 + dataid + str2;
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
		//m_Conn.m_pRecordset->Update();
		//List�ؼ���Ҳɾ��������¼
		m_diary.DeleteItem(sel);
		OninitialList();
	}
	else if (sel<0)
		MessageBox(_T("�б�����ѡ�м�¼��"));

}


//���
void CDiaryManagement::OnBnClickedBtnCleardiary()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ADOConn m_Conn;

	if (AfxMessageBox(_T("ȷ����գ�"), MB_YESNO) == IDYES)

	{
		CString xm;

		xm = "delete from ��־����"; // ������ݱ����
		_variant_t RecordsAffected;
		try
		{

			m_Conn.ExecuteSQL((_bstr_t)xm);//������ݱ�

			AfxMessageBox(_T("��ճɹ���"));

		}
		catch (_com_error *e)

		{

			AfxMessageBox(e->ErrorMessage());

			return;

		}
		OninitialList();//ˢ���б�
	}


}

//ȡ��
void CDiaryManagement::OnBnClickedBtnCanclediary()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

//�б�ˢ��
void CDiaryManagement::OninitialList(void)//�б�ˢ��
{
	m_diary.DeleteAllItems();//����б�
	_bstr_t sql;
	_variant_t var;
	sql = _T("Select * from ��־����");
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	m_Conn.GetRecordSet(sql);
	try
	{
		int i = 0;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_diary.InsertItem(i, 0);
			//var=m_Conn.m_pRecordset->GetCollect(_T("���"));
			//if(var.vt!=VT_NULL)
			m_diary.SetItemText(i, 0, (_bstr_t)(i + 1));
			var = m_Conn.m_pRecordset->GetCollect(_T("��ˮ��"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 1, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("�û�"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 2, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 3, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("����"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 4, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("ʱ��"));
			if (var.vt != VT_NULL)
				m_diary.SetItemText(i, 5, (_bstr_t)var);
			var = m_Conn.m_pRecordset->GetCollect(_T("��������"));
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
