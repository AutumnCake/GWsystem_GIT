
// GWSystem1.0.cpp : 定义应用程序的类行为。
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


// CGWSystem10App 构造

CGWSystem10App::CGWSystem10App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGWSystem10App 对象

CGWSystem10App theApp;


// CGWSystem10App 初始化

BOOL CGWSystem10App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));



	//去掉对MFC的依赖：
	//HMODULE hExe = GetModuleHandleA(NULL);
	//char nameBuf[MAX_PATH] = { 0 };
	//GetModuleFileNameA(hExe, nameBuf, MAX_PATH);
	//std::string sName(nameBuf);
	//sName = sName.substr(0, sName.rfind('\\'));
	//SetCurrentDirectory(sName.c_str());

#ifdef RELEASE_DEBUG
	//开启控制控制台
	//AllocConsole();//注意检查返回值
	//freopen("CONOUT$", "w", stdout);
	//登录
#endif

p:
	dlg_login.DoModal();   //弹出登陆对话框

		if (dlg_login.m_SuccessLogin)
		{
			//CGWSystem10Dlg *dlg = new CGWSystem10Dlg;
			//dlg->Create(IDD_GWSYSTEM10_DIALOG);
			CGWSystem10Dlg dlg;
			m_pMainWnd = &dlg;
			INT_PTR nResponse = dlg.DoModal();    //弹出主对话框
			
			/*m_pMainWnd->ShowWindow(SW_HIDE);*/

			if (nResponse == IDOK)
			{
				// TODO:  在此放置处理何时用
				//  “确定”来关闭对话框的代码
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO:  在此放置处理何时用
				//  “取消”来关闭对话框的代码
			}
			else if (nResponse == -1)
			{
				TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
				TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
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


		// 删除上面创建的 shell 管理器。
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}

		FreeConsole();

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//格式转换
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

//从完整文件夹中获取文件名（不含扩展名）
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

//CString 转String
string CGWSystem10App::CString2string(CString csStrData)
{
	string strRet(CW2A(csStrData.GetString()));
	return strRet;
}

//获取路径的前N级
CString CGWSystem10App::GetForwardPath_N(CString path, int forwardN)
{
	if (0 == forwardN)
		return path;
	string strPath = CT2A(path.GetBuffer());
	int tt[128];//记录第N级路径的位置
	int bb = 0; //记录总的路径级数
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

