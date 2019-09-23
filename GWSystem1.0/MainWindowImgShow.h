#pragma once
#include "ScanAndSaveImg.h"

//����OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "CvvImage.h"

using namespace cv;

class CMainWindowImgShow :
	public CScanAndSaveImg
{
public:
	CMainWindowImgShow();
	~CMainWindowImgShow();

	CRect rcClient;
	vector <string>  imgNames;//ͼƬ�ĵ�ַ

	char tempFilePath[MAX_PATH + 1];
	char DesTempFilePath[MAX_PATH + 1];
	char tempFileName[10000];
	WIN32_FIND_DATA file;
	WCHAR   wstr[MAX_PATH];
	WCHAR   wstr_des[MAX_PATH];
	
	Mat image;
	unsigned int Page;//��ǰ��ʾͼƬ��ҳ�룬�ӵ�0ҳ��ʼ
	int scrollrangepage;
	CScrollBar m_scroll;
	SCROLLINFO scrollinfo;
	CPoint MouseLoc;
	CRect AllPictureFrame;

	// ���ҵ�����ͼƬ��·��
	void FindAllImgFiles();
	//��ʾͼƬ��picture�ؼ�
	void ShowImage2(IplImage *img, UINT ID);
	//���picture�е�ͼƬ
	void PicClear(UINT ID);
	//��ʼ��picture�ؼ���������
	void PutPicControlIntoVector();

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	// ��һҳ
	void MainWindowLastpage();
	// ��һҳ
	void MainWindowNextpage();
	// ����������ƶ���Ӧ
	void MyMouseMove(UINT nFlags, CPoint point);
	//��������������Ӧ
	void MyMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//�����ڴ�ֱ��������Ӧ
	void MyVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// ˢ�¹������Ĵ�С
	void FreshScroll();
	// ��ʱ����ʾ��Ƭ
	void TimerShowPicture();
	// ���µ�ǰɨ�����
	bool ShowTheProgress(vector <int> X_Location,
		vector <int> Y_Location, int Current_Pic_Num,int X_correct,int array_x,int array_y);
	void rotate_arbitrarily_angle(Mat &src, Mat &dst, float angle);


};

