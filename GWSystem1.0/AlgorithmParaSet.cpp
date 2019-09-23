// AlgorithmParaSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AlgorithmParaSet.h"
#include "afxdialogex.h"

//ȫ�ֱ���
static MNPARASET MNPara;
static CHROSOMEPARASET CHROPara;

// CAlgorithmParaSet �Ի���

IMPLEMENT_DYNAMIC(CAlgorithmParaSet, CDialogEx)

CAlgorithmParaSet::CAlgorithmParaSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlgorithmParaSet::IDD, pParent)
{

}

CAlgorithmParaSet::~CAlgorithmParaSet()
{
}

void CAlgorithmParaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlgorithmParaSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFAUTEAL, &CAlgorithmParaSet::OnBnClickedBtnDefauteal)
	ON_BN_CLICKED(IDC_BTN_UPDATEAL, &CAlgorithmParaSet::OnBnClickedBtnUpdateal)
END_MESSAGE_MAP()


// CAlgorithmParaSet ��Ϣ�������


BOOL CAlgorithmParaSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�����ݿ��ȡ����
void CAlgorithmParaSet::ReadFromDatabase()
{
	//��ȡ΢�˲���
	UpdateData(FALSE);
	sql = _T("select * from ΢��ʶ�������");

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
		UpdateData(FALSE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����С���ֵ"));
		GetDlgItem(IDC_EDIT_MINPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ��ȱ������ֵ"));
		GetDlgItem(IDC_EDIT_EIPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����չ����ֵ"));
		GetDlgItem(IDC_EDIT_DRPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����ȱ������ֵ"));
		GetDlgItem(IDC_EDIT_EI_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ������չ����ֵ"));
		GetDlgItem(IDC_EDIT_DR_XBHPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//��ȡȾɫ�����
	UpdateData(false);
	sql = _T("select * from Ⱦɫ��ʶ�������");
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

		UpdateData(FALSE);

		CString ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ�������"));
		GetDlgItem(IDC_EDIT_MAXWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ����С���"));
		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ������ֵ"));
		GetDlgItem(IDC_EDIT_MIDDLEPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ⱦɫ������С���"));
		GetDlgItem(IDC_EDIT_MINCHROPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

//static ALL_PAREMETER *para = new ALL_PAREMETER;

//������д�����ݿ�
void CAlgorithmParaSet::WriteToDatabase()//������д�����ݿ�
{

	//para->Nosepiece_Abs_Position.swap(para->Nosepiece_Abs_Position);
	//д��΢�˲���
	UpdateData(false);
	sql = _T("select * from ΢��ʶ�������");
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
		GetDlgItem(IDC_EDIT_MINPARA)->GetWindowText(CStrTem);
		MNPara.MIN = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ϸ����С���ֵ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_EIPARA)->GetWindowText(CStrTem);
		MNPara.EI = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ϸ��ȱ������ֵ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_DRPARA)->GetWindowText(CStrTem);
		MNPara.DR = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ϸ����չ����ֵ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_EI_XBHPARA)->GetWindowText(CStrTem);
		MNPara.EIXBL = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ϸ����ȱ������ֵ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_DR_XBHPARA)->GetWindowText(CStrTem);
		MNPara.DRXBL = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ϸ������չ����ֵ"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//д��Ⱦɫ�����
	UpdateData(false);
	sql = _T("select * from Ⱦɫ��ʶ�������");
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

		CString CStrTem;
		GetDlgItem(IDC_EDIT_MAXWIDTHPARA)->GetWindowText(CStrTem);
		CHROPara.MAXWIDTH = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Ԥ���Ⱦɫ�������"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->GetWindowText(CStrTem);
		CHROPara.MINWIDTH = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Ԥ���Ⱦɫ����С���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MIDDLEPARA)->GetWindowText(CStrTem);
		CHROPara.MIDDLE = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Ԥ���Ⱦɫ������ֵ"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINCHROPARA)->GetWindowText(CStrTem);
		CHROPara.SINGLEMIN = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Ⱦɫ������С���"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}



}


void CAlgorithmParaSet::OnBnClickedBtnDefauteal()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
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

		UpdateData(FALSE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����С���ֵ"));
		GetDlgItem(IDC_EDIT_MINPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ��ȱ������ֵ"));
		GetDlgItem(IDC_EDIT_EIPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����չ����ֵ"));
		GetDlgItem(IDC_EDIT_DRPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ����ȱ������ֵ"));
		GetDlgItem(IDC_EDIT_EI_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ϸ������չ����ֵ"));
		GetDlgItem(IDC_EDIT_DR_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ�������"));
		GetDlgItem(IDC_EDIT_MAXWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ����С���"));
		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ԥ���Ⱦɫ������ֵ"));
		GetDlgItem(IDC_EDIT_MIDDLEPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Ⱦɫ������С���"));
		GetDlgItem(IDC_EDIT_MINCHROPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//Ĭ������Ӧ�ã����͵�������
	OnBnClickedBtnUpdateal();

}


void CAlgorithmParaSet::OnBnClickedBtnUpdateal()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WriteToDatabase();
	HWND   hwnd = ::FindWindow(NULL,_T("΢�˴���"));//������Ϣ������ˢ��ҳ��
	if (hwnd != NULL)
		::SendMessage(hwnd, WM_USER_MNParaSet, (WPARAM)&MNPara, NULL);
	else
		AfxMessageBox(_T("΢�˴�����δ��!"));
	HWND   hnd = ::FindWindow(NULL, _T("Ⱦɫ�崦��"));//������Ϣ������ˢ��ҳ��
	if (hwnd != NULL)
		::SendMessage(hnd, WM_USER_CHROParaSet, (WPARAM)&CHROPara, NULL);
	else
		AfxMessageBox(_T("Ⱦɫ�崦����δ��!"));


}
