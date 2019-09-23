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
		::CoInitialize(NULL);//初始化COM环境
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

BOOL ADOAccess::ShowADOViewAccess(CString fields[], int len, CADOViewAccess* view)     //在CADOView* view指定的窗口以列表的形式显示已经打开的数据库中的数据表
{
	int i=0;    //定义初始行的位置,第0行

	CString * str = new CString[len];  //声明一个具有len个元素的字符串数组,以存放相应字段及行对应的数据
	
	view->DeleteAllRows();    //删除所有行,以便重新构建需要的行

	if(m_pRecordset->adoEOF)       //判断记录集指针是否在数据表的末尾,如果在数据表的末尾则返回
		return TRUE;
	
	while(!m_pRecordset->adoEOF)     //循环语句,当记录集指针不在数据表末尾时执行循环体中的操作
	{
		for(int j=0;j<len;j++)       //循环语句,将数据库中数据表中的数据取出,放入str[]数组中
		{
			str[j]=(char*)_bstr_t(m_pRecordset->GetCollect(_bstr_t(fields[j])));
		}

		view->SetRow(i,str,len);     //在CADOView* view指定的窗口中,对应相应的行,以绘图或添加文本的方式,将str[]中存储的数据表中的数据加入
		
		m_pRecordset->MoveNext();    //记录集指针下移,取数据表中下一行的数据

		i++;     //CADOView* view指定窗口中的行增加一行,以添加新的数据
	}

	delete []str;   //释放new运算符添加的字符串数组
	
	return TRUE;    //执行完毕后返回真

}

bool ADOAccess::DatabaseState(void)
{
	try
	{
		::CoInitialize(NULL);//初始化COM环境
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
