#pragma once

//所有需要显示数据列表的类的抽象父类

class CADOViewAccess
{
public:
	CADOViewAccess(void);

	~CADOViewAccess(void);

public:
	virtual BOOL SetRow(int row,CString * strArray,int count)=0;

	virtual BOOL DeleteAllRows()=0;
};
