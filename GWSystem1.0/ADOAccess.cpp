#include "StdAfx.h"
#include "ADOAccess.h"


ADOAccess::ADOAccess(void)
{
}

ADOAccess::~ADOAccess(void)
{
}

void ADOAccess::OnInitADOAccess(void)
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

void ADOAccess::ExitConnect(void)
{
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	::CoUninitialize();
}

BOOL ADOAccess::ExecuteAccess(_bstr_t bstrAccess)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOAccess();
		m_pConnection->Execute(bstrAccess,NULL,adCmdText);
	    return true;
	}
	catch(_com_error e)
	{
		e.Description();
		return false;
	}
}

_RecordsetPtr& ADOAccess::GetRecordSet(_bstr_t bstrAccess)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOAccess();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrAccess,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		e.Description();
	}
	return m_pRecordset;
}

BOOL ADOAccess::ShowADOViewAccess(CString fields[], int len, CADOViewAccess* view)     //��CADOView* viewָ���Ĵ������б����ʽ��ʾ�Ѿ��򿪵����ݿ��е����ݱ�
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

bool ADOAccess::DatabaseState(void)
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

int ADOAccess::DataNumber(_bstr_t bstrAccess)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOAccess();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrAccess,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		e.Description();
	}
	return m_pRecordset->GetRecordCount();

}
