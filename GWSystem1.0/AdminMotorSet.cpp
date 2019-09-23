// AdminMotorSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminMotorSet.h"
#include "afxdialogex.h"

static ALL_PAREMETER para;
// CAdminMotorSet �Ի���

IMPLEMENT_DYNAMIC(CAdminMotorSet, CDialogEx)

CAdminMotorSet::CAdminMotorSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdminMotorSet::IDD, pParent)
{

}

CAdminMotorSet::~CAdminMotorSet()
{
}

void CAdminMotorSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdminMotorSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFAULTMOTORAD, &CAdminMotorSet::OnBnClickedBtnDefaultmotorad)
	ON_BN_CLICKED(IDC_BTN_UPDATEMOTORAD, &CAdminMotorSet::OnBnClickedBtnUpdatemotorad)
END_MESSAGE_MAP()


// CAdminMotorSet ��Ϣ�������


BOOL CAdminMotorSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�������ݿ�
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAdminMotorSet::OnBnClickedBtnDefaultmotorad()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����1"));
		GetDlgItem(IDC_EDIT_OffsetX1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����2"));
		GetDlgItem(IDC_EDIT_OffsetX2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����3"));
		GetDlgItem(IDC_EDIT_OffsetY1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����4"));
		GetDlgItem(IDC_EDIT_OffsetY2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN����ID"));
		GetDlgItem(IDC_EDIT_CanID)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CANͨ��ת��ģ��ID"));
		GetDlgItem(IDC_EDIT_CanetPort)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Switch_Objective_ZPos"));
		GetDlgItem(IDC_EDIT_SWITCHZPOS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Pulse"));
		GetDlgItem(IDC_EDIT_OILPULSE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Vel"));
		GetDlgItem(IDC_EDIT_OILVEL)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Dis"));
		GetDlgItem(IDC_EDIT_OILDIS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Max_Limit"));
		GetDlgItem(IDC_EDIT_MAXLIM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Min_Limit"));
		GetDlgItem(IDC_EDIT_MINLIM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pulse_ratio"));
		GetDlgItem(IDC_EDIT_RATIO)->SetWindowText(ReadString);




	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//Ĭ������Ӧ�ã����͵�������
	OnBnClickedBtnUpdatemotorad();

}


void CAdminMotorSet::OnBnClickedBtnUpdatemotorad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1.������д�����ݿ�
	WriteToDatabase();
	//2.�����ݴ���������
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminMotorSet, (WPARAM)&para, NULL);

}

//�����ݿ��ȡ����
void CAdminMotorSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����1"));
		GetDlgItem(IDC_EDIT_OffsetX1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����2"));
		GetDlgItem(IDC_EDIT_OffsetX2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����3"));
		GetDlgItem(IDC_EDIT_OffsetY1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��λƫ����4"));
		GetDlgItem(IDC_EDIT_OffsetY2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN����ID"));
		GetDlgItem(IDC_EDIT_CanID)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CANͨ��ת��ģ��ID"));
		GetDlgItem(IDC_EDIT_CanetPort)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Switch_Objective_ZPos"));
		GetDlgItem(IDC_EDIT_SWITCHZPOS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Pulse"));
		GetDlgItem(IDC_EDIT_OILPULSE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Vel"));
		GetDlgItem(IDC_EDIT_OILVEL)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pump_Oil_Dis"));
		GetDlgItem(IDC_EDIT_OILDIS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Max_Limit"));
		GetDlgItem(IDC_EDIT_MAXLIM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Min_Limit"));
		GetDlgItem(IDC_EDIT_MINLIM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Pulse_ratio"));
		GetDlgItem(IDC_EDIT_RATIO)->SetWindowText(ReadString);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//������д�����ݿ�
void CAdminMotorSet::WriteToDatabase()//������д�����ݿ�
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
		vector<long>().swap(para.Offset_arr);
		CString CStrTem;
		GetDlgItem(IDC_EDIT_OffsetX1)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��λƫ����1"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetX2)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��λƫ����2"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetY1)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��λƫ����3"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetY2)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��λƫ����4"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CanID)->GetWindowText(CStrTem);
		USES_CONVERSION;
		para.CANET_ID = T2A(CStrTem.GetBuffer(0));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CAN����ID"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CanetPort)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CANͨ��ת��ģ��ID"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SWITCHZPOS)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Switch_Objective_ZPos"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OILPULSE)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Pump_Oil_Pulse"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OILVEL)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Pump_Oil_Vel"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OILDIS)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Pump_Oil_Dis"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MAXLIM)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Focus_Max_Limit"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINLIM)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Focus_Min_Limit"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_RATIO)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Pulse_ratio"), _variant_t(CStrTem));


		m_Database_AdminScanSet.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}
