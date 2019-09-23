
// GWSystem1.0.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "GWSystem1.0Dlg.h"
#include "ADOConn.h"
#include"ADOAccess.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGWSystem10App

BEGIN_MESSAGE_MAP(CGWSystem10App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGWSystem10App ����

CGWSystem10App::CGWSystem10App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGWSystem10App ����

CGWSystem10App theApp;


// CGWSystem10App ��ʼ��

BOOL CGWSystem10App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));



	//ȥ����MFC��������
	//HMODULE hExe = GetModuleHandleA(NULL);
	//char nameBuf[MAX_PATH] = { 0 };
	//GetModuleFileNameA(hExe, nameBuf, MAX_PATH);
	//std::string sName(nameBuf);
	//sName = sName.substr(0, sName.rfind('\\'));
	//SetCurrentDirectory(sName.c_str());

#ifdef RELEASE_DEBUG
	//�������ƿ���̨
	//AllocConsole();//ע���鷵��ֵ
	//freopen("CONOUT$", "w", stdout);
	//��¼
#endif

p:
	dlg_login.DoModal();   //������½�Ի���

		if (dlg_login.m_SuccessLogin)
		{
			//CGWSystem10Dlg *dlg = new CGWSystem10Dlg;
			//dlg->Create(IDD_GWSYSTEM10_DIALOG);
			CGWSystem10Dlg dlg;
			m_pMainWnd = &dlg;
			INT_PTR nResponse = dlg.DoModal();    //�������Ի���
			
			/*m_pMainWnd->ShowWindow(SW_HIDE);*/

			if (nResponse == IDOK)
			{
				// TODO:  �ڴ˷��ô����ʱ��
				//  ��ȷ�������رնԻ���Ĵ���
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO:  �ڴ˷��ô����ʱ��
				//  ��ȡ�������رնԻ���Ĵ���
			}
			else if (nResponse == -1)
			{
				TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
				TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
			}

		}
		else
		{
			if (!dlg_login.m_bCancle)
			{
				goto p;
			}
			delete(m_pMainWnd);
			m_pMainWnd = NULL;
			return FALSE;
		}


		// ɾ�����洴���� shell ��������
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}

		FreeConsole();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//��ʽת��
string CGWSystem10App::WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

//�������ļ����л�ȡ�ļ�����������չ����
CString CGWSystem10App::GetFileTitleFromFileName(CString FileName, BOOL Ext)
{
	int Where;
	Where = FileName.ReverseFind('\\');
	if (Where == -1)
		Where = FileName.ReverseFind('/');
	CString FileTitle = FileName.Right(FileName.GetLength() - 1 - Where);
	if (!Ext)
	{
		int Which = FileTitle.ReverseFind('.');
		if (Which != -1)
			FileTitle = FileTitle.Left(Which);
	}
	return FileTitle;
}

//CString תString
string CGWSystem10App::CString2string(CString csStrData)
{
	string strRet(CW2A(csStrData.GetString()));
	return strRet;
}

//��ȡ·����ǰN��
CString CGWSystem10App::GetForwardPath_N(CString path, int forwardN)
{
	if (0 == forwardN)
		return path;
	string strPath = CT2A(path.GetBuffer());
	int tt[128];//��¼��N��·����λ��
	int bb = 0; //��¼�ܵ�·������
	string iTempStr;
	USES_CONVERSION;
	const char *iStr = T2A(path);
	for (size_t i = 0,j=0; i < strPath.length(); i++)
	{
		if ( iStr[i] == '\\')
		{
			tt[bb++]=i;
			//bb++;
		}
	}

		iTempStr = strPath.substr(0,tt[bb-forwardN]);

	CString mypath(iTempStr.c_str());
	return  mypath;
}

