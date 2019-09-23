// MicroNucleusHandle.cpp : 实现文件
//

#include "stdafx.h"
#include"resource.h"
#include <thread>

#include "GWSystem1.0.h"
#include "GWSystem1.0Dlg.h"
#include "MicroNucleusHandle.h"
#include"MicroNucleusAlgorithm.h"
#include "MicroNucleusResult.h"
#include "MNPrintReport.h"
#include "ReadAndWriteForAccess.h"
#include "Scanning_Control.h"

#include "afxdialogex.h"
#include <stdlib.h>
#include <direct.h>
#include"FileOperate.h"
#include<io.h>
#include "CSeries.h"

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"
//报表
//#include "CApplication.h" //word程序对象   
//#include "CDocuments.h" //文档集对象   
//#include "CDocument0.h" //docx对象 
//#include "CBookmarks.h" //
//#include "CBookmark0.h" //
//#include "CRange.h" //文档中的一个连续范围   


extern Scanning_Control *m_Scanning_Control;
extern CGWSystem10Dlg* pDlg;
extern vector <PatientWithSlide> tempAllPatients;
//全局变量
CMicroNucleusHandle * pHandleDlg;
CMicroNucleusResult * pMicroNucleusResult;
CReadAndWriteForAccess ReadAndWriteMN;
string StrFileSolve;
vector <CString> WaitingDeleteImg;
CString WaitingDeletePath;


// CMicroNucleusHandle 对话框


IMPLEMENT_DYNAMIC(CMicroNucleusHandle, CDialogEx)

CMicroNucleusHandle::CMicroNucleusHandle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMicroNucleusHandle::IDD, pParent)
{
	pMicroNucleusResult = NULL;
}

CMicroNucleusHandle::~CMicroNucleusHandle()
{
	delete pMicroHandleDlg;
	AfxMessageBox(_T("非模态对话框已销毁"));
}

void CMicroNucleusHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listmicrohandle);
	DDX_Control(pDX, IDC_TCHARTMN, m_chartmicro);
	DDX_Control(pDX, IDC_DATESTARTMN, m_datetimebegin);
	DDX_Control(pDX, IDC_DATEENDMN, m_datetimeend);
	DDX_Control(pDX, IDC_LIST_RESULTCHRO, m_resultchrolist);
}


BEGIN_MESSAGE_MAP(CMicroNucleusHandle, CDialogEx)
	ON_MESSAGE(WM_HANDLEPATH, &CMicroNucleusHandle::InitialList1) //自定义消息处理函数；
	ON_MESSAGE(WM_INIT_TABLEMN, &CMicroNucleusHandle::InitialList2) //自定义消息处理函数；
	ON_BN_CLICKED(IDC_BTN_MICROHANDLE, &CMicroNucleusHandle::OnBnClickedBtnMicrohandle)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMicroNucleusHandle::OnNMClickList1)
	ON_BN_CLICKED(IDC_BTN_MICROSOLVE, &CMicroNucleusHandle::OnBnClickedBtnMicrosolve)
	ON_BN_CLICKED(IDC_BTN_MICROPRINT, &CMicroNucleusHandle::OnBnClickedBtnMicroprint)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMicroNucleusHandle::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMicroNucleusHandle::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BTN_FINDMN, &CMicroNucleusHandle::OnBnClickedBtnFindmn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMicroNucleusHandle::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULTCHRO, &CMicroNucleusHandle::OnNMClickListResultchro)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMicroNucleusHandle 消息处理程序

