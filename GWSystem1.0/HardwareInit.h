#pragma once
#include "GWSystem1.0Dlg.h"
//����OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "CvvImage.h"

//���
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

	HANDLE m_hCamera;//������
	BOOL m_bConnected;//�������ӣ�
	BOOL m_bPreviewing;//Ԥ����
	LUCAM_FRAME_FORMAT m_lffFormat;//�������
	FLOAT m_fFrameRate;//���֡��
	ULONG m_ulCameraId;//���ID
	BOOL m_bIsColor;//�Ƿ�Ϊ��ɫ���
	FLOAT m_fZoom;//Ԥ�����ű���
	//Mat m_Luaptured_Imagem_C;//ץȡ��ͼ���ڴ���
	int m_Table_Index;//��ǰ�ﾵת��ʹ�õ��ﾵ���
//
//	BOOL m_MotorConnected;//�������ӣ�
//	BOOL m_Database_Connected;//���ݿ⽨��ͨ�ţ�
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
//	//���»������������
//	bool Update_Refresh_CameraInfo;	
//	//���������ͨ��
//	bool Connnect_Camera();		
//	//Ԥ��ͼ��
//	bool Video_Preview();		
//	//����ƵԤ����ץͼ
//	bool Take_Video_Image(Mat &m_Luaptured_Imagem_C);
//	//�����������
//	bool UpdateCameraInfo(HANDLE hCamera);
	//�˳�ϵͳ���ͷ�ϵͳ��Դ(�Ͽ�����)
	bool CleanUp();
	////��ͼ����ʾ��picture�ؼ���
	//void ShowMatImgToWnd(CWnd* pWnd, cv::Mat &img);
	bool InitHardwarePara(ALL_PAREMETER* allpara);
	//��ʾɨ��Ľ���
	void ProgressShow(int x, int y);

//private:
//	//������ɨ��ͼ��
//	LRESULT Updata_Display_Drawing(WPARAM wParam, LPARAM lParam);


public:
	virtual BOOL OnInitDialog(HWND m_pic);
	// ���ò�ͬ�ﾵ�µ��������
	bool SetCameraPara(int tablenum);

	bool CHardwareInit::GetCameraPara();

};

