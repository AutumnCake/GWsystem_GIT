
// GWSystem1.0Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0Dlg.h"
#include "afxdialogex.h"
#include<thread>

#include<atlimage.h>


//�û�����
#include "UserMangement.h"
#include "UserLoad.h"
#include "CodeAmend.h"
#include "DiaryManagement.h"

//��ʾ���в�Ƭɨ��״̬
#include"ShowAllSlides.h"
#include"SlideSet.h"

//ɨ������ʾ
#include"ScanAndSaveImg.h"
#include "ReadAndWriteForAccess.h"
#include"MainWindowImgShow.h"

//΢����Ⱦɫ�����
#include "MicroNucleusHandle.h"
#include "CChromosomeHandle.h"

//Ӳ��
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

//ȫ�ֱ���
CGWSystem10Dlg* pDlg;
CShowAllSlides ShowAllSlide;
CScanAndSaveImg ScanAndSave;
CMainWindowImgShow  MainWinImgShow;
Scanning_Control *m_Scanning_Control = new Scanning_Control;
CHardwareInit HardwareInit;
vector <PatientWithSlide> tempAllPatients;

//��������
static ALL_PAREMETER *allpara = new ALL_PAREMETER;
//static ALL_PAREMETER *s_Scanning_Para = new ALL_PAREMETER;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGWSystem10Dlg �Ի���



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
	ON_MESSAGE(WM_USER_LOAD, &CGWSystem10Dlg::InitialList1) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_MN_BTN_OK, &CGWSystem10Dlg::InitialList2) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_SENDTIMER, &CGWSystem10Dlg::InitialList3) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_USER_ScanSet, &CGWSystem10Dlg::InitialList4) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_USER_FocusSet, &CGWSystem10Dlg::InitialList5) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_USER_AdminFocusSet, &CGWSystem10Dlg::InitialList6) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_USER_AdminMotorSet, &CGWSystem10Dlg::InitialList7) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_USER_AdminScanSet, &CGWSystem10Dlg::InitialList8) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_CURRENTPOSITION, &CGWSystem10Dlg::InitialList9) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_START_HANDOPERATION, &CGWSystem10Dlg::InitialList10) //�Զ�����Ϣ��������

	//ON_MESSAGE(WM_DISPLAY_DRAWING, &CGWSystem10Dlg::Updata_Display_Drawing)//�����Ϣӳ��

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


// CGWSystem10Dlg ��Ϣ�������

BOOL CGWSystem10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

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




	pDlg = this;//ָ��öԻ����ȫ��ָ��
	theApp.dlg_login.JudgeType(theApp.dlg_login.user);

	ShowAllSlide.OnInitDialog();
	MainWinImgShow.PutPicControlIntoVector();	//��ʼ��picture�ؼ���������

	////Ӳ����ʼ��
	//HardwareInit.InitHardwarePara(allpara);


	//HWND hWnd;
	//hWnd = GetDlgItem(IDC_VIDEOFRAME)->GetSafeHwnd();
	//HardwareInit.OnInitDialog(hWnd);

	////���ݿ��ж��������ݴ���Ӳ��
	//AllParaSet HardPara;
	//HardPara.HardwareParaSet(m_Scanning_Control, allpara);

	////�ֶ�ң��
	//m_Scanning_Control->Handlehold_Thread_Running = true;
	//CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

	CRect rect;
	GetClientRect(&rect); //ȡ�ͻ�����С
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	//ShowWindow(SW_MAXIMIZE);//�Ի���Ĭ����󻯵���

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGWSystem10Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGWSystem10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);

}


////�������û���¼
//void CGWSystem10Dlg::OnBnClickedBtnLoad()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CUserLoad k;
//	k.DoModal();
//}

