// CV_picture.cpp : 实现文件
//

#include "stdafx.h"		//将其加入工程后就得包含该文件

#include "Resource.h"
#include"ChromosomeResult.h"
#include "CV_picture.h"
using namespace cv;

extern Mat ImgMat;
extern CChromosomeResult * pShow;
IMPLEMENT_DYNAMIC(CV_picture, CStatic)

CV_picture::CV_picture()
{
}
//由于关联自定义类时，无法带参数初始化该对象，因而独立一个初始化函数出来
void CV_picture::OnInit()
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	m_src_img=Mat(m_rect.Height(),m_rect.Width(),CV_8UC3);
	m_src_img=0;
	m_dst_img=m_src_img+0;
	m_drawing=m_dst_img+0;
	m_MouseDraw_img=Mat(m_dst_img.size(),m_dst_img.type(),Scalar(255,255,255));
	m_iDrawingMethod=1;
	m_ptMButtonDown=0;
	m_ptBeforeMove=Point2f(0);

	//初始化roi相关参数
	m_fRoi_scale=1;
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,50,50);
	m_dst_roi=m_dst_img(m_rect_roi);

	//默认状态下该控件可以被激活，只有控件被激活，该控件才会相应鼠标操作
	m_bEnableMSG=1;
	m_bActive=0;
	m_iDragFlag=0;
	m_iMouseDraw=1;
	m_bLButtonDown=0;

	///////////////在这里添加额外的初始化代码////////////////

	m_bMouseMoveinfoEnable=0;
}

CV_picture::~CV_picture()
{
}


//往对应图片控件加载原图片，并完成相关初始化（如果是重复载入相同尺寸的图，并且保持现在的状态不初始化则直接对m_dst_img赋值）
void CV_picture::cvLoadImage(cv::Mat &img)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	m_src_img=img+0;
	m_dst_img=m_src_img+0;
	m_MouseDraw_img=Mat(m_dst_img.size(),m_dst_img.type(),Scalar(255,255,255));
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,10,10);
	m_dst_roi=m_dst_img(m_rect_roi);
	m_fRoi_scale=1;
}

//往对应图片控件加载原图片，并完成相关初始化（输入为文件路径）
bool  CV_picture::cvLoadImage(CString &Path)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	Mat checkopen;
	string strPath = CT2A(Path.GetBuffer());
	checkopen = imread(strPath);
	if (checkopen.data==NULL)
	{
		MessageBox(_T("加载文件错误，请确认路径或文件类型是否正确！","操作失败",MB_ICONERROR));
		return 0;
	}
	m_src_img=checkopen+0;
	m_dst_img=m_src_img+0;
	m_MouseDraw_img=Mat(m_dst_img.size(),m_dst_img.type(),Scalar(255,255,255));
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,10,10);
	m_dst_roi=m_dst_img(m_rect_roi);
	m_fRoi_scale=1;

	return 1;
}

//自动调节img大小以适应对应图片控件，并将其完整显示出来，同时将该img存入该类的m_dst_img并
//初始化roi相关参数用于其它处理（相当于运行了LoadImage）
//注：对于刷新画面或高速显示相同规格的图片，应先调用一次该函数，然后使用ShowImage_roi装载其他的图片提高效率
//method缩放的方式：0表示保持图片原比例去适应m_rect,1表示拉伸图片去适应m_rect
void CV_picture::ShowImage(Mat &img,int method)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	//将img存入该类的m_dst_img并初始化roi相关参数
	m_dst_img=img+0;
	m_MouseDraw_img=Mat(m_dst_img.size(),m_dst_img.type(),Scalar(255,255,255));
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f+0.5f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f+0.5f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,10,10);
	m_dst_roi=m_dst_img(m_rect_roi);
	m_fRoi_scale=1;
	//图片大小相同，则直接跳过ResizeImage，将img原样复制到m_drawing减少计算量
	if (m_dst_img.size!=m_drawing.size)
	{
		ResizeImage(m_dst_img,m_rect,m_drawing,method);
	}else
	{
		m_drawing=m_dst_img+0;
	}

	OnPaint();
}


