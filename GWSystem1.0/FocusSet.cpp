// FocusSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "FocusSet.h"
#include "afxdialogex.h"


// CFocusSet �Ի���
static ALL_PAREMETER para;

IMPLEMENT_DYNAMIC(CFocusSet, CDialogEx)

CFocusSet::CFocusSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFocusSet::IDD, pParent)
{

}

CFocusSet::~CFocusSet()
{
}

void CFocusSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SAVE_DIR, m_savefile);
}


BEGIN_MESSAGE_MAP(CFocusSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_UPDATEFOCUS, &CFocusSet::OnBnClickedBtnUpdatefocus)
	ON_BN_CLICKED(IDC_BTN_REFRESHFOCUS, &CFocusSet::OnBnClickedBtnRefreshfocus)
END_MESSAGE_MAP()


// CFocusSet ��Ϣ�������


BOOL CFocusSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_savepath = _T("C:\\Users\\wench\\Desktop");
	//m_savefile.SetWindowTextW(m_savepath);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�������ݿ�
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�����ݿ��ȡ����
void CFocusSet::ReadFromDatabase()
{
	//tableIndex.Format(_T("%d"), Table_Index);
	UpdateData(false);
	sql = _T("select * from �û�������");

	//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
	try
	{
		m_Database_AdminScanSet.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	try
	{

		UpdateData(TRUE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X�᷽���Ͼ۽��ĵ���"));
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y�᷽���Ͼ۽��ĵ���"));
		GetDlgItem(IDC_EDIT_FOCUSNUMY)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ɨ������X����ĳ���"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ɨ������Y����ĳ���"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ֶ���ͼ·��"));
		m_savefile.SetWindowTextW(ReadString);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//������д�����ݿ�
void CFocusSet::WriteToDatabase()//������д�����ݿ�
{

	//para->Nosepiece_Abs_Position.swap(para->Nosepiece_Abs_Position);

	UpdateData(false);
	sql = _T("select * from �û�������");
	try
	{
		m_Database_AdminScanSet.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	try
	{

		//m_Conn.m_pRecordset->AddNew();
		CString CStrTem;
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->GetWindowText(CStrTem);//X�᷽���Ͼ۽��ĵ���
		para.Focusing_Num_X = _ttoi(CStrTem); 
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("X�᷽���Ͼ۽��ĵ���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FOCUSNUMY)->GetWindowText(CStrTem);
		para.Focusing_Num_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Y�᷽���Ͼ۽��ĵ���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->GetWindowText(CStrTem);
		para.Scanning_Aero_X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ɨ������X����ĳ���"), _variant_t(CStrTem));



		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->GetWindowText(CStrTem);
		para.Scanning_Aero_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ɨ������Y����ĳ���"), _variant_t(CStrTem));

		m_savefile.GetWindowText(CStrTem);
		para.savepath = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ֶ���ͼ·��"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}



void CFocusSet::OnBnClickedBtnUpdatefocus()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1.������д�����ݿ�
	WriteToDatabase();
	//2.�����ݴ���������
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_FocusSet, (WPARAM)&para, NULL);

}


void CFocusSet::OnBnClickedBtnRefreshfocus()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(false);
	sql = _T("select * from Ĭ�ϲ�����");

	//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
	try
	{
		m_Database_AdminScanSet.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	try
	{

		UpdateData(TRUE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X�᷽���Ͼ۽��ĵ���"));
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y�᷽���Ͼ۽��ĵ���"));
		GetDlgItem(IDC_EDIT_FOCUSNUMY)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ɨ������X����ĳ���"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ɨ������Y����ĳ���"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ֶ���ͼ·��"));
		m_savefile.SetWindowText(ReadString);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//Ĭ������Ӧ�ã����͵�������
	OnBnClickedBtnUpdatefocus();

}