//�Զ�����Ϣ����1----�û���¼�����ĳ�ʼ״̬
LRESULT CGWSystem10Dlg::InitialList1(WPARAM wParam, LPARAM lParam)//�Զ�����Ϣ����4----�û���¼
{
	//����Ȩ�����ò˵�
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

	if (jibie == _T("��������Ա"))
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

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����2----��ʼ���ò���
LRESULT CGWSystem10Dlg::InitialList2(WPARAM wParam, LPARAM lParam)//�Զ�����Ϣ����2----��ʼ���ò���
{
	ShowAllSlide.ShowAllSlidesState(wParam);
	Slide = (SLIDE*)wParam;
	ElementaryFile = Slide->path;
	//PatientName = slide->slidebuild[0];
	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����3----�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ
LRESULT CGWSystem10Dlg::InitialList3(WPARAM wParam, LPARAM lParam)
{

	SetTimer(2, 1000, NULL);
	return 0;
	//һ�����и�����ֵ

}

//�Զ�����Ϣ����4----��ͨ�û�ɨ���������---------------------��Ҫ
LRESULT CGWSystem10Dlg::InitialList4(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;



	return 0;
	//һ�����и�����ֵ

}

//�Զ�����Ϣ����5----��ͨ�û�ɨ��۽���������
LRESULT CGWSystem10Dlg::InitialList5(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->Focusing_Num_X = pA->Focusing_Num_X;
	allpara->Focusing_Num_Y = pA->Focusing_Num_Y;

	allpara->Scanning_Aero_X = pA->Scanning_Aero_X;
	allpara->Scanning_Aero_Y = pA->Scanning_Aero_Y;
	allpara->savepath = pA->savepath;

	return 0;
	//һ�����и�����ֵ

}

//�Զ�����Ϣ����6----����Ա�۽���������
LRESULT CGWSystem10Dlg::InitialList6(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->Focal_Domain_Radius = pA->Focal_Domain_Radius;
	allpara->Focusing_Step_Z[0] = pA->Focusing_Step_Z[0];
	allpara->Focusing_Step_Z[1] = pA->Focusing_Step_Z[1];
	allpara->Focusing_Step_Z[2] = pA->Focusing_Step_Z[2];

	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����7----����Ա�����������
LRESULT CGWSystem10Dlg::InitialList7(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	allpara->CANET_ID = pA->CANET_ID;
	allpara->CANET_Port_Num = pA->CANET_Port_Num;
	allpara->Offset_arr = pA->Offset_arr;

	m_Scanning_Control->s_Scanning_System_Para.Offset_arr = pA->Offset_arr;

	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����8----����Աɨ���������
LRESULT CGWSystem10Dlg::InitialList8(WPARAM wParam, LPARAM lParam)
{
	ALL_PAREMETER *pA = (ALL_PAREMETER*)wParam;

	//vector<long> x1; //�ﾵת�̵ľ���λ�� *2
	//int x2;//һ��ɨ��Ĳ�Ƭ�������  * 0
	//double x3; //ɨ������X����ĳ���  * 0
	//double x4; //ɨ������Y�����ϵĳ���  *0
	//int x5; //X�᷽����ɨ��ĵ���  *0
	//int x6; //Y�᷽����ɨ��ĵ���  *0
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

	//һ�����и�����ֵ

}

//�Զ�����Ϣ��9----������ǰλ�õ�
LRESULT CGWSystem10Dlg::InitialList9(WPARAM wParam, LPARAM lParam)//�Զ�����Ϣ����1----�����������
{
	LOCATION* loc = (LOCATION*)wParam;

	//AfxMessageBox(_T("sgadvsdvd"));

	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ��10----�����ֶ�����
LRESULT CGWSystem10Dlg::InitialList10(WPARAM wParam, LPARAM lParam)//�Զ�����Ϣ����1----�����������
{
	m_Scanning_Control->Handlehold_Thread_Running = true;
	CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

	return 0;
	//һ�����и�����ֵ
}


////������ɨ��ͼ��
//LRESULT CGWSystem10Dlg::Updata_Display_Drawing(WPARAM wParam, LPARAM lParam)
//{
//
//	return 0;
//}

//�˵�������־����
void CGWSystem10Dlg::OnDiaryManagement()
{
	// TODO:  �ڴ���������������
	CDiaryManagement h;
	h.DoModal();
}

//�˵������û�����
void CGWSystem10Dlg::OnUserManagement()
{
	// TODO:  �ڴ���������������
	CUserMangement l;
	l.DoModal();
}

//�˵������޸�����
void CGWSystem10Dlg::OnResetCode()
{
	// TODO:  �ڴ���������������
	CCodeAmend D;
	D.DoModal();
}

//�˵�����ע��
void CGWSystem10Dlg::OnMenuuserexit()
{
	// TODO:  �ڴ���������������
	if (AfxMessageBox(_T("ȷ��ע����"), MB_YESNO) == IDYES)
	{
		CDialogEx::OnCancel();

		//����ϵͳ
		WCHAR pBuf[MAX_PATH];
		//��ȡӦ�ó�����ȫ·������ GetCurrentDirectory ���ö���
		GetModuleFileName(NULL, pBuf, MAX_PATH);

		STARTUPINFO startupinfo;
		PROCESS_INFORMATION proc_info;
		memset(&startupinfo, 0, sizeof(STARTUPINFO));
		startupinfo.cb = sizeof(STARTUPINFO);
		// ����Ҫ�ĵط�
		::CreateProcess(pBuf, NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &proc_info);

		PostMessage(WM_QUIT);


	}

}

//��Ƭ����
void CGWSystem10Dlg::OnBnClickedBtnSlideset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSlideSet s;
	s.DoModal();
}

//��ʼɨ��
void CGWSystem10Dlg::OnBnClickedBtnScanbegin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_BTN_SCANBEGIN)->EnableWindow(false);
	//1.���ɱ�Ҫ�Ĵ洢�ļ���
	m_Scanning_Control->Handlehold_Thread_Running = false;//�Ͽ�ҡ���߳�
	AllFiles = ScanAndSave.MakeNecessaryFiles(ElementaryFile);
	//2.��ʼɨ�貢��ͼ�������Ϣ�������ݿ⣨��������һ���߳�����ɨ�裬���߳����ڷ�����
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
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, 0);/*���޸������ݵĽṹ�����*/

}

//ɨ���߳�
//void CGWSystem10Dlg::ScanThreadProc()
DWORD WINAPI ThreadProc(PVOID pParam)
{

	//�����ؽṹ��
	vector <PatientWithSlide>().swap(tempAllPatients);
	
	//��������
	//s_Scanning_Para = (ALL_PAREMETER*)pParam;
	CString csDiretory;//�Զ�ץͼͼ��洢��·��
	CA2T szr((pDlg->AllFiles.picsolve).c_str());
	csDiretory = (LPCTSTR)szr;

	CString focusPath;//�۽�ͼƬ����·��
	CA2T szr1((pDlg->AllFiles.focussolve).c_str());
	focusPath = (LPCTSTR)szr1;

	CString scanPath;//10������ɨ��ͼ��洢��·��
	CA2T szr2((pDlg->AllFiles.scan10Xsolve).c_str());
	scanPath = (LPCTSTR)szr2;

	CString posPath;//Ⱦɫ�嶨λͼ��洢��·��
	CA2T szr3((pDlg->AllFiles.CApossolve).c_str());
	posPath = (LPCTSTR)szr3;

	m_Scanning_Control->m_ScanningImage_Save_Paras.csDiretory = csDiretory;
	m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = focusPath;
	//m_Scanning_Control->m_ScanningImage_Save_Paras.scan10XPath = scanPath;
	//m_Scanning_Control->m_ScanningImage_Save_Paras.focusPath = posPath;



	CString windowname;
	pDlg->GetWindowTextW(windowname);
	//AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_SENDTIMER, NULL, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ

	/***********************���ԣ����ڸ���ͼƬ�����ɵ��ļ��У�ʹ��ʱ��ĳ�Ӳ��ץ��ͼ**********************************************/

	//��Ҫ���õĹ���
	//1.����2.��λ3.ҡ��4.�綯ƽ̨�˵�



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
	//int find_pic = 0;//��ͼƬ�����к�

	//���ã���ɾ
	CString patientname;
	CString patientmode;
	vector <bool>().swap(m_Scanning_Control->Scanning_Completed_Status);
	
	//�����patient(��Ƭ��1����Ƭ��2)
	for (size_t i = 0; i < pDlg->Slide->slidebuild.size(); i++)
	{
		bool havepatientornot = false;
		int slidenum = 0;
		//��ȡ��Ƭ���еĲ�����
		CString strDst1 = pDlg->Slide->slidebuild[i].name.Left(pDlg->Slide->slidebuild[i].name.Find(_T('��')));
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
		if (!havepatientornot)//���û�иò���
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

	//�����д�ɨ��Ĳ�Ƭ��Ϣ�������ݿ�
	for (size_t i = 0; i < pDlg->Slide->All_Slide_ID.size(); i++)
	{
		//��һ�Ų�Ƭ��ɨ����Ϣ�������ݿ�
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

		//*******��������ɨ�跶Χ�������µ���ǰɨ�������*******//
		m_Scanning_Control->s_User_Para.Scanning_Aero_X = allpara->Scanning_Aero_X * sqrt(_ttof(pDlg->Slide->slidebuild[i].range) / 100);
		m_Scanning_Control->s_User_Para.Scanning_Aero_Y = allpara->Scanning_Aero_Y * sqrt(_ttof(pDlg->Slide->slidebuild[i].range) / 100);


		m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
		m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
		m_Scanning_Control->m_ScanningImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;//LUCAM_DM_FAST
		m_Scanning_Control->m_ScanningImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;

		//��ȡ����������ģʽ��������ͼƬ
		patientname = pDlg->Slide->slidebuild[i].name;
		patientmode = pDlg->Slide->slidebuild[i].mode;
		m_Scanning_Control->m_ScanningImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_");
		m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_F");

		//ʹ�ܵ��
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(1, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(2, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_ENA(3, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(4, true);
		m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(5, true);

		if (patientmode == _T("΢��"))
		{
			//����40�������������
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_20X;
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num = pDlg->Slide->All_Slide_ID[i];
			//HardwareInit.GetCameraPara();

			//CB�۽�
			pDlg->SetTimer(3, 300, NULL);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = true;
			m_Scanning_Control->Do_Focusing_Model(200);
			//CBɨ��
			pDlg->SetTimer(4, 300, NULL);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = false;
			m_Scanning_Control->Do_Scanning_Model();


		}
		else if (patientmode == _T("Ⱦɫ��"))
		{
			m_Scanning_Control->m_ScanningImage_Save_Paras.csDiretory = scanPath;
			m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = focusPath;
			m_Scanning_Control->m_ScanningImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_10X");
			m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_F");

			//CA 10X���Զ��۽�
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_10X;
			//����10�������������
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2] = 800;
			m_Scanning_Control->s_Scanning_Para.Current_Slide_Num = pDlg->Slide->All_Slide_ID[i];
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = true;
			pDlg->SetTimer(5, 300, NULL);
			m_Scanning_Control->Do_Focusing_Model(m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2]);

			//CA 10X���Զ�ɨ�衢�Զ�Ⱦɫ���Ŷ�λ
			m_Scanning_Control->CA_Focus_Thread_Running = true;
			CWinThread* pThread = AfxBeginThread(Scanning_Control::Get_CA_Pos_10X_ThreadFunc, m_Scanning_Control);
			m_Scanning_Control->s_Scanning_Para.Forcuing_OR_Scanning = false;
			pDlg->SetTimer(6, 300, NULL);
			m_Scanning_Control->Do_Scanning_Model();


			//CA 100X���Զ��۽�ɨ��
			m_Scanning_Control->s_Scanning_Para.Objective_Num = Objective_100X;
			pDlg->SetTimer(7, 300, NULL);

			//����100�������������
			HardwareInit.SetCameraPara(m_Scanning_Control->s_Scanning_Para.Objective_Num);
			m_Scanning_Control->m_FocusingImage_Save_Paras.csDiretory = csDiretory;
			m_Scanning_Control->m_FocusingImage_Save_Paras.csFilename = patientname + "_" + patientmode + _T("_");
			//m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.DemosaicMethod = LUCAM_DM_FAST;
			//m_Scanning_Control->m_FocusingImage_Save_Paras.lcConversion.CorrectionMatrix = LUCAM_CM_HALOGEN;
			m_Scanning_Control->s_Scanning_System_Para.Focusing_Step_Z[2] = 100;
			m_Scanning_Control->Do_Focusing_100X();



		}
		else
			AfxMessageBox(_T("ģʽѡ�����"));

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
	//AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd1 = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd1, WM_START_HANDOPERATION, NULL, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ

	//������ع���
	//��Ҫ���õĹ���
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

//��һҳ�İ��Ų�Ƭ
void CGWSystem10Dlg::OnBnClickedBtnLastpage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ShowAllSlide.Last8Slides();
	//vector<int>xxx = { -20000, 0, 10000 };
	//vector<int>yyy = { -20000, 0, 10000 };

	//MainWinImgShow.ShowTheProgress(xxx,yyy,3);
}

//��һҳ�İ��Ų�Ƭ
void CGWSystem10Dlg::OnBnClickedBtnNextpage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ShowAllSlide.Next8Slides();
}

void CGWSystem10Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MainWinImgShow.MyMouseMove(nFlags, point);
	CDialogEx::OnMouseMove(nFlags, point);
}

void CGWSystem10Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MainWinImgShow.MyVScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGWSystem10Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MainWinImgShow.MyMouseWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CGWSystem10Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		//��ǰ�������ܵ�������ǰ�Ĳ�Ƭ��
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
		//��ǰ�������ܵ�������ǰ�Ĳ�Ƭ��
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

//΢�˷���
void CGWSystem10Dlg::OnBnClickedBtnIkaros()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMicroNucleusHandle *pMicroHandle = new CMicroNucleusHandle;
	pMicroHandle->Create(IDD_MICRONUCLEUSHANDLE,this);
	//����ǰ��ͼ·���ʹ������洢·�����ݸ�΢�˷��������Գ�ʼ��
	HWND   hwnd = ::FindWindow(NULL, _T("΢�˴���"));//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_HANDLEPATH, (WPARAM)&AllFiles, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ
	pMicroHandle->ShowWindow(SW_SHOW);
}

//�ر����Ի���
void CGWSystem10Dlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (AfxMessageBox(_T("ȷ���˳���"), MB_YESNO) == IDYES)
	{
		HardwareInit.CleanUp();
		CDialogEx::OnClose();
	}

}

