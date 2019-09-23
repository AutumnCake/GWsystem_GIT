#include "stdafx.h"
#include "ShowAllSlides.h"
//#include"GWSystem1.0Dlg.h"
#include "GWSystem1.0.h"
#include"resource.h"


extern CGWSystem10Dlg* pDlg;
SLIDE *MySlide = new SLIDE;


CShowAllSlides::CShowAllSlides()
{
	slidepage = 0;
	nextpageflag = TRUE;

}


CShowAllSlides::~CShowAllSlides()
{
}


BOOL CShowAllSlides::OnInitDialog()
{
	//CGWSystem10Dlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置玻片显示框字体
	static CFont font;
	static CFont font1;
	font.DeleteObject();
	font1.DeleteObject();
	font1.CreatePointFont(150, _T("Consolas"));
	font.CreatePointFont(120, _T("Consolas"));
	pDlg->m_edit1.SetFont(&font);//设置字体
	pDlg->m_edit2.SetFont(&font);//设置字体
	pDlg->m_edit3.SetFont(&font);//设置字体
	pDlg->m_edit4.SetFont(&font);//设置字体
	pDlg->m_edit5.SetFont(&font);//设置字体
	pDlg->m_edit6.SetFont(&font);//设置字体
	pDlg->m_edit7.SetFont(&font);//设置字体
	pDlg->m_edit8.SetFont(&font);//设置字体

	pDlg->m_edit11.SetFont(&font);//设置字体
	pDlg->m_edit22.SetFont(&font);//设置字体
	pDlg->m_edit33.SetFont(&font);//设置字体
	pDlg->m_edit44.SetFont(&font);//设置字体
	pDlg->m_edit55.SetFont(&font);//设置字体
	pDlg->m_edit66.SetFont(&font);//设置字体
	pDlg->m_edit77.SetFont(&font);//设置字体
	pDlg->m_edit88.SetFont(&font);//设置字体

	pDlg->m_frame1.SetFont(&font1);//设置字体
	pDlg->m_frame2.SetFont(&font1);//设置字体
	pDlg->m_frame3.SetFont(&font1);//设置字体
	pDlg->m_frame4.SetFont(&font1);//设置字体
	pDlg->m_frame5.SetFont(&font1);//设置字体
	pDlg->m_frame6.SetFont(&font1);//设置字体
	pDlg->m_frame7.SetFont(&font1);//设置字体
	pDlg->m_frame8.SetFont(&font1);//设置字体


	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS1);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS2);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS3);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS4);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS5);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS6);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS7);
	pDlg->IDC_ProgressEdit.push_back(IDC_EDIT_PROGRESS8);

	pDlg->IDC_SlideEdit.push_back(IDC_EDIT1);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT2);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT3);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT4);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT5);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT6);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT7);
	pDlg->IDC_SlideEdit.push_back(IDC_EDIT8);

	pDlg->IDC_SlideFrame.push_back(IDC_FRAME1);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME2);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME3);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME4);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME5);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME6);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME7);
	pDlg->IDC_SlideFrame.push_back(IDC_FRAME8);

	pDlg->GetDlgItem(IDC_FRAME1)->GetWindowRect(&rectSlideFrame1); pDlg->ScreenToClient(&rectSlideFrame1);  	rectSlideFrame.push_back(rectSlideFrame1);
	pDlg->GetDlgItem(IDC_FRAME2)->GetWindowRect(&rectSlideFrame2); pDlg->ScreenToClient(&rectSlideFrame2);  	rectSlideFrame.push_back(rectSlideFrame2);
	pDlg->GetDlgItem(IDC_FRAME3)->GetWindowRect(&rectSlideFrame3); pDlg->ScreenToClient(&rectSlideFrame3);  	rectSlideFrame.push_back(rectSlideFrame3);
	pDlg->GetDlgItem(IDC_FRAME4)->GetWindowRect(&rectSlideFrame4); pDlg->ScreenToClient(&rectSlideFrame4);  	rectSlideFrame.push_back(rectSlideFrame4);
	pDlg->GetDlgItem(IDC_FRAME5)->GetWindowRect(&rectSlideFrame5); pDlg->ScreenToClient(&rectSlideFrame5);  	rectSlideFrame.push_back(rectSlideFrame5);
	pDlg->GetDlgItem(IDC_FRAME6)->GetWindowRect(&rectSlideFrame6); pDlg->ScreenToClient(&rectSlideFrame6);  	rectSlideFrame.push_back(rectSlideFrame6);
	pDlg->GetDlgItem(IDC_FRAME7)->GetWindowRect(&rectSlideFrame7); pDlg->ScreenToClient(&rectSlideFrame7);  	rectSlideFrame.push_back(rectSlideFrame7);
	pDlg->GetDlgItem(IDC_FRAME8)->GetWindowRect(&rectSlideFrame8); pDlg->ScreenToClient(&rectSlideFrame8);  	rectSlideFrame.push_back(rectSlideFrame8);

	CString ss, strss;
	for (size_t i = 0; i < 8; i++)
	{
		if (slidepage  < 0)
			break;
		ss.Format(_T("%d"), i + slidepage * 8);
		strss = _T("NO.") + ss;
		pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->SetWindowText(strss);
		pDlg->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);
		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[i])->SetWindowText(NULL);

	}

	//BluePen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	WhitePen.CreatePen(PS_SOLID, 3, RGB(192, 192, 192));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CShowAllSlides::DoDataExchange(CDataExchange* pDX)
{
	// TODO:  在此添加专用代码和/或调用基类

	CGWSystem10Dlg::DoDataExchange(pDX);
}


