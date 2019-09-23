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

	CApp = (CGWSystem10App*)::AfxGetApp();//��������ָ�룬��Ҫ����

	m_username=CApp->convert1;

	m_usertype=CApp->convert2;      //��¼�ѵ�¼���û������伶��

	CTime time = CTime::GetCurrentTime(); ///����CTime����

	int m_nYear = time.GetYear(); ///��

	int m_nMonth = time.GetMonth(); ///��

	int m_nDay = time.GetDay(); ///��

	int m_nHour = time.GetHour(); ///Сʱ

	int m_nMinute = time.GetMinute(); ///����

	int m_nSecond = time.GetSecond(); ///��

	CString strDate;

	strDate.Format(_T("%d-%d-%d"),m_nYear,m_nMonth,m_nDay); //�������ڸ�ʽ

	CString x_Date;//��ˮ��ǰ8λ

	x_Date.Format(_T("%d%.2d%.2d"),m_nYear,m_nMonth,m_nDay); //�������ڸ�ʽ

	CApp->convert3 = strDate;

	CString strTime = time.Format("%H:%M:%S"); //����ʱ���ʽ

	CString x_Time = time.Format("%H%M%S"); //��ˮ�ź���λ


	ADOConn m_Conn;

	m_Conn.OnInitADOConn();

	CString sql,str1,str2,str3;

	sql="select * from ��־����";

	try
	{
		m_Conn.m_pRecordset.CreateInstance(__uuidof(Recordset));

		m_Conn.m_pRecordset->Open((_bstr_t)sql,m_Conn.m_pConnection.GetInterfacePtr(),adOpenStatic ,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)

	{

		e.Description();

	}

	int i =m_Conn.m_pRecordset->GetRecordCount();//���ô˹��ܣ�Open�����Ĳ�������Ϊ��adOpenStatic����

	try

	{

		str2=x_Date+x_Time;

		m_Conn.m_pRecordset->AddNew();

		m_Conn.m_pRecordset->PutCollect(_T("��ˮ��"),_variant_t(str2));

		m_Conn.m_pRecordset->PutCollect(_T("�û�"),_variant_t(m_username));

		m_Conn.m_pRecordset->PutCollect(_T("����"),_variant_t(m_usertype));

		m_Conn.m_pRecordset->PutCollect(_T("����"),_variant_t(strDate));

		m_Conn.m_pRecordset->PutCollect(_T("ʱ��"),_variant_t(strTime));

		m_Conn.m_pRecordset->PutCollect(_T("��������"),_variant_t(str));

		m_Conn.m_pRecordset->Update();

	}

	catch(_com_error *e)

	{

		AfxMessageBox(e->ErrorMessage());

	}

	m_Conn.ExitConnect();

}