BOOL CMicroNucleusHandle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pHandleDlg = this;//指向该对话框的全局指针
	//连接数据库
	m_Conn.OnInitADOAccess();
	GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰
	GetDlgItem(IDC_BTN_MICROSOLVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MICROPRINT)->EnableWindow(FALSE);

	//删除原数据表
	DeleteAccessTable();
	//设置表头
	CRect rectCtrl;
	CStatic *p = (CStatic*)GetDlgItem(IDC_LIST1);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listmicrohandle.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listmicrohandle.InsertColumn(0, _T("是否分析"), LVCFMT_CENTER, 80, 0);
	m_listmicrohandle.InsertColumn(1, _T("序号"), LVCFMT_CENTER, rectCtrl.Width()*0.107, 0);
	m_listmicrohandle.InsertColumn(2, _T("病人名称"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(3, _T("图片总数"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(4, _T("分析进度"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(5, _T("拍图时间"), LVCFMT_CENTER, rectCtrl.Width()*0.25);
	m_listmicrohandle.InsertColumn(6, _T("报表生成情况"), LVCFMT_CENTER, rectCtrl.Width()*0.214);
	//m_listmicrohandle.InsertColumn(7, _T("报表生成情况"), LVCFMT_CENTER, 100, 3);
	m_listmicrohandle.DeleteColumn(0); //删除第0列

	//设置结果表表头
	p = (CStatic*)GetDlgItem(IDC_LIST_RESULTCHRO);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_resultchrolist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_resultchrolist.InsertColumn(0, _T("是否分析"), LVCFMT_CENTER, 80, 0);
	m_resultchrolist.InsertColumn(1, _T("姓名"), LVCFMT_CENTER, rectCtrl.Width()*0.091, 0);
	m_resultchrolist.InsertColumn(2, _T("单核细胞数"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(3, _T("单核MNC(数)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(4, _T("单核MNC率(‰)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(5, _T("单核MN(数)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(6, _T("单核MN率(‰)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);

	m_resultchrolist.InsertColumn(7, _T("双核细胞数"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(8, _T("双核MNC(数)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(9, _T("双核MNC率(‰)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(10, _T("双核MN(数)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(11, _T("双核MN率(‰)"), LVCFMT_CENTER, rectCtrl.Width()*0.09);


	m_resultchrolist.DeleteColumn(0); //删除第0列

	//设置行高：
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listmicrohandle.SetImageList(&m_l, LVSIL_SMALL);
	m_resultchrolist.SetImageList(&m_l, LVSIL_SMALL);

	//设置字体：
	CFont m_font;
	m_font.CreatePointFont(120, _T("新宋体"));
	m_listmicrohandle.SetFont(&m_font);
	//m_resultchrolist.SetFont(&m_font);
	GetDlgItem(IDC_LISTHEADMN)->SetFont(&m_font);
	//获取路径
	//m_handlepath.GetWindowTextW(ReadPath);
	//m_resultpath.GetWindowTextW(WritePath);

	//设置时间控件的格式
	m_datetimebegin.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	m_datetimeend.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	COleDateTime MinTime(COleDateTime::GetCurrentTime() - COleDateTimeSpan(30, 0, 0, 0));
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	m_datetimebegin.SetRange(&MinTime, &MaxTime);
	m_datetimeend.SetRange(&MinTime, &MaxTime);

	//建立处理结果窗口 但隐藏
	if (pMicroNucleusResult == NULL){//判定对话框是否有所指向
		pMicroNucleusResult = new CMicroNucleusResult(this);//指向一个非模对话框示例
		pMicroNucleusResult->Create(IDD_MICRONUCLEUSRESULT, this);//创建
	}
	pMicroNucleusResult->ShowWindow(SW_HIDE);//不显示
	pMicroNucleusResult->m_comboxpatient.ResetContent();


	//将本次扫描信息导入列表 并选中
	LoadCurrentToList();

	CRect rect;
	GetClientRect(&rect); //取客户区大小
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//自定义消息函数1----传递处理文件夹路径与储存文件夹路径
LRESULT CMicroNucleusHandle::InitialList1(WPARAM wParam, LPARAM lParam)
{
	MyFiles *pA = (MyFiles*)wParam;
	CString  str;
	//str = pA->picsolve.c_str();
	//m_handlepath.SetWindowText(str);
	//str = pA->endsolve.c_str();
	//m_resultpath.SetWindowText(str);
	return 0;

	//一定得有个返回值

}

//自定义消息函数2----传递处理文件夹路径与储存文件夹路径
LRESULT CMicroNucleusHandle::InitialList2(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString mystr(b);
	SysFreeString(b);

	pHandleDlg->m_listmicrohandle.SetItemText(lParam, 3, mystr);
	pHandleDlg->UpdateData(FALSE);

	return 0;//一定得有个返回值

}

//开始分析
void CMicroNucleusHandle::OnBnClickedBtnMicrohandle()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空本次分析的数据库
	GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰

	//1.获取所选中的病人数组，加入AllPatientsChose
	int nItem, i;
	nItem = m_listmicrohandle.GetItemCount();
	for (i = 0; i < nItem; ++i)
	{
		if (m_listmicrohandle.GetCheck(i))
		{
			AllPatientsChose.push_back(AllPatients[i]);
			Row.push_back(i);
		}
	}

	//2.开始分析
	thread MNAnalyse(ThreadProcWaitMN);
	MNAnalyse.detach();
	//ThreadWait = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcWaitMN, NULL, 0, &ThreadID);
}

void  CMicroNucleusHandle::ThreadProcWaitMN()
{

	// 初始化Python
	//在使用Python系统前，必须使用Py_Initialize对其
	//进行初始化。它会载入Python的内建模块并添加系统路
	//径到模块搜索路径中。这个函数没有返回值，检查系统
	//是否初始化成功需要使用Py_IsInitialized。
	//Py_Initialize();
	//if (!Py_IsInitialized())
	//{
	//	return;
	//}

	//PyObject * pModule = NULL;
	//PyObject * pFunc = NULL;
	//
	////PyRun_SimpleString("import sys");
	////PyRun_SimpleString("sys.path.append('./')");

	//PyObject* pName = PyUnicode_FromString("test0719");
	//pModule = PyImport_Import(pName);
	//cout << "test0719:" << pModule;
	//pFunc = PyObject_GetAttrString(pModule, "classifier_pridictor_keras");
	//if (!pFunc || !PyCallable_Check(pFunc))
	//{
	//	printf("can't find function [classifier_pridictor]");
	//	getchar();
	//}
	//else
	//{
	//	cout << "open function [classifier_pridictor]" << endl;
	//}



	//init_numpy();
	// 检查初始化是否成功


	//显示python版本信息
	//cout << (Py_GetVersion()) << "\n";
	//PyObject* sys = PyImport_ImportModule("sys");
	//PyRun_SimpleString("import  sys");

	//PyRun_SimpleString("print('come in python')");
	//PyRun_SimpleString("import  sys"); // 执行 python 中的短语句  
	//PyRun_SimpleString("sys.path.append('./')");
	//PyRun_SimpleString("import cv2");

	////PyRun_SimpleString("import  sys"); // 执行 python 中的短语句  
	////PyRun_SimpleString("sys.path.append('./')");
	////PyRun_SimpleString("import cv2");

	//// load python script
	//PyObject* pName = PyUnicode_FromString("predictSX");
	//PyObject* pModule = PyImport_Import(pName);
	//if (!pModule) {
	//	//std::cout << "can't find predict.py" << std::endl;
	//	//getchar();
	//	return;
	//}
	//else
	//{
	//	//cout << "open Module" << endl;
	//}
	//PyObject* pDict = PyModule_GetDict(pModule);
	//if (!pDict)
	//{
	//	return;
	//}
	//pFunc = PyDict_GetItemString(pDict, "classifier_pridictor");
	//if (!pFunc || !PyCallable_Check(pFunc))
	//{
	//	//printf("can't find function [classifier_pridictor]");
	//	//getchar();
	//	return;
	//}
	//else
	//{
	//	//cout << "open function [classifier_pridictor]" << endl;
	//}

	//生成处理时间

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
		int row = pHandleDlg->Row[i];
		pHandleDlg->patientNewImg = true;
		//(1)生成该病人的处理结果文件夹
		CString name = pHandleDlg->AllPatientsChose[i].PatientName; //定义时间格式
		pHandleDlg->StrName = pHandleDlg->AllPatientsChose[i].ResultPath + name + "-" + pHandleDlg->StrTime;
		string strname = CT2A(pHandleDlg->StrName.GetBuffer());
		StrFileSolve = pHandleDlg->strFileSolve + strname + "\\";//生成结果的文件名 
		const char* c_filesolve = StrFileSolve.c_str();//将路径转换成 const char
		_mkdir(c_filesolve);

		//(2)开始对该病人的图片进行分析
		CMicroNucleusAlgorithm algorithm;
		CString mystr;
		mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].MicroImgNames.size());
		MN_HandleResult *pB = new MN_HandleResult; //该病人分析结果存储结构体
		memset(pB->SingleCellsWithMN, 0, sizeof(pB->SingleCellsWithMN));
		memset(pB->DoubleCellsWithMN, 0, sizeof(pB->DoubleCellsWithMN));
		memset(pB->MultiCellsWithMN, 0, sizeof(pB->MultiCellsWithMN));
		memset(pB->TripleCellsWithMN, 0, sizeof(pB->TripleCellsWithMN));

		PatientAndQuaImg thistime;
		thistime.patientname = name;
		USES_CONVERSION;
		thistime.QuestionImgPath = A2W((StrFileSolve).c_str());

		pMicroNucleusResult->m_comboxpatient.AddString(name);
		//pMicroNucleusResult->AllQuestionImgPath.push_back(thistime);
		pMicroNucleusResult->PatientNames.push_back(name);

		size_t newStart = 0;
		int deletedPicNum = 0;

		while (pHandleDlg->patientNewImg)//判断该病人是否有新的图片加入
		{
			for (size_t j = newStart; j < pHandleDlg->AllPatientsChose[i].MicroImgNames.size(); j++)
			{
				string readpath = CT2A(pHandleDlg->AllPatientsChose[i].ImgPath.GetBuffer());
				string imgname = CT2A(pHandleDlg->AllPatientsChose[i].MicroImgNames[j].GetBuffer());
				

				string picpath = readpath + "\\" + imgname;
				Mat image = imread(picpath);//37 38 40 43 44 45
				if (image.empty())
				{
					j--;
					Sleep(100);
					continue;
				}

				//处理过程文件堆放的文件夹
				pHandleDlg->strFileProcess = StrFileSolve + "过程文件\\";
				const char* c_fileprocess = pHandleDlg->strFileProcess.c_str();//将路径转换成 const char
				_mkdir(c_fileprocess);

				pHandleDlg->AllPatientsChose[i].Result = algorithm.handlemicronucleus(readpath, imgname, StrFileSolve, pHandleDlg->AllPatientsChose[i].PatientName, pB, pHandleDlg->strFileProcess);
				deletedPicNum = pHandleDlg->AllPatientsChose[i].Result->delPictureNum;
				pHandleDlg->AllPatientsChose[i].Result->patientname = pHandleDlg->AllPatientsChose[i].PatientName;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].MicroImgNames.size() + 1;
				pHandleDlg->AllPatientsChose[i].Result->sourcefile = pHandleDlg->AllPatientsChose[i].ImgPath;
				pHandleDlg->AllPatientsChose[i].Result->resultfile = pHandleDlg->AllPatientsChose[i].ResultPath;

				//删除处理过程文件堆放的文件夹
				CFileOperate x;
				/*x.DeleteFolder(A2W((pHandleDlg->strFileProcess).c_str()));*/

				CString mystr1, mystr2;
				mystr1.Format(_T("%d"), j + 1);
				mystr2 = mystr1 + _T("/") + mystr;

				HWND   hwnd = ::FindWindow(NULL, _T("微核处理"));//调用消息处理函数刷新页面
				::SendMessage(hwnd, WM_INIT_TABLEMN, (WPARAM)mystr2.AllocSysString(), (WPARAM)row);//线程中传递定时器消息，以开启定时器，刷新显示照片
			}

			//查询该病人的图片是否在增加
			vector<CString>patientNewImgPath = pHandleDlg->InquiryPatientImg(pHandleDlg->AllPatientsChose[i].PatientName, pHandleDlg->AllPatientsChose[i].ImgPath);
			int patientNewImgNum = patientNewImgPath.size();
			if (patientNewImgNum > pHandleDlg->AllPatientsChose[i].MicroImgNames.size())
			{
				pHandleDlg->patientNewImg = true;
				newStart = pHandleDlg->AllPatientsChose[i].MicroImgNames.size();
				pHandleDlg->AllPatientsChose[i].MicroImgNames = patientNewImgPath;
				mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].MicroImgNames.size());
			}
			else
			{
				//文件夹内图片数量不变，检查图片文件夹是否是当前扫描  如果是，看是否已经扫描完成
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


				//判断当前分析病人的文件夹是否和扫描文件夹相同

			}
		}

		if (pHandleDlg->AllPatientsChose[i].Result->patientname != _T("Ctrl+"))
		{
			//开辟线程删除处理完的图片
			//WaitingDeletePath = pHandleDlg->AllPatientsChose[i].ImgPath;
			//WaitingDeleteImg = pHandleDlg->AllPatientsChose[i].MicroImgNames;
			//thread DelImage(DeltePatientImage);
			//DelImage.detach();

			//计算所需要的参数
			pHandleDlg->CalculateMNParaInExcel(pB);

			//将分析结果存入数据库中
			ReadAndWriteMN.SaveToAccessMNAnalysis(pHandleDlg->AllPatientsChose[i].Result);

			//数据库中，将是否已经分析改为是

			//将表格中改成报表已生成
			CString mystr3 = _T("√");
			pHandleDlg->m_listmicrohandle.SetItemText(row, 5, mystr3);
			//可以生成该病人的报表
			pHandleDlg->MNJoinWaitToPrint(pHandleDlg->AllPatientsChose[i].Result->patientname, StrFileSolve);
			//图中显示该病人的分析结果
			pHandleDlg->ShowOneResult(pHandleDlg->AllPatientsChose[i].Result->patientname);
			//表中显示该病人的分析结果
			pHandleDlg->ShowOneResultInTable(pHandleDlg->AllPatientsChose[i].Result->patientname);

			pHandleDlg->ResultRow++;

			pHandleDlg->GetDlgItem(IDC_BTN_MICROPRINT)->EnableWindow(TRUE);
		}


	}
	pHandleDlg->GetDlgItem(IDC_BTN_MICROSOLVE)->EnableWindow(TRUE);


}

////分析线程
//DWORD WINAPI ThreadProcWaitMN(LPVOID pParam)
//{
//	//4.微核分析开始
//
//
//	return 0;
//
//}




// 将一个病人的分析结果显示到图中
bool CMicroNucleusHandle::ShowOneResult(CString patientname)
{
	MN_HandleResult pA;
	pA = ReadAndWriteMN.ReadOneMNResultFromAccess(patientname);

	ClearAllSeries();
	CSeries barSeries = (CSeries)m_chartmicro.Series(0);
	barSeries.AddXY((double)2, (double)pA.singlecell, _T("单核细胞数_校正"), RGB(255, 0, 255));
	barSeries.AddXY((double)3, (double)pA.doublecell, _T("双核细胞数_校正"), RGB(0, 255, 255));
	barSeries.AddXY((double)4, (double)pA.multiplecell, _T("多核细胞数_校正"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.singleMN, _T("单核MN_校正"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.DoubleMN, _T("双核MN_校正"), RGB(0, 255, 255));
	barSeries.AddXY((double)7, pA.MultiMN, _T("多核MN_校正"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.singlecell_wh, _T("单核MNC_校正"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.doublecell_wh, _T("双核MNC_校正"), RGB(0, 255, 255));
	barSeries.AddXY((double)7, pA.multiplecell_wh, _T("多核MNC_校正"), RGB(0, 255, 0));

	//UpdateData(false);
	return false;
}

// 将一个病人的分析结果显示到表中
bool CMicroNucleusHandle::ShowOneResultInTable(CString patientname)
{
	MN_HandleResult pA;
	pA = ReadAndWriteMN.ReadOneMNResultFromAccess(patientname);

	int j = 0;
	CString  mystr;
	mystr = pA.patientname;
	m_resultchrolist.InsertItem(ResultRow, mystr);
	mystr.Format(_T("%d"), pA.singlecell);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.SingleMNC_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.SingleMNC_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.SingleMN_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.SingleMN_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);

	mystr.Format(_T("%d"), pA.doublecell);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.DoubleMNC_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.DoubleMNC_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr );
	mystr.Format(_T("%d"), pA.DoubleMN_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.DoubleMN_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);

	//UpdateData(false);
	return false;
}



//清空图表
void CMicroNucleusHandle::ClearAllSeries(void)
{
	for (long i = 0; i<m_chartmicro.get_SeriesCount(); i++)
	{
		((CSeries)m_chartmicro.Series(i)).Clear();
	}

}




//单击列表事件
void CMicroNucleusHandle::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码


	*pResult = 0;
}

//查看处理结果
void CMicroNucleusHandle::OnBnClickedBtnMicrosolve()
{
	// TODO:  在此添加控件通知处理程序代码
	pMicroNucleusResult->ShowWindow(SW_SHOW);//不显示

}

//清空数据库中的一些表格
void CMicroNucleusHandle::DeleteAccessTable()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("delete  * from 微核图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete  * from 微核图像分析结果数据表（已删除）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	//删除原数据表
	sql = _T("delete  * from 微核分析结果数据表（本次）");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

//打印本次分析的报告
void CMicroNucleusHandle::OnBnClickedBtnMicroprint()
{
	// TODO:  在此添加控件通知处理程序代码
	CMNPrintReport *pMNPrintReport = new CMNPrintReport;
	pMNPrintReport->Create(IDD_PRINTREPORTMN, this);
	//将当前可打印的病人传到打印界面以初始化
	HWND   hwnd = ::FindWindow(NULL, _T("打印分析报告（微核）"));//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_WAITTOPRINTMN, (WPARAM)&MNWaitPrint, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片

	pMNPrintReport->ShowWindow(SW_SHOW);

}

// 加入可生成报表的结构体
void CMicroNucleusHandle::MNJoinWaitToPrint(CString patientname, string savepath)
{
	WaitToPrint mnWaitToPrint;
	mnWaitToPrint.patientname = patientname;
	mnWaitToPrint.savepath = savepath;
	MNWaitPrint.push_back(mnWaitToPrint);
}


//某个项已经发生变化
void CMicroNucleusHandle::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nItem, i;
	nItem = m_listmicrohandle.GetItemCount();

	bool mark = false;
	for (i = 0; i < nItem && !mark; ++i)
	{
		if (m_listmicrohandle.GetCheck(i))
		{
			mark = true;
		}
	}
	if (mark)
	{
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(TRUE);//有list选项选中后使删除按钮可用
	}
	else
	{
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰
	}

	*pResult = 0;
}

//双击列表事件
void CMicroNucleusHandle::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int row = m_listmicrohandle.GetSelectionMark();
	CString name = m_listmicrohandle.GetItemText(row, 1);
	ShowOneResult(name);

	*pResult = 0;
}

//查询时间段内的图片
void CMicroNucleusHandle::OnBnClickedBtnFindmn()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_FINDMN)->EnableWindow(FALSE);//无lisr选项选中使删除按钮变灰

	//是否有本次扫描的图片 如果有 加入
	LoadCurrentToList();


	//1.获得相应时间段内的玻片图片
	COleDateTime TimeBegin, TimeEnd;
	m_datetimebegin.GetTime(TimeBegin);
	m_datetimeend.GetTime(TimeEnd);
	CReadAndWriteForAccess xx;
	vector<Table_SlideSet> ReadSlideset;
	ReadSlideset = xx.QuerySaveTimeMN(TimeBegin, TimeEnd);

	//2.遍历每张拨片的路径，合并相同路径，得到路径集合数组
	vector<CString>AllMNImgPath;
	vector<CString>AllMNResPath;
	vector<COleDateTime>AllMNTime;
	for (auto iter : ReadSlideset)
	{
		vector<CString>::iterator ret;
		ret = std::find(AllMNImgPath.begin(), AllMNImgPath.end(), iter.path);
		if (ret == AllMNImgPath.end())
		{
			AllMNImgPath.push_back(iter.path);
			AllMNResPath.push_back(iter.resultpath);
			AllMNTime.push_back(iter.time);
		}
	}

	//3.将路径集合数组里面的路径下的病人图片进行归类，显示到表格
	for (size_t pathNum_i = 0; pathNum_i < AllMNImgPath.size(); pathNum_i++)
	{
		int picNum = 0;
		vector<CString> vecEmpty;
		AllMicroImg.swap(vecEmpty);//清空每一轮查找的图

		//（1）筛选所有病人的微核图片，并放入结构体
		USES_CONVERSION;
		m_Scanning_Control->critical_section.Lock();
		char* charReadPath = T2A(AllMNImgPath[pathNum_i]);
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
				if (str1 == _T("微核"))
					AllMicroImg.push_back(OneImgName);
			} while (FindNextFile(handle, &file));
		}
		m_Scanning_Control->critical_section.Unlock();
		FindClose(handle);

		//(2)建立各个病人与图片的对应结构体
		BOOL havepatientornot;
		CString strDst;
		vector <MicroTable> tempMicroAllPatients;

		for (auto iter : AllMicroImg)
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
			for (auto iterpatient : tempMicroAllPatients)
			{
				if (iterpatient.PatientName == strDst)
				{
					havepatientornot = TRUE;
					tempMicroAllPatients[picNum].MicroImgNames.push_back(iter);
				}
				picNum++;
			}
			if (!havepatientornot)//如果没有该病人
			{
				MicroTable patient;
				patient.PatientName = strDst;
				patient.MicroImgNames.push_back(iter);
				patient.ImgPath = AllMNImgPath[pathNum_i];
				patient.ResultPath = AllMNResPath[pathNum_i];
				tempMicroAllPatients.push_back(patient);
			}

		}

		//（3）将病人名称与图片总数显示到表格

		for (size_t k = 0; k < tempMicroAllPatients.size(); k++)
		{
			int j = 0;
			CString  mystr;
			mystr.Format(_T("%d"), patientRow + 1);
			m_listmicrohandle.InsertItem(patientRow, mystr);
			m_listmicrohandle.SetItemText(patientRow, ++j, tempMicroAllPatients[k].PatientName);
			mystr.Format(_T("%d"), tempMicroAllPatients[k].MicroImgNames.size());
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("0/") + mystr;
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = AllMNTime[pathNum_i].Format();
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("未生成");
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			patientRow++;
			AllPatients.push_back(tempMicroAllPatients[k]);
		}



	}


	UpdateData(FALSE);


}


void CMicroNucleusHandle::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMicroNucleusHandle::DrawText(int nItem,
	int nSubItem,
	CDC *pDC,
	COLORREF crText,
	COLORREF crBkgnd,
	CRect &rect)
{
	ASSERT(pDC);
	pDC->FillSolidRect(&rect, crBkgnd);

	CListCtrl x;
	int nProcess = x.GetItemData(nItem);
	CRect procRect = rect;
	pDC->Rectangle(procRect);

	procRect.left += 1;
	procRect.bottom -= 1;
	procRect.top += 1;
	procRect.right = procRect.left + rect.Width() * nProcess / 100;
	CBrush brush(RGB(255, 0, 0));
	pDC->FillRect(&procRect, &brush);

	CString str;
	str.Format(_T("%d%%"), nProcess);

	if (!str.IsEmpty())
	{
		UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_CENTER;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, nFormat);
	}
}

// 查找指定文件夹下病人图片的数量
vector <CString> CMicroNucleusHandle::InquiryPatientImg(CString patientname, CString imgpath)
{
	vector <CString>().swap(AllMicroImg);
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
			if (str1 == _T("微核"))
				AllMicroImg.push_back(OneImgName);
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	//(2)建立各个病人与图片的对应结构体
	CString strDst;
	MicroTable patient;

	for (auto iter : AllMicroImg)
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
			patient.MicroImgNames.push_back(iter);
		}

	}


	return patient.MicroImgNames;
}


// 从数据库中获取所有该病人所对应的玻片号
vector<int> CMicroNucleusHandle::GetPatientSlideNumFromAccess(CString patientname)
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

//计算所需要的参数
void CMicroNucleusHandle::CalculateMNParaInExcel(MN_HandleResult *pB)
{
	int SingleMNC_Num = 0;
	float SingleMNC_Rate = 0;
	int SingleMN_Num = 0;
	float SingleMN_Rate = 0;

	int DoubleMNC_Num = 0;
	float DoubleMNC_Rate = 0;
	int DoubleMN_Num = 0;
	float DoubleMN_Rate = 0;

	for (size_t i = 0; i < sizeof(pB->SingleCellsWithMN)/4; i++)
	{
		pB->singleMN += i*pB->SingleCellsWithMN[i];
	}
	pB->SingleMNC_Num = pB->singlecell_wh;
	pB->SingleMN_Num = pB->singleMN;
	if (pB->singlecell == 0)
	{
		pB->SingleMNC_Rate = 0;
		pB->SingleMN_Rate = 0;
	}
	else{
		pB->SingleMNC_Rate = ((float)pB->SingleMNC_Num / pB->singlecell)*1000;
		pB->SingleMN_Rate = ((float)pB->SingleMN_Num / pB->singlecell)*1000;
	}

	for (size_t i = 0; i < sizeof(pB->DoubleCellsWithMN)/4; i++)
	{
		pB->DoubleMN += i*pB->DoubleCellsWithMN[i];
	}
	pB->DoubleMNC_Num = pB->doublecell_wh;
	pB->DoubleMN_Num = pB->DoubleMN;

	if (pB->doublecell == 0)
	{
		pB->DoubleMNC_Rate = 0;
		pB->DoubleMN_Rate = 0;
	}
	else{
		pB->DoubleMNC_Rate = ((float)pB->DoubleMNC_Num / pB->doublecell)*1000;
		pB->DoubleMN_Rate = ((float)pB->DoubleMN_Num / pB->doublecell)*1000;
	}




}

//将本次扫描信息导入列表 并选中
bool CMicroNucleusHandle::LoadCurrentToList()
{
	m_listmicrohandle.DeleteAllItems();
	vector<MicroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//清空每一轮查找的图

	for (size_t i = 0; i < tempAllPatients.size(); i++)
	{
		if (tempAllPatients[i].mode == _T("微核"))
		{
			MicroTable patient;
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
		m_listmicrohandle.InsertItem(patientRow, mystr);
		m_listmicrohandle.SetItemText(patientRow, ++j, AllPatients[k].PatientName);
		mystr.Format(_T("%d"), AllPatients[k].MicroImgNames.size());
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("0/") + mystr;
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("当前扫描");
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("未生成");
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		patientRow++;



		//选中
		int nItem, i;
		nItem = m_listmicrohandle.GetItemCount();


		for (i = 0; i < nItem; i++)
		{
			m_listmicrohandle.SetCheck(i);

		}
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(TRUE);//有list选项选中后使删除按钮可用

	}

	return true;
}

//单击切换结果
void CMicroNucleusHandle::OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int row = m_resultchrolist.GetSelectionMark();
	CString name = m_resultchrolist.GetItemText(row, 0);
	ShowOneResult(name);
	*pResult = 0;
}


void CMicroNucleusHandle::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();
	}
	// TODO:  在此处添加消息处理程序代码
}

void CMicroNucleusHandle::ReSize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta); //取客户区大小
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD); //列出所有控件
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;

}

BOOL CMicroNucleusHandle::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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

void CMicroNucleusHandle::DeltePatientImage()
{
	for (size_t i = 0; i < WaitingDeleteImg.size(); i++)
	{
		char sourcePicPath[MAX_PATH + 1];
		WCHAR   wstr_sourpic[MAX_PATH];
		String Path = CT2A((WaitingDeletePath + WaitingDeleteImg[i]).GetBuffer());
		sprintf_s(sourcePicPath, "%s", Path.c_str());
		MultiByteToWideChar(CP_ACP, 0, sourcePicPath, -1, wstr_sourpic, sizeof(wstr_sourpic));
		int del = DeleteFile(wstr_sourpic);

		if (!del)
			AfxMessageBox(_T("删除失败"));

	}

}
