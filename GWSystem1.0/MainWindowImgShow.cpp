#include "stdafx.h"
#include "MainWindowImgShow.h"
#include "ScanAndSaveImg.h"

using namespace::cv;

extern CGWSystem10Dlg* pDlg;

CMainWindowImgShow::CMainWindowImgShow()
{
	Page = 0;
}

CMainWindowImgShow::~CMainWindowImgShow()
{
}

BEGIN_MESSAGE_MAP(CMainWindowImgShow, CScanAndSaveImg)
//	ON_WM_TIMER()
END_MESSAGE_MAP()

// 查找到所有图片的路径
void CMainWindowImgShow::FindAllImgFiles()
{
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();

	char* fileName = nullptr;
	pDlg->GetClientRect(rcClient);

	int find_pic = 0;//的图片的序列号
	vector <string>().swap(imgNames);

	//若是打开旧文件，则文件夹由。ini文件指定
	//if (openHistory)
	//{
	//	USES_CONVERSION;
	//	fileName = T2A(myhistoryFile);
	//}
	//fileName = myhistoryFile;
	//else
	//{
	char cpy[100];
	strcpy(cpy, pDlg->AllFiles.picsolve.c_str());
	fileName = cpy;    //此处应为本次抓图所存的文件夹
	//}
	sprintf_s(tempFilePath, "%s\*.bmp", fileName);

	// 多字节转换
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// 查找该文件待操作文件的相关属性读取到WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);

	if (handle != INVALID_HANDLE_VALUE)
	{
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf(tempFileName, "%s", fileName);
			imgNames.push_back(app->WChar2Ansi(file.cFileName));
			imgNames[find_pic].insert(0, tempFileName);
			find_pic++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	pDlg->SetTimer(1, 50, NULL);

}

//显示图片过程函数
void CMainWindowImgShow::ShowImage2(IplImage *img, UINT ID)//显示图片过程函数
{

	CDC *pDC = pDlg->GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	pDlg->GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	pDlg->ReleaseDC(pDC);
}

//清空图片
void CMainWindowImgShow::PicClear(UINT ID)//清空图片
{

	CStatic* pStatic = (CStatic*)pDlg->GetDlgItem(ID);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));

}

//初始将picture控件放入数组,画滚动条
void CMainWindowImgShow::PutPicControlIntoVector(){
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME1);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME2);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME3);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME4);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME5);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME6);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME7);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME8);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME9);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME10);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME11);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME12);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME13);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME14);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME15);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME16);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME17);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME18);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME19);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME20);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME21);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME22);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME23);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME24);
	pDlg->IDC_Name.push_back(IDC_FIGUREFRAME25);

	//画出滚动条
	CRect rectWind;
	pDlg->GetDlgItem(IDC_FIGUREFRAME5)->GetWindowRect(&rectWind);//获取控件的屏幕坐标
	pDlg->ScreenToClient(&rectWind);
	int iHeight = rectWind.Height();
	int iWidth = rectWind.Width();

	CRect rect(rectWind.right, rectWind.top, rectWind.right + 15, rectWind.top + 5 * iHeight);//滚动条显示在对话框右边，宽为15 
	m_scroll.Create(WS_CHILD | WS_VISIBLE | SB_VERT,//子控件 可见 垂直滚动条
		rect, pDlg, WM_USER + 1000);
	m_scroll.SetScrollRange(0, 0);//设置滚动条的最大值与最小值
	m_scroll.GetScrollInfo(&scrollinfo);
	scrollinfo.nPage = 1;//让滚动条的滑块大小根据Range自动改变
	m_scroll.SetScrollInfo(&scrollinfo, FALSE);
	m_scroll.Invalidate();

	//获取全部画框所在区域
	pDlg->m_allframe.GetWindowRect(AllPictureFrame);
}

// 上一页
void CMainWindowImgShow::MainWindowLastpage()
{
	for (int v = 0; v < 25; v++)
	{
		PicClear(pDlg->IDC_Name[v]);
	}
	Page--;

	if (Page < 0)
		Page = 0;

	FindAllImgFiles();
}

