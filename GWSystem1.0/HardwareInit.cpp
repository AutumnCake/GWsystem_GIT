#include "stdafx.h"
#include "HardwareInit.h"
#include"resource.h"


extern CGWSystem10Dlg* pDlg;
extern Scanning_Control *m_Scanning_Control;

CHardwareInit::CHardwareInit()
{
	//相机参数初始化
	m_canceled = FALSE;
	m_hCamera = NULL;                      //相机句柄
	m_bConnected = FALSE;						//建立连接？
	m_bPreviewing = FALSE;						//预览打开
	m_fFrameRate = 100000.0;				// make it run as fast as possible  相机帧率
	m_ulCameraId = 0;						//相机ID
	m_bIsColor = TRUE;						//是否为彩色相机
	m_fZoom = 0.5;
}


CHardwareInit::~CHardwareInit()
{
}

//退出系统后，释放系统资源(断开连接)
bool CHardwareInit::CleanUp()
{
	if (m_canceled)
	{
		return true;
	}
	m_canceled = true;
	if (m_bConnected)
	{
		LucamStreamVideoControl(m_hCamera, STOP_STREAMING, NULL);
	}
	return true;
}
//



//硬件初始化
BOOL CHardwareInit::OnInitDialog(HWND m_pic)
{

	// TODO:  在此添加额外的初始化
	if (!m_Scanning_Control->Connnect_Camera())
	{
		m_Scanning_Control->m_CameraConnected = FALSE;
		AfxMessageBox(_T("相机打开失败！"));		//相机
	}
	else
	{
		m_hCamera = m_Scanning_Control->m_hCamera;
		m_Scanning_Control->m_CameraConnected = TRUE;
	}
	if (!m_Scanning_Control->Video_Preview(m_pic))
	{
		AfxMessageBox(_T("预览失败！"));;			//预览
	}
	else
	{
		m_lffFormat = m_Scanning_Control->m_lffFormat;
		m_fFrameRate = m_Scanning_Control->m_fFrameRate;
		m_bPreviewing = true;
	}

	if (m_Scanning_Control->s_Dev_Drivers->CANET_Init() != 0)
	{
		AfxMessageBox(_T("电机通信失败！"));		//电机通信
		m_Scanning_Control->m_MotorConnected = FALSE;
	}
	else
		m_Scanning_Control->m_MotorConnected = TRUE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//硬件参数初始化
bool CHardwareInit::InitHardwarePara(ALL_PAREMETER* allpara)
{
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;
	m_Conn.OnInitADOAccess();
	sql = _T("select * from 用户参数表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return false;
	}
	try
	{
		vector<long>().swap(allpara->Offset_arr);
		CString ReadString = m_Conn.m_pRecordset->GetCollect(_T("复位偏移量1"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("复位偏移量2"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("复位偏移量3"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("复位偏移量4"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CAN网关ID"));
		USES_CONVERSION;
		allpara->CANET_ID = T2A(ReadString.GetBuffer(0));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CAN通信转换模块ID"));
		allpara->CANET_Port_Num = _ttoi(ReadString.GetBuffer(0));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("聚焦容差"));
		allpara->Focal_Domain_Radius = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("聚焦大步长"));
		allpara->Focusing_Step_Z[0] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("聚焦中步长"));
		allpara->Focusing_Step_Z[1] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("聚焦小步长"));
		allpara->Focusing_Step_Z[2] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("玻片实际宽"));
		allpara->Slide_Size_X = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("玻片实际长"));
		allpara->Slide_Size_y = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("玻片间隔"));
		allpara->Space_Slides = _ttof(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置1"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置2"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置3"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置4"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置5"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("物镜转盘的绝对位置6"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("运动偏差"));
		allpara->Move_Deviation = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("一次扫描的玻片最大数量"));
		allpara->Slide_Max_Num = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("扫描区域X方向的长度"));
		allpara->Scanning_Aero_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("扫描区域Y方向的长度"));
		allpara->Scanning_Aero_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("X轴方向上扫描的点数"));
		allpara->Scanning_Num_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Y轴方向上扫描的点数"));
		allpara->Scanning_Num_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("X轴方向上聚焦的点数"));
		allpara->Focusing_Num_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Y轴方向上聚焦的点数"));
		allpara->Focusing_Num_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("10X视野宽度"));
		allpara->View_Field_Width_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("10X视野高度"));
		allpara->View_Field_Higth_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("膨胀的结构元大小"));
		allpara->Dilate_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("腐蚀的结构元大小"));
		allpara->Erode_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("腐蚀膨胀单元"));
		allpara->ErodeDilate_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		allpara->Min_CA_Aero_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		allpara->Min_CA_Num_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("存图的灯光"));
		allpara->CorrectionMatrix = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("存图的模式"));
		allpara->DemosaicMethod = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CA_XPos_Compensate"));
		allpara->CA_XPos_Compensate = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CA_YPos_Compensate"));
		allpara->CA_YPos_Compensate = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Switch_Objective_ZPos"));
		allpara->Switch_Objective_ZPos = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Pump_Oil_Pulse"));
		allpara->Pump_Oil_Pulse = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Pump_Oil_Vel"));
		allpara->Pump_Oil_Vel = _ttof(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Pump_Oil_Dis"));
		allpara->Pump_Oil_Dis = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Focus_Max_Limit"));
		allpara->Focus_Max_Limit = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Focus_Min_Limit"));
		allpara->Focus_Min_Limit = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Pulse_ratio"));
		allpara->Pulse_ratio = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Focus_Equal_Dis"));
		allpara->Focus_Equal_Dis = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("手动存图路径"));
		allpara->savepath = ReadString;
		//ReadString = m_Conn.m_pRecordset->GetCollect(_T("重新开始或断点开始"));
		//ReadString = m_Conn.m_pRecordset->GetCollect(_T("图像格式"));
		//Complete_Scan->m_ScanningImage_Save_Paras.csExtension = ReadString;

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}

