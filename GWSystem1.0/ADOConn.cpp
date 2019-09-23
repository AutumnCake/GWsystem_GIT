#include "StdAfx.h"
#include "ADOConn.h"


ADOConn::ADOConn(void)
{
}

ADOConn::~ADOConn(void)
{
}

void ADOConn::OnInitADOConn(void)
{
	try
	{
		::CoInitialize(NULL);//��ʼ��COM����
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect="Dsn=Cellx64.0.01";
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

void ADOConn::ExitConnect(void)
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	::CoUninitialize();
}

BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
	    return true;
	}
	catch(_com_error e)
	{
		e.Description();
		return false;
	}
}

_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		e.Description();
	}
	return m_pRecordset;
}

BOOL ADOConn::ShowADOView(CString fields[], int len, CADOView* view)     //��CADOView* viewָ���Ĵ������б����ʽ��ʾ�Ѿ��򿪵����ݿ��е����ݱ�
{
	int i=0;    //�����ʼ�е�λ��,��0��

	CString * str = new CString[len];  //����һ������len��Ԫ�ص��ַ�������,�Դ����Ӧ�ֶμ��ж�Ӧ������
	
	view->DeleteAllRows();    //ɾ��������,�Ա����¹�����Ҫ����

	if(m_pRecordset->adoEOF)       //�жϼ�¼��ָ���Ƿ������ݱ��ĩβ,��������ݱ��ĩβ�򷵻�
		return TRUE;
	
	while(!m_pRecordset->adoEOF)     //ѭ�����,����¼��ָ�벻�����ݱ�ĩβʱִ��ѭ�����еĲ���
	{
		for(int j=0;j<len;j++)       //ѭ�����,�����ݿ������ݱ��е�����ȡ��,����str[]������
		{
			str[j]=(char*)_bstr_t(m_pRecordset->GetCollect(_bstr_t(fields[j])));
		}

		view->SetRow(i,str,len);     //��CADOView* viewָ���Ĵ�����,��Ӧ��Ӧ����,�Ի�ͼ������ı��ķ�ʽ,��str[]�д洢�����ݱ��е����ݼ���
		
		m_pRecordset->MoveNext();    //��¼��ָ������,ȡ���ݱ�����һ�е�����

		i++;     //CADOView* viewָ�������е�������һ��,������µ�����
	}

	delete []str;   //�ͷ�new�������ӵ��ַ�������
	
	return TRUE;    //ִ����Ϻ󷵻���

}

bool ADOConn::DatabaseState(void)
{
	try
	{
		::CoInitialize(NULL);//��ʼ��COM����
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect="Dsn=Cellx64.0.01";
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		return false;
	}

	return true;
}

int ADOConn::DataNumber(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		e.Description();
	}
	return m_pRecordset->GetRecordCount();

}