// 下一页
void CMainWindowImgShow::MainWindowNextpage()
{
	Page++;
	for (int v = 0; v < 25; v++)
	{
		PicClear(pDlg->IDC_Name[v]);
	}

	if (Page > scrollrangepage)
		Page = scrollrangepage;
	FindAllImgFiles();
}

// 主窗口鼠标移动响应
void CMainWindowImgShow::MyMouseMove(UINT nFlags, CPoint point)
{
	MouseLoc = point;
}

//主窗口鼠标滚动响应
void CMainWindowImgShow::MyMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (AllPictureFrame.PtInRect(MouseLoc))
	{
		if (zDelta == 120)
		{
			OnVScroll(SB_PAGEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));

		}
		else if (zDelta == -120)
		{
			OnVScroll(SB_PAGEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
		}

		pDlg->UpdateData(FALSE);
	}

}

//主窗口垂直滚动条响应
void CMainWindowImgShow::MyVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
	scrollrangepage = ceil(imgNames.size() / 25);
	scrollinfo.nMax = scrollrangepage;
	scrollinfo.nMin = 0;
	switch (nSBCode)
	{
	case SB_BOTTOM://滚动
		m_scroll.ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMax) * 10);
		scrollinfo.nPos = scrollinfo.nMax;
		m_scroll.SetScrollInfo(&scrollinfo, SIF_ALL);
		break;
	case SB_TOP:
		m_scroll.ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMin) * 10);
		scrollinfo.nPos = scrollinfo.nMin;
		m_scroll.SetScrollInfo(&scrollinfo, SIF_ALL);
		break;
	case SB_LINEUP://滚动一行
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		m_scroll.SetScrollInfo(&scrollinfo, SIF_ALL);
		MainWindowLastpage();
		break;
	case SB_LINEDOWN:
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos>scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		m_scroll.SetScrollInfo(&scrollinfo);
		MainWindowNextpage();
		break;
	case SB_PAGEUP://滚动一页
		scrollinfo.nPos -= 3;
		if (scrollinfo.nPos<scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			Page = 1;
			MainWindowLastpage();
			m_scroll.SetScrollInfo(&scrollinfo);
			break;
		}
		m_scroll.SetScrollInfo(&scrollinfo);
		Page = Page - 2;
		MainWindowLastpage();

		break;
	case SB_PAGEDOWN:
		scrollinfo.nPos += 3; //当前页码
		if (scrollinfo.nPos>(scrollinfo.nMax - scrollinfo.nPage))  //注意： 如果此语句中，只是scrollinfo.nMax ，而不是(scrollinfo.nMax - scrollinfo.nPage)，鼠标滑轮滚动时会超出设置的滚动范围
		{
			scrollinfo.nPos = scrollinfo.nMax;
			Page = scrollinfo.nMax - 1;
			MainWindowNextpage();
			m_scroll.SetScrollInfo(&scrollinfo);
			break;
		}
		Page = Page + 2;
		m_scroll.SetScrollInfo(&scrollinfo);
		//m_scroll.ScrollWindow(0, -405/scrollrangepage);//注意此语句中，的*10 要注掉，否则鼠标滑轮滚动时会超出设置的滚动范围
		MainWindowNextpage();
		break;
	case SB_ENDSCROLL://结束滚动
		// MessageBox("SB_ENDSCROLL");  
		break;
	case SB_THUMBPOSITION://拖动到指定位置的滚动框。 提供当前位置
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		//scrollinfo.nPos = nPos;  
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;
	case SB_THUMBTRACK:
		m_scroll.ScrollWindow(0, (scrollinfo.nPos - nPos)/*10*/); //注意此语句中，的*10 要注掉，否则拖动滚动条时会超出设置的滚动范围
		scrollinfo.nPos = nPos;
		Page = scrollinfo.nPos + 1;
		MainWindowLastpage();
		m_scroll.SetScrollInfo(&scrollinfo, SIF_ALL);
		break;
	}

}

// 刷新滚动条的大小
void CMainWindowImgShow::FreshScroll()
{
	scrollrangepage = ceil(imgNames.size() / 25);
	m_scroll.SetScrollRange(0, scrollrangepage);//设置滚动条的最大值与最小值
	FindAllImgFiles();
}

