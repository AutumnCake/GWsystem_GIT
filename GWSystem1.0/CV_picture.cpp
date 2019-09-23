// CV_picture.cpp : ʵ���ļ�
//

#include "stdafx.h"		//������빤�̺�͵ð������ļ�

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
//���ڹ����Զ�����ʱ���޷���������ʼ���ö����������һ����ʼ����������
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

	//��ʼ��roi��ز���
	m_fRoi_scale=1;
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,50,50);
	m_dst_roi=m_dst_img(m_rect_roi);

	//Ĭ��״̬�¸ÿؼ����Ա����ֻ�пؼ�������ÿؼ��Ż���Ӧ������
	m_bEnableMSG=1;
	m_bActive=0;
	m_iDragFlag=0;
	m_iMouseDraw=1;
	m_bLButtonDown=0;

	///////////////��������Ӷ���ĳ�ʼ������////////////////

	m_bMouseMoveinfoEnable=0;
}

CV_picture::~CV_picture()
{
}


//����ӦͼƬ�ؼ�����ԭͼƬ���������س�ʼ����������ظ�������ͬ�ߴ��ͼ�����ұ������ڵ�״̬����ʼ����ֱ�Ӷ�m_dst_img��ֵ��
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

//����ӦͼƬ�ؼ�����ԭͼƬ���������س�ʼ��������Ϊ�ļ�·����
bool  CV_picture::cvLoadImage(CString &Path)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	Mat checkopen;
	string strPath = CT2A(Path.GetBuffer());
	checkopen = imread(strPath);
	if (checkopen.data==NULL)
	{
		MessageBox(_T("�����ļ�������ȷ��·�����ļ������Ƿ���ȷ��","����ʧ��",MB_ICONERROR));
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

//�Զ�����img��С����Ӧ��ӦͼƬ�ؼ���������������ʾ������ͬʱ����img��������m_dst_img��
//��ʼ��roi��ز����������������൱��������LoadImage��
//ע������ˢ�»���������ʾ��ͬ����ͼƬ��Ӧ�ȵ���һ�θú�����Ȼ��ʹ��ShowImage_roiװ��������ͼƬ���Ч��
//method���ŵķ�ʽ��0��ʾ����ͼƬԭ����ȥ��Ӧm_rect,1��ʾ����ͼƬȥ��Ӧm_rect
void CV_picture::ShowImage(Mat &img,int method)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	//��img��������m_dst_img����ʼ��roi��ز���
	m_dst_img=img+0;
	m_MouseDraw_img=Mat(m_dst_img.size(),m_dst_img.type(),Scalar(255,255,255));
	m_rect_roi=Rect(0,0,m_dst_img.cols,m_dst_img.rows);
	m_rect_roi_center.x=(float)m_rect_roi.width/2.0f+0.5f;
	m_rect_roi_center.y=(float)m_rect_roi.height/2.0f+0.5f;
	m_MouseDraw_rect=Rect(m_rect_roi_center.x,m_rect_roi_center.y,10,10);
	m_dst_roi=m_dst_img(m_rect_roi);
	m_fRoi_scale=1;
	//ͼƬ��С��ͬ����ֱ������ResizeImage����imgԭ�����Ƶ�m_drawing���ټ�����
	if (m_dst_img.size!=m_drawing.size)
	{
		ResizeImage(m_dst_img,m_rect,m_drawing,method);
	}else
	{
		m_drawing=m_dst_img+0;
	}

	OnPaint();
}


