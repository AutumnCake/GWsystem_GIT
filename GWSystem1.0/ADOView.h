#pragma once

//所有需要显示数据列表的类的抽象父类

class CADOView
{
public:
	CADOView(void);

	~CADOView(void);

public:
	virtual BOOL SetRow(int row,CString * strArray,int count)=0;

	virtual BOOL DeleteAllRows()=0;
};