//显示扫描的进度
void CHardwareInit::ProgressShow(int x, int y)
{
	CBrush brs;
	brs.CreateSolidBrush(RGB(255, 0, 0));
	CRect picrct;//要填充的点的区域

	//获取pictureControl的区域
	pDlg->m_progressDraw.GetClientRect(&rectPicture);

	//获取pictureControl的CDC
	pDC = pDlg->m_progressDraw.GetDC();

	picrct.top = rectPicture.top + x * rectPicture.bottom / 40;
	picrct.bottom = rectPicture.top + (x + 1) *rectPicture.bottom / 40;
	picrct.left = rectPicture.left + y * rectPicture.right / 80;
	picrct.right = rectPicture.left + (y + 1) * rectPicture.right / 80;
	pDC->FillRect(&picrct, &brs);

}


// 设置不同物镜下的相机参数
bool CHardwareInit::SetCameraPara(int tablenum)
{
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;
	m_Conn.OnInitADOAccess();
	CString strtemp;
	strtemp.Format(_T("%d"), tablenum);
	sql = _T("select * from 相机参数表\
			 		  where 相机参数表.[物镜ID]='" + strtemp + "' ");
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


		CString  ReadString = m_Conn.m_pRecordset->GetCollect(_T("曝光时间"));
		CT2A strExpo1(ReadString, CP_UTF8);
		FLOAT fValue = (FLOAT)atof(strExpo1);
		int a = LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0);
	
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("总增益"));
		CT2A strExpo2(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo2);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("红色增益"));
		CT2A strExpo3(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo3);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("绿色增益1"));
		CT2A strExpo4(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo4);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN1, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("绿色增益2"));
		CT2A strExpo5(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo5);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN2, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("蓝色增益"));
		CT2A strExpo6(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo6);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Gamma"));
		CT2A strExpo7(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo7);
		LucamSetProperty(m_hCamera, LUCAM_PROP_GAMMA, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("对比度"));
		CT2A strExpo8(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo8);
		LucamSetProperty(m_hCamera, LUCAM_PROP_CONTRAST, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("亮度"));
		CT2A strExpo9(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo9);
		LucamSetProperty(m_hCamera, LUCAM_PROP_BRIGHTNESS, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("色调"));
		CT2A strExpo10(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo10);
		LucamSetProperty(m_hCamera, LUCAM_PROP_HUE, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("饱和度"));
		CT2A strExpo11(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo11);
		LucamSetProperty(m_hCamera, LUCAM_PROP_SATURATION, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("最大曝光"));
		CT2A strExpo12(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo12);
		LucamSetProperty(m_hCamera, LUCAM_PROP_AUTO_EXP_MAXIMUM, fValue, 0);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;
}

bool CHardwareInit::GetCameraPara()
{
	FLOAT fValue = 0;
	LucamGetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, &fValue,0);
	LucamGetProperty(m_hCamera, IDC_EDIT_REDGAIN, &fValue, 0);
	LucamGetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, &fValue, 0);

	return true;
}

