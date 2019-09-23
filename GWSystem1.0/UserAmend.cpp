// UserAmend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "UserAmend.h"
#include "afxdialogex.h"
#include "ADOConn.h"
#include "Diary.h"

// CUserAmend �Ի���

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


// CUserAmend ��Ϣ�������

BOOL CUserAmend::OnInitDialog()
{

	CDialog::OnInitDialog();

	usertype = theApp.convert2;
	usertype.TrimRight();
	((CComboBox*)GetDlgItem(IDC_COMBO_REASON))->AddString(_T("�û�Ȩ�޲���"));
	((CComboBox*)GetDlgItem(IDC_COMBO_REASON))->AddString(_T("������Ҫ����"));

	GetUserInformation(bh);
	if (userinfo_type == "��ͨ�û�")
	{
		((CButton *)GetDlgItem(IDC_RADIO_PT))->SetCheck(TRUE);
	}
	else if (userinfo_type == "ר��")
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
	//sql="select * from �û����� where �û���='"+str1+"'";
	str1 = "select * from �û����� where �û���='";
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
		userinfo_type = m_Conn.m_pRecordset->GetCollect(_T("����"));
		userinfo_type.TrimRight();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}


void CUserAmend::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strlb, strzt, stryy, strsj, strnr, strbz, strxg, strname;
	CString str1, str2, str3, str4;
	m_reason.GetWindowText(stryy);//yy=ԭ��
	m_time.GetWindowText(strsj);//sj=ʱ��
	m_context.GetWindowText(strnr);//nr=����
	m_amender.GetWindowText(strxg);//xg=�޸���
	m_yhm.GetWindowText(strname);//yhm=�û���
	CButton  *p_button1 = (CButton *)GetDlgItem(IDC_RADIO_PT);
	if (p_button1->GetCheck() == 1)
	{
		strlb = _T("��ͨ�û�");
	}
	CButton  *p_button2 = (CButton *)GetDlgItem(IDC_RADIO_ZJ);
	if (p_button2->GetCheck() == 1)
	{
		strlb = _T("ר��");
	}


	ADOConn m_Conn;
	m_Conn.OnInitADOConn();
	CString string1, string2, string3;
	//ɾ����¼��SQL���
	//str1="select * from �û����� where �û��� ='"+bh+"'";
	string1 = "select * from �û����� where �û��� ='";
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
			if (!strlb.IsEmpty())//������޸ġ����𡱣��򱣳�ԭ��ֵ
			{
				//str="update zhbgl set ���� ='"+strlb+"' where �û���='"+bh+"'";
				str1 = "update �û����� set ���� ='";
				str2 = "' where �û���='";
				str3 = "'";
				str4 = str1 + strlb + str2 + bh + str3;
				m_Conn.ExecuteSQL((_bstr_t)str4);

			}
			//д��
			m_Conn.m_pRecordset->PutCollect(_T("�޸���"), _variant_t(strxg));
			m_Conn.m_pRecordset->PutCollect(_T("�޸�ԭ��"), _variant_t(stryy));
			m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t(strsj));
			m_Conn.m_pRecordset->PutCollect(_T("�޸�����"), _variant_t(user_amendcontext));
			m_Conn.m_pRecordset->Update();
			CString tempstr1, tempstr2, tempstr3;
			tempstr1 = "�޸��û���";
			tempstr3 = "��";
			tempstr2 = tempstr1 + strname + tempstr3;
			diary_u2.LogRecord(tempstr2);

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.ErrorMessage());
		}
		m_Conn.ExitConnect();

		HWND   hnd = ::FindWindow(NULL, _T("�û�����"));//������Ϣ������ˢ��ҳ��

		::SendMessage(hnd, WM_USER_UPDATE, NULL, NULL);

		OnOK();
	}
	else if (stryy.IsEmpty())
		AfxMessageBox(_T("�޸�ԭ����Ϊ�գ�"));

}


void CUserAmend::OnBnClickedResercode()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_yhm.GetWindowText(username);
	codereset = new CCodeReset(this);
	codereset->Create(IDD_CODERESET, NULL);
	codereset->ShowWindow(SW_SHOW);
	codereset->user_name = username;

}


void CUserAmend::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CUserAmend::OnBnClickedRadioPt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (userinfo_type != "��ͨ�û�"&&tempstr1 == false)
	{
		user_amendcontext += "�޸��û����";
		m_context.SetWindowText(user_amendcontext);
		tempstr1 = true;

	}
}


void CUserAmend::OnBnClickedRadioZj()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (userinfo_type != "ר��"&&tempstr1 == false)
	{
		user_amendcontext += "�޸��û����";
		m_context.SetWindowText(user_amendcontext);
		tempstr1 = true;

	}
}
