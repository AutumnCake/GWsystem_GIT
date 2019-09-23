#pragma once
#include "GWSystem1.0Dlg.h"
class CShowAllSlides :
	public CGWSystem10Dlg
{
public:
	CShowAllSlides();
	~CShowAllSlides();

	vector<CRect> rectSlideFrame; //����8��slide ����
	CRect rectSlideFrame1;
	CRect rectSlideFrame2;
	CRect rectSlideFrame3;
	CRect rectSlideFrame4;
	CRect rectSlideFrame5;
	CRect rectSlideFrame6;
	CRect rectSlideFrame7;
	CRect rectSlideFrame8;

	unsigned int  Temp;//ѡ�еĲ�Ƭ��
	int slidepage;
	BOOL nextpageflag;
	int Frame;

	CPen BluePen;
	CPen WhitePen;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	// ��ʾ���в�Ƭ��������Ϣ��״̬
	void ShowAllSlidesState(WPARAM slide);
	// ��һҳ�İ��Ų�Ƭ
	void Last8Slides();
	// ��һҳ�İ��Ų�Ƭ
	void Next8Slides();
	//������Ƭ��ʾ�ò�Ƭ����Ϣ
	void ShowSlideInformation(UINT nFlags, CPoint point, SLIDE* slide);
	//ȥ�����б߿�
	void ClearAllFrame();
	//��ʾ��ǰɨ��Ĳ�Ƭ�Ľ���
	void ShowTheScanningProgress(int currentnum, int allnum,int slidenum);
	//�������Edit
	void ClearAllEdit();
};

