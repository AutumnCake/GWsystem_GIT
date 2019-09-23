// MotorControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "MotorControlDlg.h"
#include "afxdialogex.h"
#include "AllParaSet.h"

extern  ALL_PAREMETER *allpara;
// MotorControlDlg 对话框

IMPLEMENT_DYNAMIC(MotorControlDlg, CDialogEx)

MotorControlDlg::MotorControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MotorControlDlg::IDD, pParent)
{

	//手动遥感
	MotorDlg_Scanning_Control->Handlehold_Thread_Running = false;
}

MotorControlDlg::~MotorControlDlg()
{
	//手动遥感
	MotorDlg_Scanning_Control->Handlehold_Thread_Running = true;
	CWinThread* _pHandlehold_Control = AfxBeginThread(Scanning_Control::Handlehold_ThreadFunc, MotorDlg_Scanning_Control);

}

void MotorControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TABLECHOSE, m_combotablenum);
}


BEGIN_MESSAGE_MAP(MotorControlDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BTN_X_MOVING, &MotorControlDlg::OnBnClickedBtnXMoving)
	ON_BN_CLICKED(IDC_BTN_Y_MOVING, &MotorControlDlg::OnBnClickedBtnYMoving)
	ON_BN_CLICKED(IDC_BTN_STOPALL, &MotorControlDlg::OnBnClickedBtnStopall)
	ON_BN_CLICKED(IDC_BTN_TABLERESET, &MotorControlDlg::OnBnClickedBtnTablereset)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLECHOSE, &MotorControlDlg::OnCbnSelchangeComboTablechose)
	ON_BN_CLICKED(IDC_BTN_X_LEFTMAX, &MotorControlDlg::OnBnClickedBtnXLeftmax)
	ON_BN_CLICKED(IDC_BTN_X_RIGHTMAX, &MotorControlDlg::OnBnClickedBtnXRightmax)
	ON_BN_CLICKED(IDC_BTN_Y_FRONTMAX, &MotorControlDlg::OnBnClickedBtnYFrontmax)
	ON_BN_CLICKED(IDC_BTN_Y_BEHINDMAX, &MotorControlDlg::OnBnClickedBtnYBehindmax)
	ON_BN_CLICKED(IDC_BTN_Z_UPMAX, &MotorControlDlg::OnBnClickedBtnZUpmax)
	ON_BN_CLICKED(IDC_BTN_DOWNMAX, &MotorControlDlg::OnBnClickedBtnDownmax)
	ON_BN_CLICKED(IDC_BTN_X_LEFTMID, &MotorControlDlg::OnBnClickedBtnXLeftmid)
	ON_BN_CLICKED(IDC_BTN_X_RIGHTMID, &MotorControlDlg::OnBnClickedBtnXRightmid)
	ON_BN_CLICKED(IDC_BTN_Y_FRONTMID, &MotorControlDlg::OnBnClickedBtnYFrontmid)
	ON_BN_CLICKED(IDC_BTN_Y_BEHINDMID, &MotorControlDlg::OnBnClickedBtnYBehindmid)
	ON_BN_CLICKED(IDC_BTN_Z_UPMID, &MotorControlDlg::OnBnClickedBtnZUpmid)
	ON_BN_CLICKED(IDC_BTN_DOWNMID, &MotorControlDlg::OnBnClickedBtnDownmid)
	ON_BN_CLICKED(IDC_BUTTON5IDC_BTN_X_LEFTMIN, &MotorControlDlg::OnBnClickedButton5idcBtnXLeftmin)
	ON_BN_CLICKED(IDC_BTN_X_RIGHTMIN, &MotorControlDlg::OnBnClickedBtnXRightmin)
	ON_BN_CLICKED(IDC_BTN_Y_FRONTMIN, &MotorControlDlg::OnBnClickedBtnYFrontmin)
	ON_BN_CLICKED(IDC_BTN_Y_BEHINDMIN, &MotorControlDlg::OnBnClickedBtnYBehindmin)
	ON_BN_CLICKED(IDC_BTN_Z_UPMIN, &MotorControlDlg::OnBnClickedBtnZUpmin)
	ON_BN_CLICKED(IDC_BTN_DOWNMIN, &MotorControlDlg::OnBnClickedBtnDownmin)
	ON_BN_CLICKED(IDC_BTN_RESETMOTOR, &MotorControlDlg::OnBnClickedBtnResetmotor)
	ON_BN_CLICKED(IDC_BTN_X_MOVETO, &MotorControlDlg::OnBnClickedBtnXMoveto)
	ON_BN_CLICKED(IDC_BTN_Y_MOVETO, &MotorControlDlg::OnBnClickedBtnYMoveto)
	ON_BN_CLICKED(IDC_BTN_Z_MOVETO, &MotorControlDlg::OnBnClickedBtnZMoveto)
	ON_BN_CLICKED(IDC_BTN_READCURRENTPOS, &MotorControlDlg::OnBnClickedBtnReadcurrentpos)