//设置m_dst_img的roi区域，因此使用前应先用LoadImage或ShowImage加载过一个图像
//调用该方法前需先用ShowImage对该类内的m_dst_img和roi初始化一次，否则会出错
//center_vec为roi偏移向量(偏移单位为m_dst_img的单位)，m_fRoi_scale为roi相对原图的缩放比(传入-1则表示不改变)
void CV_picture::SetImage_roi(cv::Point &center_vec,float &roi_scale)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (roi_scale>=0)	//如果m_fRoi_scale变化了，则重新缩放原图
	{
		m_rect_roi_center=m_rect_roi_center+center_vec;
		//计算m_rect_roi并获取m_dst_roi
		Point tl,br;
		int roi_width,roi_height;
		roi_height=roi_scale*((float)m_dst_img.rows);
		roi_width=roi_scale*((float)m_dst_img.cols);
		//图像放大缩小的边界检测，防止越界,roi区域越大，图像越小
		//roi区域过小检测
		if ((roi_height<=roi_width)&&(roi_height<40))
		{
			roi_height=40;
			roi_scale=(float)roi_height/(float)m_dst_img.rows;
			roi_width=roi_scale*((float)m_dst_img.cols);
		}
		if ((roi_height>roi_width)&&(roi_width<40))
		{
			roi_width=40;
			roi_scale=(float)roi_width/(float)m_dst_img.cols;
			roi_height=roi_scale*((float)m_dst_img.rows);
		}
		//roi区域过大检测
		if (roi_height>m_dst_img.rows)
		{
			roi_scale=1;
			roi_height=m_dst_img.rows;
			roi_width=m_dst_img.cols;
		}
		if (roi_width>m_dst_img.cols)
		{
			roi_scale=1;
			roi_height=m_dst_img.rows;
			roi_width=m_dst_img.cols;
		}
		m_fRoi_scale=roi_scale;
		//计算新的m_rect_roi
		tl.x=(float)m_rect_roi_center.x-(float)roi_width/2.0f+0.5f;
		tl.y=(float)m_rect_roi_center.y-(float)roi_height/2.0f+0.5f;
		m_rect_roi=Rect(tl.x,tl.y,roi_width,roi_height);
		//对新的m_rect_roi进行边界检测，前面只是对缩放率进行保护，如果m_rect_roi的中心也会移动，则还需对角点进行保护
		boundary_protect(tl,m_dst_img);
		br=tl+Point(roi_width,roi_height);
		boundary_protect(br,m_dst_img);
		tl=br-Point(roi_width,roi_height);
		//计算最终的新m_rect_roi
		m_rect_roi=Rect(tl,br);
		m_rect_roi_center=Point((float)(tl.x+br.x)/2.0f,(float)(tl.y+br.y)/2.0f);
		m_dst_roi=m_dst_img(m_rect_roi);
	}else if (-1==roi_scale)	//如果m_fRoi_scale为-1则表示保持原比率，纯粹移动roi区域
	{
		m_rect_roi_center=m_rect_roi_center+center_vec;
		//计算m_rect_roi并获取m_dst_roi
		Point tl,br;
		tl.x=(float)m_rect_roi_center.x-(float)m_rect_roi.width/2.0f+0.5f;
		tl.y=(float)m_rect_roi_center.y-(float)m_rect_roi.height/2.0f+0.5f;
		m_rect_roi=Rect(tl.x,tl.y,m_rect_roi.width,m_rect_roi.height);
		//对新的m_rect_roi进行边界检测，前面只是对缩放率进行保护，如果m_rect_roi的中心也会移动，则还需对角点进行保护
		boundary_protect(tl,m_dst_img);
		br=tl+Point(m_rect_roi.width,m_rect_roi.height);
		boundary_protect(br,m_dst_img);
		tl=br-Point(m_rect_roi.width,m_rect_roi.height);
		//计算最终的新m_rect_roi
		m_rect_roi=Rect(tl,br);
		m_rect_roi_center=Point((float)(tl.x+br.x)/2.0f,(float)(tl.y+br.y)/2.0f);
		m_dst_roi=m_dst_img(m_rect_roi);
	}
}

