
// GWSystem1.0Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0Dlg.h"
#include "afxdialogex.h"
#include<thread>

#include<atlimage.h>


//用户管理
#include "UserMangement.h"
#include "UserLoad.h"
#include "CodeAmend.h"
#include "DiaryManagement.h"

//显示所有玻片扫描状态
#include"ShowAllSlides.h"
#include"SlideSet.h"

//扫描与显示
#include"ScanAndSaveImg.h"
#include "ReadAndWriteForAccess.h"
#include"MainWindowImgShow.h"

//微核与染色体分析
#include "MicroNucleusHandle.h"
#include "CChromosomeHandle.h"

//硬件
#include "MotorControlDlg.h"
#include "CameraParaSetDlg.h"
//#include "ScanSet.h"
#include"FocusSet.h"
#include "AdminSet.h"
#include "AlgorithmParaSet.h"
#include "HardwareInit.h"
#include "AllParaSet.h"


#include<string>

using namespace std;
using std::string;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局变量
CGWSystem10Dlg* pDlg;
CShowAllSlides ShowAllSlide;
CScanAndSaveImg ScanAndSave;
CMainWindowImgShow  MainWinImgShow;
Scanning_Control *m_Scanning_Control = new Scanning_Control;
CHardwareInit HardwareInit;
vector <PatientWithSlide> tempAllPatients;

//参数传递
static ALL_PAREMETER *allpara = new ALL_PAREMETER;
//static ALL_PAREMETER *s_Scanning_Para = new ALL_PAREMETER;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGWSystem10Dlg 对话框



CGWSystem10Dlg::CGWSystem10Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CGWSystem10Dlg::IDD, pParent),BStop(TRUE)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CGWSystem10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_EDIT5, m_edit5);
	DDX_Control(pDX, IDC_EDIT6, m_edit6);
	DDX_Control(pDX, IDC_EDIT7, m_edit7);
	DDX_Control(pDX, IDC_EDIT8, m_edit8);
	
	DDX_Control(pDX, IDC_EDIT_PROGRESS1, m_edit11);
	DDX_Control(pDX, IDC_EDIT_PROGRESS2, m_edit22);
	DDX_Control(pDX, IDC_EDIT_PROGRESS3, m_edit33);
	DDX_Control(pDX, IDC_EDIT_PROGRESS4, m_edit44);
	DDX_Control(pDX, IDC_EDIT_PROGRESS5, m_edit55);
	DDX_Control(pDX, IDC_EDIT_PROGRESS6, m_edit66);
	DDX_Control(pDX, IDC_EDIT_PROGRESS7, m_edit77);
	DDX_Control(pDX, IDC_EDIT_PROGRESS8, m_edit88);

	DDX_Control(pDX, IDC_FRAME1, m_frame1);
	DDX_Control(pDX, IDC_FRAME2, m_frame2);
	DDX_Control(pDX, IDC_FRAME3, m_frame3);
	DDX_Control(pDX, IDC_FRAME4, m_frame4);
	DDX_Control(pDX, IDC_FRAME5, m_frame5);
	DDX_Control(pDX, IDC_FRAME6, m_frame6);
	DDX_Control(pDX, IDC_FRAME7, m_frame7);
	DDX_Control(pDX, IDC_FRAME8, m_frame8);

	DDX_Control(pDX, IDC_ALLFRAMES, m_allframe);
	DDX_Control(pDX, IDC_PROGRESSSHOW, m_progressDraw);

	DDX_Control(pDX, IDC_BTN_CAPTUREIMG, m_bbtn1);
	DDX_Control(pDX, IDC_BTN_RESET, m_bbtn2);
	DDX_Control(pDX, IDC_BTN_PARASET, m_bbtn3);
	DDX_Control(pDX, IDC_BTN_HANDLE, m_bbtn4);
}

