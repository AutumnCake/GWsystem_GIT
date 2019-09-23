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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ò�Ƭ��ʾ������
	static CFont font;
	static CFont font1;
	font.DeleteObject();
	font1.DeleteObject();
	font1.CreatePointFont(150, _T("Consolas"));
	font.CreatePointFont(120, _T("Consolas"));
	pDlg->m_edit1.SetFont(&font);//��������
	pDlg->m_edit2.SetFont(&font);//��������
	pDlg->m_edit3.SetFont(&font);//��������
	pDlg->m_edit4.SetFont(&font);//��������
	pDlg->m_edit5.SetFont(&font);//��������
	pDlg->m_edit6.SetFont(&font);//��������
	pDlg->m_edit7.SetFont(&font);//��������
	pDlg->m_edit8.SetFont(&font);//��������

	pDlg->m_edit11.SetFont(&font);//��������
	pDlg->m_edit22.SetFont(&font);//��������
	pDlg->m_edit33.SetFont(&font);//��������
	pDlg->m_edit44.SetFont(&font);//��������
	pDlg->m_edit55.SetFont(&font);//��������
	pDlg->m_edit66.SetFont(&font);//��������
	pDlg->m_edit77.SetFont(&font);//��������
	pDlg->m_edit88.SetFont(&font);//��������

	pDlg->m_frame1.SetFont(&font1);//��������
	pDlg->m_frame2.SetFont(&font1);//��������
	pDlg->m_frame3.SetFont(&font1);//��������
	pDlg->m_frame4.SetFont(&font1);//��������
	pDlg->m_frame5.SetFont(&font1);//��������
	pDlg->m_frame6.SetFont(&font1);//��������
	pDlg->m_frame7.SetFont(&font1);//��������
	pDlg->m_frame8.SetFont(&font1);//��������


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
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CShowAllSlides::DoDataExchange(CDataExchange* pDX)
{
	// TODO:  �ڴ����ר�ô����/����û���

	CGWSystem10Dlg::DoDataExchange(pDX);
}


// ��ʾ���в�Ƭ��������Ϣ��״̬
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
			if (MySlide->slidebuild[i].mode == "΢��")
				mystr = "CBɨ��";
			else
				mystr = "CAɨ��";
			mystr = mystr + _T("\r\n���ƣ�") + MySlide->slidebuild[i].name + _T("\r\n") + _T("��Χ��")
				+ MySlide->slidebuild[i].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("ɨ����ȣ�\r\n0/0");
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




// ��һҳ�İ��Ų�Ƭ
void CShowAllSlides::Last8Slides()
{
	slidepage--;
	nextpageflag = TRUE;
	ClearAllFrame();
	//�ı�8��������	���������edit������
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
			if (MySlide->slidebuild[ii].mode == "΢��")
				mystr = "CBɨ��";
			else
				mystr = "CAɨ��";
			mystr = mystr + _T("\r\n���ƣ�") + MySlide->slidebuild[ii].name + _T("\r\n") + _T("��Χ��")
				+ MySlide->slidebuild[ii].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("ɨ����ȣ�\r\n0/0");
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[*iter1])->SetWindowTextW(mystr);
		
		}
		ii++;

	}

}


// ��һҳ�İ��Ų�Ƭ
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

	//�ı�8��������
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
			if (MySlide->slidebuild[ii].mode == "΢��")
				mystr = "CBɨ��";
			else
				mystr = "CAɨ��";
			mystr = mystr + _T("\r\n���ƣ�") + MySlide->slidebuild[ii].name + _T("\r\n") + _T("��Χ��")
				+ MySlide->slidebuild[ii].range + _T("%");
			//mystr = MySlide->slidebuild[i].name;
			pDlg->GetDlgItem(pDlg->IDC_SlideEdit[*iter1])->SetWindowTextW(mystr);
			mystr = _T("ɨ����ȣ�\r\n0/0");
			pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[*iter1])->SetWindowTextW(mystr);
		}
		ii++;

	}

}

//������Ƭ��ʾ�ò�Ƭ����Ϣ
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
		//	FrameRect(pDlg->m_frame1.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 0, 0)));//����������ɫ
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
			MessageBox(TEXT("������..."));
			break;
		}
		dc->SelectObject(&BluePen);//��dc�Ϸ��û���
		dc->SelectStockObject(NULL_BRUSH);//ʹ�����ľ�������Ϊ͸��
		dc->Rectangle(rtTop1);


}

//ȥ�����б߿�
void CShowAllSlides::ClearAllFrame()
{
	CRect rtTop1;
	CDC *dc = new CDC;
	for (size_t i = 0; i < pDlg->IDC_SlideFrame.size(); i++)
	{
		dc = pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->GetDC();
		pDlg->GetDlgItem(pDlg->IDC_SlideFrame[i])->GetClientRect(&rtTop1);
		dc->SelectObject(&WhitePen);//��dc�Ϸ��û���
		dc->SelectStockObject(NULL_BRUSH);//ʹ�����ľ�������Ϊ͸��
		dc->Rectangle(rtTop1);
	}


}

//��ʾ��ǰɨ��Ĳ�Ƭ�Ľ���
void CShowAllSlides::ShowTheScanningProgress(int currentnum, int allnum, int slidenum)
{
	if (slidenum >= 8 * slidepage && slidenum <= 8 * slidepage + 7)
	{
		CString mystr, str1, str2;
		str1.Format(_T("%d"), currentnum);
		str2.Format(_T("%d"), allnum);
		mystr = _T("ɨ����ȣ�\r\n") + str1 + "/" + str2;

		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[slidenum % 8])->SetWindowTextW(mystr);

	}
}

//�������Edit
void CShowAllSlides::ClearAllEdit()
{
	for (size_t i = 0; i < 8; i++)
	{
		pDlg->GetDlgItem(pDlg->IDC_SlideEdit[i])->SetWindowText(NULL);
		pDlg->GetDlgItem(pDlg->IDC_ProgressEdit[i])->SetWindowText(NULL);
	}
}