//显示src的roi区域内的图像，因为不带任何初始化，src的图片规格必须与m_dst_img完全一致，同时这也是一个非常高效的显示图片函数，
//适合用于刷新画面或高速显示相同规格的图片
void CV_picture::ShowImage_roi(cv::Mat &src,int method)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	m_dst_roi=src(m_rect_roi);

	//图片大小相同，则直接跳过ResizeImage，将img原样复制到m_drawing减少计算量
	if (m_dst_roi.size!=m_drawing.size)
	{
		ResizeImage(m_dst_roi,m_rect,m_drawing,method);
	}else
	{
		m_drawing=m_dst_roi+0;
	}

	OnPaint();
}



//img输入的图像，rect希望调整成的大小（类型为MFC类的CRect），dst_img调整后的图，
//该函数先格式化dst_img,再将img画到dst_img,因此不能img与dst_img绝不能是同一个图
//method缩放的方式：0表示保持图片原比例去适应rect,1表示拉伸图片去适应rect
void CV_picture::ResizeImage(Mat &img,CRect rect,Mat &dst_img,int method)
{
	// 读取图片的宽和高
	int h = img.rows;
	int w = img.cols;
	int nw,nh;

	if (0==method)
	{
		// 计算将图片缩放到dst_drawing区域所需的比例因子
		float scale1 = (float) ( (float)w /(float)rect.Width() );
		float scale2 = (float) ( (float)h /(float)rect.Height() );

		float scale=(scale1>scale2)?scale1:scale2;

		// 缩放后图片的宽和高
		nw = ((float)w)/scale;
		nh = ((float)h)/scale;

		//由于浮点也存在数据截断，为防止nw大于目标框大小，做个边界保护
		if (nw>rect.Width())
		{
			nw=rect.Width();
		}
		if (nh>rect.Height())
		{
			nh=rect.Height();
		}
	}else if (1==method)
	{
		// 缩放后图片的宽和高
		nw = rect.Width();
		nh = rect.Height();
	}

	// 为了将缩放后的图片存入 dst_drawing 的正中部位，需计算图片在 dst_drawing 左上角的期望坐标值
	int tlx = ( nw < rect.Width())? (((float)(rect.Width()-nw))/2.0f+0.5f):   0;
	int tly = ( nh < rect.Height())? (((float)(rect.Height()-nh))/2.0f+0.5f): 0;

	//重置矩阵大小在图比较大时也很费时间，加个判断可避免重复的重置矩阵大小
	if ( dst_img.rows!=rect.Height() || dst_img.cols!=rect.Width() || dst_img.type()!=img.type() )
	{
		dst_img=Mat(rect.Height(),rect.Width(),img.type());
	}
	if (0==method)
	{
		dst_img=0;
	}
	// 设置 dst_img 的 ROI 区域，用来存入更改后的图片 img
	Rect rect_roi=Rect(tlx,tly,nw,nh);

	// 对图片 img 进行缩放，并存入到 src_drawing 中
	Mat dst_img_roi=dst_img(rect_roi);

	//将img尺寸缩放至dst_img_roi尺寸,插值方法选用INTER_NEAREST，放到最大后无近似显示能将每个像素看清
	resize(img,dst_img_roi,cv::Size(nw,nh),0.0,0.0,INTER_NEAREST);
}




