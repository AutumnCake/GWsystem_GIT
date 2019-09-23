#include "stdafx.h"
#include "AllParaSet.h"



AllParaSet::AllParaSet()
{
}


AllParaSet::~AllParaSet()
{
}


// 硬件参数设置
bool AllParaSet::HardwareParaSet(Scanning_Control * ScanningParaSet, ALL_PAREMETER *allpara)
{
	ScanningParaSet->m_ScanningImage_Save_Paras.csExtension = ".bmp"; //".raw";
	ScanningParaSet->m_FocusingImage_Save_Paras.csExtension = ".bmp"; //".raw";

	ScanningParaSet->m_ScanningImage_Save_Paras.lcConversion.CorrectionMatrix = allpara->CorrectionMatrix;
	ScanningParaSet->m_ScanningImage_Save_Paras.lcConversion.DemosaicMethod = allpara->DemosaicMethod;
	ScanningParaSet->m_FocusingImage_Save_Paras.lcConversion.CorrectionMatrix = allpara->CorrectionMatrix;
	ScanningParaSet->m_FocusingImage_Save_Paras.lcConversion.DemosaicMethod = allpara->DemosaicMethod;


	ScanningParaSet->s_Scanning_System_Para.Offset_arr = allpara->Offset_arr;
	ScanningParaSet->s_Scanning_System_Para.CANET_ID = allpara->CANET_ID;
	ScanningParaSet->s_Scanning_System_Para.CANET_Port_Num = allpara->CANET_Port_Num;
	ScanningParaSet->s_Scanning_System_Para.Focusing_Step_Z[0] = allpara->Focusing_Step_Z[0];
	ScanningParaSet->s_Scanning_System_Para.Focusing_Step_Z[1] = allpara->Focusing_Step_Z[1];
	ScanningParaSet->s_Scanning_System_Para.Focusing_Step_Z[2] = allpara->Focusing_Step_Z[2];
	ScanningParaSet->s_Scanning_System_Para.Slide_Size_X = allpara->Slide_Size_X;
	ScanningParaSet->s_Scanning_System_Para.Slide_Size_y = allpara->Slide_Size_y;
	ScanningParaSet->s_Scanning_System_Para.Space_Slides = allpara->Space_Slides;
	ScanningParaSet->s_Scanning_System_Para.Nosepiece_Abs_Position = allpara->Nosepiece_Abs_Position;
	ScanningParaSet->s_Scanning_System_Para.Move_Deviation = allpara->Move_Deviation;
	ScanningParaSet->s_Scanning_System_Para.View_Field_Width_10X = allpara->View_Field_Width_10X;
	ScanningParaSet->s_Scanning_System_Para.View_Field_Higth_10X = allpara->View_Field_Higth_10X;
	ScanningParaSet->s_Scanning_System_Para.CA_XPos_Compensate = allpara->CA_XPos_Compensate;
	ScanningParaSet->s_Scanning_System_Para.CA_YPos_Compensate = allpara->CA_YPos_Compensate;
	ScanningParaSet->s_Scanning_System_Para.Switch_Objective_ZPos = allpara->Switch_Objective_ZPos;
	ScanningParaSet->s_Scanning_System_Para.Pump_Oil_Pulse = allpara->Pump_Oil_Pulse;
	ScanningParaSet->s_Scanning_System_Para.Pump_Oil_Vel = allpara->Pump_Oil_Vel;
	ScanningParaSet->s_Scanning_System_Para.Pump_Oil_Dis = allpara->Pump_Oil_Dis;
	ScanningParaSet->s_Scanning_System_Para.Focus_Max_Limit = allpara->Focus_Max_Limit;
	ScanningParaSet->s_Scanning_System_Para.Focus_Min_Limit = allpara->Focus_Min_Limit;
	ScanningParaSet->s_Scanning_System_Para.Pulse_ratio = allpara->Pulse_ratio;

	ScanningParaSet->s_User_Para.Slide_Max_Num = allpara->Slide_Max_Num;
	ScanningParaSet->s_User_Para.Scanning_Num_X = allpara->Scanning_Num_X;
	ScanningParaSet->s_User_Para.Scanning_Num_Y = allpara->Scanning_Num_Y;
	ScanningParaSet->s_User_Para.m_Focus_Spacing_X = allpara->Focusing_Num_X;
	ScanningParaSet->s_User_Para.m_Focus_Spacing_Y = allpara->Focusing_Num_Y;


	return false;
}