BEGIN_MESSAGE_MAP(CGWSystem10Dlg, CDialogEx)
	ON_MESSAGE(WM_USER_LOAD, &CGWSystem10Dlg::InitialList1) //自定义消息处理函数；
	ON_MESSAGE(WM_MN_BTN_OK, &CGWSystem10Dlg::InitialList2) //自定义消息处理函数；
	ON_MESSAGE(WM_SENDTIMER, &CGWSystem10Dlg::InitialList3) //自定义消息处理函数；
	ON_MESSAGE(WM_USER_ScanSet, &CGWSystem10Dlg::InitialList4) //自定义消息处理函数；
	ON_MESSAGE(WM_USER_FocusSet, &CGWSystem10Dlg::InitialList5) //自定义消息处理函数；
	ON_MESSAGE(WM_USER_AdminFocusSet, &CGWSystem10Dlg::InitialList6) //自定义消息处理函数；
	ON_MESSAGE(WM_USER_AdminMotorSet, &CGWSystem10Dlg::InitialList7) //自定义消息处理函数；
	ON_MESSAGE(WM_USER_AdminScanSet, &CGWSystem10Dlg::InitialList8) //自定义消息处理函数；
	ON_MESSAGE(WM_CURRENTPOSITION, &CGWSystem10Dlg::InitialList9) //自定义消息处理函数；
	ON_MESSAGE(WM_START_HANDOPERATION, &CGWSystem10Dlg::InitialList10) //自定义消息处理函数；

	//ON_MESSAGE(WM_DISPLAY_DRAWING, &CGWSystem10Dlg::Updata_Display_Drawing)//添加消息映射

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BTN_LOAD, &CGWSystem10Dlg::OnBnClickedBtnLoad)
	ON_COMMAND(ID_MENUDIARYMANAGE, &CGWSystem10Dlg::OnDiaryManagement)
	ON_COMMAND(ID_USERMANAGE, &CGWSystem10Dlg::OnUserManagement)
	ON_COMMAND(ID_MENUCODERESET, &CGWSystem10Dlg::OnResetCode)
	ON_BN_CLICKED(IDC_BTN_SCANBEGIN, &CGWSystem10Dlg::OnBnClickedBtnScanbegin)
	ON_BN_CLICKED(IDC_BTN_SLIDESET, &CGWSystem10Dlg::OnBnClickedBtnSlideset)
	ON_BN_CLICKED(IDC_BTN_LASTPAGE, &CGWSystem10Dlg::OnBnClickedBtnLastpage)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, &CGWSystem10Dlg::OnBnClickedBtnNextpage)

	
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_IKAROS, &CGWSystem10Dlg::OnBnClickedBtnIkaros)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_IKAROSRST, &CGWSystem10Dlg::OnBnClickedBtnIkarosrst)
	ON_COMMAND(ID_MENU_CAMERA, &CGWSystem10Dlg::OnMenuCamera)
	ON_COMMAND(ID_MENU_PLATFORMSET, &CGWSystem10Dlg::OnMenuPlatformset)
	ON_COMMAND(ID_MENU_FOCUS, &CGWSystem10Dlg::OnMenuFocus)
	ON_COMMAND(ID_MENU_ADMIN, &CGWSystem10Dlg::OnMenuAdmin)
	ON_COMMAND(ID_MENU_ANALYSE, &CGWSystem10Dlg::OnMenuAnalyse)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGWSystem10Dlg::OnBnClickedBtnStop)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENUUSEREXIT, &CGWSystem10Dlg::OnMenuuserexit)


	ON_BN_CLICKED(IDC_BTN_CAPTUREIMG, &CGWSystem10Dlg::OnBnClickedBtnCaptureimg)
	ON_BN_CLICKED(IDC_BTN_RESET, &CGWSystem10Dlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_PARASET, &CGWSystem10Dlg::OnBnClickedBtnParaset)
	ON_BN_CLICKED(IDC_BTN_HANDLE, &CGWSystem10Dlg::OnBnClickedBtnHandle)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGWSystem10Dlg 消息处理程序

BOOL CGWSystem10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	ATL::CImage img;
	img.Load(_T(".\\res\\camera.jpg"));
	CBitmap* hBitmap1 = new CBitmap;
	hBitmap1->Attach(img.Detach());
	m_bbtn1.SetBkPicture(hBitmap1);

	img.Load(_T(".\\res\\edit_undo.jpg"));
	CBitmap* hBitmap2 = new CBitmap;
	hBitmap2->Attach(img.Detach());
	m_bbtn2.SetBkPicture(hBitmap2);

	img.Load(_T(".\\res\\setting.jpg"));
	CBitmap* hBitmap3 = new CBitmap;
	hBitmap3->Attach(img.Detach());
	m_bbtn3.SetBkPicture(hBitmap3);

	img.Load(_T(".\\res\\game_pad.jpg"));
	CBitmap* hBitmap4 = new CBitmap;
	hBitmap4->Attach(img.Detach());
	m_bbtn4.SetBkPicture(hBitmap4);




	pDlg = this;//指向该对话框的全局指针
	theApp.dlg_login.JudgeType(theApp.dlg_login.user);

	ShowAllSlide.OnInitDialog();
	MainWinImgShow.PutPicControlIntoVector();	//初始将picture控件放入数组

	////硬件初始化
	//HardwareInit.InitHardwarePara(allpara);


	//HWND hWnd;
	//hWnd = GetDlgItem(IDC_VIDEOFRAME)->GetSafeHwnd();
	//HardwareInit.OnInitDialog(hWnd);

	////数据库中读到的数据传给硬件
	//AllParaSet HardPara;
	//HardPara.HardwareParaSet(m_Scanning_Control, allpara);

	////手动遥感
	//m_Scanning_Control->Handlehold_Thread_Running = true;
	//CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

	CRect rect;
	GetClientRect(&rect); //取客户区大小
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	//ShowWindow(SW_MAXIMIZE);//对话框默认最大化弹出

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGWSystem10Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGWSystem10Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGWSystem10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);

}