//##############################################消息处理##############################################################
BEGIN_MESSAGE_MAP(CV_picture, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	
END_MESSAGE_MAP()


// CV_picture 消息处理程序
void CV_picture::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//鼠标中间按下情况下计算两次进入该消息时指针位置偏差
	//(为避免累计整形数据截断误差，整个算法应该计算相对于鼠标按下时的整个位移而不是直接计算每次进入该消息时的小段位移)
	Point2f move_vec;	//本次消息移动前roi相对于初始位置的位移
	CPoint mouse_vec;	//本次消息鼠标位置相对于初始鼠标位置的位移
	Point center_vec;	//本次消息roi应该移动的位移
	if (1==m_iDragFlag)
	{
		move_vec=(Point2f)m_rect_roi.tl()-m_ptBeforeMove;
		mouse_vec=m_ptMButtonDown-point;
		//根据两种缩放模式分别计算roi移动距离与鼠标移动距离的比
		if (0==m_iDrawingMethod)
		{
			// 计算将图片缩放到dst_drawing区域所需的比例因子
			float scale1 = (float) ( (float)m_rect_roi.width /(float)m_rect.Width() );
			float scale2 = (float) ( (float)m_rect_roi.height /(float)m_rect.Height());
			m_scale_move.x=(scale1>scale2)?scale1:scale2;
			m_scale_move.y=m_scale_move.x;
		}
		else{
			m_scale_move.x=(float)m_rect_roi.width/(float)m_rect.Width();
			m_scale_move.y=(float)m_rect_roi.height/(float)m_rect.Height();
		}
		center_vec=Point((float)mouse_vec.x*m_scale_move.x-move_vec.x,(float)mouse_vec.y*m_scale_move.y-move_vec.y);
		//只有在移动距离大于3才进行图片的移动
		if( center_vec.dot(center_vec)>0.5 )
		{
			float scale_nochange=-1;
			SetImage_roi(center_vec,scale_nochange);
		}
	}

	//画图程序
	if (2==m_iMouseDraw)
	{
		//根据鼠标位置计算画刷矩形位置
		Point tl,br;
		tl=Point(point.x-m_MouseDraw_rect.width/2,point.y-m_MouseDraw_rect.height/2);
		boundary_protect(tl,m_MouseDraw_img);
		br=tl+Point(m_MouseDraw_rect.width,m_MouseDraw_rect.height);
		boundary_protect(br,m_MouseDraw_img);
		tl=br-Point(m_MouseDraw_rect.width,m_MouseDraw_rect.height);
		m_MouseDraw_rect=Rect(tl,br);
		//计算画刷对应在roi内的矩形
		Rect draw_rect_inROI;
		Point tl_inROI,br_inROI;
		static Point tl_inROI_last;
		tl_inROI.x=(float)tl.x/(float)m_rect.Width()*(float)m_rect_roi.width+0.5f;
		tl_inROI.y=(float)tl.y/(float)m_rect.Height()*(float)m_rect_roi.height+0.5f;
		br_inROI.x=(float)br.x/(float)m_rect.Width()*(float)m_rect_roi.width+0.5f;
		br_inROI.y=(float)br.y/(float)m_rect.Height()*(float)m_rect_roi.height+0.5f;
		if (tl_inROI!=tl_inROI_last)
		{
			draw_rect_inROI=Rect(tl_inROI,br_inROI);
			//使用画刷将图画黑
			cv::rectangle(m_MouseDraw_img(m_rect_roi),draw_rect_inROI,Scalar(0,0,0),-1);
			Point nomove(0,0);
			float nochange=-1;
			::SendMessageA(this->GetParent()->m_hWnd,WM_CV_picture,0,LPARAM(&m_MouseDraw_img));
		}
		tl_inROI_last=tl_inROI;
	}

	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//显示当前标记的点的数目
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();

	if (m_bMouseMoveinfoEnable)
	{
		m_ptMouseMove=point;
			//在坐标控件上显示当前鼠标的坐标
			//根据鼠标位置计算位于图像上的位置
		Point point_in_img;
		point_in_img.x=(float)point.x/(float)m_rect.Width()*(float)m_rect_roi.width;
		point_in_img.y=(float)point.y/(float)m_rect.Height()*(float)m_rect_roi.height;
		point_in_img.x=point_in_img.x+m_rect_roi.x;
		point_in_img.y=point_in_img.y+m_rect_roi.y;
		//下面是将当前坐标信息显示到对话框控件的模板
// 		char x[10];
// 		sprintf(x,"%.5f",height);
// 		CWnd *pCWnd_parent;
// 		pCWnd_parent=GetParent();
// 		pCWnd_parent->GetDlgItem(IDC_EDIT1)->SetWindowTextA(x);
	}

	CStatic::OnMouseMove(nFlags, point);
}


