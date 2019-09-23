#pragma once
#include "GWSystem1.0Dlg.h"
//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "CvvImage.h"

//相机
#include"lucamapi.h"
#include"lucamerr.h"

using namespace cv;

class CHardwareInit :
	public CGWSystem10Dlg
{
public:
	CHardwareInit();
	~CHardwareInit();

//private:

	HANDLE m_hCamera;//相机句柄
	BOOL m_bConnected;//建立连接？
	BOOL m_bPreviewing;//预览打开
	LUCAM_FRAME_FORMAT m_lffFormat;//相机参数
	FLOAT m_fFrameRate;//相机帧率
	ULONG m_ulCameraId;//相机ID
	BOOL m_bIsColor;//是否为彩色相机
	FLOAT m_fZoom;//预览缩放比例
	//Mat m_Luaptured_Imagem_C;//抓取的图像，内存中
	int m_Table_Index;//当前物镜转盘使用的物镜编号
//
//	BOOL m_MotorConnected;//建立连接？
//	BOOL m_Database_Connected;//数据库建立通信？
	BOOL m_canceled;
	CDC *pDC;
	CRect rectPicture;
//
//	CRect m_crRect;
//	ULONG m_ulWidth;
//	ULONG m_ulHeight;
//	ULONG m_ulMaxWidth;
//	ULONG m_ulMaxHeight;
//
//
//public:
//	//更新或设置相机参数
//	bool Update_Refresh_CameraInfo;	
//	//与相机建立通信
//	bool Connnect_Camera();		
//	//预览图像
//	bool Video_Preview();		
//	//从视频预览中抓图
//	bool Take_Video_Image(Mat &m_Luaptured_Imagem_C);
//	//更新相机参数
//	bool UpdateCameraInfo(HANDLE hCamera);
	//退出系统后，释放系统资源(断开连接)
	bool CleanUp();
	////将图像显示到picture控件上
	//void ShowMatImgToWnd(CWnd* pWnd, cv::Mat &img);
	bool InitHardwarePara(ALL_PAREMETER* allpara);
	//显示扫描的进度
	void ProgressShow(int x, int y);

//private:
//	//更新显扫描图像
//	LRESULT Updata_Display_Drawing(WPARAM wParam, LPARAM lParam);


public:
	virtual BOOL OnInitDialog(HWND m_pic);
	// 设置不同物镜下的相机参数
	bool SetCameraPara(int tablenum);

	bool CHardwareInit::GetCameraPara();

};