// 显示所有玻片的设置信息与状态
void CShowAllSlides::ShowAllSlidesState(WPARAM slide)
{
	ClearAllEdit();
	MySlide = (SLIDE*)slide;
	CString mystr;
	int i = 0;
	for (vector<int>::iterator iter1 = MySlide->All_Slide_ID.begin(); iter1 != MySlide->All_Slide_ID.end(); iter1++)
	{
		if (0 <= *iter1&&*iter1 <= 7)
		{
			if (MySlide->slidebuild[i].mode == "微核")
				mystr = "CB扫描";
			else
				mystr = "CA扫描";
			mystr = mystr + _T("\r\n名称：") + MySlide->slidebuild[i].name + _T("\r\n") + _T("范围：")
				+ MySlide->slidebuild[i].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("扫描进度：\r\n0/0");
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[*iter1])->SetWindowTextW(mystr);

			//SetDlgItemText(pDlg->IDC_SlideEdit[*iter1], mystr);
			i++;
		}

	}

	//int slidenumber;

	//GetDlgItem(IDC_BUTTON_LIST)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_IKAROS)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_IKAROS_RST)->EnableWindow(TRUE);

	//Complete_Scan.s_User_Para.m_Slide_ID = MySlide->All_Slide_ID;
}




// 上一页的八张玻片
void CShowAllSlides::Last8Slides()
{
	slidepage--;
	nextpageflag = TRUE;
	ClearAllFrame();
	//改变8个框的序号	，清除所有edit的内容
	CString ss,strss;
	for (size_t i = 0; i < 8; i++)
	{
		if (slidepage  < 0)
			break;
		ss.Format(_T("%d"), i + slidepage * 8);
		strss = _T("NO.")+ss;
		pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->SetWindowText(strss);
		pDlg->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);
		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[i])->SetWindowText(NULL);

	}
	CString mystr;
	int ii = 0;
	for (vector<int>::iterator iter1 = MySlide->All_Slide_ID.begin(); iter1 != MySlide->All_Slide_ID.end(); iter1++)
	{
		if (0 + 8 * slidepage <= *iter1&&*iter1 <= 7 + 8 * slidepage)
		{
			if (MySlide->slidebuild[ii].mode == "微核")
				mystr = "CB扫描";
			else
				mystr = "CA扫描";
			mystr = mystr + _T("\r\n名称：") + MySlide->slidebuild[ii].name + _T("\r\n") + _T("范围：")
				+ MySlide->slidebuild[ii].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("扫描进度：\r\n0/0");
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[*iter1])->SetWindowTextW(mystr);
		
		}
		ii++;

	}

}


