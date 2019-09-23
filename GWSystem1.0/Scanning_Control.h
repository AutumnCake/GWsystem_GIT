#pragma once

#include "Control_Dev.h"
#include <string>
#include <vector>
#include "Scanning_Capture.h"

//加载OpenCVAPI
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
	CString csDiretory;//图像存储的路径
	CString csFilename;//图像文件名称
	CString csExtension;//图像的格式.bmp; .raw等
	//INT iIndexSize;//图像的索引值，（这个索引值，不能在外部给，扫描类的内部会自动给定）
	//DOUBLE dInterval;
	//CEvent *pceMCExitEvent;
	//HANDLE hCamera;
	//LUCAM_FRAME_FORMAT lffFormat;
	LUCAM_CONVERSION lcConversion;//图像质量参数，主要包含两个
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
	int x_all;//X方向总点数
	int y_all;//Y方向总点数
	int x_now;
	int y_now;
};

enum Search_model{ Big_Step, Middle_Step, Little_Step };

class Scanning_Control :public Scanning_Capture
{
public:
	Scanning_Control();
	~Scanning_Control();
	/*----------------相机相关-------------------*/
	//相机参数
	HANDLE m_hCamera;                      //相机句柄
	BOOL m_bConnected;						//建立连接？
	BOOL m_bPreviewing;						//预览打开
	LUCAM_FRAME_FORMAT m_lffFormat;			//相机参数
	FLOAT m_fFrameRate;						//相机帧率
	ULONG m_ulCameraId;						//相机ID
	BOOL m_bIsColor;						//是否为彩色相机
	INT m_iSize;							//图像尺寸
	CRect m_crRect;
	ULONG m_ulWidth;
	ULONG m_ulHeight;
	ULONG m_ulMaxWidth;
	ULONG m_ulMaxHeight;
	FLOAT m_fZoom;							//预览缩放比例
	//Mat m_Luaptured_Imagem_C;				//抓取的图像（单张）
	MULTICAPTURE_PARAMS m_ScanningImage_Save_Paras;
	MULTICAPTURE_PARAMS m_FocusingImage_Save_Paras;

	//函数
	bool Connnect_Camera();					//与相机建立通信
	bool Get_Camera_Info();			     //在启动相机进行单次抓图或扫描前获取最新的相机参数
	bool Capture_Video_Image(Mat &Out_Video_Imagem);			//抓取视频图像
	bool Video_Preview(HWND Dlg_hWnd);							//图像预览
	bool Take_Video_Image(Mat &m_Luaptured_Imagem_C);
	bool Read_Raw_And_ConvertToRGB(CString FilePath, CString FileName, Mat &RGB_Image, 
		int pixelFormat = LUCAM_PF_8, int Convert_Demosaic = 2, int Convert_CorrectionMatrix = 3, int Image_Width = 2048, int Image_Height = 2048);//本地读取Raw文件，并转化为RGB

	/*----------------电机控制相关-------------------*/




	/*--------------------扫描相关--------------------*/

	//扫描运动参数
	int name;
	int Focus_Pic_Num;

	int m_point_i;
	double m_height_best;

	vector <int > m_Slide_ID;//储存各个聚焦点的焦距

	long m_MaxFocLen_Z_Pos;//当前最佳焦距Z轴绝对位置
	double m_MaxFocLen;//当前最佳焦距值
	int Focusing_Next_Step;//下一步聚焦步长
	int Focusing_Previous_Step;//上一步聚焦步长
	int Focusing_Search_Num;//当前聚焦模式下聚焦的次数，实时变量
	bool m_Z_Move_Arrived;//用于在Do_Search_Optimal_Focal_Distance内部Z轴运动时，表示当前Z轴是否运动完成
	int m_Searching_Model;//实时变化的搜索模式，分为3步，big，middle，little
	//bool Obtain_Optimal_Focal;//获得最佳焦距后为true(移动到了Scanning_Capture中)

	bool m_XVelocty_Model_Starting;			//速度模式运行中
	bool m_YVelocty_Model_Starting;
	bool m_MotorConnected;
	bool m_CameraConnected;
	bool m_StartCapture_Image;//启动抓图标志位
	bool m_EndCapture_Image;//关闭自动抓图标志


	//自动聚焦的临时变量
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
	vector<Mat> Focus_Img;//存放聚焦过程中的图像
	Mat Current_Pic;//当前抓取的图像
	int Best_Focus_Num;//在Focus_Img中最好的图像下标