////界面上用户登录
//void CGWSystem10Dlg::OnBnClickedBtnLoad()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	CUserLoad k;
//	k.DoModal();
//}

//自定义消息函数1----用户登录后界面的初始状态
LRESULT CGWSystem10Dlg::InitialList1(WPARAM wParam, LPARAM lParam)//自定义消息函数4----用户登录
{
	//根据权限设置菜单
	CMenu*   mmenu = GetMenu();
	mmenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(6, MF_BYPOSITION | MF_ENABLED);
	mmenu->EnableMenuItem(7, MF_BYPOSITION | MF_ENABLED);

	CString user, jibie;
	User_Load *pA = (User_Load*)wParam;
	user = pA->username;
	jibie = pA->rank;

	if (jibie == _T("超级管理员"))
	{
		CMenu*   submenu = mmenu->GetSubMenu(1);
		submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);

	}
	else
	{
		CMenu*   submenu = mmenu->GetSubMenu(1);
		submenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		submenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
		submenu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);

	}




	return 0;

	//一定得有个返回值

}

//自定义消息函数2----初始设置参数
LRESULT CGWSystem10Dlg::InitialList2(WPARAM wParam, LPARAM lParam)//自定义消息函数2----初始设置参数
{
	ShowAllSlide.ShowAllSlidesState(wParam);
	Slide = (SLIDE*)wParam;
	ElementaryFile = Slide->path;
	//PatientName = slide->slidebuild[0];
	return 0;

	//一定得有个返回值

}

//自定义消息函数3----线程中传递定时器消息，以开启定时器，刷新显示照片
LRESULT CGWSystem10Dlg::InitialList3(WPARAM wParam, LPARAM lParam)
{

	SetTimer(2, 1000, NULL);
	return 0;
	//一定得有个返回值

}

//自定义消息函数4----普通用户扫描参数传递---------------------不要
LRESULT CGWSystem10Dlg::InitialList4(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;



	return 0;
	//一定得有个返回值

}

//自定义消息函数5----普通用户扫描聚焦参数传递
LRESULT CGWSystem10Dlg::InitialList5(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->Focusing_Num_X = pA->Focusing_Num_X;
	allpara->Focusing_Num_Y = pA->Focusing_Num_Y;

	allpara->Scanning_Aero_X = pA->Scanning_Aero_X;
	allpara->Scanning_Aero_Y = pA->Scanning_Aero_Y;
	allpara->savepath = pA->savepath;

	return 0;
	//一定得有个返回值

}

//自定义消息函数6----管理员聚焦参数传递
LRESULT CGWSystem10Dlg::InitialList6(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->Focal_Domain_Radius = pA->Focal_Domain_Radius;
	allpara->Focusing_Step_Z[0] = pA->Focusing_Step_Z[0];
	allpara->Focusing_Step_Z[1] = pA->Focusing_Step_Z[1];
	allpara->Focusing_Step_Z[2] = pA->Focusing_Step_Z[2];

	return 0;

	//一定得有个返回值

}

//自定义消息函数7----管理员电机参数传递
LRESULT CGWSystem10Dlg::InitialList7(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->CANET_ID = pA->CANET_ID;
	allpara->CANET_Port_Num = pA->CANET_Port_Num;
	allpara->Offset_arr = pA->Offset_arr;

	m_Scanning_Control->s_Scanning_System_Para.Offset_arr = pA->Offset_arr;

	return 0;

	//一定得有个返回值

}

//自定义消息函数8----管理员扫描参数传递
LRESULT CGWSystem10Dlg::InitialList8(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	//vector<long> x1; //物镜转盘的绝对位置 *2
	//int x2;//一次扫描的玻片最大数量  * 0
	//double x3; //扫描区域X方向的长度  * 0
	//double x4; //扫描区域Y方向上的长度  *0
	//int x5; //X轴方向上扫面的点数  *0
	//int x6; //Y轴方向上扫面的点数  *0
	allpara->Slide_Size_X = pA->Slide_Size_X;
	allpara->Slide_Size_y = pA->Slide_Size_y;
	allpara->Space_Slides = pA->Space_Slides;
	allpara->csExtension = pA->csExtension;
	allpara->Nosepiece_Abs_Position = pA->Nosepiece_Abs_Position;

	allpara->Scanning_Num_X = pA->Scanning_Num_X;
	allpara->Scanning_Num_Y = pA->Scanning_Num_Y;
	allpara->Slide_Max_Num = pA->Slide_Max_Num;
	allpara->Move_Deviation = pA->Move_Deviation;

	allpara->Focal_Domain_Radius = pA->Focal_Domain_Radius;
	allpara->Focusing_Step_Z[0] = pA->Focusing_Step_Z[0];
	allpara->Focusing_Step_Z[1] = pA->Focusing_Step_Z[1];
	allpara->Focusing_Step_Z[2] = pA->Focusing_Step_Z[2];

	return 0;

	//一定得有个返回值

}

