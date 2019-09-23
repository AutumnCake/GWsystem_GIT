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
	// 查询时间段内的图片
	vector<Table_SlideSet> QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime);

	vector<Table_SlideSet> QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime);


	// 数据库中读取一个病人的分析结果
	//读取一个病人本次的染色体分析结果  用于生成个人表
	CHRO_HandleResult ReadOneResultFromAccess(CString patientname);

	// 将一张图片的分析结果存到数据库
	void SaveOnePicResult(CHRO_HandleResultOnePic resultstruct);
	// 一个病人的分析结果保存到数据库
	bool SaveToAccess(CHRO_HandleResult * pb);
	//读取选中的病人的分析结果 用于生成总表
	vector<CHRO_HandleResult> ReadAllResultFromAccess();

	//读取一个病人本次的微核分析结果  用于生成个人表
	MN_HandleResult ReadOneMNResultFromAccess(CString patientname);

	// 数据库中读取一个病人的系统分析结果
	MN_HandleResult ReadOneMNSystemResultFromAccess(CString patientname);


	// 将一张图片的分析结果存到数据库
	void SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct);

	// 一个病人的微核分析结果保存到数据库
	bool SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult);

	// 数据库中搜索当前病人的处理结果
	vector<MN_HandleResultOnePic> FindCurrentPatientResult(CString patientname);

	//读取选中的病人的分析结果 用于生成总表
	vector<MN_HandleResult> ReadAllMNResultFromAccess();

	// 一个病人的微核分析结果保存到数据库--分析时
	bool SaveToAccessMNAnalysis(MN_HandleResult * pb);
};