// 定时器显示照片
void CMainWindowImgShow::TimerShowPicture()
{
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();

	if (pDlg->BStop)
	{
		int j = 0;
		if (25 * Page < imgNames.size())
		{

			for (int i = imgNames.size() - 1 - 25 * Page; j < 25 && i >= 0; i--)
			{
				//判断照片是否已经标记为合格或是不合格，若是，则改变边框
				CString WholePath, MyFileName;
				WholePath = imgNames[i].c_str();
				MyFileName = app->GetFileTitleFromFileName(WholePath, TRUE);
				CStringA strA(MyFileName.GetBuffer(0));
				MyFileName.ReleaseBuffer();
				string str = strA.GetBuffer(0);

				string quaFile = pDlg->AllFiles.quasolve + str;
				string unqFile = pDlg->AllFiles.unqsolve + str;


				image = imread(imgNames[i], 1);//读取图片
				IplImage *Cvimage1 = &IplImage(image);
				ShowImage2(Cvimage1, pDlg->IDC_Name[j]);//显示图片

				////判断图片是否合格，若合格，边框标为绿色
				//if (!_access(quaFile.c_str(), 0))
				//{
				//	CRect rtTop1;
				//	switch (j)
				//	{
				//	case 0:
				//		m_pic1.GetClientRect(&rtTop1);
				//		FrameRect(m_pic1.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 1:
				//		m_pic2.GetClientRect(&rtTop1);
				//		FrameRect(m_pic2.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 2:
				//		m_pic3.GetClientRect(&rtTop1);
				//		FrameRect(m_pic3.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 3:
				//		m_pic4.GetClientRect(&rtTop1);
				//		FrameRect(m_pic4.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 4:
				//		m_pic5.GetClientRect(&rtTop1);
				//		FrameRect(m_pic5.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 5:
				//		m_pic6.GetClientRect(&rtTop1);
				//		FrameRect(m_pic6.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 6:
				//		m_pic7.GetClientRect(&rtTop1);
				//		FrameRect(m_pic7.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 7:
				//		m_pic8.GetClientRect(&rtTop1);
				//		FrameRect(m_pic8.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 8:
				//		m_pic9.GetClientRect(&rtTop1);
				//		FrameRect(m_pic9.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 9:
				//		m_pic10.GetClientRect(&rtTop1);
				//		FrameRect(m_pic10.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 10:
				//		m_pic11.GetClientRect(&rtTop1);
				//		FrameRect(m_pic11.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 11:
				//		m_pic12.GetClientRect(&rtTop1);
				//		FrameRect(m_pic12.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 12:
				//		m_pic13.GetClientRect(&rtTop1);
				//		FrameRect(m_pic13.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 13:
				//		m_pic14.GetClientRect(&rtTop1);
				//		FrameRect(m_pic14.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	case 14:
				//		m_pic15.GetClientRect(&rtTop1);
				//		FrameRect(m_pic15.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(0, 255, 0)));//填充该区域颜色
				//		break;
				//	default:
				//		MessageBox(TEXT("出错了..."));
				//		break;
				//	}

				//}
				//else if (!_access(unqFile.c_str(), 0))
				//{
				//	CRect rtTop1;
				//	switch (j)
				//	{
				//	case 0:
				//		m_pic1.GetClientRect(&rtTop1);
				//		FrameRect(m_pic1.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 1:
				//		m_pic2.GetClientRect(&rtTop1);
				//		FrameRect(m_pic2.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 2:
				//		m_pic3.GetClientRect(&rtTop1);
				//		FrameRect(m_pic3.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 3:
				//		m_pic4.GetClientRect(&rtTop1);
				//		FrameRect(m_pic4.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 4:
				//		m_pic5.GetClientRect(&rtTop1);
				//		FrameRect(m_pic5.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 5:
				//		m_pic6.GetClientRect(&rtTop1);
				//		FrameRect(m_pic6.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 6:
				//		m_pic7.GetClientRect(&rtTop1);
				//		FrameRect(m_pic7.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 7:
				//		m_pic8.GetClientRect(&rtTop1);
				//		FrameRect(m_pic8.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 8:
				//		m_pic9.GetClientRect(&rtTop1);
				//		FrameRect(m_pic9.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 9:
				//		m_pic10.GetClientRect(&rtTop1);
				//		FrameRect(m_pic10.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 10:
				//		m_pic11.GetClientRect(&rtTop1);
				//		FrameRect(m_pic11.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 11:
				//		m_pic12.GetClientRect(&rtTop1);
				//		FrameRect(m_pic12.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 12:
				//		m_pic13.GetClientRect(&rtTop1);
				//		FrameRect(m_pic13.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 13:
				//		m_pic14.GetClientRect(&rtTop1);
				//		FrameRect(m_pic14.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	case 14:
				//		m_pic15.GetClientRect(&rtTop1);
				//		FrameRect(m_pic15.GetDC()->GetSafeHdc(), &rtTop1, CBrush(RGB(255, 255, 0)));//填充该区域颜色
				//		break;
				//	default:
				//		MessageBox(TEXT("我也不知道出什么错了..."));
				//		break;
				//	}
				//}

				j++;
			}

		}


		if (j >= 25)
		{
			j = 0;

			//第一次打开显示完照片就暂停
			pDlg->KillTimer(1);
		}

		//if ((unsigned int)i >= imgNames.size())
		//{
		//	KillTimer(1);
		//}

	}

}


