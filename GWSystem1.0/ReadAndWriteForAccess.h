#pragma once
#include "ADOAccess.h"
#include "GWSystem1.0.h"
#include<vector>

using std::vector;



class CReadAndWriteForAccess
{
public:
	CReadAndWriteForAccess();
	~CReadAndWriteForAccess();

public:
	ADOAccess m_Conn;
	_bstr_t sql;
	_variant_t var;


public:

	bool WriteToSlideSet(Table_SlideSet slideset);

	vector<Table_SlideSet> ReadFromSlideSet();
	// ��ѯʱ����ڵ�ͼƬ
	vector<Table_SlideSet> QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime);


	// ���ݿ��ж�ȡһ�����˵ķ������
	//��ȡһ�����˱��ε�Ⱦɫ��������  �������ɸ��˱�
	CHRO_HandleResult ReadOneResultFromAccess(CString patientname);

	// ��һ��ͼƬ�ķ�������浽���ݿ�
	void SaveOnePicResult(CHRO_HandleResultOnePic resultstruct);
	// һ�����˵ķ���������浽���ݿ�
	bool SaveToAccess(CHRO_HandleResult * pb);
	//��ȡѡ�еĲ��˵ķ������ ���������ܱ�
	vector<CHRO_HandleResult> ReadAllResultFromAccess();

	//��ȡһ�����˱��ε�΢�˷������  �������ɸ��˱�
	MN_HandleResult ReadOneMNResultFromAccess(CString patientname);

	// ���ݿ��ж�ȡһ�����˵�ϵͳ�������
	MN_HandleResult ReadOneMNSystemResultFromAccess(CString patientname);


	// ��һ��ͼƬ�ķ�������浽���ݿ�
	void SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct);

	// һ�����˵�΢�˷���������浽���ݿ�
	bool SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult);

	// ���ݿ���������ǰ���˵Ĵ�����
	vector<MN_HandleResultOnePic> FindCurrentPatientResult(CString patientname);

	//��ȡѡ�еĲ��˵ķ������ ���������ܱ�
	vector<MN_HandleResult> ReadAllMNResultFromAccess();

	// һ�����˵�΢�˷���������浽���ݿ�--����ʱ
	bool SaveToAccessMNAnalysis(MN_HandleResult * pb);
};


