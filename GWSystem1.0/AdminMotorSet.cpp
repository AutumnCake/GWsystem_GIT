// AdminMotorSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminMotorSet.h"
#include "afxdialogex.h"

static ALL_PAREMETER para;
// CAdminMotorSet 对话框

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


// CAdminMotorSet 消息处理程序


BOOL CAdminMotorSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//连接数据库
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAdminMotorSet::OnBnClickedBtnDefaultmotorad()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量1"));
		GetDlgItem(IDC_EDIT_OffsetX1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量2"));
		GetDlgItem(IDC_EDIT_OffsetX2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量3"));
		GetDlgItem(IDC_EDIT_OffsetY1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量4"));
		GetDlgItem(IDC_EDIT_OffsetY2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN网关ID"));
		GetDlgItem(IDC_EDIT_CanID)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN通信转换模块ID"));
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
	//默认数据应用，发送到主窗口
	OnBnClickedBtnUpdatemotorad();

}


void CAdminMotorSet::OnBnClickedBtnUpdatemotorad()
{
	// TODO:  在此添加控件通知处理程序代码
	//1.将数据写入数据库
	WriteToDatabase();
	//2.将数据传到主界面
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminMotorSet, (WPARAM)&para, NULL);

}

//从数据库读取参数
void CAdminMotorSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量1"));
		GetDlgItem(IDC_EDIT_OffsetX1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量2"));
		GetDlgItem(IDC_EDIT_OffsetX2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量3"));
		GetDlgItem(IDC_EDIT_OffsetY1)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("复位偏移量4"));
		GetDlgItem(IDC_EDIT_OffsetY2)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN网关ID"));
		GetDlgItem(IDC_EDIT_CanID)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CAN通信转换模块ID"));
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

//将数据写入数据库
void CAdminMotorSet::WriteToDatabase()//将数据写入数据库
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
		vector<long>().swap(para.Offset_arr);
		CString CStrTem;
		GetDlgItem(IDC_EDIT_OffsetX1)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("复位偏移量1"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetX2)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("复位偏移量2"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetY1)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("复位偏移量3"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_OffsetY2)->GetWindowText(CStrTem);
		para.Offset_arr.push_back(_ttol(CStrTem));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("复位偏移量4"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CanID)->GetWindowText(CStrTem);
		USES_CONVERSION;
		para.CANET_ID = T2A(CStrTem.GetBuffer(0));
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CAN网关ID"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_CanetPort)->GetWindowText(CStrTem);
		para.CANET_Port_Num = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("CAN通信转换模块ID"), _variant_t(CStrTem));

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
