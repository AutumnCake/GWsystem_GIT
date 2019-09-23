// CChromosomeHandle.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CChromosomeHandle.h"
#include"ChromosomeAlgorithm.h"
#include"ChromosomeResult.h"
#include "CHROPrintReport.h"
#include "afxdialogex.h"
#include "GWSystem1.0Dlg.h"
#include "ReadAndWriteForAccess.h"
#include "Scanning_Control.h"
#include<thread>

#include "CSeries.h"
#include <stdlib.h>
#include <direct.h>
#include<io.h>
#include"FileOperate.h"

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"



extern Scanning_Control *m_Scanning_Control;
extern CGWSystem10Dlg* pDlg;
extern vector <PatientWithSlide> tempAllPatients;
//全局变量
CChromosomeHandle * pHandleDlg;
CChromosomeResult * pChromosomeResult;
CReadAndWriteForAccess ReadAndWrite;
string StrFileSolveChro;


// CChromosomeHandle 对话框

IMPLEMENT_DYNAMIC(CChromosomeHandle, CDialogEx)

CChromosomeHandle::CChromosomeHandle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChromosomeHandle::IDD, pParent)
{
	pChromosomeResult = NULL;
	patientNewImg = true;
}

CChromosomeHandle::~CChromosomeHandle()
{
	
}

void CChromosomeHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listchrohandle);
	DDX_Control(pDX, IDC_TCHARTCHRO, m_chartchrom);
	DDX_Control(pDX, IDC_DATESTARTCHRO, m_datetimebegin);
	DDX_Control(pDX, IDC_DATEENDCHRO, m_datetimeend);
	DDX_Control(pDX, IDC_LIST_RESULTCHRO, m_listchroresult);
	//DDX_Control(pDX, IDC_GRID_CHRO, m_pGridchro);
}


BEGIN_MESSAGE_MAP(CChromosomeHandle, CDialogEx)
	ON_MESSAGE(WM_HANDLEPATHCHRO, &CChromosomeHandle::InitialList1) //自定义消息处理函数；
	ON_MESSAGE(WM_INIT_TABLECHRO, &CChromosomeHandle::InitialList2) //自定义消息处理函数；
	ON_BN_CLICKED(IDC_BTN_CHROHANDLE, &CChromosomeHandle::OnBnClickedBtnChrohandle)
	ON_BN_CLICKED(IDC_BTN_CHROSOLVE, &CChromosomeHandle::OnBnClickedBtnChrosolve)
	ON_BN_CLICKED(IDC_BTN_CHROPRINT, &CChromosomeHandle::OnBnClickedBtnChroprint)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CChromosomeHandle::OnNMClickList1)
	ON_BN_CLICKED(IDC_BTN_FINDCHRO, &CChromosomeHandle::OnBnClickedBtnFindchro)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CChromosomeHandle::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CChromosomeHandle::OnNMDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULTCHRO, &CChromosomeHandle::OnNMClickListResultchro)
END_MESSAGE_MAP()


// CChromosomeHandle 消息处理程序