//Ⱦɫ�����
void CGWSystem10Dlg::OnBnClickedBtnIkarosrst()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CChromosomeHandle *pChroHandle = new CChromosomeHandle;
	pChroHandle->Create(IDD_CHROSOMEHANDLE, this);
	//����ǰ��ͼ·���ʹ������洢·�����ݸ�΢�˷��������Գ�ʼ��
	HWND   hwnd = ::FindWindow(NULL, _T("Ⱦɫ�崦��"));//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_HANDLEPATHCHRO, (WPARAM)&AllFiles, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ
	pChroHandle->ShowWindow(SW_SHOW);

}

//�������
void CGWSystem10Dlg::OnMenuCamera()
{
	// TODO:  �ڴ���������������
	if (!m_Scanning_Control->m_CameraConnected)
	{
		AfxMessageBox(_T("���δ���ӣ������������������"));
		return;
	}
	CameraParaSetDlg k;
	k.PassHandleCameera(m_Scanning_Control->m_hCamera, m_Scanning_Control->m_bPreviewing, 3);
	k.DoModal();
}

//�˵����綯ƽ̨����
void CGWSystem10Dlg::OnMenuPlatformset()
{
	// TODO:  �ڴ���������������
	if (!m_Scanning_Control->m_MotorConnected)
	{
		AfxMessageBox(_T("���δ���ӣ������ӵ����������"));
		return;
	}

	AllParaSet HardPara;
	HardPara.HardwareParaSet(m_Scanning_Control, allpara);
	MotorControlDlg k;
	k.Pass_Motor_Paras(m_Scanning_Control, m_Scanning_Control->m_MotorConnected);
	k.DoModal();
}


