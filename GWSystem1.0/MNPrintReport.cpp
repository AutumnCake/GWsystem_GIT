// MNPrintReport.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "MNPrintReport.h"
#include "afxdialogex.h"
#include"ReadAndWriteForAccess.h"

//报表
#include "IllusionExcelFile.h"



// CMNPrintReport 对话框
CReadAndWriteForAccess ReadMicro;

IMPLEMENT_DYNAMIC(CMNPrintReport, CDialogEx)

CMNPrintReport::CMNPrintReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMNPrintReport::IDD, pParent)
{
	m_Conn.OnInitADOAccess();
}

CMNPrintReport::~CMNPrintReport()
{
}

void CMNPrintReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboxreportmn);
	DDX_Control(pDX, IDC_LIST_MNREPORT, m_listmn);
}


BEGIN_MESSAGE_MAP(CMNPrintReport, CDialogEx)
	ON_MESSAGE(WM_WAITTOPRINTMN, &CMNPrintReport::InitialList1) //自定义消息处理函数；

	ON_BN_CLICKED(IDC_BTN_ADDMN, &CMNPrintReport::OnBnClickedBtnAddmn)
	ON_BN_CLICKED(IDC_BTN_MNPRINT, &CMNPrintReport::OnBnClickedBtnMnprint)
	ON_BN_CLICKED(IDC_BTN_DELMN, &CMNPrintReport::OnBnClickedBtnDelmn)
	ON_BN_CLICKED(IDC_BTN_MNCANCLE, &CMNPrintReport::OnBnClickedBtnMncancle)
	ON_BN_CLICKED(IDC_BTN_PAINTALLMN, &CMNPrintReport::OnBnClickedBtnPaintallmn)
END_MESSAGE_MAP()


// CMNPrintReport 消息处理程序
LRESULT CMNPrintReport::InitialList1(WPARAM wParam, LPARAM lParam)//传递可打印报表的病人
{
	 p = (vector<WaitToPrint>* )wParam;
	//刷新可打印报告的病人
	m_comboxreportmn.ResetContent();
	for (size_t i = 0; i < p->size(); i++)
	{
		m_comboxreportmn.InsertString(i, (*p)[i].patientname);
	}
	m_comboxreportmn.SetCurSel(0);

	//画表格
	//设置表头
	m_listmn.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_listmn.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60, 0);
	m_listmn.InsertColumn(1, _T("病人名称"), LVCFMT_CENTER, 90, 0);
	m_listmn.InsertColumn(2, _T("报表存储路径"), LVCFMT_CENTER, 480, 0);
	m_listmn.DeleteColumn(0); //删除第0列
	//设置行高：
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listmn.SetImageList(&m_l, LVSIL_SMALL);

	//设置字体：
	CFont m_font;
	m_font.CreatePointFont(120, _T("新宋体"));
	m_listmn.SetFont(&m_font);

	return 0;
}

//添加病人
void CMNPrintReport::OnBnClickedBtnAddmn()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取combox中的病人
	CString patient = (*p)[m_comboxreportmn.GetCurSel()].patientname;
	string stringpatient = (CW2A)patient;
	string path = (*p)[m_comboxreportmn.GetCurSel()].savepath + stringpatient + ".xlsx";
	WaitToPrint combox2list;
	combox2list.patientname = patient;
	combox2list.savepath = path;
	//查询PList中是否已经有该人
	vector<CString>::iterator it = find(PatientInList.begin(), PatientInList.end(), patient);
	if (it == PatientInList.end())
	{
		PatientInList.push_back(patient);
		pList.push_back(combox2list);
	}

	//显示病人
	FreshPatient();
}

//打印报告
void CMNPrintReport::OnBnClickedBtnMnprint()
{
	// TODO:  在此添加控件通知处理程序代码
	for (size_t i = 0; i < pList.size(); i++)
	{
		MakeOneReporte(pList[i].patientname, pList[i].savepath);
	}
	AfxMessageBox(_T("生成成功！"));
}

