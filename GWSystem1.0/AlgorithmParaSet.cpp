// AlgorithmParaSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "AlgorithmParaSet.h"
#include "afxdialogex.h"

//全局变量
static MNPARASET MNPara;
static CHROSOMEPARASET CHROPara;

// CAlgorithmParaSet 对话框

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


// CAlgorithmParaSet 消息处理程序


BOOL CAlgorithmParaSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//从数据库读取参数
void CAlgorithmParaSet::ReadFromDatabase()
{
	//读取微核参数
	UpdateData(FALSE);
	sql = _T("select * from 微核识别参数表");

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
		UpdateData(FALSE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞最小面积值"));
		GetDlgItem(IDC_EDIT_MINPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞缺陷率阈值"));
		GetDlgItem(IDC_EDIT_EIPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞延展率阈值"));
		GetDlgItem(IDC_EDIT_DRPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞核缺陷率阈值"));
		GetDlgItem(IDC_EDIT_EI_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞核延展率阈值"));
		GetDlgItem(IDC_EDIT_DR_XBHPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//读取染色体参数
	UpdateData(false);
	sql = _T("select * from 染色体识别参数表");
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

		CString ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体最大宽度"));
		GetDlgItem(IDC_EDIT_MAXWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体最小宽度"));
		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体宽度中值"));
		GetDlgItem(IDC_EDIT_MIDDLEPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("染色单体最小宽度"));
		GetDlgItem(IDC_EDIT_MINCHROPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

//static ALL_PAREMETER *para = new ALL_PAREMETER;

//将数据写入数据库
void CAlgorithmParaSet::WriteToDatabase()//将数据写入数据库
{

	//para->Nosepiece_Abs_Position.swap(para->Nosepiece_Abs_Position);
	//写入微核参数
	UpdateData(false);
	sql = _T("select * from 微核识别参数表");
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
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("细胞最小面积值"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_EIPARA)->GetWindowText(CStrTem);
		MNPara.EI = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("细胞缺陷率阈值"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_DRPARA)->GetWindowText(CStrTem);
		MNPara.DR = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("细胞延展率阈值"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_EI_XBHPARA)->GetWindowText(CStrTem);
		MNPara.EIXBL = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("细胞核缺陷率阈值"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_DR_XBHPARA)->GetWindowText(CStrTem);
		MNPara.DRXBL = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("细胞核延展率阈值"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//写入染色体参数
	UpdateData(false);
	sql = _T("select * from 染色体识别参数表");
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
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("预设的染色体最大宽度"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->GetWindowText(CStrTem);
		CHROPara.MINWIDTH = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("预设的染色体最小宽度"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MIDDLEPARA)->GetWindowText(CStrTem);
		CHROPara.MIDDLE = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("预设的染色体宽度中值"), _variant_t(CStrTem));

		GetDlgItem(IDC_EDIT_MINCHROPARA)->GetWindowText(CStrTem);
		CHROPara.SINGLEMIN = CStrTem;
		m_Database_AdminScanSet.m_pRecordset->PutCollect(_T("染色单体最小宽度"), _variant_t(CStrTem));

		m_Database_AdminScanSet.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}



}


void CAlgorithmParaSet::OnBnClickedBtnDefauteal()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(FALSE);
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

		UpdateData(FALSE);

		CString  ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞最小面积值"));
		GetDlgItem(IDC_EDIT_MINPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞缺陷率阈值"));
		GetDlgItem(IDC_EDIT_EIPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞延展率阈值"));
		GetDlgItem(IDC_EDIT_DRPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞核缺陷率阈值"));
		GetDlgItem(IDC_EDIT_EI_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("细胞核延展率阈值"));
		GetDlgItem(IDC_EDIT_DR_XBHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体最大宽度"));
		GetDlgItem(IDC_EDIT_MAXWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体最小宽度"));
		GetDlgItem(IDC_EDIT_MINWIDTHPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("预设的染色体宽度中值"));
		GetDlgItem(IDC_EDIT_MIDDLEPARA)->SetWindowText(ReadString);

		ReadString = m_Database_AdminScanSet.m_pRecordset->GetCollect(_T("染色单体最小宽度"));
		GetDlgItem(IDC_EDIT_MINCHROPARA)->SetWindowText(ReadString);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//默认数据应用，发送到主窗口
	OnBnClickedBtnUpdateal();

}


void CAlgorithmParaSet::OnBnClickedBtnUpdateal()
{
	// TODO:  在此添加控件通知处理程序代码
	WriteToDatabase();
	HWND   hwnd = ::FindWindow(NULL,_T("微核处理"));//调用消息处理函数刷新页面
	if (hwnd != NULL)
		::SendMessage(hwnd, WM_USER_MNParaSet, (WPARAM)&MNPara, NULL);
	else
		AfxMessageBox(_T("微核处理窗口未打开!"));
	HWND   hnd = ::FindWindow(NULL, _T("染色体处理"));//调用消息处理函数刷新页面
	if (hwnd != NULL)
		::SendMessage(hnd, WM_USER_CHROParaSet, (WPARAM)&CHROPara, NULL);
	else
		AfxMessageBox(_T("染色体处理窗口未打开!"));


}