//�˵����ϵľ۽���������
void CGWSystem10Dlg::OnMenuFocus()
{
	// TODO:  �ڴ���������������
	CFocusSet l;
	l.DoModal();
}

//����Ա��������
void CGWSystem10Dlg::OnMenuAdmin()
{
	// TODO:  �ڴ���������������
	CAdminSet a;
	a.DoModal();
}

//�����㷨��������
void CGWSystem10Dlg::OnMenuAnalyse()
{
	// TODO:  �ڴ���������������
	CAlgorithmParaSet aa;
	aa.DoModal();
}

//ɨ��ֹͣ��ֹͣ���е����
void CGWSystem10Dlg::OnBnClickedBtnStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�Ͽ�ɨ���߳�
	TerminateThread(hThread, 0);
	
	//ֹͣ���
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(1, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(2, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_ENA(3, false);
	m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(4,false);
	m_Scanning_Control->s_Dev_Drivers->Dev_PUSIRobot_ENA(5, false);

}

// ������ǰ���յ�
int CGWSystem10Dlg::DrawCurrentPosition(LOCATION currrentLocation)
{
	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��	//HWND hwnd = ::GetParent(m_hWnd);
	::SendMessage(hwnd, WM_CURRENTPOSITION, (WPARAM)&currrentLocation, NULL);
	return 0;
}

//���������Ӧ
void CGWSystem10Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowAllSlide.ShowSlideInformation(nFlags, point, Slide);

	CDialogEx::OnLButtonDown(nFlags, point);
}


//�ֶ�ץͼ
void CGWSystem10Dlg::OnBnClickedBtnCaptureimg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

//��λ
void CGWSystem10Dlg::OnBnClickedBtnReset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_Scanning_Control->Handlehold_Thread_Running = false;
	Sleep(100);
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[0] << endl;
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[1] << endl; 
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[2] << endl;
	cout << m_Scanning_Control->s_Scanning_System_Para.Offset_arr[3] << endl;
	m_Scanning_Control->Do_restting_Model();
	//�ֶ�ң��
	m_Scanning_Control->Handlehold_Thread_Running = true;
	CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, m_Scanning_Control);

}


void CGWSystem10Dlg::OnBnClickedBtnParaset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFocusSet l;
	l.DoModal();
}


void CGWSystem10Dlg::OnBnClickedBtnHandle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();
	}
}

void CGWSystem10Dlg::ReSize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;
	GetClientRect(&recta); //ȡ�ͻ�����С
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�
	CPoint OldBRPoint, BRPoint; //���½�
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD); //�г����пؼ�
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID
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