//自定义消息函9----画出当前位置点
LRESULT CGWSystem10Dlg::InitialList9(WPARAM wParam, LPARAM lParam)//自定义消息函数1----相机参数设置
{
	LOCATION* loc = (LOCATION*)wParam;

	//AfxMessageBox(_T("sgadvsdvd"));

	return 0;

	//一定得有个返回值

}

//自定义消息函10----启用手动操作
LRESULT CGWSystem10Dlg::InitialList10(WPARAM wParam, LPARAM lParam)//自定义消息函数1----相机参数设置
{
	m_Scanning_Control->Handlehold_Thread_Running = true;
	CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

	return 0;
	//一定得有个返回值
}


////更新显扫描图像
//LRESULT CGWSystem10Dlg::Updata_Display_Drawing(WPARAM wParam, LPARAM lParam)
//{
//
//	return 0;
//}

//菜单栏中日志管理
void CGWSystem10Dlg::OnDiaryManagement()
{
	// TODO:  在此添加命令处理程序代码
	CDiaryManagement h;
	h.DoModal();
}

//菜单栏中用户管理
void CGWSystem10Dlg::OnUserManagement()
{
	// TODO:  在此添加命令处理程序代码
	CUserMangement l;
	l.DoModal();
}

//菜单栏中修改密码
void CGWSystem10Dlg::OnResetCode()
{
	// TODO:  在此添加命令处理程序代码
	CCodeAmend D;
	D.DoModal();
}

//菜单栏中注销
void CGWSystem10Dlg::OnMenuuserexit()
{
	// TODO:  在此添加命令处理程序代码
	if (AfxMessageBox(_T("确定注销？"), MB_YESNO) == IDYES)
	{
		CDialogEx::OnCancel();

		//重启系统
		WCHAR pBuf[MAX_PATH];
		//获取应用程序完全路径，比 GetCurrentDirectory 好用多了
		GetModuleFileName(NULL, pBuf, MAX_PATH);

		STARTUPINFO startupinfo;
		PROCESS_INFORMATION proc_info;
		memset(&startupinfo, 0, sizeof(STARTUPINFO));
		startupinfo.cb = sizeof(STARTUPINFO);
		// 最重要的地方
		::CreateProcess(pBuf, NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &proc_info);

		PostMessage(WM_QUIT);


	}

}

//玻片设置
void CGWSystem10Dlg::OnBnClickedBtnSlideset()
{
	// TODO:  在此添加控件通知处理程序代码
	CSlideSet s;
	s.DoModal();
}

//开始扫描
void CGWSystem10Dlg::OnBnClickedBtnScanbegin()
{
	// TODO:  在此添加控件通知处理程序代码
	//GetDlgItem(IDC_BTN_SCANBEGIN)->EnableWindow(false);
	//1.生成必要的存储文件夹
	m_Scanning_Control->Handlehold_Thread_Running = false;//断开摇杆线程
	AllFiles = ScanAndSave.MakeNecessaryFiles(ElementaryFile);
	//2.开始扫描并存图，相关信息存入数据库（单独开辟一个线程用于扫描，主线程用于分析）
	//hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, allpara, 0, 0);

	GetDlgItem(IDC_BTN_CAPTUREIMG)->EnableWindow(false);
	GetDlgItem(IDC_BTN_RESET)->EnableWindow(false);
	GetDlgItem(IDC_BTN_PARASET)->EnableWindow(false);
	GetDlgItem(IDC_BTN_HANDLE)->EnableWindow(false);

	CMenu*   mmenu = GetMenu();
	CMenu*   sucmenu = mmenu->GetSubMenu(3);
	sucmenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	sucmenu->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	CMenu*   sudmenu = mmenu->GetSubMenu(4);
	sudmenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	//thread ScanThread(ScanThreadProc);
	//ScanThread.detach();
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, 0);/*待修改所传递的结构体参数*/

}

