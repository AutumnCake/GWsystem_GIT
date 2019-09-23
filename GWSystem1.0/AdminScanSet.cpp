// AdminScanSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminScanSet.h"
#include "afxdialogex.h"
#include"lucamapi.h"

static ALL_PAREMETER para;

// CAdminScanSet �Ի���

IMPLEMENT_DYNAMIC(CAdminScanSet, CDialogEx)

CAdminScanSet::CAdminScanSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdminScanSet::IDD, pParent)
{

}

CAdminScanSet::~CAdminScanSet()
{
}

void CAdminScanSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FILEEXTENTION, m_combofileextention);
	DDX_Control(pDX, IDC_COMBO_CCM, m_comboccm);
	DDX_Control(pDX, IDC_COMBO_DM, m_combodm);
	DDX_Control(pDX, IDC_EDIT_SLIDELENGTH, m_slidelength);
	DDX_Control(pDX, IDC_EDIT_SLIDEWIDTH, m_slidewidth);
	DDX_Control(pDX, IDC_EDIT_SLIDEDISTANCE, m_slidedistance);
}


BEGIN_MESSAGE_MAP(CAdminScanSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFAULTSCANAD, &CAdminScanSet::OnBnClickedBtnDefaultscanad)
	ON_BN_CLICKED(IDC_BTN_UPDATESCANAD, &CAdminScanSet::OnBnClickedBtnUpdatescanad)
END_MESSAGE_MAP()


// CAdminScanSet ��Ϣ�������


BOOL CAdminScanSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//----------������Ͽ���Ŀ------------//
	m_combofileextention.AddString(_T(".bmp"));
	m_combofileextention.AddString(_T(".jpg"));
	m_combofileextention.AddString(_T(".png"));
	m_combofileextention.AddString(_T(".raw"));
	m_combofileextention.AddString(_T(".tif"));
	m_combofileextention.SetCurSel(0);

	m_comboccm.AddString(_T("LUCAM_CM_NONE"));
	m_comboccm.AddString(_T("LUCAM_CM_FLUORESCENT"));
	m_comboccm.AddString(_T("LUCAM_CM_DAYLIGHT"));
	m_comboccm.AddString(_T("LUCAM_CM_INCANDESCENT"));
	m_comboccm.AddString(_T("LUCAM_CM_XENON_FLASH"));
	m_comboccm.AddString(_T("LUCAM_CM_HALOGEN"));
	m_comboccm.AddString(_T("LUCAM_CM_LED"));
	m_comboccm.AddString(_T("LUCAM_CM_IDENTITY"));
	m_comboccm.AddString(_T("LUCAM_CM_CUSTOM"));
	m_comboccm.SetCurSel(6);


	m_combodm.AddString(_T("LUCAM_DM_NONE"));
	m_combodm.AddString(_T("LUCAM_DM_FAST"));
	m_combodm.AddString(_T("LUCAM_DM_HIGH_QUALITY"));
	m_combodm.AddString(_T("LUCAM_DM_HIGHER_QUALITY"));
	m_combodm.AddString(_T("LUCAM_DM_SIMPLE"));
	m_combodm.SetCurSel(0);




	//�������ݿ�
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAdminScanSet::OnBnClickedBtnDefaultscanad()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��1"));
		GetDlgItem(IDC_EDIT_WheelPos1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��2"));
		GetDlgItem(IDC_EDIT_WheelPos2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��3"));
		GetDlgItem(IDC_EDIT_WheelPos3)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��4"));
		GetDlgItem(IDC_EDIT_WheelPos4)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��5"));
		GetDlgItem(IDC_EDIT_WheelPos5)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��6"));
		GetDlgItem(IDC_EDIT_WheelPos6)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�˶�ƫ��"));
		GetDlgItem(IDC_EDIT_MoveDeviation)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("һ��ɨ��Ĳ�Ƭ�������"));
		GetDlgItem(IDC_EDIT_SlideMaxNum)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X�᷽����ɨ��ĵ���"));
		GetDlgItem(IDC_EDIT_Scanning_Num_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y�᷽����ɨ��ĵ���"));
		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭʵ�ʳ�"));
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭʵ�ʿ�"));
		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭ���"));
		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ͼ���ʽ"));
		GetDlgItem(IDC_COMBO_FILEEXTENTION)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ͼ�ĵƹ�"));
		//GetDlgItem(IDC_COMBO_CCM)->SetWindowText(ReadString);
		m_comboccm.SetCurSel(_ttoi(ReadString));


		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ͼ��ģʽ"));
		//GetDlgItem(IDC_COMBO_DM)->SetWindowText(ReadString);
		m_combodm.SetCurSel(_ttoi(ReadString));

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//Ĭ������Ӧ�ã����͵�������
	OnBnClickedBtnUpdatescanad();

}


void CAdminScanSet::OnBnClickedBtnUpdatescanad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//1.������д�����ݿ�
	WriteToDatabase();
	//2.�����ݴ���������
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminScanSet, (WPARAM)&para, NULL);

}

