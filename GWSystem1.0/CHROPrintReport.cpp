// CHROPrintReport.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CHROPrintReport.h"
#include "afxdialogex.h"
#include"ReadAndWriteForAccess.h"
//报表
#include "IllusionExcelFile.h"

// CCHROPrintReport 对话框
CReadAndWriteForAccess Read;

IMPLEMENT_DYNAMIC(CCHROPrintReport, CDialogEx)

CCHROPrintReport::CCHROPrintReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCHROPrintReport::IDD, pParent)
{

}

CCHROPrintReport::~CCHROPrintReport()
{
}

void CCHROPrintReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REPORTCHRO, m_reportchro);
	DDX_Control(pDX, IDC_LIST_CHREPORT, m_listchro);
}


BEGIN_MESSAGE_MAP(CCHROPrintReport, CDialogEx)
	ON_MESSAGE(WM_WAITTOPRINTCHRO, &CCHROPrintReport::InitialList1) //自定义消息处理函数；
	ON_BN_CLICKED(IDC_BTN_ADDCHRO, &CCHROPrintReport::OnBnClickedBtnAddchro)
	ON_BN_CLICKED(IDC_BTN_PAINTALLCHRO, &CCHROPrintReport::OnBnClickedBtnPaintallchro)
	ON_BN_CLICKED(IDC_BTN_CHROPRINT, &CCHROPrintReport::OnBnClickedBtnChroprint)
	ON_BN_CLICKED(IDC_BTN_DELCHRO, &CCHROPrintReport::OnBnClickedBtnDelchro)
	ON_BN_CLICKED(IDC_BTN_CHROCANCLE, &CCHROPrintReport::OnBnClickedBtnChrocancle)
END_MESSAGE_MAP()


// CCHROPrintReport 消息处理程序

LRESULT CCHROPrintReport::InitialList1(WPARAM wParam, LPARAM lParam)//传递可打印报表的病人
{
	p = (vector<WaitToPrint>*)wParam;
	//刷新可打印报告的病人
	m_reportchro.ResetContent();
	for (size_t i = 0; i < p->size(); i++)
	{
		m_reportchro.InsertString(i, (*p)[i].patientname);
	}
	m_reportchro.SetCurSel(0);

	//画表格
	//设置表头
	m_listchro.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_listchro.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60, 0);
	m_listchro.InsertColumn(1, _T("病人名称"), LVCFMT_CENTER, 90, 0);
	m_listchro.InsertColumn(2, _T("报表存储路径"), LVCFMT_CENTER, 480, 0);
	m_listchro.DeleteColumn(0); //删除第0列
	//设置行高：
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listchro.SetImageList(&m_l, LVSIL_SMALL);

	//设置字体：
	CFont m_font;
	m_font.CreatePointFont(120, _T("新宋体"));
	m_listchro.SetFont(&m_font);

	return 0;
}


//添加病人
void CCHROPrintReport::OnBnClickedBtnAddchro()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取combox中的病人
	CString patient = (*p)[m_reportchro.GetCurSel()].patientname;
	string stringpatient = (CW2A)patient;
	string path = (*p)[m_reportchro.GetCurSel()].savepath + stringpatient + ".xlsx";
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

//打印总表
void CCHROPrintReport::OnBnClickedBtnPaintallchro()
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
			CString strSheetName = excl.GetSheetName(2);
			bool bLoad = excl.LoadSheet(strSheetName);
			CString strValue;

			vector<CHRO_HandleResult>AllChosePatient = Read.ReadAllResultFromAccess();
			for (size_t i = 0; i < AllChosePatient.size(); i++)
			{
				CHRO_HandleResult pA;
				pA = AllChosePatient[i];

				strValue.Format(_T("%d"), i+1);
				excl.SetCellString(8 + i, 2, strValue); //分析细胞总数

				strValue = pA.patientname;
				excl.SetCellString(8 + i, 3, strValue); //姓名
				strValue.Format(_T("%d"), pA.allcell);
				excl.SetCellString(8 + i, 5, strValue); //分析细胞总数
				strValue.Format(_T("%d"), pA.abnormal);
				excl.SetCellString(8 + i, 6, strValue); //畸变细胞数
				strValue.Format(_T("%f"), pA.Y*100);
				excl.SetCellString(8 + i, 7, strValue); //畸变率
				strValue.Format(_T("%d"), pA.dic);
				excl.SetCellString(8 + i, 8, strValue); //双着数
				//strValue.Format(_T("%d"), pA.tri);
				//excl.SetCellString(8 + i, 9, strValue);
				//strValue.Format(_T("%d"), pA.ten);
				//excl.SetCellString(8 + i, 10, strValue);
				//strValue.Format(_T("%d"), pA.ace);
				//excl.SetCellString(8 + i, 11, strValue);
				//strValue.Format(_T("%d"), pA.round);
				//excl.SetCellString(8 + i, 12, strValue);
				//strValue.Format(_T("%d"), pA.t);
				//excl.SetCellString(8 + i, 13, strValue);
				//strValue.Format(_T("%d"), pA.inv);
				//excl.SetCellString(8 + i, 14, strValue);
				//strValue.Format(_T("%d"), pA.del);
				//excl.SetCellString(8 + i, 15, strValue);

				//int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
				int sum = pA.dic ;
				strValue.Format(_T("%d"), sum);
				excl.SetCellString(8 + i, 17, strValue);
				float rate = (float)sum / pA.allcell;
				strValue.Format(_T("%f"), rate * 100);
				CString str = strValue;
				excl.SetCellString(8 + i, 18, str);

				//strValue.Format(_T("%d"), pA.ctg);
				//excl.SetCellString(8 + i, 18, strValue);
				//strValue.Format(_T("%d"), pA.csg);
				//excl.SetCellString(8 + i, 19, strValue);
				//strValue.Format(_T("%d"), pA.ctb);
				//excl.SetCellString(8 + i, 20, strValue);
				//strValue.Format(_T("%d"), pA.cte);
				//excl.SetCellString(8 + i, 21, strValue);

				//sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
				//strValue.Format(_T("%d"), sum);
				//excl.SetCellString(8 + i, 22, strValue);
				//rate = (float)sum / pA.allcell;
				//strValue.Format(_T("%f"), rate * 100);
				//str = strValue + _T("%");
				//excl.SetCellString(8 + i, 23, strValue);
			}

			CString strFile;
			CFileDialog dlg(FALSE, _T("xlsx"), _T("汇总表"));//FALSE表示为“另存为”对话框，否则为“打开”对话框
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