//扫描线程
//void CGWSystem10Dlg::ScanThreadProc()
DWORD WINAPI ThreadProc(PVOID pParam)
{

	//清空相关结构体
	vector <PatientWithSlide>().swap(tempAllPatients);
	
	//参数传递
	//s_Scanning_Para = (ALL_PAREMETER*)pParam;
	CString csDiretory;//自动抓图图像存储的路径
	CA2T szr((pDlg->AllFiles.picsolve).c_str());
	csDiretory = (LPCTSTR)szr;

	CString focusPath;//聚焦图片保存路径
	CA2T szr1((pDlg->AllFiles.focussolve).c_str());
	focusPath = (LPCTSTR)szr1;

	CString scanPath;//10倍镜下扫描图像存储的路径
	CA2T szr2((pDlg->AllFiles.scan10Xsolve).c_str());
	scanPath = (LPCTSTR)szr2;

	CString posPath;//染色体定位图像存储的路径
	CA2T szr3((pDlg->AllFiles.CApossolve).c_str());
	posPath = (LPCTSTR)szr3;

	m_Scanning_Control->m_ScanningImage_Save_Paras.csDiretory = csDiretory;
	m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = focusPath;
	//m_Scanning_Control->m_ScanningImage_Save_Paras.scan10XPath = scanPath;
	//m_Scanning_Control->m_ScanningImage_Save_Paras.focusPath = posPath;



	CString windowname;
	pDlg->GetWindowTextW(windowname);
	//AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_SENDTIMER, NULL, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片

	/***********************测试，用于复制图片到生成的文件夹，使用时需改成硬件抓存图**********************************************/

	//需要禁用的功能
	//1.拍照2.复位3.摇杆4.电动平台菜单



	//pDlg->GetDlgItem(ID_MENU_CAMERALINK)->EnableWindow(FALSE);
	//pDlg->GetDlgItem(ID_MENU_CAMERACUT)->EnableWindow(FALSE);
	//pDlg->GetDlgItem(ID_MENU_PLATFORMSET)->EnableWindow(FALSE);


	//char tempFilePath[MAX_PATH + 1];
	//WCHAR   wstr[MAX_PATH];
	//WCHAR   wstr_des[MAX_PATH];
	//char tempFileName[10000];
	//WIN32_FIND_DATA file;
	//CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
	//vector <string>  imgNames;
	//CString WholePath, MyFileName;
	//const char* DstFile;
	//char DesTempFilePath[MAX_PATH + 1];
	//int find_pic = 0;//的图片的序列号

	//有用，勿删
	CString patientname;
	CString patientmode;
	vector <bool>().swap(m_Scanning_Control->Scanning_Completed_Status);
	
	//整理成patient(玻片号1，拨片号2)
	for (size_t i = 0; i < pDlg->Slide->slidebuild.size(); i++)
	{
		bool havepatientornot = false;
		int slidenum = 0;
		//获取照片名中的病人名
		CString strDst1 = pDlg->Slide->slidebuild[i].name.Left(pDlg->Slide->slidebuild[i].name.Find(_T('（')));
		CString strDst2 = pDlg->Slide->slidebuild[i].name.Left(pDlg->Slide->slidebuild[i].name.Find(_T('(')));
		CString strDst;
		if (strDst1.IsEmpty() && !strDst2.IsEmpty())
			strDst = strDst2;
		if (!strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = strDst1;
		if (strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = pDlg->Slide->slidebuild[i].name;
		for (auto iterpatient : tempAllPatients)
		{
			if (iterpatient.PatientName == strDst)
			{
				havepatientornot = true;
				tempAllPatients[slidenum].SlideID.push_back(pDlg->Slide->All_Slide_ID[i]);
			}
			slidenum++;
		}
		if (!havepatientornot)//如果没有该病人
		{
			PatientWithSlide patient;
			patient.path = pDlg->AllFiles.picsolve.c_str();
			patient.resultpath = pDlg->AllFiles.endsolve.c_str();
			patient.PatientName = strDst;
			patient.SlideID.push_back(pDlg->Slide->All_Slide_ID[i]);
			patient.mode = pDlg->Slide->slidebuild[i].mode;
			tempAllPatients.push_back(patient);
		}
	}

	vector <int >().swap(pDlg->Slide->All_Slide_ID);
	for (size_t i = 0; i < tempAllPatients.size(); i++)
	{
		for (size_t j = 0; j < tempAllPatients[i].SlideID.size(); j++)
		{
			pDlg->Slide->All_Slide_ID.push_back(tempAllPatients[i].SlideID[j]);
		}

	}

	//将所有待扫描的玻片信息存入数据库
	for (size_t i = 0; i < pDlg->Slide->All_Slide_ID.size(); i++)
	{
		//将一张玻片的扫描信息存入数据库
		CReadAndWriteForAccess SaveToAccess;
		Table_SlideSet slideset;
		patientname = pDlg->Slide->slidebuild[i].name;
		patientmode = pDlg->Slide->slidebuild[i].mode;
		slideset.name = patientname;
		slideset.mode = patientmode;
		slideset.path = pDlg->AllFiles.picsolve.c_str();
		slideset.resultpath = pDlg->AllFiles.endsolve.c_str();
		slideset.range = pDlg->Slide->slidebuild[i].range;
		SaveToAccess.WriteToSlideSet(slideset);
	}

	for (size_t i = 0; i < pDlg->Slide->All_Slide_ID.size(); i++)
	{

		//*******进行缩放扫描范围，并更新到当前扫描参数中*******//
		m_Scanning_Control->s_User_Para.Scanning_Aero_X = allpara->Scanning_Aero_X * sqrt(_ttof(pDlg->Slide->slidebuild[i].range) / 100);
		m_Scanning_Control->s_User_Para.Scanning_Aero_Y = allpara->Scanning_Aero_Y * sqrt(_ttof(pDlg->Slide->slidebuild[i].range) / 100);


		m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
		m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
		m_Scanning_Control->m_ScanningImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;//LUCAM_DM_FAST
		m_Scanning_Control->m_ScanningImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;

		//获取病人名称与模式，以命名图片
		patientname = pDlg->Slide->slidebuild[i].name;
		patientmode = pDlg->Slide->slidebuild[i].mode;
		m_Scanning_Control->m_ScanningImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_");
		m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_F");

		//使能电机
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(1, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(2, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(3, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(4, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(5, true);

		if (patientmode == _T("微核"))
		{
			//设置40倍镜下相机参数
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_20X;
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num = pDlg->Slide->All_Slide_ID[i];
			//HardwareInit.GetCameraPara();

			//CB聚焦
			pDlg->SetTimer(3, 300, NULL);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = true;
			m_Scanning_Control->Do_Focusing_Model(200);
			//CB扫描
			pDlg->SetTimer(4, 300, NULL);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = false;
			m_Scanning_Control->Do_Scanning_Model();


		}
		else if (patientmode == _T("染色体"))
		{
			m_Scanning_Control->m_ScanningImage_Save_Paras.csDiretory = scanPath;
			m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = focusPath;
			m_Scanning_Control->m_ScanningImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_10X");
			m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_F");

			//CA 10X下自动聚焦
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_10X;
			//设置10倍镜下相机参数
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2] = 800;
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num = pDlg->Slide->All_Slide_ID[i];
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = true;
			pDlg->SetTimer(5, 300, NULL);
			m_Scanning_Control->Do_Focusing_Model(m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2]);

			//CA 10X下自动扫描、自动染色体团定位
			m_Scanning_Control->CA_Focus_Thread_Running = true;
			CWinThread* pThread = AfxBeginThread(Scanning_Control::Get_CA_Pos_10X_ThreadFunc, m_Scanning_Control);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = false;
			pDlg->SetTimer(6, 300, NULL);
			m_Scanning_Control->Do_Scanning_Model();


			//CA 100X下自动聚焦扫描
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_100X;
			pDlg->SetTimer(7, 300, NULL);

			//设置100倍镜下相机参数
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = csDiretory;
			m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_");
			//m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_FAST;
			//m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
			m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2] = 100;
			m_Scanning_Control->Do_Focusing_100X();



		}
		else
			AfxMessageBox(_T("模式选择错误！"));

		for (size_t i = 0; i < tempAllPatients.size(); i++)
		{
			for (size_t j = 0; j < tempAllPatients[i].SlideID.size(); j++)
			{
				if (tempAllPatients[i].SlideID[j] == m_Scanning_Control->s_Scanning_Para.Current_Slide_Num)
				{
					tempAllPatients[i].SlideID[j] = -1;
					break;
				}

			}
		}

	/*	if (m_Scanning_Control->Scanning_Completed_Status.size() >= m_Scanning_Control->s_Scanning_Para.Current_Slide_Num + 1)
			Scanning_End = m_Scanning_Control->Scanning_Completed_Status[m_Scanning_Control->s_Scanning_Para.Current_Slide_Num];*/

	}

	pDlg->GetWindowTextW(windowname);
	//AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd1 = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面
	::SendMessage(hwnd1, WM_START_HANDOPERATION, NULL, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片

	//启用相关功能
	//需要禁用的功能
	pDlg->GetDlgItem(IDC_BTN_CAPTUREIMG)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_BTN_RESET)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_BTN_PARASET)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_BTN_HANDLE)->EnableWindow(true);

	CMenu*   mmenu = pDlg->GetMenu();
	CMenu*   sucmenu = mmenu->GetSubMenu(3);
	sucmenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	sucmenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
	CMenu*   sudmenu = mmenu->GetSubMenu(4);
	sudmenu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);



}

