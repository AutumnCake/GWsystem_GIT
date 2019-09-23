#include "stdafx.h"
#include "FileOperate.h"
#include "io.h"
#include<direct.h>


CFileOperate::CFileOperate()
{
}


CFileOperate::~CFileOperate()
{
}


// 删除文件夹下的文件
void CFileOperate::DeleteFolder(CString sPath)
{
	CFileFind ff;
	BOOL bFound;
	bFound = ff.FindFile(sPath + "\\*.*");
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		if (ff.IsDirectory())
		{
			if (!ff.IsDots())
			{
				DeleteFolder(sFilePath);
			}
		}
		else
		{
			if (ff.IsReadOnly())
			{
				SetFileAttributes(sFilePath, FILE_ATTRIBUTE_NORMAL);
			}
			DeleteFile(sFilePath);
		}
	}
	ff.Close();

	//上面已经把文件夹下面的文件全部删除了，如果需要把文件夹也删除掉则加上一下代码  
	  SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //设置文件夹的属性  
	  RemoveDirectory(sPath);  //删除文件夹  
}

//获取路径的前N级
CString CFileOperate::GetForwardPath_N(CString path, int forwardN)
{
	if (0 == forwardN)
		return path;
	string strPath = CT2A(path.GetBuffer());
	int tt[128];//记录第N级路径的位置
	int bb = 0; //记录总的路径级数
	string iTempStr;
	USES_CONVERSION;
	const char *iStr = T2A(path);
	for (size_t i = 0, j = 0; i < strPath.length(); i++)
	{
		if (iStr[i] == '\\')
		{
			tt[bb++] = i;
			//bb++;
		}
	}

	iTempStr = strPath.substr(0, tt[bb - forwardN]);

	CString mypath(iTempStr.c_str());
	return  mypath;
}


