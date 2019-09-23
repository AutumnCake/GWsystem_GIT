// AdminFocusSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminFocusSet.h"
#include "afxdialogex.h"

static ALL_PAREMETER para;
// CAdminFocusSet �Ի���

IMPLEMENT_DYNAMIC(CAdminFocusSet, CDialogEx)

CAdminFocusSet::CAdminFocusSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdminFocusSet::IDD, pParent)
{

}

CAdminFocusSet::~CAdminFocusSet()
{
}

void CAdminFocusSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminFocusSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFAULTMOCUSAD, &CAdminFocusSet::OnBnClickedBtnDefaultmocusad)
	ON_BN_CLICKED(IDC_BTN_UPDATEFOCUSAD, &CAdminFocusSet::OnBnClickedBtnUpdatefocusad)
END_MESSAGE_MAP()


// CAdminFocusSet ��Ϣ�������





BOOL CAdminFocusSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�������ݿ�
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAdminFocusSet::OnBnClickedBtnDefaultmocusad()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��ݲ�"));
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��󲽳�"));
		GetDlgItem(IDC_EDIT_LARGESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��в���"));
		GetDlgItem(IDC_EDIT_MIDSTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽�С����"));
		GetDlgItem(IDC_EDIT_LITTLESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X��Ұ���"));
		GetDlgItem(IDC_EDIT_10XWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X��Ұ�߶�"));
		GetDlgItem(IDC_EDIT_10XHEIGHT)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		GetDlgItem(IDC_EDIT_MINCAAERO)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		GetDlgItem(IDC_EDIT_MINCANUM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_XPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAXCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_YPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAYCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ʴ�ĽṹԪ��С"));
		GetDlgItem(IDC_EDIT_FUSHI)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("���͵ĽṹԪ��С"));
		GetDlgItem(IDC_EDIT_PENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ʴ���͵�Ԫ"));
		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Equal_Dis"));
		GetDlgItem(IDC_EDIT_FOCUSEQUALDIS)->SetWindowText(ReadString);




	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//Ĭ������Ӧ�ã����͵�������
	OnBnClickedBtnUpdatefocusad();

}


void CAdminFocusSet::OnBnClickedBtnUpdatefocusad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1.������д�����ݿ�
	WriteToDatabase();
	//2.�����ݴ���������
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminFocusSet, (WPARAM)&para, NULL);

}

//�����ݿ��ȡ����
void CAdminFocusSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��ݲ�"));
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��󲽳�"));
		GetDlgItem(IDC_EDIT_LARGESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽��в���"));
		GetDlgItem(IDC_EDIT_MIDSTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�۽�С����"));
		GetDlgItem(IDC_EDIT_LITTLESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X��Ұ���"));
		GetDlgItem(IDC_EDIT_10XWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X��Ұ�߶�"));
		GetDlgItem(IDC_EDIT_10XHEIGHT)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		GetDlgItem(IDC_EDIT_MINCAAERO)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		GetDlgItem(IDC_EDIT_MINCANUM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_XPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAXCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_YPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAYCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ʴ�ĽṹԪ��С"));
		GetDlgItem(IDC_EDIT_FUSHI)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("���͵ĽṹԪ��С"));
		GetDlgItem(IDC_EDIT_PENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ʴ���͵�Ԫ"));
		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Equal_Dis"));
		GetDlgItem(IDC_EDIT_FOCUSEQUALDIS)->SetWindowText(ReadString);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//������д�����ݿ�
void CAdminFocusSet::WriteToDatabase()//������д�����ݿ�
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
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->GetWindowText(CStrTem);
		para.Focal_Domain_Radius = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�۽��ݲ�"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_LARGESTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[0] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�۽��󲽳�"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MIDSTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[1] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�۽��в���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_LITTLESTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[2] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�۽�С����"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_10XWIDTH)->GetWindowText(CStrTem);
		para.View_Field_Width_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("10X��Ұ���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_10XHEIGHT)->GetWindowText(CStrTem);
		para.View_Field_Higth_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("10X��Ұ�߶�"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINCAAERO)->GetWindowText(CStrTem);
		para.Min_CA_Aero_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Min_CA_Aero_10X"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINCANUM)->GetWindowText(CStrTem);
		para.Min_CA_Num_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Min_CA_Num_10X"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CAXCOMPENSATE)->GetWindowText(CStrTem);
		para.CA_XPos_Compensate = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CA_XPos_Compensate"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CAYCOMPENSATE)->GetWindowText(CStrTem);
		para.CA_YPos_Compensate = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CA_YPos_Compensate"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FUSHI)->GetWindowText(CStrTem);
		para.Erode_element = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��ʴ�ĽṹԪ��С"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_PENGZHANG)->GetWindowText(CStrTem);
		para.Dilate_element = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("���͵ĽṹԪ��С"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->GetWindowText(CStrTem);
		para.ErodeDilate_element = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��ʴ���͵�Ԫ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FOCUSEQUALDIS)->GetWindowText(CStrTem);
		para.Focus_Equal_Dis = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Focus_Equal_Dis"), _variant_t(CStrTem));




		m_Database_AdminScanSet.m_pRecordset->Update();



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}