//上一页的八张玻片
void CGWSystem10Dlg::OnBnClickedBtnLastpage()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowAllSlide.Last8Slides();
	//vector<int>xxx = { -20000, 0, 10000 };
	//vector<int>yyy = { -20000, 0, 10000 };

	//MainWinImgShow.ShowTheProgress(xxx,yyy,3);
}

//下一页的八张玻片
void CGWSystem10Dlg::OnBnClickedBtnNextpage()
{
	// TODO:  在此添加控件通知处理程序代码
	ShowAllSlide.Next8Slides();
}

void CGWSystem10Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	MainWinImgShow.MyMouseMove(nFlags, point);
	CDialogEx::OnMouseMove(nFlags, point);
}

void CGWSystem10Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	MainWinImgShow.MyVScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGWSystem10Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	MainWinImgShow.MyMouseWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CGWSystem10Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (1 == nIDEvent)
		MainWinImgShow.TimerShowPicture();
	if (2 == nIDEvent)
		MainWinImgShow.FreshScroll();
	if (3 == nIDEvent)
	{
		int X_correct = (m_Scanning_Control->s_Scanning_System_Para.Space_Slides + m_Scanning_Control->s_Scanning_System_Para.Slide_Size_X)
			* m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
		MainWinImgShow.ShowTheProgress(m_Scanning_Control->s_Scanning_Para.X_Focus_Location,
			m_Scanning_Control->s_Scanning_Para.Y_Focus_Location, m_Scanning_Control->s_Scanning_Para.Current_Point_Num, X_correct,
			m_Scanning_Control->s_User_Para.Scanning_Aero_X, m_Scanning_Control->s_User_Para.Scanning_Aero_Y);
	}
	if (4 == nIDEvent)
	{
		KillTimer(3);
		int X_correct = (m_Scanning_Control->s_Scanning_System_Para.Space_Slides + m_Scanning_Control->s_Scanning_System_Para.Slide_Size_X)
			* m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
		MainWinImgShow.ShowTheProgress(m_Scanning_Control->s_Scanning_Para.X_Scanning_Location,
			m_Scanning_Control->s_Scanning_Para.Y_Scanning_Location, m_Scanning_Control->s_Scanning_Para.Current_Point_Num, X_correct,
			m_Scanning_Control->s_User_Para.Scanning_Aero_X, m_Scanning_Control->s_User_Para.Scanning_Aero_Y);
		//当前点数，总点数，当前的玻片号
		ShowAllSlide.ShowTheScanningProgress(m_Scanning_Control->s_Scanning_Para.Current_Point_Num,
			m_Scanning_Control->s_User_Para.Scanning_Num_X * m_Scanning_Control->s_User_Para.Scanning_Num_Y,
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num);
	}
	if (5 == nIDEvent)
	{
		int X_correct = (m_Scanning_Control->s_Scanning_System_Para.Space_Slides + m_Scanning_Control->s_Scanning_System_Para.Slide_Size_X)
			* m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
		MainWinImgShow.ShowTheProgress(m_Scanning_Control->s_Scanning_Para.X_Focus_Location,
			m_Scanning_Control->s_Scanning_Para.Y_Focus_Location, m_Scanning_Control->s_Scanning_Para.Current_Point_Num, X_correct,
			m_Scanning_Control->s_User_Para.Scanning_Aero_X, m_Scanning_Control->s_User_Para.Scanning_Aero_Y);
	}
	if (6 == nIDEvent)
	{
		KillTimer(5);
		int X_correct = (m_Scanning_Control->s_Scanning_System_Para.Space_Slides + m_Scanning_Control->s_Scanning_System_Para.Slide_Size_X)
			* m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
		MainWinImgShow.ShowTheProgress(m_Scanning_Control->s_Scanning_Para.X_Scanning_Location,
			m_Scanning_Control->s_Scanning_Para.Y_Scanning_Location, m_Scanning_Control->s_Scanning_Para.Current_Point_Num, X_correct,
			m_Scanning_Control->s_User_Para.Scanning_Aero_X, m_Scanning_Control->s_User_Para.Scanning_Aero_Y);
		//当前点数，总点数，当前的玻片号
		ShowAllSlide.ShowTheScanningProgress(m_Scanning_Control->s_Scanning_Para.Current_Point_Num,
			m_Scanning_Control->s_User_Para.Scanning_Num_X * m_Scanning_Control->s_User_Para.Scanning_Num_Y,
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num);
	}
	if (7 == nIDEvent)
	{
		KillTimer(6);
		int Current_CA_Num = m_Scanning_Control->CA_Real_Pos_Y.size();

		int X_correct = (m_Scanning_Control->s_Scanning_System_Para.Space_Slides + m_Scanning_Control->s_Scanning_System_Para.Slide_Size_X)
			* m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
		MainWinImgShow.ShowTheProgress(m_Scanning_Control->CA_Real_Pos_X,
			m_Scanning_Control->CA_Real_Pos_Y, Current_CA_Num, X_correct,
			m_Scanning_Control->s_User_Para.Scanning_Aero_X, m_Scanning_Control->s_User_Para.Scanning_Aero_Y);
	}

	BStop = TRUE;
	CDialogEx::OnTimer(nIDEvent);
}

