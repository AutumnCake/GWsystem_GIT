// UserBuild.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserBuild.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "UserMangement.h"
#include "ADOAccess.h"
#include "Diary.h"

// CUserBuild �Ի���

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


// CUserBuild ��Ϣ�������


BOOL CUserBuild::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//���ڳ�ʼ��ʱ�������û�����ӵ��ؼ���
	_bstr_t sql;
	UpdateData(false);

	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();
	CString user_type;

	builder = CApp->convert1;

	user_type = CApp->convert2;
	user_type.TrimRight();
	if (user_type == "��������Ա")
	{
		GetDlgItem(IDC_RADIO_ADMINISTER)->EnableWindow(FALSE);
	}
	m_builder.SetWindowTextW(builder);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CUserBuild::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			if (strlb == _T("��ͨ�û�"))

			{
				ADOAccess m_Conn;
				m_Conn.OnInitADOAccess();
				_bstr_t sql;
				_variant_t var;
				UpdateData(false);
				sql = _T("select * from �û����� ");
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
					m_Conn.m_pRecordset->PutCollect(_T("�û���"), _variant_t(strname));
					m_Conn.m_pRecordset->PutCollect(_T("����"), _variant_t(code));
					m_Conn.m_pRecordset->PutCollect(_T("����"), _variant_t(strlb));
					m_Conn.m_pRecordset->PutCollect(_T("������"), _variant_t(builder));
					m_Conn.m_pRecordset->PutCollect(_T("��������"), _variant_t(date));
					m_Conn.m_pRecordset->PutCollect(_T("�޸���"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�ԭ��"), _variant_t("��"));

					m_Conn.m_pRecordset->Update();
					AfxMessageBox(_T("��Ӽ�¼�ɹ���"));
					CString str1, str2, str3;
					str1 = "�����û���";
					str3 = "��";
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
				sql = _T("select * from �û����� ");
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
					m_Conn.m_pRecordset->PutCollect(_T("�û���"), _variant_t(strname));
					m_Conn.m_pRecordset->PutCollect(_T("����"), _variant_t(code));
					m_Conn.m_pRecordset->PutCollect(_T("����"), _variant_t(strlb));
					m_Conn.m_pRecordset->PutCollect(_T("������"), _variant_t(builder));
					m_Conn.m_pRecordset->PutCollect(_T("��������"), _variant_t(date));
					m_Conn.m_pRecordset->PutCollect(_T("�޸���"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t("��"));
					m_Conn.m_pRecordset->PutCollect(_T("�޸�ԭ��"), _variant_t("��"));
					m_Conn.m_pRecordset->Update();
					AfxMessageBox(_T("��Ӽ�¼�ɹ���"));
					m_Conn.ExitConnect();
					CString str1, str2, str3;
					str1 = "�����û���";
					str3 = "��";
					str2 = str1 + strname + str3;
					diary_u1.LogRecord(str2);

				}
				catch (_com_error e)
				{
					AfxMessageBox(e.Description());
				}


			}
			HWND   hnd = ::FindWindow(NULL, _T("�û�����"));

			::SendMessage(hnd, WM_USER_UPDATE, NULL, NULL);

			OnOK();

		}
		else

			AfxMessageBox(_T("���û����Ѿ����ڣ�"));
	}
	else if (strname.IsEmpty())
		AfxMessageBox(_T("�û�������Ϊ�գ�"));
	else
		AfxMessageBox(_T("���벻��Ϊ�գ�"));

}


void CUserBuild::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

//�ж��û��Ƿ�ѡ�����û����
bool CUserBuild::UserTypeSelect(CString &str)//�ж��û��Ƿ�ѡ�����û����
{

	CButton  *p_button1 = (CButton *)GetDlgItem(IDC_RADIO_NORMAL);
	CButton  *p_button2 = (CButton *)GetDlgItem(IDC_RADIO_SPECIALIST);
	CButton  *p_button3 = (CButton *)GetDlgItem(IDC_RADIO_ADMINISTER);
	if (p_button1->GetCheck() == 1)
	{
		str = _T("��ͨ�û�");

		return true;
	}

	else if (p_button2->GetCheck() == 1)
	{
		str = _T("ר��");
		return true;
	}

	else if (p_button3->GetCheck() == 1)
	{
		str = _T("��������Ա");
		return true;
	}
	else
	{
		AfxMessageBox(_T("��ѡ���û����"));
		return false;
	}
}

//�ж��û����Ƿ����
bool CUserBuild::IsNameExist(CString str)
{
	///������ж�


	CString strlb;
	UserTypeSelect(strlb);
	CString sql;
	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	sql = _T("select * from �û�����");
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
			if (strlb == _T("��ͨ�û�"))
			{

				if (m_Conn.m_pRecordset->GetCollect(_T("�û���")) == str)
				{
					return true;
				}
				m_Conn.m_pRecordset->MoveNext();

			}
			else
			{

				if (m_Conn.m_pRecordset->GetCollect(_T("�û���")) == str)
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