END_MESSAGE_MAP()


// MotorControlDlg 消息处理程序


BOOL MotorControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combotablenum.AddString(_T("4 X"));
	m_combotablenum.AddString(_T("10 X"));
	m_combotablenum.AddString(_T("20 X"));
	m_combotablenum.AddString(_T("40 X"));
	m_combotablenum.AddString(_T("60 X"));
	m_combotablenum.AddString(_T("100 X"));
	m_combotablenum.SetCurSel(3);

	GetDlgItem(IDC_EDIT_X_MOVESPEED)->SetWindowText(_T("0.1"));
	GetDlgItem(IDC_EDIT_Y_MOVESPEED)->SetWindowText(_T("0.1"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void MotorControlDlg::Pass_Motor_Paras(Scanning_Control *s_Scanning_Control, BOOL s_MotorConnected)
{
	MotorDlg_Scanning_Control = s_Scanning_Control;
	BOOL m_MotorConnected = s_MotorConnected;

}


void MotorControlDlg::OnBnClickedBtnXMoving()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!MotorDlg_Scanning_Control->m_XVelocty_Model_Starting)
	{
		CString csTemp;
		double V_Model_Speed;
		GetDlgItem(IDC_EDIT_X_MOVESPEED)->GetWindowText(csTemp);

		V_Model_Speed = atof(CT2A(csTemp));
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 2);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_START, V_Model_Speed);
		MotorDlg_Scanning_Control->m_XVelocty_Model_Starting = true;
		GetDlgItem(IDC_BTN_X_MOVING)->SetWindowText(_T("X 连续 停止"));
	}
	else
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Velocity_Model(1, Velocity_Model_STOP);
		MotorDlg_Scanning_Control->m_XVelocty_Model_Starting = false;
		GetDlgItem(IDC_BTN_X_MOVING)->SetWindowText(_T("X 连续 启动"));
	}

}


void MotorControlDlg::OnBnClickedBtnYMoving()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!MotorDlg_Scanning_Control->m_YVelocty_Model_Starting)
	{
		CString csTemp;
		double V_Model_Speed;
		GetDlgItem(IDC_EDIT_Y_MOVESPEED)->GetWindowText(csTemp);

		V_Model_Speed = atof(CT2A(csTemp));
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 2);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_START, V_Model_Speed);
		MotorDlg_Scanning_Control->m_YVelocty_Model_Starting = true;
		GetDlgItem(IDC_BTN_Y_MOVING)->SetWindowText(_T("Y 连续 停止"));
	}
	else
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Velocity_Model(2, Velocity_Model_STOP);
		MotorDlg_Scanning_Control->m_YVelocty_Model_Starting = false;
		GetDlgItem(IDC_BTN_Y_MOVING)->SetWindowText(_T("Y 连续 启动"));
	}

}


void MotorControlDlg::OnBnClickedBtnStopall()
{
	// TODO:  在此添加控件通知处理程序代码
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(1, 0);
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(2, 0);
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(3, 0);
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(4, 0);
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(5, 0);

}


void MotorControlDlg::OnBnClickedBtnTablereset()
{
	// TODO:  在此添加控件通知处理程序代码
	MotorDlg_Scanning_Control->Do_Resetting(4);

	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 500) && (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 4 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		Sleep(100);
		k++;
	}

	int Table_Num_Now;
	Table_Num_Now = m_combotablenum.GetCurSel();
	//GetDlgItem(IDC_COMBO_TABLECHOSE)->GetWindowText(csTemp);
	//Table_Num_Now = atol(CT2A(csTemp));

	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(4, 0);
	MotorDlg_Scanning_Control->Do_Move_To_Object(Table_Num_Now);

	status_arr = { 0 };
	k = 1;
	while ((k < 500) && (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 4 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		Sleep(100);
		k++;
	}

	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(4, false);
}


void MotorControlDlg::OnCbnSelchangeComboTablechose()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString csTemp;
	int Table_Num_Now;
	Table_Num_Now = m_combotablenum.GetCurSel();
	//GetDlgItem(IDC_COMBO_TABLECHOSE)->GetWindowText(csTemp);
	//Table_Num_Now = atol(CT2A(csTemp));

	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(4, 0);
	MotorDlg_Scanning_Control->Do_Move_To_Object(Table_Num_Now);

	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 500) && (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 4 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		Sleep(100);
		k++;
	}
	MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_ENA(4, false);

}


void MotorControlDlg::OnBnClickedBtnXLeftmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 1 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}

	long X_Real_Pos = 0;
	k = 0;

	while (!MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Pos(1, X_Real_Pos))
	{
		if (k > 10)
		{
			AfxMessageBox(_T("点动运动失败，请重试或取消！"));
			return;
		}
		k++;

	}
	X_Real_Pos += -2800;
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1,0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, X_Real_Pos, 10, true);
	}

}


