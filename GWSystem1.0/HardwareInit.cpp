#include "stdafx.h"
#include "HardwareInit.h"
#include"resource.h"


extern CGWSystem10Dlg* pDlg;
extern Scanning_Control *m_Scanning_Control;

CHardwareInit::CHardwareInit()
{
	//���������ʼ��
	m_canceled = FALSE;
	m_hCamera = NULL;                      //������
	m_bConnected = FALSE;						//�������ӣ�
	m_bPreviewing = FALSE;						//Ԥ����
	m_fFrameRate = 100000.0;				// make it run as fast as possible  ���֡��
	m_ulCameraId = 0;						//���ID
	m_bIsColor = TRUE;						//�Ƿ�Ϊ��ɫ���
	m_fZoom = 0.5;
}


CHardwareInit::~CHardwareInit()
{
}

//�˳�ϵͳ���ͷ�ϵͳ��Դ(�Ͽ�����)
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



//Ӳ����ʼ��
BOOL CHardwareInit::OnInitDialog(HWND m_pic)
{

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!m_Scanning_Control->Connnect_Camera())
	{
		m_Scanning_Control->m_CameraConnected = FALSE;
		AfxMessageBox(_T("�����ʧ�ܣ�"));		//���
	}
	else
	{
		m_hCamera = m_Scanning_Control->m_hCamera;
		m_Scanning_Control->m_CameraConnected = TRUE;
	}
	if (!m_Scanning_Control->Video_Preview(m_pic))
	{
		AfxMessageBox(_T("Ԥ��ʧ�ܣ�"));;			//Ԥ��
	}
	else
	{
		m_lffFormat = m_Scanning_Control->m_lffFormat;
		m_fFrameRate = m_Scanning_Control->m_fFrameRate;
		m_bPreviewing = true;
	}

	if (m_Scanning_Control->s_Dev_Drivers->CANET_Init() != 0)
	{
		AfxMessageBox(_T("���ͨ��ʧ�ܣ�"));		//���ͨ��
		m_Scanning_Control->m_MotorConnected = FALSE;
	}
	else
		m_Scanning_Control->m_MotorConnected = TRUE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//Ӳ��������ʼ��
bool CHardwareInit::InitHardwarePara(ALL_PAREMETER* allpara)
{
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;
	m_Conn.OnInitADOAccess();
	sql = _T("select * from �û�������");
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
		CString ReadString = m_Conn.m_pRecordset->GetCollect(_T("��λƫ����1"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��λƫ����2"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��λƫ����3"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��λƫ����4"));
		allpara->Offset_arr.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CAN����ID"));
		USES_CONVERSION;
		allpara->CANET_ID = T2A(ReadString.GetBuffer(0));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("CANͨ��ת��ģ��ID"));
		allpara->CANET_Port_Num = _ttoi(ReadString.GetBuffer(0));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�۽��ݲ�"));
		allpara->Focal_Domain_Radius = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�۽��󲽳�"));
		allpara->Focusing_Step_Z[0] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�۽��в���"));
		allpara->Focusing_Step_Z[1] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�۽�С����"));
		allpara->Focusing_Step_Z[2] = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��Ƭʵ�ʿ�"));
		allpara->Slide_Size_X = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��Ƭʵ�ʳ�"));
		allpara->Slide_Size_y = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��Ƭ���"));
		allpara->Space_Slides = _ttof(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��1"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��2"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��3"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��4"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��5"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ﾵת�̵ľ���λ��6"));
		allpara->Nosepiece_Abs_Position.push_back(_ttol(ReadString));
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�˶�ƫ��"));
		allpara->Move_Deviation = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("һ��ɨ��Ĳ�Ƭ�������"));
		allpara->Slide_Max_Num = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("ɨ������X����ĳ���"));
		allpara->Scanning_Aero_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("ɨ������Y����ĳ���"));
		allpara->Scanning_Aero_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("X�᷽����ɨ��ĵ���"));
		allpara->Scanning_Num_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Y�᷽����ɨ��ĵ���"));
		allpara->Scanning_Num_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("X�᷽���Ͼ۽��ĵ���"));
		allpara->Focusing_Num_X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Y�᷽���Ͼ۽��ĵ���"));
		allpara->Focusing_Num_Y = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("10X��Ұ���"));
		allpara->View_Field_Width_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("10X��Ұ�߶�"));
		allpara->View_Field_Higth_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("���͵ĽṹԪ��С"));
		allpara->Dilate_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ʴ�ĽṹԪ��С"));
		allpara->Erode_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ʴ���͵�Ԫ"));
		allpara->ErodeDilate_element = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Min_CA_Aero_10X"));
		allpara->Min_CA_Aero_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Min_CA_Num_10X"));
		allpara->Min_CA_Num_10X = _ttoi(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ͼ�ĵƹ�"));
		allpara->CorrectionMatrix = _ttol(ReadString);
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ͼ��ģʽ"));
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
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ֶ���ͼ·��"));
		allpara->savepath = ReadString;
		//ReadString = m_Conn.m_pRecordset->GetCollect(_T("���¿�ʼ��ϵ㿪ʼ"));
		//ReadString = m_Conn.m_pRecordset->GetCollect(_T("ͼ���ʽ"));
		//Complete_Scan->m_ScanningImage_Save_Paras.csExtension = ReadString;

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}

	return true;
}