//����m_dst_img��roi�������ʹ��ǰӦ����LoadImage��ShowImage���ع�һ��ͼ��
//���ø÷���ǰ������ShowImage�Ը����ڵ�m_dst_img��roi��ʼ��һ�Σ���������
//center_vecΪroiƫ������(ƫ�Ƶ�λΪm_dst_img�ĵ�λ)��m_fRoi_scaleΪroi���ԭͼ�����ű�(����-1���ʾ���ı�)
void CV_picture::SetImage_roi(cv::Point &center_vec,float &roi_scale)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (roi_scale>=0)	//���m_fRoi_scale�仯�ˣ�����������ԭͼ
	{
		m_rect_roi_center=m_rect_roi_center+center_vec;
		//����m_rect_roi����ȡm_dst_roi
		Point tl,br;
		int roi_width,roi_height;
		roi_height=roi_scale*((float)m_dst_img.rows);
		roi_width=roi_scale*((float)m_dst_img.cols);
		//ͼ��Ŵ���С�ı߽��⣬��ֹԽ��,roi����Խ��ͼ��ԽС
		//roi�����С���
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
		//roi���������
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
		//�����µ�m_rect_roi
		tl.x=(float)m_rect_roi_center.x-(float)roi_width/2.0f+0.5f;
		tl.y=(float)m_rect_roi_center.y-(float)roi_height/2.0f+0.5f;
		m_rect_roi=Rect(tl.x,tl.y,roi_width,roi_height);
		//���µ�m_rect_roi���б߽��⣬ǰ��ֻ�Ƕ������ʽ��б��������m_rect_roi������Ҳ���ƶ�������Խǵ���б���
		boundary_protect(tl,m_dst_img);
		br=tl+Point(roi_width,roi_height);
		boundary_protect(br,m_dst_img);
		tl=br-Point(roi_width,roi_height);
		//�������յ���m_rect_roi
		m_rect_roi=Rect(tl,br);
		m_rect_roi_center=Point((float)(tl.x+br.x)/2.0f,(float)(tl.y+br.y)/2.0f);
		m_dst_roi=m_dst_img(m_rect_roi);
	}else if (-1==roi_scale)	//���m_fRoi_scaleΪ-1���ʾ����ԭ���ʣ������ƶ�roi����
	{
		m_rect_roi_center=m_rect_roi_center+center_vec;
		//����m_rect_roi����ȡm_dst_roi
		Point tl,br;
		tl.x=(float)m_rect_roi_center.x-(float)m_rect_roi.width/2.0f+0.5f;
		tl.y=(float)m_rect_roi_center.y-(float)m_rect_roi.height/2.0f+0.5f;
		m_rect_roi=Rect(tl.x,tl.y,m_rect_roi.width,m_rect_roi.height);
		//���µ�m_rect_roi���б߽��⣬ǰ��ֻ�Ƕ������ʽ��б��������m_rect_roi������Ҳ���ƶ�������Խǵ���б���
		boundary_protect(tl,m_dst_img);
		br=tl+Point(m_rect_roi.width,m_rect_roi.height);
		boundary_protect(br,m_dst_img);
		tl=br-Point(m_rect_roi.width,m_rect_roi.height);
		//�������յ���m_rect_roi
		m_rect_roi=Rect(tl,br);
		m_rect_roi_center=Point((float)(tl.x+br.x)/2.0f,(float)(tl.y+br.y)/2.0f);
		m_dst_roi=m_dst_img(m_rect_roi);
	}
}

//��ʾsrc��roi�����ڵ�ͼ����Ϊ�����κγ�ʼ����src��ͼƬ��������m_dst_img��ȫһ�£�ͬʱ��Ҳ��һ���ǳ���Ч����ʾͼƬ������
//�ʺ�����ˢ�»���������ʾ��ͬ����ͼƬ
void CV_picture::ShowImage_roi(cv::Mat &src,int method)
{
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	m_dst_roi=src(m_rect_roi);

	//ͼƬ��С��ͬ����ֱ������ResizeImage����imgԭ�����Ƶ�m_drawing���ټ�����
	if (m_dst_roi.size!=m_drawing.size)
	{
		ResizeImage(m_dst_roi,m_rect,m_drawing,method);
	}else
	{
		m_drawing=m_dst_roi+0;
	}

	OnPaint();
}



