// ShowImgIntoFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "ShowImgIntoFrame.h"
#include "afxdialogex.h"


// CShowImgIntoFrame 对话框

IMPLEMENT_DYNAMIC(CShowImgIntoFrame, CDialogEx)

CShowImgIntoFrame::CShowImgIntoFrame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowImgIntoFrame::IDD, pParent)
{
	//将图片显示框ID存入数组
	FrameName.push_back(IDC_FIGUREFRAME1);
	FrameName.push_back(IDC_FIGUREFRAME2);
	FrameName.push_back(IDC_FIGUREFRAME3);
	FrameName.push_back(IDC_FIGUREFRAME4);
	FrameName.push_back(IDC_FIGUREFRAME5);
	FrameName.push_back(IDC_FIGUREFRAME6);
	FrameName.push_back(IDC_FIGUREFRAME7);
	FrameName.push_back(IDC_FIGUREFRAME8);
	FrameName.push_back(IDC_FIGUREFRAME9);
	FrameName.push_back(IDC_FIGUREFRAME10);
	FrameName.push_back(IDC_FIGUREFRAME11);
	FrameName.push_back(IDC_FIGUREFRAME12);
	FrameName.push_back(IDC_FIGUREFRAME13);
	FrameName.push_back(IDC_FIGUREFRAME14);
	FrameName.push_back(IDC_FIGUREFRAME15);
}

CShowImgIntoFrame::~CShowImgIntoFrame()
{
}

void CShowImgIntoFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//图片显示框绑定变量
	DDX_Control(pDX, IDC_FIGUREFRAME1, m_frame1);
	DDX_Control(pDX, IDC_FIGUREFRAME2, m_frame2);
	DDX_Control(pDX, IDC_FIGUREFRAME3, m_frame3);
	DDX_Control(pDX, IDC_FIGUREFRAME4, m_frame4);
	DDX_Control(pDX, IDC_FIGUREFRAME5, m_frame5);
	DDX_Control(pDX, IDC_FIGUREFRAME6, m_frame6);
	DDX_Control(pDX, IDC_FIGUREFRAME7, m_frame7);
	DDX_Control(pDX, IDC_FIGUREFRAME8, m_frame8);
	DDX_Control(pDX, IDC_FIGUREFRAME9, m_frame9);
	DDX_Control(pDX, IDC_FIGUREFRAME10, m_frame10);
	DDX_Control(pDX, IDC_FIGUREFRAME11, m_frame11);
	DDX_Control(pDX, IDC_FIGUREFRAME12, m_frame12);
	DDX_Control(pDX, IDC_FIGUREFRAME13, m_frame13);
	DDX_Control(pDX, IDC_FIGUREFRAME14, m_frame14);
	DDX_Control(pDX, IDC_FIGUREFRAME15, m_frame15);
}


BEGIN_MESSAGE_MAP(CShowImgIntoFrame, CDialogEx)
END_MESSAGE_MAP()


// CShowImgIntoFrame 消息处理程序


BOOL CShowImgIntoFrame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获得15个图片显示框对应的区域，转换为client坐标，并存入数组
	GetDlgItem(IDC_FIGUREFRAME1)->GetWindowRect(&rect1); ScreenToClient(&rect1); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME2)->GetWindowRect(&rect2); ScreenToClient(&rect2); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME3)->GetWindowRect(&rect3); ScreenToClient(&rect3); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME4)->GetWindowRect(&rect4); ScreenToClient(&rect4); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME5)->GetWindowRect(&rect5); ScreenToClient(&rect5); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME6)->GetWindowRect(&rect6); ScreenToClient(&rect6); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME7)->GetWindowRect(&rect7); ScreenToClient(&rect7); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME8)->GetWindowRect(&rect8); ScreenToClient(&rect8); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME9)->GetWindowRect(&rect9); ScreenToClient(&rect9); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME10)->GetWindowRect(&rect10); ScreenToClient(&rect10); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME11)->GetWindowRect(&rect11); ScreenToClient(&rect11); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME12)->GetWindowRect(&rect12); ScreenToClient(&rect12); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME13)->GetWindowRect(&rect13); ScreenToClient(&rect13); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME14)->GetWindowRect(&rect14); ScreenToClient(&rect14); rect.push_back(rect1);
	GetDlgItem(IDC_FIGUREFRAME15)->GetWindowRect(&rect15); ScreenToClient(&rect15); rect.push_back(rect1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


// 查找文件夹里的所有图片路径
void CShowImgIntoFrame::FindAllImgFilePath()
{
	char* fileName = nullptr;
	int find_pic = 0;//的图片的序列号
	vector <string>().swap(imgNames);
}