// 生成一个病人的报表
bool CMNPrintReport::MakeOneReporte(CString patientname, string path)
{
	//读取数据
	MN_HandleResult pA;
	pA = ReadMicro.ReadOneMNResultFromAccess(patientname);

	IllusionExcelFile excl;
	bool bInit = excl.InitExcel();
	bool bRet = excl.OpenExcelFile(_T("C:\\model.xlsx"));
	CString strSheetName = excl.GetSheetName(3);
	bool bLoad = excl.LoadSheet(strSheetName);

	CString strValue;
	strValue = pA.patientname;
	excl.SetCellString(3, 5, strValue); //姓名
	strValue.Format(_T("%d"), pA.doublecell);
	excl.SetCellString(8, 2, strValue); //分析细胞总数
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[1]);
	excl.SetCellString(8, 3, strValue); //
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[2]);
	excl.SetCellString(8, 4, strValue); 
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[3]);
	excl.SetCellString(8, 5, strValue); 
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[4]);
	excl.SetCellString(8, 6, strValue);
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[5]);
	excl.SetCellString(8, 7, strValue);
	strValue.Format(_T("%d"), pA.DoubleCellsWithMN[6]);
	excl.SetCellString(8, 8, strValue);

	strValue.Format(_T("%d"), pA.DoubleMNC_Num);
	excl.SetCellString(8, 13, strValue);
	strValue.Format(_T("%.2f"), pA.DoubleMNC_Rate);
	CString str = strValue;
	excl.SetCellString(8, 14, str);

	strValue.Format(_T("%d"), pA.DoubleMN);
	excl.SetCellString(8, 15, strValue);
	strValue.Format(_T("%.2f"), pA.DoubleMN_Rate);
	str = strValue;
	excl.SetCellString(8, 16, str);

	CString strFile(path.c_str());

	excl.SaveasXSLFile(strFile);

	string str12 = CT2A(strFile.GetBuffer());
	string command_rd = "start  excel " + str12;
	system(command_rd.c_str());
	return false;
}

//删除一行
void CMNPrintReport::OnBnClickedBtnDelmn()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_listmn.GetSelectionMark();
	if (sel >= 0)
	{
		//vector中删除
		auto iter = pList.erase(std::begin(pList) + sel);
		auto iter1 = PatientInList.erase(std::begin(PatientInList) + sel);

		//List控件中也删除该条记录
		m_listmn.DeleteItem(sel);
		FreshPatient();
	}
	else if (sel<0)
		MessageBox(_T("列表中无选中记录！"));
}


// 刷新显示病人
void CMNPrintReport::FreshPatient()
{
	CString strpath;
	m_listmn.DeleteAllItems();
	for (size_t i = 0; i < pList.size(); i++)
	{
		m_listmn.InsertItem(i, pList[i].patientname);//增加一行
		USES_CONVERSION;
		strpath = pList[i].savepath.c_str();
		m_listmn.SetItemText(i, 1, strpath);
	}
}

//取消
void CMNPrintReport::OnBnClickedBtnMncancle()
{
	// TODO:  在此添加控件通知处理程序代码
	OnOK();
}

//打印微核总表
void CMNPrintReport::OnBnClickedBtnPaintallmn()
{
	IllusionExcelFile excl;
	bool bInit = excl.InitExcel();
	if (!bInit)
	{
		AfxMessageBox(_T("初始化Excel失败！请确认本机是否安装Excel！"));
	}
	else
	{
		bool bRet = excl.OpenExcelFile(_T("C:\\model.xlsx"));
		if (!bRet)
		{
			AfxMessageBox(_T("打开Excel模板失败！请确认模板是否存在！"));
		}
		else{
			CString strSheetName = excl.GetSheetName(4);
			bool bLoad = excl.LoadSheet(strSheetName);
			CString strValue;

			vector<MN_HandleResult>AllChosePatient = ReadMicro.ReadAllMNResultFromAccess();
			for (size_t i = 0; i < AllChosePatient.size(); i++)
			{
				MN_HandleResult pA;
				pA = AllChosePatient[i];

				CString strValue;
				strValue.Format(_T("%d"), i+1);
				excl.SetCellString(7 + i, 2, strValue); //序号
				strValue = pA.patientname;
				excl.SetCellString(7 + i, 3, strValue); //姓名
				strValue.Format(_T("%d"), pA.doublecell);
				excl.SetCellString(6+i, 5, strValue); //分析细胞总数


				strValue.Format(_T("%d"), pA.DoubleMNC_Num);
				excl.SetCellString(7 + i, 10, strValue);
				strValue.Format(_T("%.2f"), pA.DoubleMNC_Rate);
				CString str = strValue;
				excl.SetCellString(7 + i, 11, str);

				strValue.Format(_T("%d"), pA.DoubleMN);
				excl.SetCellString(7 + i, 12, strValue);
				strValue.Format(_T("%.2f"), pA.DoubleMN_Rate);
				str = strValue ;
				excl.SetCellString(7 + i, 13, str);

			}

			CString strFile;
			CFileDialog dlg(FALSE, _T("xlsx"), _T("微核汇总表"));//FALSE表示为“另存为”对话框，否则为“打开”对话框
			if (dlg.DoModal() == IDOK)
			{
				strFile = dlg.GetPathName();//获取完整路径
				excl.SaveasXSLFile(strFile);
				string str12 = CT2A(strFile.GetBuffer());
				string command_rd = "start  excel " + str12;
				system(command_rd.c_str());
			}

		}

	}
}