//img�����ͼ��rectϣ�������ɵĴ�С������ΪMFC���CRect����dst_img�������ͼ��
//�ú����ȸ�ʽ��dst_img,�ٽ�img����dst_img,��˲���img��dst_img��������ͬһ��ͼ
//method���ŵķ�ʽ��0��ʾ����ͼƬԭ����ȥ��Ӧrect,1��ʾ����ͼƬȥ��Ӧrect
void CV_picture::ResizeImage(Mat &img,CRect rect,Mat &dst_img,int method)
{
	// ��ȡͼƬ�Ŀ�͸�
	int h = img.rows;
	int w = img.cols;
	int nw,nh;

	if (0==method)
	{
		// ���㽫ͼƬ���ŵ�dst_drawing��������ı�������
		float scale1 = (float) ( (float)w /(float)rect.Width() );
		float scale2 = (float) ( (float)h /(float)rect.Height() );

		float scale=(scale1>scale2)?scale1:scale2;

		// ���ź�ͼƬ�Ŀ�͸�
		nw = ((float)w)/scale;
		nh = ((float)h)/scale;

		//���ڸ���Ҳ�������ݽضϣ�Ϊ��ֹnw����Ŀ����С�������߽籣��
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
		// ���ź�ͼƬ�Ŀ�͸�
		nw = rect.Width();
		nh = rect.Height();
	}

	// Ϊ�˽����ź��ͼƬ���� dst_drawing �����в�λ�������ͼƬ�� dst_drawing ���Ͻǵ���������ֵ
	int tlx = ( nw < rect.Width())? (((float)(rect.Width()-nw))/2.0f+0.5f):   0;
	int tly = ( nh < rect.Height())? (((float)(rect.Height()-nh))/2.0f+0.5f): 0;

	//���þ����С��ͼ�Ƚϴ�ʱҲ�ܷ�ʱ�䣬�Ӹ��жϿɱ����ظ������þ����С
	if ( dst_img.rows!=rect.Height() || dst_img.cols!=rect.Width() || dst_img.type()!=img.type() )
	{
		dst_img=Mat(rect.Height(),rect.Width(),img.type());
	}
	if (0==method)
	{
		dst_img=0;
	}
	// ���� dst_img �� ROI ��������������ĺ��ͼƬ img
	Rect rect_roi=Rect(tlx,tly,nw,nh);

	// ��ͼƬ img �������ţ������뵽 src_drawing ��
	Mat dst_img_roi=dst_img(rect_roi);

	//��img�ߴ�������dst_img_roi�ߴ�,��ֵ����ѡ��INTER_NEAREST���ŵ������޽�����ʾ�ܽ�ÿ�����ؿ���
	resize(img,dst_img_roi,cv::Size(nw,nh),0.0,0.0,INTER_NEAREST);
}




//##############################################��Ϣ����##############################################################
BEGIN_MESSAGE_MAP(CV_picture, CStatic)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	
END_MESSAGE_MAP()


// CV_picture ��Ϣ�������
void CV_picture::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����м䰴������¼������ν������Ϣʱָ��λ��ƫ��
	//(Ϊ�����ۼ��������ݽض��������㷨Ӧ�ü����������갴��ʱ������λ�ƶ�����ֱ�Ӽ���ÿ�ν������Ϣʱ��С��λ��)
	Point2f move_vec;	//������Ϣ�ƶ�ǰroi����ڳ�ʼλ�õ�λ��
	CPoint mouse_vec;	//������Ϣ���λ������ڳ�ʼ���λ�õ�λ��
	Point center_vec;	//������ϢroiӦ���ƶ���λ��
	if (1==m_iDragFlag)
	{
		move_vec=(Point2f)m_rect_roi.tl()-m_ptBeforeMove;
		mouse_vec=m_ptMButtonDown-point;
		//������������ģʽ�ֱ����roi�ƶ�����������ƶ�����ı�
		if (0==m_iDrawingMethod)
		{
			// ���㽫ͼƬ���ŵ�dst_drawing��������ı�������
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
		//ֻ�����ƶ��������3�Ž���ͼƬ���ƶ�
		if( center_vec.dot(center_vec)>0.5 )
		{
			float scale_nochange=-1;
			SetImage_roi(center_vec,scale_nochange);
		}
	}

	//��ͼ����
	if (2==m_iMouseDraw)
	{
		//�������λ�ü��㻭ˢ����λ��
		Point tl,br;
		tl=Point(point.x-m_MouseDraw_rect.width/2,point.y-m_MouseDraw_rect.height/2);
		boundary_protect(tl,m_MouseDraw_img);
		br=tl+Point(m_MouseDraw_rect.width,m_MouseDraw_rect.height);
		boundary_protect(br,m_MouseDraw_img);
		tl=br-Point(m_MouseDraw_rect.width,m_MouseDraw_rect.height);
		m_MouseDraw_rect=Rect(tl,br);
		//���㻭ˢ��Ӧ��roi�ڵľ���
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
			//ʹ�û�ˢ��ͼ����
			cv::rectangle(m_MouseDraw_img(m_rect_roi),draw_rect_inROI,Scalar(0,0,0),-1);
			Point nomove(0,0);
			float nochange=-1;
			::SendMessageA(this->GetParent()->m_hWnd,WM_CV_picture,0,LPARAM(&m_MouseDraw_img));
		}
		tl_inROI_last=tl_inROI;
	}

	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//��ʾ��ǰ��ǵĵ����Ŀ
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();

	if (m_bMouseMoveinfoEnable)
	{
		m_ptMouseMove=point;
			//������ؼ�����ʾ��ǰ��������
			//�������λ�ü���λ��ͼ���ϵ�λ��
		Point point_in_img;
		point_in_img.x=(float)point.x/(float)m_rect.Width()*(float)m_rect_roi.width;
		point_in_img.y=(float)point.y/(float)m_rect.Height()*(float)m_rect_roi.height;
		point_in_img.x=point_in_img.x+m_rect_roi.x;
		point_in_img.y=point_in_img.y+m_rect_roi.y;
		//�����ǽ���ǰ������Ϣ��ʾ���Ի���ؼ���ģ��
// 		char x[10];
// 		sprintf(x,"%.5f",height);
// 		CWnd *pCWnd_parent;
// 		pCWnd_parent=GetParent();
// 		pCWnd_parent->GetDlgItem(IDC_EDIT1)->SetWindowTextA(x);
	}

	CStatic::OnMouseMove(nFlags, point);
}


