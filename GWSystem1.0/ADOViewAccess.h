#pragma once

//������Ҫ��ʾ�����б����ĳ�����

class CADOViewAccess
{
public:
	CADOViewAccess(void);

	~CADOViewAccess(void);

public:
	virtual BOOL SetRow(int row,CString * strArray,int count)=0;

	virtual BOOL DeleteAllRows()=0;
};