// 更新当前扫描进度
bool CMainWindowImgShow::ShowTheProgress(vector <int> X_Location,
	vector <int> Y_Location, int Current_Pic_Num, int X_correct, int array_x, int array_y)
{
	//int dir = 1;
	//int x[10000],y[10000];
	//x[0] = 0; y[0] = 0;
	//Point current_point;
	//画大的区域矩形
	if (X_Location.size() < Current_Pic_Num)
		return false;
	//默认玻片涂片区域大小为：55mm*25mm，对应的像素大小为：275*125；1mm=5像素；
	//电机运动脉冲系数为：10000plus/mm；所以像素与脉冲的关系为：1像素=2000脉冲
	Mat image(275, 125, CV_8UC3, Scalar(192, 192, 192));

	//画出扫描区域的网格图
	Point rectangle_point1, rectangle_point2;
	rectangle_point1.y = 137.5 - (float)array_y / 2 * 5;
	rectangle_point1.x = 62.5 - (float)array_x / 2 * 5;
	rectangle_point2.y = 137.5 + (float)array_y / 2 * 5;
	rectangle_point2.x = 62.5 + (float)array_x / 2 * 5;

	rectangle(image, rectangle_point1, rectangle_point2, Scalar(234, 234, 234));

	for (size_t i = 0; i < array_x ; i++)
	{
		rectangle_point1.x = rectangle_point1.x + 5;
		rectangle_point2.x = rectangle_point1.x;
		rectangle_point2.y = rectangle_point1.y + array_y * 5;
		line(image, rectangle_point1, rectangle_point2, Scalar(234, 234, 234), 1);
	}

	for (size_t i = 0; i < array_y; i++)
	{
		rectangle_point1.y = rectangle_point1.y + 5;
		rectangle_point2.y = rectangle_point1.y;
		rectangle_point2.x = rectangle_point1.x - array_x * 5;
		line(image, rectangle_point1, rectangle_point2, Scalar(234, 234, 234), 1);
	}

	for (int i = 0; i < Current_Pic_Num; i++)
	{
		Point current_point;

		current_point.y = 137.5 - (float)Y_Location[i] / 2000;
		current_point.x = 62.5 - ((float)X_Location[i] / 10000 + (float)X_correct) * 5;
		Rect rect2(current_point.x, current_point.y, 2, 2);
		rectangle(image, rect2, Scalar(0, 0, 255), CV_FILLED);
		//int thickness = -1;
		//int lineType = 8;
		//circle(image,current_point,1,Scalar(0, 0, 255),thickness,lineType);
	}
	//画点
	//Rect rect2(0, 0, 10, 10);
	//rectangle(image, rect2, Scalar(0, 0, 255), CV_FILLED);
	//for (size_t i = 1;  i<currentnum; i++)
	//{
	//	if (i % length != 0)
	//		x[i] = x[i - 1] + dir;
	//	else
	//	{
	//		x[i] = x[i - 1];
	//		dir = -dir;
	//	}
	//	y[i] = floor(i/length);
	//	current_point.x = x[i]*10;
	//	current_point.y = y[i]*10;
	//	Rect rect2(current_point.y, current_point.x, 10, 10);
	//	rectangle(image, rect2, Scalar(0, 0, 255), CV_FILLED);
	//	//circle(image, current_point, 3, Scalar(0, 0, 255), -1); //第五个参数我设为-1，表明这是个
	//}
	Mat imageResult;
	flip(image, imageResult,1);//0代表x轴旋转，任意正数代表y轴旋转，任意负数代表x和y轴同时旋转
	Mat mask_R(imageResult.cols, imageResult.rows, CV_8UC3, Scalar(0, 0, 0));

	//Mat mask_R = Mat::zeros(img.size(), CV_8U);//同原图一样大小，纯黑的mask图像//CV_8UC3
	imageResult.at<Vec3b>(100, 100) = Vec3b(255, 255, 255);
	for (int i = 0; i < imageResult.rows; i++)
	{
		for (int j = 0; j < imageResult.cols; j++)
		{
			mask_R.at<Vec3b>(imageResult.cols - j - 1, i) = imageResult.at<Vec3b>(i, j);
		}
	}

	//将Mat图片显示到picture控件
	if (mask_R.empty())	return false;
	IplImage *Cvimage1 = &IplImage(mask_R);
	ShowImage2(Cvimage1, IDC_PROGRESSSHOW);//显示图片
	//static BITMAPINFO *bitMapinfo = NULL;
	//static bool First = TRUE;
	//if (First)
	//{
	//	BYTE *bitBuffer = new BYTE[40 + 4 * 256];//开辟一个内存区域
	//	if (bitBuffer == NULL)
	//	{
	//		return false;
	//	}
	//	First = FALSE;
	//	memset(bitBuffer, 0, 40 + 4 * 256);
	//	bitMapinfo = (BITMAPINFO *)bitBuffer;
	//	bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//	bitMapinfo->bmiHeader.biPlanes = 1;
	//	for (int i = 0; i<256; i++)
	//	{ //颜色的取值范围 (0-255)
	//		bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
	//	}
	//}
	//bitMapinfo->bmiHeader.biHeight = -image.rows;
	//bitMapinfo->bmiHeader.biWidth = image.cols;
	//bitMapinfo->bmiHeader.biBitCount = image.channels() * 8;

	//CWnd *pictureWnd = pDlg->GetDlgItem(IDC_PROGRESSSHOW);
	//CRect drect;
	//pictureWnd->GetClientRect(drect);    //pWnd指向CWnd类的一个指针 
	//CClientDC dc(pictureWnd);
	//HDC hDC = dc.GetSafeHdc();                  //HDC是Windows的一种数据类型，是设备描述句柄；
	//SetStretchBltMode(hDC, COLORONCOLOR);
	//StretchDIBits(hDC,
	//	0,
	//	0,
	//	drect.right,  //显示窗口宽度
	//	drect.bottom,  //显示窗口高度
	//	0,
	//	0,
	//	image.cols,     //图像宽度
	//	image.rows,     //图像高度
	//	image.data,
	//	bitMapinfo,
	//	DIB_RGB_COLORS,
	//	SRCCOPY);

	return true;
}