//��ǰ�ؼ������ý��㣬���ܲ�׽��������Ϣ
BOOL CV_picture::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//����ͼƬ�ؼ�������
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if ((m_rect_win.PtInRect(pt))&&(1==m_bActive))
	{
		//��������src_roi�����src�����ű�
		//m_fRoi_scale = m_fRoi_scale + m_fRoi_scale*0.2f*(float)zDelta / 120.0f;
		m_fRoi_scale = m_fRoi_scale + m_fRoi_scale*0.2f*(float)zDelta / 5000.0f;
		//���㵱ǰ�����Ի������ĵ�ƫ��
		CPoint src_draw_tl=m_rect_win.TopLeft();
		Point pt_in_draw=Point((pt.x-src_draw_tl.x),(pt.y-src_draw_tl.y));
		Point vec(pt_in_draw.x-m_rect.CenterPoint().x,pt_in_draw.y-m_rect.CenterPoint().y);
		//��ǰ���űȵ����roi���ĵ�ƫ��
		//Point center_vec = Point((float(vec.x)*m_fRoi_scale + 0.5f), (float(vec.y)*m_fRoi_scale + 0.5f));
		Point center_vec = Point((float(vec.x)*m_fRoi_scale), (float(vec.y)*m_fRoi_scale));
		//��ʾ�������roi����
		SetImage_roi(center_vec,m_fRoi_scale);
		//������������ģʽ�ֱ����roi�ƶ�����������ƶ�����ı�
		if (0==m_iDrawingMethod)
		{
			// ���㽫ͼƬ���ŵ�dst_drawing��������ı�������
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
	//��ʾ��ǰ��ǵĵ����Ŀ
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void CV_picture::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	//����ͼƬ�ؼ�������
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (1==m_bEnableMSG)
	{
		m_bActive=1;
		m_iDragFlag=1;
		m_ptMButtonDown=point;
		m_ptBeforeMove=m_rect_roi.tl();
		//ʹ��ǰ�ؼ���ý��㣬�����Ŀؼ����ܲ�׽��������Ϣ
		this->SetFocus();
	}

	//////////// TODO: �ڴ���ӿؼ�����Ĵ���������/////////////////////////////

	CStatic::OnMButtonDown(nFlags, point);
}


void CV_picture::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_iDragFlag=0;
	m_ptMButtonDown=0;
	m_ptBeforeMove=m_rect_roi.tl();
	//////////// TODO: �ڴ���ӿؼ�����Ĵ���������/////////////////////////////

	CStatic::OnMButtonUp(nFlags, point);
}