//当前控件必须获得焦点，才能捕捉鼠标滚轮消息
BOOL CV_picture::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//更新图片控件的坐标
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if ((m_rect_win.PtInRect(pt))&&(1==m_bActive))
	{
		//计算现在src_roi相对于src的缩放比
		//m_fRoi_scale = m_fRoi_scale + m_fRoi_scale*0.2f*(float)zDelta / 120.0f;
		m_fRoi_scale = m_fRoi_scale + m_fRoi_scale*0.2f*(float)zDelta / 5000.0f;
		//计算当前鼠标相对画板中心的偏移
		CPoint src_draw_tl=m_rect_win.TopLeft();
		Point pt_in_draw=Point((pt.x-src_draw_tl.x),(pt.y-src_draw_tl.y));
		Point vec(pt_in_draw.x-m_rect.CenterPoint().x,pt_in_draw.y-m_rect.CenterPoint().y);
		//当前缩放比的算出roi中心的偏移
		//Point center_vec = Point((float(vec.x)*m_fRoi_scale + 0.5f), (float(vec.y)*m_fRoi_scale + 0.5f));
		Point center_vec = Point((float(vec.x)*m_fRoi_scale), (float(vec.y)*m_fRoi_scale));
		//显示调整后的roi区域
		SetImage_roi(center_vec,m_fRoi_scale);
		//根据两种缩放模式分别计算roi移动距离与鼠标移动距离的比
		if (0==m_iDrawingMethod)
		{
			// 计算将图片缩放到dst_drawing区域所需的比例因子
			float scale1 = (float) ( (float)m_rect_roi.width /(float)m_rect.Width() );
			float scale2 = (float) ( (float)m_rect_roi.height /(float)m_rect.Height());
			m_scale_move.x=(scale1>scale2)?scale1:scale2;
			m_scale_move.y=m_scale_move.x;
		}
		else{
			m_scale_move.x=(float)m_rect_roi.width/(float)m_rect.Width();
			m_scale_move.y=(float)m_rect_roi.height/(float)m_rect.Height();
		}
	}

	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//显示当前标记的点的数目
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void CV_picture::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	//更新图片控件的坐标
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (1==m_bEnableMSG)
	{
		m_bActive=1;
		m_iDragFlag=1;
		m_ptMButtonDown=point;
		m_ptBeforeMove=m_rect_roi.tl();
		//使当前控件获得焦点，这样改控件才能捕捉鼠标滚轮消息
		this->SetFocus();
	}

	//////////// TODO: 在此添加控件额外的处理程序代码/////////////////////////////

	CStatic::OnMButtonDown(nFlags, point);
}


void CV_picture::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_iDragFlag=0;
	m_ptMButtonDown=0;
	m_ptBeforeMove=m_rect_roi.tl();
	//////////// TODO: 在此添加控件额外的处理程序代码/////////////////////////////

	CStatic::OnMButtonUp(nFlags, point);
}


