#pragma once

//������Ҫ��ʾ�����б����ĳ�����

class CADOView
{
public:
	CADOView(void);

	~CADOView(void);

public:
	virtual BOOL SetRow(int row,CString * strArray,int count)=0;

	virtual BOOL DeleteAllRows()=0;
};