//图片旋转任意角度
void CMainWindowImgShow::rotate_arbitrarily_angle(Mat &src, Mat &dst, float angle)
{
	float radian = (float)(angle / 180.0 * CV_PI);     //填充图像   
	int maxBorder =(int) (max(src.cols, src.rows)* 1.414 ); //即为sqrt(2)*max    
	int dx = (maxBorder - src.cols)/2;    int dy = (maxBorder - src.rows)/2;    
	copyMakeBorder(src, dst, dy, dy, dx, dx, BORDER_CONSTANT);     //旋转   
	Point2f center( (float)(dst.cols/2) , (float) (dst.rows/2));   
	Mat affine_matrix = getRotationMatrix2D( center, angle, 1.0 );//求得旋转矩阵  
	warpAffine(dst, dst, affine_matrix, dst.size());     //计算图像旋转之后包含图像的最大的矩形   
	float sinVal = abs(sin(radian));   
	float cosVal = abs(cos(radian));  
	Size targetSize( (int)(src.cols * cosVal +src.rows * sinVal),     
		(int)(src.cols * sinVal + src.rows * cosVal) );     //剪掉多余边框  
	int x = (dst.cols - targetSize.width) / 2;  
	int y = (dst.rows - targetSize.height) / 2;  
	Rect rect(x, y, targetSize.width, targetSize.height);   
	dst = Mat(dst,rect);
}
