// AdminScanSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminScanSet.h"
#include "afxdialogex.h"
#include"lucamapi.h"

static ALL_PAREMETER para;

// CAdminScanSet 对话框

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


// CAdminScanSet 消息处理程序


BOOL CAdminScanSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//----------设置组合框项目------------//
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




	//连接数据库
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAdminScanSet::OnBnClickedBtnDefaultscanad()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(false);
	sql = _T("select * from 默认参数表");

	//where 染色体图像分析结果数据表.[玻片编号]='" + SlideNumber + "'and 染色体图像分析结果数据表.[照片序数]='" + PicName + "' ");
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置1"));
		GetDlgItem(IDC_EDIT_WheelPos1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置2"));
		GetDlgItem(IDC_EDIT_WheelPos2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置3"));
		GetDlgItem(IDC_EDIT_WheelPos3)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置4"));
		GetDlgItem(IDC_EDIT_WheelPos4)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置5"));
		GetDlgItem(IDC_EDIT_WheelPos5)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置6"));
		GetDlgItem(IDC_EDIT_WheelPos6)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("运动偏差"));
		GetDlgItem(IDC_EDIT_MoveDeviation)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("一次扫描的玻片最大数量"));
		GetDlgItem(IDC_EDIT_SlideMaxNum)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X轴方向上扫描的点数"));
		GetDlgItem(IDC_EDIT_Scanning_Num_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y轴方向上扫描的点数"));
		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片实际长"));
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片实际宽"));
		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片间隔"));
		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("图像格式"));
		GetDlgItem(IDC_COMBO_FILEEXTENTION)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("存图的灯光"));
		//GetDlgItem(IDC_COMBO_CCM)->SetWindowText(ReadString);
		m_comboccm.SetCurSel(_ttoi(ReadString));


		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("存图的模式"));
		//GetDlgItem(IDC_COMBO_DM)->SetWindowText(ReadString);
		m_combodm.SetCurSel(_ttoi(ReadString));

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//默认数据应用，发送到主窗口
	OnBnClickedBtnUpdatescanad();

}


void CAdminScanSet::OnBnClickedBtnUpdatescanad()
{
	// TODO:  在此添加控件通知处理程序代码
	//1.将数据写入数据库
	WriteToDatabase();
	//2.将数据传到主界面
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminScanSet, (WPARAM)&para, NULL);

}

//从数据库读取参数
void CAdminScanSet::ReadFromDatabase()
{
	//tableIndex.Format(_T("%d"), Table_Index);
	UpdateData(false);
	sql = _T("select * from 用户参数表");

	//where 染色体图像分析结果数据表.[玻片编号]='" + SlideNumber + "'and 染色体图像分析结果数据表.[照片序数]='" + PicName + "' ");
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置1"));
		GetDlgItem(IDC_EDIT_WheelPos1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置2"));
		GetDlgItem(IDC_EDIT_WheelPos2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置3"));
		GetDlgItem(IDC_EDIT_WheelPos3)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置4"));
		GetDlgItem(IDC_EDIT_WheelPos4)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置5"));
		GetDlgItem(IDC_EDIT_WheelPos5)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置6"));
		GetDlgItem(IDC_EDIT_WheelPos6)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("运动偏差"));
		GetDlgItem(IDC_EDIT_MoveDeviation)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("一次扫描的玻片最大数量"));
		GetDlgItem(IDC_EDIT_SlideMaxNum)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X轴方向上扫描的点数"));
		GetDlgItem(IDC_EDIT_Scanning_Num_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y轴方向上扫描的点数"));
		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("存图的灯光"));
		m_comboccm.SetCurSel(_ttoi(ReadString));
		//GetDlgItem(IDC_COMBO_CCM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("存图的模式"));
		m_combodm.SetCurSel(_ttoi(ReadString));

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片实际长"));
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片实际宽"));
		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("玻片间隔"));
		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("图像格式"));
		GetDlgItem(IDC_COMBO_FILEEXTENTION)->SetWindowText(ReadString);



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//将数据写入数据库
void CAdminScanSet::WriteToDatabase()//将数据写入数据库
{

	//para->Nosepiece_Abs_Position.swap(para->Nosepiece_Abs_Position);

	UpdateData(false);
	sql = _T("select * from 用户参数表");
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
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置1"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos2)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置2"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos3)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置3"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos4)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置4"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos5)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置5"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_WheelPos6)->GetWindowText(CStrTem);
		para.Nosepiece_Abs_Position.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("物镜转盘的绝对位置6"), _variant_t(CStrTem));


		GetDlgItem(IDC_EDIT_MoveDeviation)->GetWindowText(CStrTem);
		para.Move_Deviation = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("运动偏差"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SlideMaxNum)->GetWindowText(CStrTem);
		para.Slide_Max_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("一次扫描的玻片最大数量"), _variant_t(CStrTem));




		GetDlgItem(IDC_EDIT_Scanning_Num_X)->GetWindowText(CStrTem);
		para.Scanning_Num_X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("X轴方向上扫描的点数"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_Scanning_Num_Y)->GetWindowText(CStrTem);
		para.Scanning_Num_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Y轴方向上扫描的点数"), _variant_t(CStrTem));
		
		
		GetDlgItem(IDC_EDIT_SLIDELENGTH)->GetWindowText(CStrTem);
		para.Slide_Size_X = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("玻片实际长"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SLIDEWIDTH)->GetWindowText(CStrTem);
		para.Slide_Size_y = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("玻片实际宽"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_SLIDEDISTANCE)->GetWindowText(CStrTem);
		para.Space_Slides = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("玻片间隔"), _variant_t(CStrTem));

		GetDlgItem(IDC_COMBO_FILEEXTENTION)->GetWindowText(CStrTem);
		para.csExtension = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("图像格式"), _variant_t(CStrTem));


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
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("存图的灯光"), _variant_t(para.CorrectionMatrix));


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

		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("存图的模式"), _variant_t(para.DemosaicMethod));

		m_Database_AdminScanSet.m_pRecordset->Update();

		



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}