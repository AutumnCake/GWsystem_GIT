// FocusSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "FocusSet.h"
#include "afxdialogex.h"


// CFocusSet 对话框
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


// CFocusSet 消息处理程序


BOOL CFocusSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_savepath = _T("C:\\Users\\wench\\Desktop");
	//m_savefile.SetWindowTextW(m_savepath);
	// TODO:  在此添加额外的初始化
	//连接数据库
	m_Database_AdminScanSet.OnInitADOAccess();
	ReadFromDatabase();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//从数据库读取参数
void CFocusSet::ReadFromDatabase()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X轴方向上聚焦的点数"));
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y轴方向上聚焦的点数"));
		GetDlgItem(IDC_EDIT_FOCUSNUMY)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("扫描区域X方向的长度"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("扫描区域Y方向的长度"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("手动存图路径"));
		m_savefile.SetWindowTextW(ReadString);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

//将数据写入数据库
void CFocusSet::WriteToDatabase()//将数据写入数据库
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
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->GetWindowText(CStrTem);//X轴方向上聚焦的点数
		para.Focusing_Num_X = _ttoi(CStrTem); 
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("X轴方向上聚焦的点数"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_FOCUSNUMY)->GetWindowText(CStrTem);
		para.Focusing_Num_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("Y轴方向上聚焦的点数"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->GetWindowText(CStrTem);
		para.Scanning_Aero_X = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("扫描区域X方向的长度"), _variant_t(CStrTem));



		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->GetWindowText(CStrTem);
		para.Scanning_Aero_Y = _ttoi(CStrTem);
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("扫描区域Y方向的长度"), _variant_t(CStrTem));

		m_savefile.GetWindowText(CStrTem);
		para.savepath = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("手动存图路径"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();



	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}



void CFocusSet::OnBnClickedBtnUpdatefocus()
{
	// TODO:  在此添加控件通知处理程序代码
	//1.将数据写入数据库
	WriteToDatabase();
	//2.将数据传到主界面
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_USER_FocusSet, (WPARAM)&para, NULL);

}


void CFocusSet::OnBnClickedBtnRefreshfocus()
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

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("X轴方向上聚焦的点数"));
		GetDlgItem(IDC_EDIT_FOCUSNUMX)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("Y轴方向上聚焦的点数"));
		GetDlgItem(IDC_EDIT_FOCUSNUMY)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("扫描区域X方向的长度"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_X)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("扫描区域Y方向的长度"));
		GetDlgItem(IDC_EDIT_Scanning_Aero_Y)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("手动存图路径"));
		m_savefile.SetWindowText(ReadString);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//默认数据应用，发送到主窗口
	OnBnClickedBtnUpdatefocus();

}
