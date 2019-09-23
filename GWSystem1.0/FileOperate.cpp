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


// ɾ���ļ����µ��ļ�
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

	//�����Ѿ����ļ���������ļ�ȫ��ɾ���ˣ������Ҫ���ļ���Ҳɾ���������һ�´���  
	  SetFileAttributes(sPath, FILE_ATTRIBUTE_NORMAL);  //�����ļ��е�����  
	  RemoveDirectory(sPath);  //ɾ���ļ���  
}

//��ȡ·����ǰN��
CString CFileOperate::GetForwardPath_N(CString path, int forwardN)
{
	if (0 == forwardN)
		return path;
	string strPath = CT2A(path.GetBuffer());
	int tt[128];//��¼��N��·����λ��
	int bb = 0; //��¼�ܵ�·������
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