//�����ݿ��ȡ����
void CAdminScanSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��1"));
		GetDlgItem(IDC_EDIT_WheelPos1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��2"));
		GetDlgItem(IDC_EDIT_WheelPos2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��3"));
		GetDlgItem(IDC_EDIT_WheelPos3)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��4"));
		GetDlgItem(IDC_EDIT_WheelPos4)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��5"));
		GetDlgItem(IDC_EDIT_WheelPos5)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��6"));
		GetDlgItem(IDC_EDIT_WheelPos6)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("�˶�ƫ��"));
		GetDlgItem(IDC_EDIT_MoveDeviation)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("һ��ɨ��Ĳ�Ƭ�������"));
		GetDlgItem(IDC_EDIT_SlideMaxNum)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X�᷽����ɨ��ĵ���"));
		GetDlgItem(IDC_EDIT_Scanning_Num_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y�᷽����ɨ��ĵ���"));
		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ͼ�ĵƹ�"));
		m_comboccm.SetCurSel(_ttoi(ReadString));
		//GetDlgItem(IDC_COMBO_CCM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��ͼ��ģʽ"));
		m_combodm.SetCurSel(_ttoi(ReadString));

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭʵ�ʳ�"));
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭʵ�ʿ�"));
		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("��Ƭ���"));
		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("ͼ���ʽ"));
		GetDlgItem(IDC_COMBO_FILEEXTENTION)->SetWindowText(ReadString);



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//������д�����ݿ�
void CAdminScanSet::WriteToDatabase()//������д�����ݿ�
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
		GetDlgItem(IDC_EDIT_WheelPos1)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��1"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos2)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��2"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos3)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��3"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos4)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��4"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos5)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��5"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos6)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�ﾵת�̵ľ���λ��6"), _variant_t(CStrTem));


		GetDlgItem(IDC_EDIT_MoveDeviation)->GetWindowText(CStrTem);
		para.Move_Deviation = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("�˶�ƫ��"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SlideMaxNum)->GetWindowText(CStrTem);
		para.Slide_Max_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("һ��ɨ��Ĳ�Ƭ�������"), _variant_t(CStrTem));




		GetDlgItem(IDC_EDIT_Scanning_Num_X)->GetWindowText(CStrTem);
		para.Scanning_Num_X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("X�᷽����ɨ��ĵ���"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->GetWindowText(CStrTem);
		para.Scanning_Num_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Y�᷽����ɨ��ĵ���"), _variant_t(CStrTem));
		
		
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->GetWindowText(CStrTem);
		para.Slide_Size_X = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��Ƭʵ�ʳ�"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->GetWindowText(CStrTem);
		para.Slide_Size_y = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��Ƭʵ�ʿ�"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->GetWindowText(CStrTem);
		para.Space_Slides = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��Ƭ���"), _variant_t(CStrTem));

		GetDlgItem(IDC_COMBO_FILEEXTENTION)->GetWindowText(CStrTem);
		para.csExtension = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("ͼ���ʽ"), _variant_t(CStrTem));


		switch (m_comboccm.GetCurSel())
		{
		case 0:
			para.CorrectionMatrix = LUCAM_CM_NONE;
			break;

		case 1:
			para.CorrectionMatrix = LUCAM_CM_FLUORESCENT;
			break;

		case 2:
			para.CorrectionMatrix = LUCAM_CM_DAYLIGHT;
			break;

		case 3:
			para.CorrectionMatrix = LUCAM_CM_INCANDESCENT;
			break;

		case 4:
			para.CorrectionMatrix = LUCAM_CM_XENON_FLASH;
			break;

		case 5:
			para.CorrectionMatrix = LUCAM_CM_HALOGEN;
			break;

		case 6:
			para.CorrectionMatrix = LUCAM_CM_LED;
			break;

		case 7:
			para.CorrectionMatrix = LUCAM_CM_IDENTITY;
			break;

		case 8:
			para.CorrectionMatrix = LUCAM_CM_CUSTOM;
			break;

		default:
			break;
		}
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��ͼ�ĵƹ�"), _variant_t(para.CorrectionMatrix));


		switch (m_combodm.GetCurSel())
		{
		case 0:
			para.DemosaicMethod = LUCAM_DM_NONE;
			break;

		case 1:
			para.DemosaicMethod = LUCAM_DM_FAST;
			break;

		case 2:
			para.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
			break;

		case 3:
			para.DemosaicMethod = LUCAM_DM_HIGHER_QUALITY;
			break;

		case 4:
			para.DemosaicMethod = LUCAM_DM_SIMPLE;
			break;
		default:
			break;
		}

		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("��ͼ��ģʽ"), _variant_t(para.DemosaicMethod));

		m_Database_AdminScanSet.m_pRecordset->Update();

		



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}