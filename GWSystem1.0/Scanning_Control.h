#pragma once

#include "Control_Dev.h"
#include <string>
#include <vector>
#include "Scanning_Capture.h"

//����OpenCVAPI
#include<opencv2/opencv.hpp> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
//#include "CvvImage.h"
#include "lucamapi.h"
#include "lucamerr.h"

#define Laplacian_Operator 1
#define Thenengrad_Operator 2
#define MeanStd_Operator 3



typedef struct _MULTICAPTURE_PARAMS
{
	CString csDiretory;//ͼ��洢��·��
	CString csFilename;//ͼ���ļ�����
	CString csExtension;//ͼ��ĸ�ʽ.bmp; .raw��
	//INT iIndexSize;//ͼ�������ֵ�����������ֵ���������ⲿ����ɨ������ڲ����Զ�������
	//DOUBLE dInterval;
	//CEvent *pceMCExitEvent;
	//HANDLE hCamera;
	//LUCAM_FRAME_FORMAT lffFormat;
	LUCAM_CONVERSION lcConversion;//ͼ��������������Ҫ��������
	//int iThreadIndex;
	//ULONG ulCameraId;
	//BOOL bConnected;
	//BOOL bPreviewing;
	//LUCAM_SNAPSHOT lsSnapshot;
	//CWinThread *cwtSnapThread;
	//bool bSlowSnaps;
	//ULONG lSnapMax;
	//bool bVerbose;
}MULTICAPTURE_PARAMS, *PMULTICAPTURE_PARAMS;

typedef struct S_LOCATION{
	int x_all;//X�����ܵ���
	int y_all;//Y�����ܵ���
	int x_now;
	int y_now;
};

enum Search_model{ Big_Step, Middle_Step, Little_Step };

class Scanning_Control :public Scanning_Capture
{
public:
	Scanning_Control();
	~Scanning_Control();
	/*----------------������-------------------*/
	//�������
	HANDLE m_hCamera;                      //������
	BOOL m_bConnected;						//�������ӣ�
	BOOL m_bPreviewing;						//Ԥ����
	LUCAM_FRAME_FORMAT m_lffFormat;			//�������
	FLOAT m_fFrameRate;						//���֡��
	ULONG m_ulCameraId;						//���ID
	BOOL m_bIsColor;						//�Ƿ�Ϊ��ɫ���
	INT m_iSize;							//ͼ��ߴ�
	CRect m_crRect;
	ULONG m_ulWidth;
	ULONG m_ulHeight;
	ULONG m_ulMaxWidth;
	ULONG m_ulMaxHeight;
	FLOAT m_fZoom;							//Ԥ�����ű���
	//Mat m_Luaptured_Imagem_C;				//ץȡ��ͼ�񣨵��ţ�
	MULTICAPTURE_PARAMS m_ScanningImage_Save_Paras;
	MULTICAPTURE_PARAMS m_FocusingImage_Save_Paras;

	//����
	bool Connnect_Camera();					//���������ͨ��
	bool Get_Camera_Info();			     //������������е���ץͼ��ɨ��ǰ��ȡ���µ��������
	bool Capture_Video_Image(Mat &Out_Video_Imagem);			//ץȡ��Ƶͼ��
	bool Video_Preview(HWND Dlg_hWnd);							//ͼ��Ԥ��
	bool Take_Video_Image(Mat &m_Luaptured_Imagem_C);
	bool Read_Raw_And_ConvertToRGB(CString FilePath, CString FileName, Mat &RGB_Image, 
		int pixelFormat = LUCAM_PF_8, int Convert_Demosaic = 2, int Convert_CorrectionMatrix = 3, int Image_Width = 2048, int Image_Height = 2048);//���ض�ȡRaw�ļ�����ת��ΪRGB

	/*----------------����������-------------------*/




	/*--------------------ɨ�����--------------------*/

	//ɨ���˶�����
	int name;
	int Focus_Pic_Num;

	int m_point_i;
	double m_height_best;

	vector <int > m_Slide_ID;//��������۽���Ľ���

	long m_MaxFocLen_Z_Pos;//��ǰ��ѽ���Z�����λ��
	double m_MaxFocLen;//��ǰ��ѽ���ֵ
	int Focusing_Next_Step;//��һ���۽�����
	int Focusing_Previous_Step;//��һ���۽�����
	int Focusing_Search_Num;//��ǰ�۽�ģʽ�¾۽��Ĵ�����ʵʱ����
	bool m_Z_Move_Arrived;//������Do_Search_Optimal_Focal_Distance�ڲ�Z���˶�ʱ����ʾ��ǰZ���Ƿ��˶����
	int m_Searching_Model;//ʵʱ�仯������ģʽ����Ϊ3����big��middle��little
	//bool Obtain_Optimal_Focal;//�����ѽ����Ϊtrue(�ƶ�����Scanning_Capture��)

	bool m_XVelocty_Model_Starting;			//�ٶ�ģʽ������
	bool m_YVelocty_Model_Starting;
	bool m_MotorConnected;
	bool m_CameraConnected;
	bool m_StartCapture_Image;//����ץͼ��־λ
	bool m_EndCapture_Image;//�ر��Զ�ץͼ��־


	//�Զ��۽�����ʱ����
	bool m_Sobel_Wrong;
	Mat m_Score_Aero;
	bool m_Focusing_Equal_Scaning;
	int Last_MaxFocus_Pos;
	int Z_Upper_Limit;
	int Z_Low_Limit;
	int Z_Big_Step_Upper_Limit;
	int Z_Big_Step_Low_Limit;
	int Z_Big_Step_MaxFocus_Pos;
	vector<vector<Point>> contours_Right;

