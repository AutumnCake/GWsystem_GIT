
// GWSystem1.0Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include"Resource.h"
#include "GWSystem1.0.h"
#include"Scanning_Control.h"
#include"ADOAccess.h"
#include<vector>

#include"ButtonST.h"

using std::vector;

// CGWSystem10Dlg 对话框
class CGWSystem10Dlg : public CDialogEx 
{
// 构造
public:
	CGWSystem10Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GWSYSTEM10_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

public:
	CString users, usertype; //用户与用户类型

	HANDLE hThread;//开辟的线程句柄
	BOOL BStop;//定时器显示图片停止的标志
	/*thread ScanThread;*/

	vector<UINT> IDC_SlideFrame;
	vector<UINT> IDC_SlideEdit;
	vector<UINT> IDC_ProgressEdit;

	vector<UINT> IDC_Name;//picture控件的数组

	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CEdit m_edit4;
	CEdit m_edit5;
	CEdit m_edit6;
	CEdit m_edit7;
	CEdit m_edit8;

	CEdit m_edit11;
	CEdit m_edit22;
	CEdit m_edit33;
	CEdit m_edit44;
	CEdit m_edit55;
	CEdit m_edit66;
	CEdit m_edit77;
	CEdit m_edit88;

	CStatic m_frame1;
	CStatic m_frame2;
	CStatic m_frame3;
	CStatic m_frame4;
	CStatic m_frame5;
	CStatic m_frame6;
	CStatic m_frame7;
	CStatic m_frame8;

	CToolBar m_wndToolBar;
	CButtonST m_bbtn1;
	CButtonST m_bbtn2;
	CButtonST m_bbtn3;
	CButtonST m_bbtn4;


	CString ElementaryFile;
	MyFiles AllFiles;
	SLIDE *Slide = new SLIDE;
	
	//对话框最大化
	void ReSize();
	POINT old;
	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//用户登录
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//初始设置参数
	afx_msg LRESULT InitialList3(WPARAM wParam, LPARAM lParam);//传递消息给定时器
	afx_msg LRESULT InitialList4(WPARAM wParam, LPARAM lParam);//传递扫描参数到主界面
	afx_msg LRESULT InitialList5(WPARAM wParam, LPARAM lParam);//普通用户聚焦参数传递
	afx_msg LRESULT InitialList6(WPARAM wParam, LPARAM lParam);//管理员聚焦参数设置
	afx_msg LRESULT InitialList7(WPARAM wParam, LPARAM lParam);//管理员电机参数设置
	afx_msg LRESULT InitialList8(WPARAM wParam, LPARAM lParam);//管理员扫描参数设置
	afx_msg LRESULT InitialList9(WPARAM wParam, LPARAM lParam);//画出当前位置点
	afx_msg LRESULT InitialList10(WPARAM wParam, LPARAM lParam);//启用手动操作

	//afx_msg LRESULT Updata_Display_Drawing(WPARAM wParam, LPARAM lParam);//更新显扫描图像





public:
	CStatic m_allframe;
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;

	afx_msg void OnDiaryManagement();
	afx_msg void OnUserManagement();
	afx_msg void OnResetCode();
	afx_msg void OnBnClickedBtnScanbegin();
	afx_msg void OnBnClickedBtnSlideset();
	afx_msg void OnBnClickedBtnLastpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnIkaros();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnIkarosrst();
	afx_msg void OnMenuCamera();
	afx_msg void OnMenuPlatformset();

	afx_msg void OnMenuFocus();
	afx_msg void OnMenuAdmin();
	afx_msg void OnMenuAnalyse();
	CStatic m_progressDraw;
	afx_msg void OnBnClickedBtnStop();
	// 画出当前拍照点
	int DrawCurrentPosition(LOCATION currrentLocation);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


	afx_msg void OnMenuuserexit();

	//开辟的扫描线程执行函数
	void static ScanThreadProc();


	afx_msg void OnBnClickedBtnCaptureimg();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnParaset();
	afx_msg void OnBnClickedBtnHandle();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

DWORD WINAPI ThreadProc(PVOID pParam);//开辟的扫描线程执行函数