	bool Get_Dic_F;
	bool Get_Dic_N;
	int Focus_Values_Max_Index;//清晰度vector中最大清晰度值的下表
	//聚焦函数
	double Calculate_Sharpness(int Score_Function, int height, Mat picture);
	double Calculate_Sharpness_Every_Point(int Focus_Level_100X = 0);
	//判断两副是否一样
	bool Img_Same(Mat Input_Img1, Mat Input_Img2);
	//直接保存图像
	bool Save_CVimg(MULTICAPTURE_PARAMS Capture_Save_Paras, Mat Img);
	/*
	input_image:表示输入图像
	Right_CAGroup 返回是否是一个合格的染色体团
	vector<vector<Point>> 的返回值是染色体团的区域范围（整个）
	*/
	vector<vector<Point>> Get_CAGroup_Region_100X(Mat input_image, bool &Right_CAGroup);
	//找平函数
	double Move_To_Focus(vector <int>a, vector <int>b, vector <int>c, int x, int y);
	int Do_Optimal_Focal_Distance_Searching_Model(int Search_Model);//自动聚焦

	int Do_Focusing_Model(int Focusing_Step);
	int Do_restting_Model();
	int Do_Scanning_Model();
	//单个位置逐个点聚焦
	//Focus_LevelSearch_Step_Z：运动的步长；Focus_Method：聚焦函数
	//BigORLittle：大区间或小区间，为true表示在大区间范围内逐个聚焦，false：则在小区间内寻找
	//NoCenter为false表示在整个玻片中心区域聚焦，为true表示非中心区域聚焦
	int Do_Find_Focus_Level_100X(int Focus_LevelSearch_Step_Z, int Focus_Method = MeanStd_Operator, 
		bool BigORLittle = true, bool Current_Point = false);//获取一个物镜下的参考焦平面
	int Do_Find_Focus_Level_100X_Climb(int Focus_LevelSearch_Step_Z, int Focus_Method, bool Current_Point);

	//Focus_Method,清晰度值计算方法
	int Calculate_Sharpness_OneByOne(int Z_OneByOne_Uper,int Focus_Method);//逐个点的聚焦函数
	int Do_Focusing_100X();
	int Pump_Oil_100X_Func();//100X下泵油函数，对整个扫描区域有限个点处泵油

	int Calculate_Scanning_Focal();//根据已经完成的聚焦值，计算
	bool TakeVeido_And_Save_Image_Raw(MULTICAPTURE_PARAMS Capture_Save_Paras, bool push2Vector=false);//抓图保存图像
	bool Save_To_Database();//保存当前的数据到数据库中
	bool Do_Search_Optimal_Focal_Length(bool mean_lalace, int Search_Model);//寻找最佳焦距值
	int Do_CB_Scanning();//微核图像扫描

	bool Set_Camera_Paras(unsigned int Objective_Num);
	//获取准确的10X下，染色体区域contours区域
	vector < vector<Point> > Get_CA_Pos_10X(Mat Intput_Image, vector<vector<Point>> contours,
												vector<unsigned int> &ROI_Size, bool Masked_Img = false);
	Mat Read_Img_From_Dish(int Current_Index, int Current_Solide_Num, MULTICAPTURE_PARAMS Img_Dish_Paras);
	static vector<Mat> Scanning_10X_img;

	//线程函数
	static UINT Handlehold_ThreadFunc(LPVOID Handlehold_ThreadArg);
	static bool Handlehold_Thread_Running;

	static UINT Get_CA_Pos_10X_ThreadFunc(LPVOID CA_10X_ThreadArg);
	static bool CA_Focus_Thread_Running;//表示10倍下正在进行染色体聚焦和定位，这个变量正常应该在定位线程中关闭

	UINT Get_CA_Pos_10X_Func(int Focus_CA_10X_Num);

	int DrawCurrentPosition(S_LOCATION currrentLocation);
	//扫描点位置坐标
	S_LOCATION s_Location;
	//手动摇杆零点位置
	int s_Zero_Pos_X;
	int s_Zero_Pos_Y;
	int s_Zero_Pos_Z;
	//临时变量
	long M_Z_Pos;
	bool Pass_TakeImg;//表示在100X下，当不满足染色体团的基本要求时，跳过当前图像

	


};

