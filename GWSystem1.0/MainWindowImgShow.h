#pragma once
#include "ScanAndSaveImg.h"

//加载OpenCVAPI
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
	vector <string>  imgNames;//图片的地址

	char tempFilePath[MAX_PATH + 1];
	char DesTempFilePath[MAX_PATH + 1];
	char tempFileName[10000];
	WIN32_FIND_DATA file;
	WCHAR   wstr[MAX_PATH];
	WCHAR   wstr_des[MAX_PATH];
	
	Mat image;
	unsigned int Page;//当前显示图片的页码，从第0页开始
	int scrollrangepage;
	CScrollBar m_scroll;
	SCROLLINFO scrollinfo;
	CPoint MouseLoc;
	CRect AllPictureFrame;

	// 查找到所有图片的路径
	void FindAllImgFiles();
	//显示图片到picture控件
	void ShowImage2(IplImage *img, UINT ID);
	//清空picture中的图片
	void PicClear(UINT ID);
	//初始将picture控件放入数组
	void PutPicControlIntoVector();

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	// 上一页
	void MainWindowLastpage();
	// 下一页
	void MainWindowNextpage();
	// 主窗口鼠标移动响应
	void MyMouseMove(UINT nFlags, CPoint point);
	//主窗口鼠标滚动响应
	void MyMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//主窗口垂直滚动条响应
	void MyVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 刷新滚动条的大小
	void FreshScroll();
	// 定时器显示照片
	void TimerShowPicture();
	// 更新当前扫描进度
	bool ShowTheProgress(vector <int> X_Location,
		vector <int> Y_Location, int Current_Pic_Num,int X_correct,int array_x,int array_y);
	void rotate_arbitrarily_angle(Mat &src, Mat &dst, float angle);


};

