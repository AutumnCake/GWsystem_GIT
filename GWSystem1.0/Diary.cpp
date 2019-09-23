#include "StdAfx.h"
#include "Diary.h"
#include "ADOConn.h"
#include "GWSystem1.0.h"
#include "time.h"

CDiary::CDiary(void)
{
}

CDiary::~CDiary(void)
{
}

void CDiary::LogRecord(CString str)
{
	CString m_username,m_usertype;

	CGWSystem10App *CApp;

	CApp = (CGWSystem10App*)::AfxGetApp();//获得主框架指针，重要！！

	m_username=CApp->convert1;

	m_usertype=CApp->convert2;      //记录已登录的用户名及其级别

	CTime time = CTime::GetCurrentTime(); ///构造CTime对象

	int m_nYear = time.GetYear(); ///年

	int m_nMonth = time.GetMonth(); ///月

	int m_nDay = time.GetDay(); ///日

	int m_nHour = time.GetHour(); ///小时

	int m_nMinute = time.GetMinute(); ///分钟

	int m_nSecond = time.GetSecond(); ///秒

	CString strDate;

	strDate.Format(_T("%d-%d-%d"),m_nYear,m_nMonth,m_nDay); //定义日期格式

	CString x_Date;//流水号前8位

	x_Date.Format(_T("%d%.2d%.2d"),m_nYear,m_nMonth,m_nDay); //定义日期格式

	CApp->convert3 = strDate;

	CString strTime = time.Format("%H:%M:%S"); //定义时间格式

	CString x_Time = time.Format("%H%M%S"); //流水号后六位


	ADOConn m_Conn;

	m_Conn.OnInitADOConn();

	CString sql,str1,str2,str3;

	sql="select * from 日志管理";

	try
	{
		m_Conn.m_pRecordset.CreateInstance(__uuidof(Recordset));

		m_Conn.m_pRecordset->Open((_bstr_t)sql,m_Conn.m_pConnection.GetInterfacePtr(),adOpenStatic ,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)

	{

		e.Description();

	}

	int i =m_Conn.m_pRecordset->GetRecordCount();//欲用此功能，Open函数的参数必须为“adOpenStatic”！

	try

	{

		str2=x_Date+x_Time;

		m_Conn.m_pRecordset->AddNew();

		m_Conn.m_pRecordset->PutCollect(_T("流水号"),_variant_t(str2));

		m_Conn.m_pRecordset->PutCollect(_T("用户"),_variant_t(m_username));

		m_Conn.m_pRecordset->PutCollect(_T("级别"),_variant_t(m_usertype));

		m_Conn.m_pRecordset->PutCollect(_T("日期"),_variant_t(strDate));

		m_Conn.m_pRecordset->PutCollect(_T("时间"),_variant_t(strTime));

		m_Conn.m_pRecordset->PutCollect(_T("操作内容"),_variant_t(str));

		m_Conn.m_pRecordset->Update();

	}

	catch(_com_error *e)

	{

		AfxMessageBox(e->ErrorMessage());

	}

	m_Conn.ExitConnect();

}