//打印报告
void CCHROPrintReport::OnBnClickedBtnChroprint()
{
	// TODO:  在此添加控件通知处理程序代码
	for (size_t i = 0; i < pList.size(); i++)
	{
		MakeOneReporte(pList[i].patientname, pList[i].savepath);
	}
	AfxMessageBox(_T("生成成功！"));

}

// 生成一个病人的报表
bool CCHROPrintReport::MakeOneReporte(CString patientname, string path)
{
	//读取数据
	CHRO_HandleResult pA;
	pA = Read.ReadOneResultFromAccess(patientname);

	IllusionExcelFile excl;
	bool bInit = excl.InitExcel();
	bool bRet = excl.OpenExcelFile(_T("C:\\model.xlsx"));
	CString strSheetName = excl.GetSheetName(1);
	bool bLoad = excl.LoadSheet(strSheetName);

	CString strValue;
	strValue = pA.patientname;
	excl.SetCellString(3, 4, strValue); //姓名
	strValue.Format(_T("%d"),pA.allcell);
	excl.SetCellString(8, 2, strValue); //分析细胞总数
	strValue.Format(_T("%d"), pA.abnormal);
	excl.SetCellString(8, 3, strValue); //畸变细胞数
	strValue.Format(_T("%f"), pA.Y);
	excl.SetCellString(8, 4, strValue); //畸变率
	strValue.Format(_T("%d"), pA.dic);
	excl.SetCellString(8, 5, strValue); //双着数
	//strValue.Format(_T("%d"), pA.tri);
	//excl.SetCellString(8, 6, strValue);
	//strValue.Format(_T("%d"), pA.ten);
	//excl.SetCellString(8, 7, strValue);
	//strValue.Format(_T("%d"), pA.ace);
	//excl.SetCellString(8, 8, strValue);
	//strValue.Format(_T("%d"), pA.round);
	//excl.SetCellString(8, 9, strValue);
	//strValue.Format(_T("%d"), pA.t);
	//excl.SetCellString(8, 10, strValue);
	//strValue.Format(_T("%d"), pA.inv);
	//excl.SetCellString(8, 11, strValue);
	//strValue.Format(_T("%d"), pA.del);
	//excl.SetCellString(8, 12, strValue);

	//int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
	int sum = pA.dic;
	strValue.Format(_T("%d"), sum);
	excl.SetCellString(8, 13, strValue);
	float rate = (float)sum / pA.allcell;
	strValue.Format(_T("%f"), rate*100);
	CString str = strValue;
	excl.SetCellString(8, 14, str);

	//strValue.Format(_T("%d"), pA.ctg);
	//excl.SetCellString(8, 15, strValue);
	//strValue.Format(_T("%d"), pA.csg);
	//excl.SetCellString(8, 16, strValue);
	//strValue.Format(_T("%d"), pA.ctb);
	//excl.SetCellString(8, 17, strValue);
	//strValue.Format(_T("%d"), pA.cte);
	//excl.SetCellString(8, 18, strValue);

	//sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
	//strValue.Format(_T("%d"), sum);
	//excl.SetCellString(8, 19, strValue);
	//rate = (float)sum / pA.allcell;
	//strValue.Format(_T("%f"), rate*100);
	//str = strValue + _T("%");
	//excl.SetCellString(8, 20, str);

	CString strFile(path.c_str());
	//CFileDialog dlg(FALSE, _T("xlsx"), patientname);//FALSE表示为“另存为”对话框，否则为“打开”对话框
	//if (dlg.DoModal() == IDOK)
	//{
	//	strFile = dlg.GetPathName();//获取完整路径
	//}

	excl.SaveasXSLFile(strFile);
	string str12 = CT2A(strFile.GetBuffer());
	string command_rd = "start  excel " + str12;
	system(command_rd.c_str());
	return false;
}


//删除一行
void CCHROPrintReport::OnBnClickedBtnDelchro()
{
	// TODO:  在此添加控件通知处理程序代码
	int sel = m_listchro.GetSelectionMark();
	if (sel >= 0)
	{
		//vector中删除
		auto iter = pList.erase(std::begin(pList) + sel);
		auto iter1 = PatientInList.erase(std::begin(PatientInList) + sel);

		//List控件中也删除该条记录
		m_listchro.DeleteItem(sel);
		FreshPatient();
	}
	else if (sel<0)
		MessageBox(_T("列表中无选中记录！"));

}

// 刷新显示病人
void CCHROPrintReport::FreshPatient()
{
	CString strpath;
	m_listchro.DeleteAllItems();
	for (size_t i = 0; i < pList.size(); i++)
	{
		m_listchro.InsertItem(i, pList[i].patientname);//增加一行
		USES_CONVERSION;
		strpath = pList[i].savepath.c_str();
		m_listchro.SetItemText(i, 1, strpath);
	}
}

//取消
void CCHROPrintReport::OnBnClickedBtnChrocancle()
{
	// TODO:  在此添加控件通知处理程序代码
	OnOK();
}