//微核分析
void CGWSystem10Dlg::OnBnClickedBtnIkaros()
{
	// TODO:  在此添加控件通知处理程序代码
	CMicroNucleusHandle *pMicroHandle = new CMicroNucleusHandle;
	pMicroHandle->Create(IDD_MICRONUCLEUSHANDLE,this);
	//将当前存图路径和处理结果存储路径传递给微核分析界面以初始化
	HWND   hwnd = ::FindWindow(NULL, _T("微核处理"));//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_HANDLEPATH, (WPARAM)&AllFiles, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片
	pMicroHandle->ShowWindow(SW_SHOW);
}

//关闭主对话框
void CGWSystem10Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox(_T("确定退出？"), MB_YESNO) == IDYES)
	{
		HardwareInit.CleanUp();
		CDialogEx::OnClose();
	}

}

//染色体分析
void CGWSystem10Dlg::OnBnClickedBtnIkarosrst()
{
	// TODO:  在此添加控件通知处理程序代码
	CChromosomeHandle *pChroHandle = new CChromosomeHandle;
	pChroHandle->Create(IDD_CHROSOMEHANDLE, this);
	//将当前存图路径和处理结果存储路径传递给微核分析界面以初始化
	HWND   hwnd = ::FindWindow(NULL, _T("染色体处理"));//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_HANDLEPATHCHRO, (WPARAM)&AllFiles, NULL);//线程中传递定时器消息，以开启定时器，刷新显示照片
	pChroHandle->ShowWindow(SW_SHOW);

}

