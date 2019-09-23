// AdminFocusSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AdminFocusSet.h"
#include "afxdialogex.h"

static ALL_PAREMETER para;
// CAdminFocusSet 对话框

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


// CAdminFocusSet 消息处理程序





BOOL CAdminFocusSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//连接数据库
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CAdminFocusSet::OnBnClickedBtnDefaultmocusad()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦容差"));
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦大步长"));
		GetDlgItem(IDC_EDIT_LARGESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦中步长"));
		GetDlgItem(IDC_EDIT_MIDSTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦小步长"));
		GetDlgItem(IDC_EDIT_LITTLESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X视野宽度"));
		GetDlgItem(IDC_EDIT_10XWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X视野高度"));
		GetDlgItem(IDC_EDIT_10XHEIGHT)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		GetDlgItem(IDC_EDIT_MINCAAERO)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		GetDlgItem(IDC_EDIT_MINCANUM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_XPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAXCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_YPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAYCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("腐蚀的结构元大小"));
		GetDlgItem(IDC_EDIT_FUSHI)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("膨胀的结构元大小"));
		GetDlgItem(IDC_EDIT_PENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("腐蚀膨胀单元"));
		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Equal_Dis"));
		GetDlgItem(IDC_EDIT_FOCUSEQUALDIS)->SetWindowText(ReadString);




	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//默认数据应用，发送到主窗口
	OnBnClickedBtnUpdatefocusad();

}


void CAdminFocusSet::OnBnClickedBtnUpdatefocusad()
{
	// TODO:  在此添加控件通知处理程序代码
	//1.将数据写入数据库
	WriteToDatabase();
	//2.将数据传到主界面
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_AdminFocusSet, (WPARAM)&para, NULL);

}

//从数据库读取参数
void CAdminFocusSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦容差"));
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦大步长"));
		GetDlgItem(IDC_EDIT_LARGESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦中步长"));
		GetDlgItem(IDC_EDIT_MIDSTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("聚焦小步长"));
		GetDlgItem(IDC_EDIT_LITTLESTEP)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X视野宽度"));
		GetDlgItem(IDC_EDIT_10XWIDTH)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("10X视野高度"));
		GetDlgItem(IDC_EDIT_10XHEIGHT)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		GetDlgItem(IDC_EDIT_MINCAAERO)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		GetDlgItem(IDC_EDIT_MINCANUM)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_XPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAXCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("CA_YPos_Compensate"));
		GetDlgItem(IDC_EDIT_CAYCOMPENSATE)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("腐蚀的结构元大小"));
		GetDlgItem(IDC_EDIT_FUSHI)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("膨胀的结构元大小"));
		GetDlgItem(IDC_EDIT_PENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("腐蚀膨胀单元"));
		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Focus_Equal_Dis"));
		GetDlgItem(IDC_EDIT_FOCUSEQUALDIS)->SetWindowText(ReadString);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//将数据写入数据库
void CAdminFocusSet::WriteToDatabase()//将数据写入数据库
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
		GetDlgItem(IDC_EDIT_FOCALDOMAINRADIUS)->GetWindowText(CStrTem);
		para.Focal_Domain_Radius = _ttol(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("聚焦容差"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_LARGESTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[0] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("聚焦大步长"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MIDSTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[1] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("聚焦中步长"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_LITTLESTEP)->GetWindowText(CStrTem);
		para.Focusing_Step_Z[2] = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("聚焦小步长"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_10XWIDTH)->GetWindowText(CStrTem);
		para.View_Field_Width_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("10X视野宽度"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_10XHEIGHT)->GetWindowText(CStrTem);
		para.View_Field_Higth_10X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("10X视野高度"), _variant_t(CStrTem));

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
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("腐蚀的结构元大小"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_PENGZHANG)->GetWindowText(CStrTem);
		para.Dilate_element = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("膨胀的结构元大小"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FUSHIPENGZHANG)->GetWindowText(CStrTem);
		para.ErodeDilate_element = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("腐蚀膨胀单元"), _variant_t(CStrTem));

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