//��ʾɨ��Ľ���
void CHardwareInit::ProgressShow(int x, int y)
{
	CBrush brs;
	brs.CreateSolidBrush(RGB(255, 0, 0));
	CRect picrct;//Ҫ���ĵ������

	//��ȡpictureControl������
	pDlg->m_progressDraw.GetClientRect(&rectPicture);

	//��ȡpictureControl��CDC
	pDC = pDlg->m_progressDraw.GetDC();

	picrct.top = rectPicture.top + x * rectPicture.bottom / 40;
	picrct.bottom = rectPicture.top + (x + 1) *rectPicture.bottom / 40;
	picrct.left = rectPicture.left + y * rectPicture.right / 80;
	picrct.right = rectPicture.left + (y + 1) * rectPicture.right / 80;
	pDC->FillRect(&picrct, &brs);

}


// ���ò�ͬ�ﾵ�µ��������
bool CHardwareInit::SetCameraPara(int tablenum)
{
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;
	m_Conn.OnInitADOAccess();
	CString strtemp;
	strtemp.Format(_T("%d"), tablenum);
	sql = _T("select * from ���������\
			 		  where ���������.[�ﾵID]='" + strtemp + "' ");
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


		CString  ReadString = m_Conn.m_pRecordset->GetCollect(_T("�ع�ʱ��"));
		CT2A strExpo1(ReadString, CP_UTF8);
		FLOAT fValue = (FLOAT)atof(strExpo1);
		int a = LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0);
	
		ReadString = m_Conn.m_pRecordset->GetCollect(_T("������"));
		CT2A strExpo2(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo2);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ɫ����"));
		CT2A strExpo3(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo3);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ɫ����1"));
		CT2A strExpo4(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo4);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN1, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ɫ����2"));
		CT2A strExpo5(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo5);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN2, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ɫ����"));
		CT2A strExpo6(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo6);
		a = LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("Gamma"));
		CT2A strExpo7(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo7);
		LucamSetProperty(m_hCamera, LUCAM_PROP_GAMMA, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("�Աȶ�"));
		CT2A strExpo8(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo8);
		LucamSetProperty(m_hCamera, LUCAM_PROP_CONTRAST, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("����"));
		CT2A strExpo9(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo9);
		LucamSetProperty(m_hCamera, LUCAM_PROP_BRIGHTNESS, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("ɫ��"));
		CT2A strExpo10(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo10);
		LucamSetProperty(m_hCamera, LUCAM_PROP_HUE, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("���Ͷ�"));
		CT2A strExpo11(ReadString, CP_UTF8);
		fValue = (FLOAT)atof(strExpo11);
		LucamSetProperty(m_hCamera, LUCAM_PROP_SATURATION, fValue, 0);

		ReadString = m_Conn.m_pRecordset->GetCollect(_T("����ع�"));
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