	vector<double> Focus_Values;
	vector<double> Focus_Values_Later;
	Mat Best_Img;
	Mat Best_Img_Later;
	double Max_Focus_Value;
	double Max_Focus_Value_Later;

	vector<int> Focus_Poss;
	vector<int> Last_Focus_X;
	vector<int> Last_Focus_Y;
	vector<int> Last_Focus_Z;
	vector<int> CA_Real_Pos_Y;
	vector<int> CA_Real_Pos_X;
	vector<bool> Scanning_Completed_Status;
	vector<Mat> Focus_Img;//��ž۽������е�ͼ��
	Mat Current_Pic;//��ǰץȡ��ͼ��
	int Best_Focus_Num;//��Focus_Img����õ�ͼ���±�


	bool Get_Dic_F;
	bool Get_Dic_N;
	int Focus_Values_Max_Index;//������vector�����������ֵ���±�
	//�۽�����
	double Calculate_Sharpness(int Score_Function, int height, Mat picture);
	double Calculate_Sharpness_Every_Point(int Focus_Level_100X = 0);
	//�ж������Ƿ�һ��
	bool Img_Same(Mat Input_Img1, Mat Input_Img2);
	//ֱ�ӱ���ͼ��
	bool Save_CVimg(MULTICAPTURE_PARAMS Capture_Save_Paras, Mat Img);
	/*
	input_image:��ʾ����ͼ��
	Right_CAGroup �����Ƿ���һ���ϸ��Ⱦɫ����
	vector<vector<Point>> �ķ���ֵ��Ⱦɫ���ŵ�����Χ��������
	*/
	vector<vector<Point>> Get_CAGroup_Region_100X(Mat input_image, bool &Right_CAGroup);
	//��ƽ����
	double Move_To_Focus(vector <int>a, vector <int>b, vector <int>c, int x, int y);
	int Do_Optimal_Focal_Distance_Searching_Model(int Search_Model);//�Զ��۽�

	int Do_Focusing_Model(int Focusing_Step);
	int Do_restting_Model();
	int Do_Scanning_Model();
	//����λ�������۽�
	//Focus_LevelSearch_Step_Z���˶��Ĳ�����Focus_Method���۽�����
	//BigORLittle���������С���䣬Ϊtrue��ʾ�ڴ����䷶Χ������۽���false������С������Ѱ��
	//NoCenterΪfalse��ʾ��������Ƭ��������۽���Ϊtrue��ʾ����������۽�
	int Do_Find_Focus_Level_100X(int Focus_LevelSearch_Step_Z, int Focus_Method = MeanStd_Operator, 
		bool BigORLittle = true, bool Current_Point = false);//��ȡһ���ﾵ�µĲο���ƽ��
	int Do_Find_Focus_Level_100X_Climb(int Focus_LevelSearch_Step_Z, int Focus_Method, bool Current_Point);

	//Focus_Method,������ֵ���㷽��
	int Calculate_Sharpness_OneByOne(int Z_OneByOne_Uper,int Focus_Method);//�����ľ۽�����
	int Do_Focusing_100X();
	int Pump_Oil_100X_Func();//100X�±��ͺ�����������ɨ���������޸��㴦����

	int Calculate_Scanning_Focal();//�����Ѿ���ɵľ۽�ֵ������
	bool TakeVeido_And_Save_Image_Raw(MULTICAPTURE_PARAMS Capture_Save_Paras, bool push2Vector=false);//ץͼ����ͼ��
	bool Save_To_Database();//���浱ǰ�����ݵ����ݿ���
	bool Do_Search_Optimal_Focal_Length(bool mean_lalace, int Search_Model);//Ѱ����ѽ���ֵ
	int Do_CB_Scanning();//΢��ͼ��ɨ��

	bool Set_Camera_Paras(unsigned int Objective_Num);
	//��ȡ׼ȷ��10X�£�Ⱦɫ������contours����
	vector < vector<Point> > Get_CA_Pos_10X(Mat Intput_Image, vector<vector<Point>> contours,
												vector<unsigned int> &ROI_Size, bool Masked_Img = false);
	Mat Read_Img_From_Dish(int Current_Index, int Current_Solide_Num, MULTICAPTURE_PARAMS Img_Dish_Paras);
	static vector<Mat> Scanning_10X_img;

	//�̺߳���
	static UINT Handlehold_ThreadFunc(LPVOID Handlehold_ThreadArg);
	static bool Handlehold_Thread_Running;

	static UINT Get_CA_Pos_10X_ThreadFunc(LPVOID CA_10X_ThreadArg);
	static bool CA_Focus_Thread_Running;//��ʾ10�������ڽ���Ⱦɫ��۽��Ͷ�λ�������������Ӧ���ڶ�λ�߳��йر�

	UINT Get_CA_Pos_10X_Func(int Focus_CA_10X_Num);

	int DrawCurrentPosition(S_LOCATION currrentLocation);
	//ɨ���λ������
	S_LOCATION s_Location;
	//�ֶ�ҡ�����λ��
	int s_Zero_Pos_X;
	int s_Zero_Pos_Y;
	int s_Zero_Pos_Z;
	//��ʱ����
	long M_Z_Pos;
	bool Pass_TakeImg;//��ʾ��100X�£���������Ⱦɫ���ŵĻ���Ҫ��ʱ��������ǰͼ��

	


};