BOOL CChromosomeHandle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pHandleDlg = this;//指向该对话框的全局指针

	//连接数据库
	m_Conn.OnInitADOAccess();
	GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰
	GetDlgItem(IDC_BTN_CHROPRINT)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰
	GetDlgItem(IDC_BTN_CHROSOLVE)->EnableWindow(FALSE);
	//删除原数据表
	DeleteAccessTable();
	//设置表头
	CRect rectCtrl;
	CStatic *p = (CStatic*)GetDlgItem(IDC_LIST1);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listchrohandle.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listchrohandle.InsertColumn(0, _T("是否分析"), LVCFMT_CENTER, 80, 0);
	m_listchrohandle.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectCtrl.Width()*0.107, 0);
	m_listchrohandle.InsertColumn(2, _T("病人名称"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(3, _T("图片总数"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(4, _T("分析进度"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(5, _T("拍图时间"), LVCFMT_CENTER, rectCtrl.Width()*0.25);
	m_listchrohandle.InsertColumn(6, _T("报表生成情况"), LVCFMT_CENTER, rectCtrl.Width()*0.214);
	m_listchrohandle.DeleteColumn(0); //删除第0列

	//设置结果表表头
	p = (CStatic*)GetDlgItem(IDC_LIST_RESULTCHRO);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listchroresult.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_listchroresult.InsertColumn(0, _T("是否分析"), LVCFMT_CENTER, 80, 0);
	m_listchroresult.InsertColumn(1, _T("姓名"), LVCFMT_CENTER, rectCtrl.Width()*0.073);
	m_listchroresult.InsertColumn(2, _T("分析细胞总数"), LVCFMT_CENTER, rectCtrl.Width()*0.085);
	m_listchroresult.InsertColumn(3, _T("畸变细胞数"), LVCFMT_CENTER, rectCtrl.Width()*0.065);
	m_listchroresult.InsertColumn(4, _T("畸变率"), LVCFMT_CENTER, rectCtrl.Width()*0.045);
	m_listchroresult.InsertColumn(5, _T("dic"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(6, _T("tri"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(7, _T("ten"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(8, _T("ace"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(9, _T("r"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(10, _T("t"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(11, _T("inv"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(12, _T("del"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(13, _T("染色体型畸变数"), LVCFMT_CENTER, rectCtrl.Width()*0.096);
	m_listchroresult.InsertColumn(14, _T("染色体型畸变率"), LVCFMT_CENTER, rectCtrl.Width()*0.096);
	m_listchroresult.InsertColumn(15, _T("ctg"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(16, _T("csg"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(17, _T("ctb"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(18, _T("cte"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(19, _T("单体型畸变数"), LVCFMT_CENTER, rectCtrl.Width()*0.090);
	m_listchroresult.InsertColumn(20, _T("单体型畸变率"), LVCFMT_CENTER, rectCtrl.Width()*0.090);
	m_listchroresult.DeleteColumn(0); //删除第0列


	//设置行高：
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listchrohandle.SetImageList(&m_l, LVSIL_SMALL);
	//m_listchroresult.SetImageList(&m_l, LVSIL_SMALL);

	//设置字体：
	CFont m_font;
	m_font.CreatePointFont(120, _T("新宋体"));
	m_listchrohandle.SetFont(&m_font);
	GetDlgItem(IDC_LISTHEADCHRO)->SetFont(&m_font);
	/*m_listchroresult.SetFont(&m_font);*/

	////获取路径
	//m_handlepathchro.GetWindowTextW(ReadPath);
	//m_resultpathchro.GetWindowTextW(WritePath);

	//设置时间控件的格式
	m_datetimebegin.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	m_datetimeend.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	COleDateTime MinTime(COleDateTime::GetCurrentTime() - COleDateTimeSpan(30, 0, 0, 0));
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	m_datetimebegin.SetRange(&MinTime, &MaxTime);
	m_datetimeend.SetRange(&MinTime, &MaxTime);

	//建立处理结果窗口 但隐藏
	if (pChromosomeResult == NULL){//判定对话框是否有所指向
		pChromosomeResult = new CChromosomeResult(this);//指向一个非模对话框示例
		pChromosomeResult->Create(IDD_CHROMOSOMERESULT, this);//创建
	}
	pChromosomeResult->ShowWindow(SW_HIDE);//不显示
	pChromosomeResult->m_comboxpatient.ResetContent();


	//将本次扫描信息导入列表 并选中
	LoadCurrentToList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


// 清空数据库中的一些表格
int CChromosomeHandle::DeleteAccessTable()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("delete  * from 染色体图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete  * from 染色体图像分析结果数据表（已删除）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}


	sql = _T("delete  * from 染色体分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	return 0;
}


//自定义消息函数1----传递处理文件夹路径与储存文件夹路径
LRESULT CChromosomeHandle::InitialList1(WPARAM wParam, LPARAM lParam)
{
	MyFiles *pA = (MyFiles*)wParam;
	//CString  str;
	//str = pA->picsolve.c_str();
	//m_handlepathchro.SetWindowText(str);
	//str = pA->endsolve.c_str();
	//m_resultpathchro.SetWindowText(str);
	return 0;

	//一定得有个返回值

}

//自定义消息函数2----传递处理文件夹路径与储存文件夹路径
LRESULT CChromosomeHandle::InitialList2(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString mystr(b);
	SysFreeString(b);

	pHandleDlg->m_listchrohandle.SetItemText(lParam, 3, mystr);
	pHandleDlg->UpdateData(FALSE);

	return 0;//一定得有个返回值

}

//开始分析
void CChromosomeHandle::OnBnClickedBtnChrohandle()
{
	GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰

	//1.获取所选中的病人数组，加入AllPatientsChose
	int nItem, i;
	nItem = m_listchrohandle.GetItemCount();
	for (i = 0; i < nItem; ++i)
	{
		if (m_listchrohandle.GetCheck(i))
		{
			AllPatientsChose.push_back(AllPatients[i]);
			Row.push_back(i);
		}
	}

	thread ChroAnalyse(ThreadProcWaitCHRO);
	ChroAnalyse.detach();

}

//分析线程
void CChromosomeHandle::ThreadProcWaitCHRO()
{
	//初始化python
	//在使用Python系统前，必须使用Py_Initialize对其
	//进行初始化。它会载入Python的内建模块并添加系统路
	//径到模块搜索路径中。这个函数没有返回值，检查系统
	//是否初始化成功需要使用Py_IsInitialized。
	Py_Initialize();
	// 检查初始化是否成功
	if (!Py_IsInitialized())
	{
		AfxMessageBox(_T("python初始化失败!\n"));
	}
	PyRun_SimpleString("print('come in python')");
	//显示python版本信息
	cout << "显示python版本信息：";
	cout << (Py_GetVersion()) << "\n";
	/*PyObject* sys = PyImport_ImportModule("sys");*/
	//PyRun_SimpleString("import sys"); // 执行 python 中的短语句  
	//PyRun_SimpleString("sys.path.append('./')");
	//PyRun_SimpleString("import cv2");

	//生成处理时间文件夹
	string strWrite = CT2A(pHandleDlg->WritePath.GetBuffer());
	string writeroad;
	writeroad = strWrite + "\\";

	CTime time = CTime::GetCurrentTime(); ///构造CTime对象
	int m_nYear = time.GetYear(); ///年
	int m_nMonth = time.GetMonth(); ///月
	int m_nDay = time.GetDay(); ///日
	int m_nHour = time.GetHour(); ///小时
	int m_nMinute = time.GetMinute(); ///分钟
	int m_nSecond = time.GetSecond(); ///秒
	CString strDate;
	strDate.Format(_T("%d年%d月%d日"), m_nYear, m_nMonth, m_nDay); //定义日期格式
	CString strTime = time.Format(_T("%H-%M-%S")); //定义时间格式
	CString index(_T("的处理结果"));
	pHandleDlg->StrTime = strDate + strTime + index;

	pHandleDlg->ResultRow = 0;
	for (size_t i = 0; i < pHandleDlg->AllPatientsChose.size(); i++)
	{
		//vector<bool>().swap(m_Scanning_Control->Scanning_Completed_Status);
		pHandleDlg->patientNewImg = true;//刚开始分析 肯定有新照片

		int row = pHandleDlg->Row[i];
		pHandleDlg->PatientSlideNum = pHandleDlg->GetPatientSlideNumFromAccess(pHandleDlg->AllPatientsChose[i].PatientName);
		//(1)生成该病人的处理结果文件夹
		CString name = pHandleDlg->AllPatientsChose[i].PatientName; //定义时间格式
		pHandleDlg->StrName = pHandleDlg->AllPatientsChose[i].ResultPath + name + "-" + pHandleDlg->StrTime;
		string strname = CT2A(pHandleDlg->StrName.GetBuffer());
		StrFileSolveChro = pHandleDlg->strFileSolve + strname + "\\";//生成结果的文件名 
		const char* c_filesolve = StrFileSolveChro.c_str();//将路径转换成 const char
		_mkdir(c_filesolve);

		//(2)开始对该病人的图片进行分析
		CChromosomeAlgorithm algorithm;
		CHRO_HandleResult *pB = new CHRO_HandleResult; //该病人分析结果存储结构体
		CString mystr;
		mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].ChroImgNames.size());

		PatientAndQuaImg thistime;
		thistime.patientname = name;
		USES_CONVERSION;
		thistime.QuestionImgPath = A2W((StrFileSolveChro).c_str());

		pChromosomeResult->m_comboxpatient.AddString(name);
		pChromosomeResult->AllQuestionImgPath.push_back(thistime);
		pChromosomeResult->PatientNames.push_back(name);

		size_t newStart = 0;
		while (pHandleDlg->patientNewImg)//判断该病人是否有新的图片加入
		{
			for (size_t j = newStart; j < pHandleDlg->AllPatientsChose[i].ChroImgNames.size(); j++)
			{
				string readpath = CT2A(pHandleDlg->AllPatientsChose[i].ImgPath.GetBuffer());
				string imgname = CT2A(pHandleDlg->AllPatientsChose[i].ChroImgNames[j].GetBuffer());
				pHandleDlg->AllPatientsChose[i].Result = algorithm.DicMain(readpath, imgname, StrFileSolveChro, pHandleDlg->AllPatientsChose[i].PatientName, pB);
				pHandleDlg->AllPatientsChose[i].Result->patientname = pHandleDlg->AllPatientsChose[i].PatientName;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].ChroImgNames.size() + 1;
				pHandleDlg->AllPatientsChose[i].Result->sourcefile = pHandleDlg->AllPatientsChose[i].ImgPath;
				pHandleDlg->AllPatientsChose[i].Result->resultfile = pHandleDlg->AllPatientsChose[i].ResultPath;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].ChroImgNames.size();

				CString mystr1, mystr2;
				mystr1.Format(_T("%d"), j + 1);
				mystr2 = mystr1 + _T("/") + mystr;

				HWND   hwnd = ::FindWindow(NULL, _T("染色体处理"));//调用消息处理函数刷新页面
				::SendMessage(hwnd, WM_INIT_TABLECHRO, (WPARAM)mystr2.AllocSysString(), (WPARAM)row);//线程中传递定时器消息，以开启定时器，刷新显示照片
			}
			//查询该病人的图片是否在增加
			vector<CString>patientNewImgPath = pHandleDlg->InquiryPatientImg(pHandleDlg->AllPatientsChose[i].PatientName, pHandleDlg->AllPatientsChose[i].ImgPath);
			int patientNewImgNum = patientNewImgPath.size();
			if (patientNewImgNum > pHandleDlg->AllPatientsChose[i].ChroImgNames.size())
			{
				pHandleDlg->patientNewImg = true;
				newStart = pHandleDlg->AllPatientsChose[i].ChroImgNames.size();
				pHandleDlg->AllPatientsChose[i].ChroImgNames = patientNewImgPath;
				mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].ChroImgNames.size());

			}
			else
			{

				CString strPath;//10倍镜下扫描图像存储的路径
				CA2T szr2((pDlg->AllFiles.picsolve).c_str());
				strPath = (LPCTSTR)szr2;
				if (pHandleDlg->AllPatientsChose[i].ImgPath == strPath)
				{
					for (size_t j = 0; j < tempAllPatients.size(); j++)
					{
						if (pHandleDlg->AllPatientsChose[i].PatientName == tempAllPatients[j].PatientName)
						{
							int AllOver = 0;
							for (size_t k = 0; k < tempAllPatients[j].SlideID.size(); k++)
							{
								if (tempAllPatients[j].SlideID[k] == -1)
									AllOver += 0;
								else
									AllOver += 1;
							}
							if (AllOver>0)
								pHandleDlg->patientNewImg = true;
							else
								pHandleDlg->patientNewImg = false;
						}

					}
					
				
				}
				else
					pHandleDlg->patientNewImg = false;

				//bool Scanning_End = false;

				//if (m_Scanning_Control->Scanning_Completed_Status.size() >= m_Scanning_Control->s_Scanning_Para.Current_Slide_Num+1)
				//	Scanning_End = m_Scanning_Control->Scanning_Completed_Status[m_Scanning_Control->s_Scanning_Para.Current_Slide_Num];
				//for (int m = 0; m < pHandleDlg->PatientSlideNum.size(); m++)
				//{
				//	bool Scanning_Over = pHandleDlg->PatientSlideNum[m] == m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
				//	//if ((pHandleDlg->PatientSlideNum[m] == m_Scanning_Control->s_Scanning_Para.Current_Slide_Num))
				//	if (Scanning_End && Scanning_Over)
				//		pHandleDlg->patientNewImg = false;
				//	else
				//		pHandleDlg->patientNewImg = true;
				//}

			}
				
		}

		//将分析结果存入数据库中
		ReadAndWrite.SaveToAccess(pHandleDlg->AllPatientsChose[i].Result);
		//可以生成该病人的报表
		pHandleDlg->CHROWaitToPrint(pHandleDlg->AllPatientsChose[i].Result->patientname, StrFileSolveChro);
		//将表格中改成报表已生成
		CString mystr3 = _T("√");
		pHandleDlg->m_listchrohandle.SetItemText(row, 5, mystr3);
		//图表中显示该病人的分析结果
		pHandleDlg->ShowOneResult(pHandleDlg->AllPatientsChose[i].Result->patientname);
		pHandleDlg->ShowOneResultInTable(pHandleDlg->AllPatientsChose[i].Result->patientname);

		pHandleDlg->ResultRow++;

		pHandleDlg->GetDlgItem(IDC_BTN_CHROPRINT)->EnableWindow(TRUE);//无lisr选项选中使删除按钮变灰
		pHandleDlg->GetDlgItem(IDC_BTN_CHROSOLVE)->EnableWindow(TRUE);//无lisr选项选中使删除按钮变灰

	}

	//关闭python调用
	//**************************************************************************************************
	Py_Finalize();
}


// 加入可生成报表的结构体
void CChromosomeHandle::CHROWaitToPrint(CString patientname, string savepath)
{
	WaitToPrint chroWaitToPrint;
	chroWaitToPrint.patientname = patientname;
	chroWaitToPrint.savepath = savepath;
	CHROWaitPrint.push_back(chroWaitToPrint);
}

// 将一个病人的分析结果显示到图中
bool CChromosomeHandle::ShowOneResult(CString patientname)
{
	//显示到图中
	CHRO_HandleResult pA;
	pA = ReadAndWrite.ReadOneResultFromAccess(patientname);
	ClearAllSeries();
	CSeries barSeries = (CSeries)m_chartchrom.Series(0);
	barSeries.AddXY((double)1, (double)pA.allcell, _T("分析细胞总数"), RGB(255, 255, 0));
	barSeries.AddXY((double)2, (double)pA.dic, _T("双着丝粒体数"), RGB(255, 0, 255));
	barSeries.AddXY((double)3, (double)pA.round, _T("着丝粒环数"), RGB(0, 255, 255));
	barSeries.AddXY((double)4, (double)pA.ace, _T("无着丝粒体数"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.t, _T("相互易位数"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.inv, _T("倒位数"), RGB(0, 255, 255));
	//UpdateData(false);

	return false;
}

// 将一个病人的分析结果显示到表中
bool CChromosomeHandle::ShowOneResultInTable(CString patientname)
{

	CHRO_HandleResult pA;
	pA = ReadAndWrite.ReadOneResultFromAccess(patientname);

	//显示到表中
	int j = 0;
	CString  mystr;
	mystr = pA.patientname;
	m_listchroresult.InsertItem(ResultRow, mystr);
	mystr.Format(_T("%d"), pA.allcell);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.abnormal);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	float rate = (float)pA.abnormal / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	CString str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	mystr.Format(_T("%d"), pA.dic);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.tri);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ten);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ace);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.round);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.t);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.inv);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.del);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
	mystr.Format(_T("%d"), sum);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	rate = (float)sum / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	mystr.Format(_T("%d"), pA.ctg);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.csg);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ctb);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.cte);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
	mystr.Format(_T("%d"), sum);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	rate = (float)sum / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	return false;
}


//清空图表
void CChromosomeHandle::ClearAllSeries(void)
{
	for (long i = 0; i<m_chartchrom.get_SeriesCount(); i++)
	{
		((CSeries)m_chartchrom.Series(i)).Clear();
	}

}

//查看处理结果
void CChromosomeHandle::OnBnClickedBtnChrosolve()
{
	// TODO:  在此添加控件通知处理程序代码
	pChromosomeResult->ShowWindow(SW_SHOW);//不显示

}

//打印本次分析的报告
void CChromosomeHandle::OnBnClickedBtnChroprint()
{
	// TODO:  在此添加控件通知处理程序代码
	CCHROPrintReport *pCHPrintReport = new CCHROPrintReport;
	pCHPrintReport->Create(IDD_PRINTREPORTCHRO, this);
	//将当前可打印的病人传到打印界面以初始化
	HWND   hwnd = ::FindWindow(NULL, _T("打印分析报告（染色体）"));//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_WAITTOPRINTCHRO, (WPARAM)&CHROWaitPrint, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片

	pCHPrintReport->ShowWindow(SW_SHOW);
}

//单击列表事件
void CChromosomeHandle::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//int row = m_listchrohandle.GetSelectionMark();
	//CString name = m_listchrohandle.GetItemText(row, 1);
	//ShowOneResult(name);
	*pResult = 0;
}

//查询
void CChromosomeHandle::OnBnClickedBtnFindchro()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空原表格
	m_listchrohandle.DeleteAllItems();

	vector<ChroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//清空每一轮查找的图

	//1.获得相应时间段内的玻片图片
	COleDateTime TimeBegin, TimeEnd;
	m_datetimebegin.GetTime(TimeBegin);
	m_datetimeend.GetTime(TimeEnd);
	CReadAndWriteForAccess xx;
	vector<Table_SlideSet> ReadSlideset;
	ReadSlideset = xx.QuerySaveTimeCHRO(TimeBegin, TimeEnd);

	//2.遍历每张拨片的路径，合并相同路径，得到路径集合数组
	vector<CString>AllCHROImgPath;
	vector<CString>AllCHROResPath;
	vector<COleDateTime>AllCHROTime;
	for (auto iter : ReadSlideset)
	{
		vector<CString>::iterator ret;
		ret = std::find(AllCHROImgPath.begin(), AllCHROImgPath.end(), iter.path);
		if (ret == AllCHROImgPath.end())
		{
			AllCHROImgPath.push_back(iter.path);
			AllCHROResPath.push_back(iter.resultpath);
			AllCHROTime.push_back(iter.time);
		}
	}

	//3.将路径集合数组里面的路径下的病人图片进行归类，显示到表格
	int patientRow = 0;
	for (size_t pathNum_i = 0; pathNum_i < AllCHROImgPath.size(); pathNum_i++)
	{
		int picNum = 0;
		vector<CString> vecEmpty;
		AllChroImg.swap(vecEmpty);//清空每一轮查找的图


		//（1）筛选所有病人的微核图片，并放入结构体
		USES_CONVERSION;
		char* charReadPath = T2A(AllCHROImgPath[pathNum_i]);
		sprintf_s(tempFilePath, "%s\\*.bmp", charReadPath);
		MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
		HANDLE handle = FindFirstFile(wstr, &file);
		if (handle != INVALID_HANDLE_VALUE)
		{
			// 循环遍历得到文件夹的所有文件名    
			do
			{
				OneImgName = file.cFileName;
				int iPos = OneImgName.Find(_T("_"));
				CString str1 = OneImgName.Mid(iPos + 1, OneImgName.GetLength() - iPos - 1);
				iPos = str1.Find(_T("_"));
				str1 = str1.Left(iPos);
				if (str1 == _T("染色体"))
					AllChroImg.push_back(OneImgName);
			} while (FindNextFile(handle, &file));
		}
		FindClose(handle);

		//(2)建立各个病人与图片的对应结构体
		BOOL havepatientornot;
		CString strDst;
		vector <ChroTable> tempAllPatients;

		for (auto iter : AllChroImg)
		{
			picNum = 0;
			havepatientornot = FALSE;

			//获取照片名中的病人名
			CString strDst1 = iter.Left(iter.Find(_T('（')));
			CString strDst2 = iter.Left(iter.Find(_T('(')));
			CString strDst3 = iter.Left(iter.Find(_T('_')));
			if (strDst1.IsEmpty() && !strDst2.IsEmpty())
				strDst = strDst2;
			if (!strDst1.IsEmpty() && strDst2.IsEmpty())
				strDst = strDst1;
			if (strDst1.IsEmpty() && strDst2.IsEmpty())
				strDst = strDst3;
			for (auto iterpatient : tempAllPatients)
			{
				if (iterpatient.PatientName == strDst)
				{
					havepatientornot = TRUE;
					tempAllPatients[picNum].ChroImgNames.push_back(iter);
				}
				picNum++;
			}
			if (!havepatientornot)//如果没有该病人
			{
				ChroTable patient;
				patient.PatientName = strDst;
				patient.ChroImgNames.push_back(iter);
				patient.ImgPath = AllCHROImgPath[pathNum_i];
				patient.ResultPath = AllCHROResPath[pathNum_i];
				tempAllPatients.push_back(patient);
			}

		}

		//（3）将病人名称与图片总数显示到表格
		
		for (size_t k = 0; k < tempAllPatients.size(); k++)
		{
			int j = 0;
			CString  mystr;
			mystr.Format(_T("%d"), patientRow + 1);
			m_listchrohandle.InsertItem(patientRow, mystr);
			m_listchrohandle.SetItemText(patientRow, ++j, tempAllPatients[k].PatientName);
			mystr.Format(_T("%d"), tempAllPatients[k].ChroImgNames.size());
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("0/") + mystr;
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = AllCHROTime[pathNum_i].Format();
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("未生成");
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			patientRow++;
			AllPatients.push_back(tempAllPatients[k]);
		}

	}


 	UpdateData(FALSE);



}

void CChromosomeHandle::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nItem, i;
	nItem = m_listchrohandle.GetItemCount();

	bool mark = false;
	for (i = 0; i < nItem && !mark; ++i)
	{
		if (m_listchrohandle.GetCheck(i))
		{
			mark = true;
		}
	}
	if (mark)
	{
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(TRUE);//有list选项选中后使删除按钮可用
	}
	else
	{
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰
	}


	*pResult = 0;
}

void CChromosomeHandle::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int row = m_listchrohandle.GetSelectionMark();
	CString name = m_listchrohandle.GetItemText(row, 1);
	ShowOneResult(name);
	*pResult = 0;
}

// 查找指定文件夹下病人图片的数量
vector <CString> CChromosomeHandle::InquiryPatientImg(CString patientname, CString imgpath)
{
	vector <CString>().swap(AllChroImg);
	//（1）筛选所有病人的微核图片，并放入结构体
	USES_CONVERSION;
	const char* charReadPath = T2A(imgpath);
	sprintf_s(tempFilePath, "%s\\*.bmp", charReadPath);
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			OneImgName = file.cFileName;
			int iPos = OneImgName.Find(_T("_"));
			CString str1 = OneImgName.Mid(iPos + 1, OneImgName.GetLength() - iPos - 1);
			iPos = str1.Find(_T("_"));
			str1 = str1.Left(iPos);
			if (str1 == _T("染色体"))
				AllChroImg.push_back(OneImgName);
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	//(2)建立各个病人与图片的对应结构体
	CString strDst;
	ChroTable patient;

	for (auto iter : AllChroImg)
	{

		//获取照片名中的病人名
		CString strDst1 = iter.Left(iter.Find(_T('（')));
		CString strDst2 = iter.Left(iter.Find(_T('(')));
		CString strDst3 = iter.Left(iter.Find(_T('_')));
		if (strDst1.IsEmpty() && !strDst2.IsEmpty())
			strDst = strDst2;
		if (!strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = strDst1;
		if (strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = strDst3;
		if (strDst == patientname)
		{
			patient.PatientName = strDst;
			patient.ChroImgNames.push_back(iter);
		}

	}


	return patient.ChroImgNames;
}

// 从数据库中获取所有该病人所对应的玻片号
vector<int> CChromosomeHandle::GetPatientSlideNumFromAccess(CString patientname)
{
	vector<int>patientslidenum;
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from 玻片信息表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			if (!(str.Find(patientname) == -1))
			{
				str = m_Conn.m_pRecordset->GetCollect(_T("玻片序号"));
				int num = _ttoi(str);
				patientslidenum.push_back(num);
			}

			m_Conn.m_pRecordset->MoveNext();
		}

		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return patientslidenum;
}

//将本次扫描信息导入列表 并选中
bool CChromosomeHandle::LoadCurrentToList()
{
	m_listchrohandle.DeleteAllItems();
	vector<ChroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//清空每一轮查找的图

	for (size_t i = 0; i < tempAllPatients.size(); i++)
	{
		if (tempAllPatients[i].mode == _T("染色体"))
		{
			ChroTable patient;
			patient.PatientName = tempAllPatients[i].PatientName;
			//patient.MicroImgNames.push_back(iter);
			patient.ImgPath = tempAllPatients[i].path;
			patient.ResultPath = tempAllPatients[i].resultpath;
			AllPatients.push_back(patient);
		}

	}

	patientRow = 0;
	for (size_t k = 0; k < AllPatients.size(); k++)
	{
		int j = 0;
		CString  mystr;
		mystr.Format(_T("%d"), patientRow + 1);
		m_listchrohandle.InsertItem(patientRow, mystr);
		m_listchrohandle.SetItemText(patientRow, ++j, AllPatients[k].PatientName);
		mystr.Format(_T("%d"), AllPatients[k].ChroImgNames.size());
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("0/") + mystr;
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("当前扫描");
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("未生成");
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		patientRow++;



		//选中
		int nItem, i;
		nItem = m_listchrohandle.GetItemCount();


		for (i = 0; i < nItem; i++)
		{
			m_listchrohandle.SetCheck(i);

		}
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(TRUE);//有list选项选中后使删除按钮可用

	}

	return true;
}

//初始化Grid控件
//void CChromosomeHandle::GridCtrlInit()
//{
//	try{
//		m_pGridchro.SetRowCount(8);
//		m_pGridchro.SetColumnCount(21);
//		m_pGridchro.SetFixedRowCount(3);
//		m_pGridchro.SetFixedColumnCount(0);
//	}
//	catch (CMemoryException *e){
//		e->ReportError();
//		e->Delete();
//	}
//
//	for (int row = 0; row<m_pGridchro.GetRowCount(); row++) //行
//	for (int col = 0; col < m_pGridchro.GetColumnCount(); col++){    //列
//		GV_ITEM item;
//		item.mask = GVIF_TEXT | GVIF_FORMAT;
//		item.row = row;
//		item.col = col;
//	
//		if (row < 1){
//
//			item.nFormat = DT_LEFT | DT_WORDBREAK;
//			switch (col){
//			case 1:
//				item.szText.Format(_T("序号"));
//				break;
//			case 2:
//				item.szText.Format(_T("姓名"));
//				break;
//			case 3:
//				item.szText.Format(_T("分析细胞总数"));
//				break;
//			case 4:
//				item.szText.Format(_T("畸变细胞数"));
//				break;
//			case 5:
//				item.szText.Format(_T("畸变率"));
//				break;
//			case 6:
//				item.szText.Format(_T("染色体畸变"));
//				break;
//			case 16:
//				item.szText.Format(_T("单体型畸变"));
//				break;
//			}//swtich结束
//		}
//		else{ //else
//			item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//			item.szText.Format(_T(" "));
//
//		}//if结束
//
//		if (row==1){
//			switch (col){
//			case 6:
//				item.szText.Format(_T("Cu"));
//				break;
//			case 11:
//				item.szText.Format(_T("Cs"));
//				break;
//			case 14:
//				item.szText.Format(_T("合计"));
//				break;
//			case 16:
//				item.szText.Format(_T("ctg"));
//				break;
//			case 17:
//				item.szText.Format(_T("csg"));
//				break;
//			case 18:
//				item.szText.Format(_T("ctb"));
//				break;
//			case 19:
//				item.szText.Format(_T("cte"));
//				break;
//			case 20:
//				item.szText.Format(_T("合计"));
//				break;
//
//			}
//		}
//
//		if (row == 1){
//			switch (col){
//			case 6:
//				item.szText.Format(_T("dic"));
//				break;
//			case 7:
//				item.szText.Format(_T("tri"));
//				break;
//			case 8:
//				item.szText.Format(_T("ten"));
//				break;
//			case 9:
//				item.szText.Format(_T("ace"));
//				break;
//			case 10:
//				item.szText.Format(_T("r"));
//				break;
//			case 11:
//				item.szText.Format(_T("t"));
//				break;
//			case 12:
//				item.szText.Format(_T("inv"));
//				break;
//			case 13:
//				item.szText.Format(_T("del"));
//				break;
//			case 14:
//				item.szText.Format(_T("数量"));
//				break;
//			case 15:
//				item.szText.Format(_T("百分率(%)"));
//				break;
//			case 20:
//				item.szText.Format(_T("数量"));
//				break;
//			case 21:
//				item.szText.Format(_T("百分率(%)"));
//				break;
//			}
//		}
//
//		m_pGridchro.SetItem(&item);
//
//	}
//
//	m_pGridchro.AutoSize();
//	m_pGridchro.SetRowHeight(0, 3 * m_pGridchro.GetRowHeight(0) / 2);
//
//	CDC *pDC = m_pGridchro.GetDC();
//	CSize  cellSize;
//	GV_ITEM item;
//	item.mask = GVIF_TEXT | GVIF_FORMAT;
//	item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//	CRect tmpRect;
//	/*
//	下面的代码是用来合并单元格的
//	*/
//
//	if (m_pGridchro.GetSafeHwnd()){
//		m_pGridchro.SetFixedCellCombine(0, 1, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 2, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 3, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 4, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 10, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 16, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 22, 1, 5);
//	}
//
//}


BOOL CChromosomeHandle::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO:  在此添加专用代码和/或调用基类
	NMHEADER* pNMHeader = (NMHEADER*)lParam;
	if (((pNMHeader->hdr.code == HDN_BEGINTRACKW) |
		(pNMHeader->hdr.code == HDN_DIVIDERDBLCLICKW)))
	{
		*pResult = TRUE;
		return TRUE;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CChromosomeHandle::OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int row = m_listchroresult.GetSelectionMark();
	CString name = m_listchroresult.GetItemText(row, 0);
	ShowOneResult(name);
	*pResult = 0;
}
