#pragma once
#include <string>

using std::string;

class CFileOperate
{
public:
	CFileOperate();
	~CFileOperate();

	void DeleteFolder(CString sPath);
	CString GetForwardPath_N(CString path, int forwardN);


};