void CV_picture::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pShow->EditOrNot)
	{
		cv::Mat currentMat;
		CRect myshowwinrect;
		pShow->GetDlgItem(IDC_CHMAINFRAME)->GetWindowRect(myshowwinrect);//�����Ļ����
		pShow->ScreenToClient(&myshowwinrect);//ת���ͻ����������
		if (point.x >= myshowwinrect.left && point.x <= myshowwinrect.right && point.y >= myshowwinrect.top && point.y <= myshowwinrect.bottom)//�����Picture Control �ؼ��Ͻ����˵�������
		{
			int img_x = (double)m_dst_roi.cols*(double)(point.x - myshowwinrect.left) / (double)myshowwinrect.Width();//������������������ͼƬ�ϵĺ�λ��
			int img_y = (double)m_dst_roi.rows*(double)(point.y - myshowwinrect.top) / (double)myshowwinrect.Height();
			int changepoint = img_y * m_dst_roi.step / sizeof(uchar)+img_x * ImgMat.channels();

			//��ͼƬ����Ӧ����Ϊ��ɫ
			Point p(img_x, img_y);
			circle(m_dst_roi, p, 10, Scalar(0, 255, 0), -1);  // ���뾶Ϊ1��Բ(���㣩
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

			//��ʾ��ǰ��ǵĵ����Ŀ
			CString strnum;
			strnum.Format(_T("%d"), pShow->PointImg.size());
			pShow->m_point_sum.SetWindowTextW(strnum);
			pShow->m_chro_num.BringWindowToTop();

		}


	}


	SetCapture();
	m_bLButtonDown=1;

	//����ͼƬ�ؼ�������
	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	if (1==m_bEnableMSG)
	{
		m_bActive=1;
		//ʹ��ǰ�ؼ���ý��㣬�����Ŀؼ����ܲ�׽��������Ϣ
		this->SetFocus();
	}
	if (1==m_iMouseDraw)
	{
		m_iMouseDraw=2;
	}

	if (!m_dst_img.empty())
	{
		ShowImage_roi(m_dst_img, m_iDrawingMethod);

		//��ʾ��ǰ��ǵĵ����Ŀ
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
	//��ʾ��ǰ��ǵĵ����Ŀ
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	CStatic::OnLButtonUp(nFlags, point);
}


void CV_picture::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ShowImage_roi(m_dst_img,m_iDrawingMethod);
	//��ʾ��ǰ��ǵĵ����Ŀ
	CString strnum;
	strnum.Format(_T("%d"), pShow->PointImg.size());
	pShow->m_point_sum.SetWindowTextW(strnum);
	pShow->m_chro_num.BringWindowToTop();


	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CV_picture::OnPaint()
{
	//������Ӧ�����ػ���Ϣ,ɾ���� WM_PAINT��Ϣ�޷�����Ϣ����������������𲻶ϵĴ����ػ�
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CDC* pDC=this->GetDC();	
	HDC hDC = pDC->GetSafeHdc();

	this->GetClientRect(&m_rect);
	this->GetWindowRect(&m_rect_win);

	//��Matͼ���ڻ�����������м����

	IplImage* drawing_ipl = &IplImage(m_drawing);
	CvvImage Cvvimg;
	Cvvimg.CopyOf(drawing_ipl);
	// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	Cvvimg.DrawToHDC( hDC, &m_rect );

	ReleaseDC(pDC);		//һ��GetDC�����Ӧһ��ReleaseDC������������ص��ڴ�й¶
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	///////////////////��������Ӷ���Ļ��ƴ���///////////////////////
}


//��ı߽籣��������㳬���˸���ͼ��ķ�Χ���򽫳��������궨λ����Ӧ�߽�,��Ҫ�Ƕ�roi�߽籣����
//�����Ҫ�����ĵ��Ǿ����������걣������������߽紦�ĵ�����roi�Ľǵ��ǵ�rols����������������ֻ�ܵ�rols-1��
//��9��ʽ��ͼƬ�߽�ֱ�߷ָ�ƽ�棬�����ظõ�����������1-9,����-1��ʾ����
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