void CV_picture::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pShow->EditOrNot)
	{
		cv::Mat currentMat;
		CRect myshowwinrect;
		pShow->GetDlgItem(IDC_CHMAINFRAME)->GetWindowRect(myshowwinrect);//获得屏幕坐标
		pShow->ScreenToClient(&myshowwinrect);//转到客户区相对坐标
		if (point.x >= myshowwinrect.left && point.x <= myshowwinrect.right && point.y >= myshowwinrect.top && point.y <= myshowwinrect.bottom)//鼠标在Picture Control 控件上进行了单击操作
		{
			int img_x = (double)m_dst_roi.cols*(double)(point.x - myshowwinrect.left) / (double)myshowwinrect.Width();//计算鼠标点击横坐标相对图片上的横位置
			int img_y = (double)m_dst_roi.rows*(double)(point.y - myshowwinrect.top) / (double)myshowwinrect.Height();
			int changepoint = img_y * m_dst_roi.step / sizeof(uchar)+img_x * ImgMat.channels();

			//将图片的相应点标记为白色
			Point p(img_x, img_y);
			circle(m_dst_roi, p, 10, Scalar(0, 255, 0), -1);  // 画半径为1的圆(画点）
			//ImgMat.data[changepoint + 0] = 0;
			//ImgMat.data[changepoint + 1] = 255;
			//ImgMat.data[changepoint + 2] = 0;
			pShow->currentPointStep = 0;

			pShow->PointImg.push_back(p);
			currentMat = m_dst_roi;
			pShow->GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);


		}
		if (!currentMat.empty())
		{
			pShow->m_bigImgchro.ShowImage(currentMat, 1);

			//显示当前标记的点的数目
			CString strnum;
			strnum.Format(_T("%d"), pShow->PointImg.size());
			pShow->m_point_sum.SetWindowTextW(strnum);
			pShow->m_chro_num.BringWindowToTop();

		}


	}


	SetCapture();
	m_bLButtonDown=1;

	//更新图片控件的坐标
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (1==m_bEnableMSG)
	{
		m_bActive=1;
		//使当前控件获得焦点，这样改控件才能捕捉鼠标滚轮消息
		this->SetFocus();
	}
	if (1==m_iMouseDraw)
	{
		m_iMouseDraw=2;
	}

	if (!m_dst_img.empty())
	{
		ShowImage_roi(m_dst_img, m_iDrawingMethod);

		//显示当前标记的点的数目
		CString strnum;
		strnum.Format(_T("%d"), pShow->PointImg.size());
		pShow->m_point_sum.SetWindowTextW(strnum);
		pShow->m_chro_num.BringWindowToTop();

	}

	CStatic::OnLButtonDown(nFlags, point);
}


void CV_picture::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_bLButtonDown=0;

	if (2==m_iMouseDraw)
	{
		m_iMouseDraw=1;
	}

	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//显示当前标记的点的数目
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	CStatic::OnLButtonUp(nFlags, point);
}


void CV_picture::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//显示当前标记的点的数目
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CV_picture::OnPaint()
{
	//用于响应窗口重绘消息,删除后 WM_PAINT消息无法从消息队列中清除，将引起不断的窗口重画
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CDC* pDC=this->GetDC();	
	HDC hDC = pDC->GetSafeHdc();

	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	//将Mat图画在画板上所需的中间变量

	IplImage* drawing_ipl = &IplImage(m_drawing);
	CvvImage Cvvimg;
	Cvvimg.CopyOf(drawing_ipl);
	// 将图片绘制到显示控件的指定区域内
	Cvvimg.DrawToHDC( hDC, &m_rect );

	ReleaseDC(pDC);		//一个GetDC必须对应一个ReleaseDC，否则造成严重的内存泄露
	// 不为绘图消息调用 CStatic::OnPaint()

	///////////////////在这里添加额外的绘制代码///////////////////////
}


//点的边界保护，如果点超出了给定图像的范围，则将超出的坐标定位到对应边界,主要是对roi边界保护，
//如果需要保护的点是矩阵数据坐标保护，则对于最大边界处的点会出错（roi的角点是到rols，而矩阵数据坐标只能到rols-1）
//按9格方式以图片边界直线分割平面，并返回该点所处的区域1-9,返回-1表示错误
int CV_picture::boundary_protect(Point &pt,Mat &in_img)
{
	int x_area=1,y_area=1;
	int area;
	if((pt.x<0)||(pt.x>in_img.cols+20000))
	{
		pt.x=0;
		x_area=0;
	}else if(pt.x>in_img.cols)
	{
		pt.x=in_img.cols;
		x_area=2;
	}
	if ((pt.y<0)||(pt.y>in_img.rows+20000))
	{
		pt.y=0;
		y_area=0;
	}else if (pt.y>in_img.rows)
	{
		pt.y=in_img.rows;
		y_area=2;
	}
	area=x_area+y_area*10;

	switch (area)
	{
	case 0 : return 1;
	case 01: return 2;
	case 02: return 3;
	case 10: return 4;
	case 11: return 5;
	case 12: return 6;
	case 20: return 7;
	case 21: return 8;
	case 22: return 9;
	default: return -1;
	}
}