//相机设置
void CGWSystem10Dlg::OnMenuCamera()
{
	// TODO:  在此添加命令处理程序代码
	if (!m_Scanning_Control->m_CameraConnected)
	{
		AfxMessageBox(_T("相机未连接，请连接相机后再设置"));
		return;
	}
	CameraParaSetDlg k;
	k.PassHandleCameera(m_Scanning_Control->m_hCamera, m_Scanning_Control->m_bPreviewing, 3);
	k.DoModal();
}

//菜单栏电动平台设置
void CGWSystem10Dlg::OnMenuPlatformset()
{
	// TODO:  在此添加命令处理程序代码
	if (!m_Scanning_Control->m_MotorConnected)
	{
		AfxMessageBox(_T("电机未连接，请连接电机后再设置"));
		return;
	}

	AllParaSet HardPara;
	HardPara.HardwareParaSet(m_Scanning_Control, allpara);
	MotorControlDlg k;
	k.Pass_Motor_Paras(m_Scanning_Control, m_Scanning_Control->m_MotorConnected);
	k.DoModal();
}


//菜单栏上的聚焦参数设置
void CGWSystem10Dlg::OnMenuFocus()
{
	// TODO:  在此添加命令处理程序代码
	CFocusSet l;
	l.DoModal();
}

//管理员参数设置
void CGWSystem10Dlg::OnMenuAdmin()
{
	// TODO:  在此添加命令处理程序代码
	CAdminSet a;
	a.DoModal();
}

//分析算法参数设置
void CGWSystem10Dlg::OnMenuAnalyse()
{
	// TODO:  在此添加命令处理程序代码
	CAlgorithmParaSet aa;
	aa.DoModal();
}

//扫描停止（停止所有电机）
void CGWSystem10Dlg::OnBnClickedBtnStop()
{
	// TODO:  在此添加控件通知处理程序代码
	//断开扫描线程
	TerminateThread(hThread, 0);
	
	//停止电机
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(1, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(2, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(3, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(4,false);
	m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(5, false);

}

// 画出当前拍照点
int CGWSystem10Dlg::DrawCurrentPosition(LOCATION currrentLocation)
{
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_CURRENTPOSITION, (WPARAM)&currrentLocation, NULL);
	return 0;
}

//左键单击响应
void CGWSystem10Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ShowAllSlide.ShowSlideInformation(nFlags, point, Slide);

	CDialogEx::OnLButtonDown(nFlags, point);
}


//手动抓图
void CGWSystem10Dlg::OnBnClickedBtnCaptureimg()
{
	// TODO:  在此添加控件通知处理程序代码
	MULTICAPTURE_PARAMS MultiC_Params;
	CString csTemp;
	MultiC_Params.csDiretory = allpara->savepath;
	MultiC_Params.csExtension = ".bmp"; //".raw";
	MultiC_Params.csFilename = "Capture_";
	MultiC_Params.lcConversion.DemosaicMethod = LUCAM_DM_FAST;
	MultiC_Params.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
	m_Scanning_Control->s_Scanning_Para.Current_Point_Num++;
	m_Scanning_Control->TakeVeido_And_Save_Image_Raw(MultiC_Params);
}

//复位
void CGWSystem10Dlg::OnBnClickedBtnReset()
{
	// TODO:  在此添加控件通知处理程序代码

	m_Scanning_Control->Handlehold_Thread_Running = false;
	Sleep(100);
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[0] << endl;
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[1] << endl; 
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[2] << endl;
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[3] << endl;
	m_Scanning_Control->Do_restting_Model();
	//手动遥感
	m_Scanning_Control->Handlehold_Thread_Running = true;
	CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

}


void CGWSystem10Dlg::OnBnClickedBtnParaset()
{
	// TODO:  在此添加控件通知处理程序代码
	CFocusSet l;
	l.DoModal();
}


void CGWSystem10Dlg::OnBnClickedBtnHandle()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_Scanning_Control->Handlehold_Thread_Running)
		m_Scanning_Control->Handlehold_Thread_Running = false;
	else
	{
		m_Scanning_Control->Handlehold_Thread_Running = true;
		CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);
	}
}


void CGWSystem10Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();
	}
}

void CGWSystem10Dlg::ReSize()
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