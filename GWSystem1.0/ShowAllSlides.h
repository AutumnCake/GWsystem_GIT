#pragma once
#include "GWSystem1.0Dlg.h"
class CShowAllSlides :
	public CGWSystem10Dlg
{
public:
	CShowAllSlides();
	~CShowAllSlides();

	vector<CRect> rectSlideFrame; //定义8个slide 区域
	CRect rectSlideFrame1;
	CRect rectSlideFrame2;
	CRect rectSlideFrame3;
	CRect rectSlideFrame4;
	CRect rectSlideFrame5;
	CRect rectSlideFrame6;
	CRect rectSlideFrame7;
	CRect rectSlideFrame8;

	unsigned int  Temp;//选中的玻片号
	int slidepage;
	BOOL nextpageflag;
	int Frame;

	CPen BluePen;
	CPen WhitePen;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	// 显示所有玻片的设置信息与状态
	void ShowAllSlidesState(WPARAM slide);
	// 上一页的八张玻片
	void Last8Slides();
	// 下一页的八张玻片
	void Next8Slides();
	//单击玻片显示该玻片的信息
	void ShowSlideInformation(UINT nFlags, CPoint point, SLIDE* slide);
	//去掉所有边框
	void ClearAllFrame();
	//显示当前扫描的玻片的进度
	void ShowTheScanningProgress(int currentnum, int allnum,int slidenum);
	//清空所有Edit
	void ClearAllEdit();
};