void MotorControlDlg::OnBnClickedBtnXRightmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;

	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 1 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	long X_Real_Pos = 0;
	k = 0;
	while (!MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Pos(1, X_Real_Pos))
	{
		if (k > 10)
		{
			AfxMessageBox(_T("点动运动失败，请重试或取消！"));
			return;
		}
		k++;

	}
	X_Real_Pos += 2800;
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, X_Real_Pos, 10, true);
	}
		

}


void MotorControlDlg::OnBnClickedBtnYFrontmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 2 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(2, -2800, 10, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnYBehindmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 2 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(2, 2800, 10, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnZUpmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, -2048, 5, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnDownmax()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{	
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, 2048, 5, false);
	}
		
}


void MotorControlDlg::OnBnClickedBtnXLeftmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 1 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, -200, 10, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnXRightmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 1 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, 200, 10, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnYFrontmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 2 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(2, -200, 10, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnYBehindmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 2 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(2, 200, 10, false);
	}
		
}

void MotorControlDlg::OnBnClickedBtnZUpmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, -80, 5, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnDownmid()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, 80, 5, false);
	}
		
}


void MotorControlDlg::OnBnClickedButton5idcBtnXLeftmin()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, -100, 5, false);
	}
		
}


void MotorControlDlg::OnBnClickedBtnXRightmin()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, 100, 5, false);
	}
		
}


void MotorControlDlg::OnBnClickedBtnYFrontmin()
{
	// TODO:  在此添加控件通知处理程序代码

}


void MotorControlDlg::OnBnClickedBtnYBehindmin()
{
	// TODO:  在此添加控件通知处理程序代码
}


void MotorControlDlg::OnBnClickedBtnZUpmin()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, -20.48, 5, false);
	}
		

}


void MotorControlDlg::OnBnClickedBtnDownmin()
{
	// TODO:  在此添加控件通知处理程序代码
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, 20.48, 5, false);
	}
		
}


void MotorControlDlg::OnBnClickedBtnResetmotor()
{
	// TODO:  在此添加控件通知处理程序代码

	MotorDlg_Scanning_Control->Do_restting_Model();
}


void MotorControlDlg::OnBnClickedBtnXMoveto()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Movetopos_str;
	GetDlgItem(IDC_EDIT_X_MOVETOPOS)->GetWindowTextW(Movetopos_str);
	long MovetoPos = _ttol(Movetopos_str);
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 1 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(1, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(1, MovetoPos, 10, true);
	}
		
}


void MotorControlDlg::OnBnClickedBtnYMoveto()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Movetopos_str;
	GetDlgItem(IDC_EDIT_Y_MOVETOPOS)->GetWindowTextW(Movetopos_str);
	long MovetoPos = _ttol(Movetopos_str);
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 2 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(2, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(2, MovetoPos, 10, true);
	}

}


void MotorControlDlg::OnBnClickedBtnZMoveto()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Movetopos_str;
	GetDlgItem(IDC_EDIT_Z_MOVETOPOS)->GetWindowTextW(Movetopos_str);
	long MovetoPos = _ttol(Movetopos_str);
	vector<bool> status_arr = { 0 };
	int k = 1;
	while ((k < 5) || (!status_arr[0]))
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = { 3 };
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		k++;
	}
	if (status_arr[0])//20表示回零完成，4表示相对运动到位
	{
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov_Para(3, 0);
		MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Single_Mov(3, MovetoPos, 3, true);
	}
		

}


void MotorControlDlg::OnBnClickedBtnReadcurrentpos()
{
	// TODO:  在此添加控件通知处理程序代码
	long real_Pos = 0;
	CString Real_Pos;
	if (MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Pos(1, real_Pos))
	{
		Real_Pos.Format(_T("%ld"), real_Pos);
		GetDlgItem(IDC_EDIT_X_CURRENTPOS)->SetWindowText(Real_Pos);
	}
	else
	{
		AfxMessageBox(_T("获取位置信息失败！"));
	}
	if (MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Pos(2, real_Pos))
	{
		Real_Pos.Format(_T("%ld"), real_Pos);
		GetDlgItem(IDC_EDIT_Y_CURRENTPOS)->SetWindowText(Real_Pos);
	}
	else
	{
		AfxMessageBox(_T("获取位置信息失败！"));
	}
	if (MotorDlg_Scanning_Control->s_Dev_Drivers->Dev_Read_Pos(3, real_Pos))
	{
		Real_Pos.Format(_T("%ld"), real_Pos);
		GetDlgItem(IDC_EDIT_Z_CURRENTPOS)->SetWindowText(Real_Pos);
	}
	else
	{
		AfxMessageBox(_T("获取位置信息失败！"));
	}

}