// 下一页的八张玻片
void CShowAllSlides::Next8Slides()
{
	ClearAllFrame();
	if (nextpageflag)
	{
		for (size_t i = 0; i < 8; i++)
		{
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);
			pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->SetWindowText(NULL);
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[i])->SetWindowText(NULL);

		}
		slidepage++;
	}

	//改变8个框的序号
	CString ss,strss;
	for (size_t i = 0; i < 8; i++)
	{
		if (i + slidepage * 8 + 1 > ITEMLENGTH)
		{
			nextpageflag = FALSE;
			break;
		}
		ss.Format(_T("%d"), i + slidepage * 8);
		strss = _T("NO.") + ss;
		pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->SetWindowText(strss);
	}



	CString mystr;
	int ii = 0;
	for (vector<int>::iterator iter1 = MySlide->All_Slide_ID.begin(); iter1 != MySlide->All_Slide_ID.end(); iter1++)
	{
		if (0 + 8 * slidepage <= *iter1&&*iter1 <= 7 + 8 * slidepage)
		{
			if (MySlide->slidebuild[ii].mode == "微核")
				mystr = "CB扫描";
			else
				mystr = "CA扫描";
			mystr = mystr + _T("\r\n名称：") + MySlide->slidebuild[ii].name + _T("\r\n") + _T("范围：")
				+ MySlide->slidebuild[ii].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("扫描进度：\r\n0/0");
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[*iter1])->SetWindowTextW(mystr);
		}
		ii++;

	}

}

//单击玻片显示该玻片的信息
void CShowAllSlides::ShowSlideInformation(UINT nFlags, CPoint point, SLIDE* slide)
{
	ClearAllFrame();
	for (unsigned int i = 0; i < 8; i++)
	{
		if (rectSlideFrame[i].PtInRect(point))
		{
			Frame = i;
			Temp = slidepage * 8 + Frame;
			break;
		}
	}


		CRect rtTop1;
		CDC *dc = new CDC;
		switch (Frame)
		{
		//case 0:
		//	pDlg->m_frame1.GetClientRect(&rtTop1);
		//	FrameRect(pDlg->m_frame1.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 0, 0)));//填充该区域颜色
		//	break;
		case 0:
			dc = pDlg->m_frame1.GetDC();
			pDlg->m_frame1.GetClientRect(&rtTop1);
			break;
		case 1:
			dc = pDlg->m_frame2.GetDC();
			pDlg->m_frame2.GetClientRect(&rtTop1);
			break;
		case 2:
			dc = pDlg->m_frame3.GetDC();
			pDlg->m_frame3.GetClientRect(&rtTop1);
			break;
		case 3:
			dc = pDlg->m_frame4.GetDC();
			pDlg->m_frame4.GetClientRect(&rtTop1);
			break;
		case 4:
			dc = pDlg->m_frame5.GetDC();
			pDlg->m_frame5.GetClientRect(&rtTop1);
			break;
		case 5:
			dc = pDlg->m_frame6.GetDC();
			pDlg->m_frame6.GetClientRect(&rtTop1);
			break;
		case 6:
			dc = pDlg->m_frame7.GetDC();
			pDlg->m_frame7.GetClientRect(&rtTop1);
			break;
		case 7:
			dc = pDlg->m_frame8.GetDC();
			pDlg->m_frame8.GetClientRect(&rtTop1);
			break;

		default:
			MessageBox(TEXT("出错了..."));
			break;
		}
		dc->SelectObject(&BluePen);//在dc上放置画笔
		dc->SelectStockObject(NULL_BRUSH);//使画出的矩形区域为透明
		dc->Rectangle(rtTop1);


}

//去掉所有边框
void CShowAllSlides::ClearAllFrame()
{
	CRect rtTop1;
	CDC *dc = new CDC;
	for (size_t i = 0; i < pDlg->IDC_SlideFrame.size(); i++)
	{
		dc = pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->GetDC();
		pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->GetClientRect(&rtTop1);
		dc->SelectObject(&WhitePen);//在dc上放置画笔
		dc->SelectStockObject(NULL_BRUSH);//使画出的矩形区域为透明
		dc->Rectangle(rtTop1);
	}


}

//显示当前扫描的玻片的进度
void CShowAllSlides::ShowTheScanningProgress(int currentnum, int allnum, int slidenum)
{
	if (slidenum >= 8 * slidepage && slidenum <= 8 * slidepage + 7)
	{
		CString mystr, str1, str2;
		str1.Format(_T("%d"), currentnum);
		str2.Format(_T("%d"), allnum);
		mystr = _T("扫描进度：\r\n") + str1 + "/" + str2;

		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[slidenum % 8])->SetWindowTextW(mystr);

	}
}

//清空所有Edit
void CShowAllSlides::ClearAllEdit()
{
	for (size_t i = 0; i < 8; i++)
	{
		pDlg->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);
		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[i])->SetWindowText(NULL);
	}
}
