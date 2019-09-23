#include "stdafx.h"
#include "Scanning_Capture.h"

//用于线程同步的 临界区
CCriticalSection Scanning_Capture::critical_section;

Control_Dev *Scanning_Capture::s_Dev_Drivers = NULL;
Scanning_Para Scanning_Capture::s_Scanning_Para = {};// { 0, 0, true, 3, 0, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0, 0, 0, 0, 0};//{ cvPoint(0,0) }
Scanning_System_Para Scanning_Capture::s_Scanning_System_Para = { { -94000, 231000, 58000, -2550 }, "192.168.1.5", 5000, 
 0.05, { 2000, 300, 800 }, 25, 55, 3.4, { 0, 1250, 2150 * 2, 2150 * 3, 2150 * 4, 2150 * 5 },20,
 11235, 11235, -300, 500, 150000, 3500, 0.5, 10, 45000 ,-50000,10000,12170,12405,12220,400,3};
//手动摇杆，旧独立摇杆：14030,12176,12284，200
//电机复位偏移量（较中心位置）：{ -87000, 231000, 58000, -33950 }
//当用40X进行聚焦的时候，搜索步长为：{ 600, 300, 200 }，新算法中，只有单步长聚焦，只用到了最后一位补偿（200）
//当用10X进行聚焦的时候，搜索步长为：{ 2000, 1000, 800 }，新算法中，只有单步长聚焦，只用到了最后一位补偿（800）
//17, 35
User_Para Scanning_Capture::s_User_Para = { 8, 20, 40, 10, 20, 4, 6, true };//{ 8, 15, 25, 50, 80, 6, 9, true };//{ 8, 9, 9, 30, 30, 4, 4, true }
Scanning_10X_Para Scanning_Capture::s_Scanning_10X_Para = { 5, 8,15, 200 ,5};

Scanning_Capture::Scanning_Capture()
{

	 InitializeCriticalSection(critical_section);//对临界区 变量进行初始化
	 s_Scanning_Para = { 0, 0, true, 3, 0, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0, 0, 0, 0, 0 };//{ cvPoint(0,0) }

	 m_Current_Slide_Start_X = ((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)*s_Scanning_Para.Current_Slide_Num
		 + s_User_Para.Scanning_Aero_X / 2) * 10000;
	 m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
	 m_Scanning_Num_X = s_User_Para.Scanning_Num_X;
	 m_Scanning_Num_Y = s_User_Para.Scanning_Num_Y;
	 m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (s_User_Para.Scanning_Num_X - 1)) * 10000;
	 m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];

	 //聚焦用局部变量
	 m_Focusing_Num_X=2; //X轴方向上聚焦的点数
	 m_Focusing_Num_Y=2; //Y轴方向上聚焦的点数
	//注意这里X轴方向的扫描步长是根据扫描区域X轴方向的点数决定了，因此扫描点数最好要给出范围
	 m_Scanning_Step_Y = -(s_User_Para.Scanning_Aero_Y / (s_User_Para.Scanning_Num_Y - 1)) * 10000;

	 s_Scanning_Para.Focal_Length.resize(m_Focusing_Num_X*m_Focusing_Num_Y);
	 s_Scanning_Para.Scanning_Length.resize(s_User_Para.Scanning_Num_X*s_User_Para.Scanning_Num_Y);

	 /*
	 m_Focusing_Focus_Distance.resize(s_User_Para.Focusing_Num_Y);
	 for (int i = 0; i < s_User_Para.Focusing_Num_Y; i++)
		 m_Focusing_Focus_Distance[i].resize(s_User_Para.Focusing_Num_X);
	 m_Scanning_Focus_Distance.resize(s_User_Para.Scanning_Num_Y);
	 for (int i = 0; i < s_User_Para.Scanning_Num_Y; i++)
		 m_Scanning_Focus_Distance[i].resize(s_User_Para.Scanning_Num_X);


	//默认初始化两个存放焦距位移的二维向量数组
	
	 for (int i = 0; i < s_User_Para.Focusing_Num_Y; i++)
	{
		 for (int j = 0; j <s_User_Para.Focusing_Num_X; j++)
		{
			m_Focusing_Focus_Distance[i][j] = (i+1)*(j+1)*2000;
		}
	}
	
	 for (int i = 0; i < s_User_Para.Scanning_Num_Y; i++)
	{
		 for (int j = 0; j < s_User_Para.Scanning_Num_X; j++)
		{
			m_Scanning_Focus_Distance[i][j] = (i + 1)*(j + 1) * 1000;
		}
	}
	/****************************************************************************************/
	
	m_Respond_Right = false;
	m_Respond_Check_Num = 0;
	
}


Scanning_Capture::~Scanning_Capture()
{

}
/*
//设置下一步的扫描状态
void Scanning_Capture::Set_States(Scanning_States State)
{
	return State;
}

//寻找最佳焦距值
enum Scanning_States Scanning_Capture::Get_States()
{
	return m_Scanning_States;
}
*/

/*取某张玻片的已经获取的自动聚焦时的焦距值
1、如果找到最佳焦距，则返回true，否则返回 false

*/
int  Scanning_Capture::Do_Search_Optimal_Focal_Distance()
{

	//*******************首先进行抓图处理，*****************************//



	//************************//
	cout << "寻找最佳焦距值" << endl;

	/***************以下添加搜索最佳焦距的算法，例如爬山法算法*********************/
	//说明，如果找到了最佳焦距，那么直接将这个最佳焦距，赋值到自动聚焦相应的矩阵中去
	bool Obtain_Best_focal=false;//如果下面的判断中，得到当前的图像时最清新的图像，那么这个bool变量被赋值为真



	Obtain_Best_focal = true;


	/******************以下是获取了最佳焦距后，进行的系列操作，读取当前的位置，保存当前抓取的图像，***********************/
	if (Obtain_Best_focal)
	{

		long Z_Real_Pos;//当前最佳焦距值；
		s_Dev_Drivers->Dev_Read_Pos({ 3 }, Z_Real_Pos);
		s_Scanning_Para.Focal_Length[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//保存当前的焦距值
		s_Dev_Drivers->Dev_Read_Pos({ 1 }, Z_Real_Pos);
		s_Scanning_Para.X_Focus_Location[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//保存当前的坐标的X绝对值
		s_Dev_Drivers->Dev_Read_Pos({ 2 }, Z_Real_Pos);
		s_Scanning_Para.Y_Focus_Location[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//保存当前的坐标的Y绝对值


		//long gg = Z_Real_Pos[0];
		//int jj = 0;

		//****************将聚焦Z轴步长重新设置为粗步长*******************//
		m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
		//*********************************//

		return State_Finished;
	}
	else
	{
		//****************将聚焦Z轴下一步步长进行估计与预测 *******************//

		m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[2];

		//*********************************//

		return State_Freeze;
	}
		
}

//读取某张玻片的已经获取的自动聚焦时的焦距值
long Scanning_Capture::Read_Focal_Distance(int Next_Slide_Num)
{

	cout << "读取一张玻片的自动聚焦的焦距值"<<endl;


	return 0;
}
//保存某张玻片的获取的聚焦值,输入参数为True时表示保存自动聚焦时的焦距值，否则就是保存扫描点的焦距值
int  Scanning_Capture::Save_Focal_Distance(bool Focusing_Or_Scanning)
{
	if (Focusing_Or_Scanning)
	{
		cout << "保存一张玻片自动聚焦获得的焦距值" << endl;

	}
	
	else
	{
		cout << "保存一张玻片自动扫描点的焦距值" << endl;


	}
	return 0;
}

/*根据有限聚焦位移值，计算扫描点的焦距位移值**************/
void Scanning_Capture::Calculate_Focal_Distance(int Scan_Index_Row, int Scan_Index_Col)
{
	// 将扫描数组下的第I个下标索引 映射到已知的聚焦位移数组下的下标范围内
	double Ref_Index_Row = (double)(Scan_Index_Row * (m_Focusing_Num_Y - 1)) / (m_Focusing_Num_Y - 1);
	double Ref_Index_Col = (double)(Scan_Index_Col * (m_Focusing_Num_X - 1)) / (m_Focusing_Num_X - 1);
	int Focus_Index_Row_Low, Focus_Index_Row_High;//定义聚焦值数组下，X坐标的，映射下标附近的前后实际坐标值
	int Focus_Index_Col_Low, Focus_Index_Col_High;//定义聚焦值数组下，Y坐标的，映射下标附近的前后实际坐标值
	//获取包围的四个点的下标值如下
	Focus_Index_Row_Low = Ref_Index_Row;
	if (Focus_Index_Row_Low == m_Focusing_Num_Y - 1)
		Focus_Index_Row_High = Ref_Index_Row;
	else
		Focus_Index_Row_High = Ref_Index_Row + 1;

	Focus_Index_Col_Low = Ref_Index_Col;
	if (Focus_Index_Col_Low == m_Focusing_Num_X - 1)
		Focus_Index_Col_High = Ref_Index_Col;
	else
		Focus_Index_Col_High = Ref_Index_Col + 1;

	/*可以得到目标点（s_Scanning_Para.Current_Scanning_I_X，s_Scanning_Para.Current_Scanning_J_Y）
	正好被下标为（Focus_Index_X_Low，Focus_Index_Y_Low）；（Focus_Index_X_Low，Focus_Index_Y_High）；
				（Focus_Index_X_High，Focus_Index_Y_Low）；（Focus_Index_X_High，Focus_Index_Y_High）；
				的四个点包围；
	*/
	//然后计算这四个点对应的实际坐标
	double Scan_Space_X = s_User_Para.Scanning_Aero_X / s_User_Para.Scanning_Num_X; //扫描点间X轴方向间隔
	double Scan_Space_Y = s_User_Para.Scanning_Aero_Y / s_User_Para.Scanning_Num_Y; //扫描点间X轴方向间隔
	double Focal_Space_X = s_User_Para.Scanning_Aero_X / m_Focusing_Num_X; //聚焦点间X轴方向间隔
	double Focal_Space_Y = s_User_Para.Scanning_Aero_Y / m_Focusing_Num_Y; //聚焦点间X轴方向间隔
	/*建立坐标系，包围的四个点的实际坐标为（X,Y）平面上，（0，0）；（0，Focal_Space_Y）
	（Focal_Space_X，0）；（Focal_Space_X，Focal_Space_Y）；
	*/
	//目标点在新坐标系中的实际坐标为
	double Cardiate_Coords_X = Scan_Index_Col* Scan_Space_X - Focus_Index_Col_Low * Focal_Space_X;
	double Cardiate_Coords_Y = Scan_Index_Row* Scan_Space_Y - Focus_Index_Row_Low * Focal_Space_Y;
	/*
	//判断目标点是否处于其中一点的领域范围内，如果处于领域范围内则用该已知点的焦距位移值作为目标点的焦距位移值
	if ((Cardiate_Coords_X <s_Scanning_System_Para.Focal_Domain_Radius)//如果在坐标第一个点的领域正方形范围内，就直接用第一点焦距位移值
		
		& (Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << " 第一    计算扫描焦距 " << m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low] << endl;
		return;
	}

	if ((Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//同理在第二点范围内
		& (Focal_Space_Y - Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  第二     计算扫描焦距" << m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High] << endl;
		return;
	}

	if ((Focal_Space_X - Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//同理在第三点范围内
		& (Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << " 第三     计算扫描焦距 " << m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low] << endl;
		return;
	}

	if ((Focal_Space_X - Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//同理在第四点范围内
		& (Focal_Space_Y - Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  第四     计算扫描焦距" << m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High] << endl;
		return;
	}

	//如果目标点不处于四个包围的已知点的任何一个点的领域正方形范围之内，则基于这四个点计算目标点的焦距位移值

	//先求目标点在第一个点和第二个点组成的面上的投影点的焦距位移值
	long Shadow_1 = (m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High] - m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low])
		*Cardiate_Coords_Y / Focal_Space_Y + m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low];
	//再求目标点在第三个点和第四个点组成的面上的投影点的焦距位移值
	long Shadow_2 = (m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High] - m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low])
		*Cardiate_Coords_Y / Focal_Space_Y + m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low];
	//最后求目标点的聚焦位移值大小
	m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] = (Shadow_2 - Shadow_1)*Cardiate_Coords_X / Focal_Space_X + Shadow_1;
	cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  第五     计算扫描焦距" << endl;
	*/

}

/*扫描初始化函数

Control_Dev Dev_Drivers：一个运动控制对象
ScanningPara：输入的参数是扫描参数结构体
bool Communication_Successed：与下位机的通信是否建立成功，通常在打开软件时，就会建立一个自动建立通信，如果没有建立通信，这个
                              函数会自动尝试建立通信。
 返回值问题：如果网关通信失败，则返回为-1；通信成功返回为0；

*/
int Scanning_Capture::Do_Scanning_intial()
{
	m_Respond_Right = false;
	m_Respond_Check_Num = 0;
	//long m_X_Moveto = 0;//当前X轴绝对运动位移
	//long m_Y_Moveto = 0;
	//long m_Z_Moveto = 0;
	//long m_T_Moveto = 0;
	//long m_5_Moveto = 0;

	double Real_View_Field_Higth = 0;
	double Real_View_Field_Width = 0;
	switch (s_Scanning_Para.Objective_Num)
	{
	case Objective_4X:
		Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X*2.5;
		Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X*2.5;
		break;
	case Objective_10X:
		Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X;
		Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X;
		break;
	case Objective_20X:
		Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X/2;
		Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X/2;
		break;
	case Objective_40X:
		Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X/4;
		Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X/4;
		break;
	case Objective_100X:
		Real_View_Field_Higth = s_Scanning_System_Para.View_Field_Higth_10X/10;
		Real_View_Field_Width = s_Scanning_System_Para.View_Field_Width_10X/10;
		break;
	default :
		break;
	}

	//首先基于当前物镜类型，以及扫描区域大小
	//系统参数中已经包含在10X下一个视野的宽度脉冲数
	s_User_Para.Scanning_Num_Y = s_User_Para.Scanning_Aero_Y * s_Scanning_System_Para.Pulse_ratio / Real_View_Field_Higth + 1;
	s_User_Para.Scanning_Num_X = s_User_Para.Scanning_Aero_X * s_Scanning_System_Para.Pulse_ratio / Real_View_Field_Width + 1;


	m_Respond_Check_Num = 0;
	if (s_Scanning_Para.Forcuing_OR_Scanning)//如果是自动聚焦操作
	{
		m_Scanning_Num_X = m_Focusing_Num_X;
		m_Scanning_Num_Y = m_Focusing_Num_X;
	    m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
		m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (m_Scanning_Num_X - 1)) * 10000;
		//注意这里X轴方向的扫描步长是根据扫描区域X轴方向的点数决定了，因此扫描点数最好要给出范围
		m_Scanning_Step_Y = -(s_User_Para.Scanning_Aero_Y / (m_Scanning_Num_Y - 1)) * 10000;
	}
	else
	{	//实际上，m_Scanning_Step_X在扫描里面是没用到的，这个表示的是聚焦时用。
		m_Scanning_Num_X = s_User_Para.Scanning_Num_X;
		m_Scanning_Num_Y = s_User_Para.Scanning_Num_Y;
    	m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (m_Scanning_Num_X - 1)) * 10000;
		//注意这里X轴方向的扫描步长是根据扫描区域X轴方向的点数决定了，因此扫描点数最好要给出范围
		m_Scanning_Step_Y = -(s_User_Para.Scanning_Aero_Y / (m_Scanning_Num_Y - 1)) * 10000;
		s_Scanning_Para.X_Scanning_Location.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
		s_Scanning_Para.Y_Scanning_Location.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
		s_Scanning_Para.Scanning_Length.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
	}

	
	//重新检查
	//m_Current_Slide_Start_X = (s_User_Para.Scanning_Aero_X / 2) * 10000;
	//m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;

	/*
	m_Focusing_Focus_Distance.resize(s_User_Para.Focusing_Num_Y);
	for (int i = 0; i < s_User_Para.Focusing_Num_Y; i++)
		m_Focusing_Focus_Distance[i].resize(s_User_Para.Focusing_Num_X);
	m_Scanning_Focus_Distance.resize(s_User_Para.Scanning_Num_Y);
	for (int i = 0; i < s_User_Para.Scanning_Num_Y; i++)
		m_Scanning_Focus_Distance[i].resize(s_User_Para.Scanning_Num_X);
		*/
	if (!s_Dev_Drivers->Communication_Success)
	{
		int err = s_Dev_Drivers->CANET_Init(s_Scanning_System_Para.CANET_ID, s_Scanning_System_Para.CANET_Port_Num);  //建立通信
		if (err)
		{
			MessageBox(NULL, TEXT("与CAN网关建立通信失败!！！"), TEXT("错误提示"), 0);
			return Scanning_Finished;
		}
		else
		{
			//给出下一步的运动状态，进行复位操作
			return State_Finished;
		}

	}
	else
	{
		//给出下一步的运动状态，进行复位操作
		return State_Finished;
	}
	//对相关变量进行初始化操作



	/*以下为测试代码***********************************************************************************************/
	/*
	s_Dev_Drivers.Dev_heart_beat(1, FALSE);//关闭报文
	s_Dev_Drivers.Dev_heart_beat(2, FALSE);//关闭报文
	s_Dev_Drivers.Dev_heart_beat(3, FALSE);//关闭报文
	s_Dev_Drivers.Dev_ENA(3, 1);//3号使能
	s_Dev_Drivers.Dev_Set_Zero_Postion(3);
	vector<long> Real_Pos = {12};
	s_Dev_Drivers.Dev_Read_Pos({ 3 }, Real_Pos);
	long postion3 = Real_Pos[0];
	cout <<Real_Pos[0] << endl;
	s_Dev_Drivers.Dev_Single_Mov(3, 20000, 5, true, true);
	s_Dev_Drivers.Dev_Read_Pos({ 3 }, Real_Pos);
	postion3 = Real_Pos[0];
	cout << Real_Pos[0] << endl;
	/*以上为测试代码*********************************************************************************************/

}
/*复位函数
*/
int Scanning_Capture::Do_Resetting(int Motor_ID,bool fast_slow)
{
	s_Dev_Drivers->Dev_clear_alarm(1);//清楚警告
	s_Dev_Drivers->Dev_clear_alarm(2);//清楚警告
	s_Dev_Drivers->Dev_clear_alarm(3);//清楚警告

	s_Dev_Drivers->Dev_clear_alarm(4);//清楚警告
	s_Dev_Drivers->Dev_clear_alarm(5);//清楚警告

	s_Dev_Drivers->Dev_heart_beat(1, FALSE);//关闭报文
	s_Dev_Drivers->Dev_heart_beat(2, FALSE);//关闭报文
	s_Dev_Drivers->Dev_heart_beat(3, FALSE);//关闭报文
	s_Dev_Drivers->Dev_heart_beat(4, FALSE);//关闭报文
	s_Dev_Drivers->Dev_heart_beat(5, FALSE);//关闭报文
	s_Dev_Drivers->Dev_ENA(1, 1);//1号使能
	s_Dev_Drivers->Dev_ENA(2, 1);//2号使能
	s_Dev_Drivers->Dev_ENA(3, 1);//3号使能
	s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);//3号使能
	s_Dev_Drivers->Dev_PUSIRobot_ENA(5, 0);//3号使能
	//s_Dev_Drivers->Dev_ENA(4, 0);//3号使能
	//s_Dev_Drivers->Dev_ENA(5, 1);//3号使能

	long lk;
	vector<long>Home_offset;
	switch (Motor_ID)
	{
	case 1:
	case 2:
		Home_offset.push_back(s_Scanning_System_Para.Offset_arr[0]);
		cout << "Offset_arr[0]" << s_Scanning_System_Para.Offset_arr[0] << endl;
		cout << "Offset_arr[1]" << s_Scanning_System_Para.Offset_arr[1] << endl;
		cout << "Offset_arr[2]" << s_Scanning_System_Para.Offset_arr[2] << endl;
		cout << "Offset_arr[3]" << s_Scanning_System_Para.Offset_arr[3] << endl;
		Home_offset.push_back(s_Scanning_System_Para.Offset_arr[1]);
		
		s_Dev_Drivers->Dev_Home({ 1, 2 }, Home_offset);//进行复位，这里只有1号轴和2号轴的复位操作
		break;
	case 3:
		Home_offset.push_back(s_Scanning_System_Para.Offset_arr[2]);
		 lk= Home_offset[0];
		 if (fast_slow)
			 s_Dev_Drivers->Dev_Home({ 3 }, Home_offset);
		 else
		   s_Dev_Drivers->Dev_Home({ 3 }, Home_offset,true,false);
		break;
	case 4:
		Home_offset.push_back(s_Scanning_System_Para.Offset_arr[3]);
		s_Dev_Drivers->Dev_Home({ 4 }, Home_offset);
		break;
	default:
		break;
	}
	

	return State_Finished;
	
}
/*查询复位完成函数
*/
int Scanning_Capture::Do_Resetting_Finished(int Motor_ID)
{
	bool Dap_01 = FALSE;
	bool Dap_02 = FALSE;
	vector<bool> status_arr = { 0, 0 };

	switch (Motor_ID)
	{
	case 1:
	case 2:
		s_Dev_Drivers->Dev_Read_Status({ 1, 2 }, status_arr, 0);
		break;
	case 3:
		Dap_02 = true;
		s_Dev_Drivers->Dev_Read_Status({ 3 }, status_arr, 0);
		break;
	case 4:
		Dap_02 = true;
		s_Dev_Drivers->Dev_PUSIRobot_Read_Status({ 4 }, status_arr, 1);
		break;
	default:
		break;
	}

	Dap_01 = (Dap_01 || (status_arr[0]));//20表示回零完成，4表示相对运动到位
	Dap_02 = (Dap_02 || (status_arr[1]));//20表示回零完成，4表示相对运动到位
	if (Dap_01 & Dap_02)
	{
		if (Motor_ID == 4) s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);
		return State_Finished;//检测到复位完成，给出下一步的状态，进入判断分支，关键分支
	}
	else
	{
		return State_Freeze;//检测到复位没有完成，给出下一步的状态，进行复位完成查询
	}
	
}

/*运动到一张玻片的初始扫描点，或是运动到历史扫描点,运动完成；
void Do_Slide_Start_Postion_Finished()
*/

int Scanning_Capture::Do_Slide_Start_Postion()
{
	if (!s_User_Para.NewStart_OR_BreakPoint)
	{//如果是断点开始，则X\Y\Z的初始运动位移，是在启动前就给定的
		s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
		s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);
		s_User_Para.NewStart_OR_BreakPoint = true;
		return Move_To_Slide_Start_Postion_Finished;//转到检测完成的分支
	}
	if (s_Scanning_Para.Current_Slide_Num == 0)//运动到第一张玻片的起点扫描位置
	{
		//m_Current_Slide_Start_X = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		//m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		m_X_Moveto = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
		s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);

		if (!s_Scanning_Para.Forcuing_OR_Scanning)
			Read_Focal_Distance(s_Scanning_Para.Current_Slide_Num);//如果是自动扫描，则先读取下一张玻片的一直焦距值

	}
	else//运动到底I张玻片的七点扫描位置
	{
		/*
		m_Current_Slide_Start_X = -((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)
			* s_Scanning_Para.Current_Slide_Num - s_User_Para.Scanning_Aero_X / 2) * 10000;
		m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		*/
		m_X_Moveto = -((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)
			* s_Scanning_Para.Current_Slide_Num - s_User_Para.Scanning_Aero_X / 2) * 10000;
		m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
		s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);
	}

	return Move_To_Slide_Start_Postion_Finished;//转到检测完成的分支

}

/*查询运动到一张玻片的扫描起点
*/
int Scanning_Capture::Do_Slide_Start_Postion_Finished()
{
	long X_Real_Pos, Y_Real_Pos; 
	bool X_Read = s_Dev_Drivers->Dev_Read_Pos(1, X_Real_Pos);
	bool Y_Read = s_Dev_Drivers->Dev_Read_Pos(2, Y_Real_Pos);
	//bool X_Read = s_Dev_Drivers->Dev_Read_Pos_Theory(1, X_Real_Pos);
	//bool Y_Read = s_Dev_Drivers->Dev_Read_Pos_Theory(2, Y_Real_Pos);
	if (X_Read && Y_Read)
	{
		//m_Current_Slide_Start_X  m_Current_Slide_Start_Y
		bool X_Arrived = (X_Real_Pos >= (m_X_Moveto - s_Scanning_System_Para.Move_Deviation)) &&
			(X_Real_Pos <= (m_X_Moveto + s_Scanning_System_Para.Move_Deviation));
		bool Y_Arrived = (Y_Real_Pos >=(m_Y_Moveto - s_Scanning_System_Para.Move_Deviation)) &&
			(Y_Real_Pos <= (m_Y_Moveto + s_Scanning_System_Para.Move_Deviation));
		if (Y_Arrived && X_Arrived)
			return State_Finished;
		else
			return State_Freeze;
	}
	else
		return State_Freeze;
	/*
	bool Dap_01 = FALSE;
	bool Dap_02 = FALSE;
	vector<bool> status_arr = { 0, 0 };
	s_Dev_Drivers->Dev_Read_Status({ 1, 2 }, status_arr, 1);
	Dap_01 = (Dap_01 || (status_arr[0]));//20表示回零完成，4表示相对运动到位
	Dap_02 = (Dap_02 || (status_arr[1]));//20表示回零完成，4表示相对运动到位
	if (Dap_01 & Dap_02)
	{
		return State_Finished;

	}
	else
	{
		return State_Freeze;//检测是否到位，没有到位重新进入本分支，继续检测
	}*/
}

/*判断扫描是否完成，函数
*/
int Scanning_Capture::Do_Is_Scanning_Finished()
{

	//int Num_X, Num_Y;

	s_Scanning_Para.Current_Scanning_Num_Y = s_Scanning_Para.Current_Point_Num / m_Scanning_Num_X;
	s_Scanning_Para.Current_Scanning_Num_X = s_Scanning_Para.Current_Point_Num % m_Scanning_Num_X;

		if (s_Scanning_Para.Current_Scanning_Num_Y < m_Scanning_Num_Y)//判断当前的玻片是否扫描完成
		{
			if (s_Scanning_Para.Current_Scanning_Num_X == 0)//判断当前X轴方向扫描是否完成
			{
				m_Scanning_Step_X = -m_Scanning_Step_X;
				return Y_Stepping;////X轴方向扫描已经完成，开始步进一个Y方向位移
			}
			else
			{				
				return X_Stepping;//X轴方向没有完成，继续转到X轴方向扫描上去
			}
		}
		
		else//当前玻片扫描完毕
		{
			/*此处添加保存和读取聚焦值的函数*/
			Save_Focal_Distance(s_Scanning_Para.Forcuing_OR_Scanning);//保存自动聚焦或自动扫面点的聚焦值，整张玻片保存
			/*******************************/
			return Scanning_Finished;//自动聚焦时只会聚焦一张玻片
	
		}

}

/*X轴步进运动
*/

int Scanning_Capture::Do_Stepping(int Motor_ID,long Motor_Postion)
{


	vector<UINT> Dev_id = { 0 };
	Dev_id[0] = Motor_ID;
	vector<bool> status_arr = { 0 };
	s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
	while (!status_arr[0])
	{
		s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
	}


	long Current_Pos;
    
	switch (Motor_ID)
	{
	case 1:
		//m_X_Moveto = Current_Pos + Motor_Postion;
		//m_X_Moveto = m_X_Moveto + Motor_Postion;
		s_Dev_Drivers->Dev_Single_Mov(Motor_ID, Motor_Postion, 10, true);//10
		break;
	case 2:
		//m_Y_Moveto = Current_Pos + Motor_Postion;
		//m_Y_Moveto = m_Y_Moveto + Motor_Postion;
		s_Dev_Drivers->Dev_Single_Mov(Motor_ID, Motor_Postion, 10, true);
		break;
	case 3:
			m_Z_Moveto = Motor_Postion;

			s_Dev_Drivers->Dev_Single_Mov(Motor_ID, m_Z_Moveto, 10, true);
		break;
	case 4:
		m_T_Moveto = Motor_Postion;
		s_Dev_Drivers->Dev_Single_Mov(Motor_ID, m_T_Moveto, 5, true);
		break;
	case 5:
		//if (!s_Dev_Drivers->Dev_Read_Pos(Motor_ID, Current_Pos))
		//	return State_Freeze;
		//m_5_Moveto = Current_Pos + Motor_Postion;
		//s_Dev_Drivers->Dev_Set_Zero_Postion(5);//首先进行清零
		//Sleep(100);
		m_5_Moveto =  Motor_Postion;
		s_Dev_Drivers->Dev_Single_Mov(Motor_ID, m_5_Moveto, s_Scanning_System_Para.Pump_Oil_Vel, false);
		break;
	default:
		break;
	}
		
	return State_Finished;
	
}
/*X轴步进运动,检测是否完成运动
返回值为：State_Freeze：表示当前没查询到运动完成，
	State_Finished：表示查询到运动完成
	Stepping：表示查询错误，建议重新发送之前的运动指令，再重新查询
*/
int Scanning_Capture::Do_Stepping_Finished(int Motor_ID,long Move_to)
{

	if (m_Respond_Check_Num < 200)//进行驱动器返回报文
	{
		vector<UINT> Dev_id = { 0 };
		Dev_id[0] = Motor_ID;
		vector<bool> status_arr = { 0 };
		s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);
		if (!status_arr[0])
		{
			m_Respond_Check_Num++;
			return State_Freeze;
		}
		long Current_Pos = 0;
		bool Get_Pos_Move = s_Dev_Drivers->Dev_Read_Pos(Motor_ID, Current_Pos);
		if (!Get_Pos_Move)
			return State_Freeze;
		if (Motor_ID == 1)
			int jjjj = 0;
		bool Move_In_Deviation = (Current_Pos >=(Move_to - s_Scanning_System_Para.Move_Deviation)) &&
			(Current_Pos <= (Move_to + s_Scanning_System_Para.Move_Deviation));
		Sleep(10);
		Get_Pos_Move = s_Dev_Drivers->Dev_Read_Pos(Motor_ID, Current_Pos);
		if (!Get_Pos_Move)
			return State_Freeze;
		if (Motor_ID == 1)
			int jjjj = 0;
		bool Move_In_Deviation1 = (Current_Pos >= (Move_to - s_Scanning_System_Para.Move_Deviation)) &&
			(Current_Pos <= (Move_to + s_Scanning_System_Para.Move_Deviation));

		//s_Dev_Drivers->Dev_Read_Status(Dev_id, status_arr, 1);

		if (Move_In_Deviation && status_arr[0] && Move_In_Deviation1)
		{
			m_Respond_Right = false;
			if (s_Scanning_Para.Forcuing_OR_Scanning)
				m_Respond_Check_Num = 0;
			s_Dev_Drivers->Status_Ref = false;//******添加的运动完成标志位//
			return State_Finished;
		}
		else
		{
			if (status_arr[0] && (m_Respond_Check_Num > 20))
			{
				int Dev_Status = -1;
				while (Dev_Status == -1)
					Dev_Status = s_Dev_Drivers->Dev_Read_InputIO_Status(3);
				if (Dev_Status != Motor_Move_In_Range)
				{
					if (Motor_ID == 1) m_X_Moveto = Current_Pos;
					if (Motor_ID == 2) m_Y_Moveto = Current_Pos;
					if (Motor_ID == 3) m_Z_Moveto = Current_Pos;
					Obtain_Optimal_Focal = TRUE;
					return State_Finished;
				}
			}
			m_Respond_Check_Num++;
			return State_Freeze;
		}


	}
	else//如果超过了报文查询次数，还是没有查到上次指令发送后驱动器的响应报文，那么则重新执行上次指令
	{
		m_Respond_Right = false;
		m_Respond_Check_Num = 0;
		//重新建立连接
		//s_Dev_Drivers->CANET_Init("192.168.1.5", 5000);
		//Sleep(20);

		return Stepping;
		//cout << "重复东一次"<<endl;
	}

}

/*Y轴步进运动*/
/*
int Scanning_Capture::Do_Y_Stepping()
{
	s_Dev_Drivers->Dev_Single_Mov(2, m_Scanning_Step_Y);//
	return Y_Stepping_Finished;
}
/*Y轴步进运动,检测是否完成运动
*/
/*
int Scanning_Capture::Do_Y_Stepping_Finished()
{
	bool Dap_02= FALSE;
	vector<bool> status_arr = { 0, 0 };
	s_Dev_Drivers->Dev_Read_Status({ 2 }, status_arr, 1);
	Dap_02 = (Dap_02 || (status_arr[0]));//20表示回零完成，4表示相对运动到位
	if (Dap_02)
	{
		s_Scanning_Para.Current_Scanning_I_X = 0;
		m_Scanning_Step_X = -m_Scanning_Step_X;//更改X轴的运动位移和方向
		return State_Finished;

	}
	else
	{
		return Y_Stepping_Finished;
	}
}

/*Z轴步进运动*/
/*
int Scanning_Capture::Do_Z_Stepping()
{
	if (s_Scanning_Para.Forcuing_OR_Scanning)
	{
		///*--------------------一以下为自动聚焦时的Z轴运动------------------------------------
		//以下Z轴采用相对位置运动，来实现逐步的自动对焦，这里需要注意，需要在Search_Optimal_Focal_Distance()函数中，预先指定步进的方向

		s_Dev_Drivers->Dev_Single_Mov(3, s_Scanning_Para.Focusing_Step_Z, 5, false, true);
		///*-----------------------------------------------------------------------------------------------
	}
	else
	{
		///*--------------------一以下为自动扫描的Z轴运动-------------------------------------//
		//以下开始计算Z轴需要运动的位移大小，以下是对扫描时的Z轴运动程序，这里是绝对运动，采用
		int Scanning_Dic = pow(-1, s_Scanning_Para.Current_Scanning_J_Y);//判断反向扫描状态，这时数组下标是递减的
		int j;
		if (Scanning_Dic<0)
			j = s_Scanning_Para.Scanning_Num_X + s_Scanning_Para.Current_Scanning_I_X * Scanning_Dic - 1;
		else
			j = s_Scanning_Para.Current_Scanning_I_X * Scanning_Dic;

		Calculate_Focal_Distance(s_Scanning_Para.Current_Scanning_J_Y, j);//计算运动到该点的焦距值

		s_Dev_Drivers->Dev_Single_Mov(3, m_Scanning_Focus_Distance[s_Scanning_Para.Current_Scanning_J_Y][j], 5, true, true);
		///*-----------------------------------------------------------------------------------------------//
	}

	return Z_Stepping_Finished;

}
/*Z轴步进运动完成*/
/*
int Scanning_Capture::Do_Z_Stepping_Finished()
{
	bool Dap_03 = FALSE;
	vector<bool> status_arr = { 0 };
	s_Dev_Drivers->Dev_Read_Status({ 3 }, status_arr, 1);
	Dap_03 = (Dap_03 || (status_arr[0]));//20表示回零完成，4表示相对运动到位
	if (Dap_03)
	{
		return Capture_Image;
	}
	else
	{
		return Z_Stepping_Finished;
	}
}


/*********************************************************************************************************************************/
/*抓取图像*/

int Scanning_Capture::Do_Capture_Image()
{

	cout << "当前拍摄的图片是第 " << s_Scanning_Para.Current_Slide_Num << " 玻片，第 " << s_Scanning_Para.Current_Point_Num % s_User_Para.Scanning_Num_X
		<< " 列, 第 " << s_Scanning_Para.Current_Point_Num / s_User_Para.Scanning_Num_X << " 行的图像！" << endl;

	/* 请在此处添加需要抓图的代码*/
	
	
	
	
	
	
	/*+++++++++++++++++++++++++++++++*/

	return State_Finished;//拍摄、保存


}

/*保存图像*/
int Scanning_Capture::Do_Save_Image()
{

	cout << "保存当前图片 " << endl;

	/* 请在此处添加需要抓图和保存图片的代码
	/*********对于自动聚焦模式下，一般只有在获取到最佳图像时才对图像进行保存；*************





	*/

	if (s_Scanning_Para.Forcuing_OR_Scanning)   /*****如果是自动聚焦，则进行一下操作，******/
	{


			/* 这里应该增加上下限，Z 运动的范围考察 */
			return Z_Stepping;


	}
	else    //************如果是自动扫描，则进行一下操作**********//
	{

		return Is_Scanning_Finished;//拍摄、保存完成，转移到运动判断分支
	}

}


/***************************************
Move_To_Object:运动到对应的物镜
int Object_Num:目标物镜编号，0号表示4倍镜
int Dir：运动的方向；
*/
int Scanning_Capture::Do_Move_To_Object(int Object_Num, int Dir, int Speed)
{

	switch (Object_Num)
	{
	case Objective_4X:
		s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, 0, Speed, true);
		break;
	case Objective_10X:
		if (Dir>0)
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, s_Scanning_System_Para.Nosepiece_Abs_Position[1], Speed, true);
		else
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, -1 * s_Scanning_System_Para.Nosepiece_Abs_Position[1], Speed, true);
		break;
	case Objective_20X:
		if (Dir>0)
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, s_Scanning_System_Para.Nosepiece_Abs_Position[2], Speed, true);
		else
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, -1 * s_Scanning_System_Para.Nosepiece_Abs_Position[5], Speed, true);
		break;
	case Objective_40X:
		if (Dir>0)
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, s_Scanning_System_Para.Nosepiece_Abs_Position[3], Speed, true);
		else
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, -1 * s_Scanning_System_Para.Nosepiece_Abs_Position[4], Speed, true);
		break;
	case Objective_60X:
		if (Dir>0)
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, s_Scanning_System_Para.Nosepiece_Abs_Position[4], Speed, true);
		else
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, -1 * s_Scanning_System_Para.Nosepiece_Abs_Position[3], Speed, true);
		break;
	case Objective_100X:
		if (Dir>0)
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, s_Scanning_System_Para.Nosepiece_Abs_Position[5], Speed, true);
		else
			s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(4, -1 * s_Scanning_System_Para.Nosepiece_Abs_Position[2], Speed, true);
		break;
	default:
		break;
	}
	return Move_To_Object_Fininshed;
}
/***************************************
Move_To_Object_Fininshed:运动到对应的物镜

*/
int Scanning_Capture::Do_Move_To_Object_Fininshed()
{
	vector<bool> status_arr = { 0};
	s_Dev_Drivers->Dev_PUSIRobot_Read_Status({ 4 }, status_arr, 1);
	if (status_arr[0])
	{
		Sleep(200);
		s_Dev_Drivers->Dev_PUSIRobot_ENA(4,false);
		return State_Finished;

	}
	else
	{
		return State_Freeze;
	}
}

/********************************************************************************************************************************/

/*扫描结束
*/
bool Scanning_Capture::Do_Scanning_Finished()
{
	s_Dev_Drivers->Dev_ENA(1,0);//1号使能
	s_Dev_Drivers->Dev_ENA(2, 0);//2号使能
	s_Dev_Drivers->Dev_ENA(3, 0);//3号使能
	s_Dev_Drivers->Dev_ENA(4, 0);//2号使能
	s_Dev_Drivers->Dev_ENA(5, 0);//3号使能
	return TRUE;
}

int Scanning_Capture::Do_Focusing_Model()
{
	Do_restting_Model();//现复位

	int Back_Num;
	int states = Move_To_Object;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******运动到本次扫描/聚焦需要用到的物镜******//
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
				states = Move_To_Slide_Start_Postion;
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Move_To_Object_Fininshed;
			break;
			//******运动到扫描的第一个点位置******//
		case Move_To_Slide_Start_Postion:
			states = Do_Slide_Start_Postion();
			break;
		case Move_To_Slide_Start_Postion_Finished:
			Back_Num = Do_Slide_Start_Postion_Finished();
			if (Back_Num == State_Finished)
				states = Search_Optimal_Focal_Distance;
			if (Back_Num == State_Freeze)
				states = Move_To_Slide_Start_Postion_Finished;
			break;
			//*************自动聚焦*************//
		case Search_Optimal_Focal_Distance:
			Back_Num = Do_Search_Optimal_Focal_Distance();
			if (Back_Num == State_Finished)
			{
				s_Scanning_Para.Current_Point_Num++;//表示当前点已经拍摄完成，扫描点序号自加1
				states = Is_Scanning_Finished;
			}
			if (Back_Num == State_Freeze)
				states = Z_Stepping;
			break;
		case Z_Stepping:
			Back_Num = Do_Stepping(3, m_Focus_Step);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_Z_Moveto);
			if (Back_Num == State_Finished)
				states = Search_Optimal_Focal_Distance;
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = Z_Stepping;
			break;
			//************判断*****************//
		case Is_Scanning_Finished:
			states = Do_Is_Scanning_Finished();
			break;
			//*************运动到下一个点*******************//
		case X_Stepping:
			Back_Num = Do_Stepping(1, m_Scanning_Step_X);
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(1, m_X_Moveto);
			if (Back_Num == State_Finished)
				states = Search_Optimal_Focal_Distance;
			if (Back_Num == State_Freeze)
				states = X_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = X_Stepping;
			break;
		case Y_Stepping:
			Back_Num = Do_Stepping(2, m_Scanning_Step_Y);
			states = Y_Stepping_Finished;
			break;
		case Y_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(2, m_Z_Moveto);
			if (Back_Num == State_Finished)
				states = Search_Optimal_Focal_Distance;
			if (Back_Num == State_Freeze)
				states = Y_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = Y_Stepping;
			break;
		default:
			break;
		}

	}

	Do_Scanning_Finished();
	return 1;
}


int Scanning_Capture::Do_restting_Model()
{
	int Back_Num;
	int states = Scanning_intial;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
		case Scanning_intial:
			Back_Num = Do_Scanning_intial();
			if (Back_Num == State_Finished)
				states = Resetting_Z;
			break;
			//******复位过程，首先Z轴复位，然后物镜转盘复位，最后X,Y同时复位******//
		case Resetting_Z:
			Back_Num = Do_Resetting(3,true);//快速复位
			states = Resetting_Z_Finished;
			break;
		case Resetting_Z_Finished:
			Back_Num = Do_Resetting_Finished(3);
			if (Back_Num == State_Finished)
				states = Resetting_Z_Again;
			if (Back_Num == State_Freeze)
				states = Resetting_Z_Finished;
			break;
		case Resetting_Z_Again:
			Back_Num = Do_Resetting(3,false);//低速复位
			states = Resetting_Z_Again_Finished;
			break;
		case Resetting_Z_Again_Finished:
			Back_Num = Do_Resetting_Finished(3);
			if (Back_Num == State_Finished)
				states = Resetting_Table;
			if (Back_Num == State_Freeze)
				states = Resetting_Z_Again_Finished;
			break;
		case Resetting_Table:
			Back_Num = Do_Resetting(4);
			states = Resetting_Table_Finished;
			break;
		case Resetting_Table_Finished:
			Back_Num = Do_Resetting_Finished(4);
			if (Back_Num == State_Finished)
				states = Resetting_X_Y;
			if (Back_Num == State_Freeze)
				states = Resetting_Table_Finished;
			break;
		case Resetting_X_Y:
			Back_Num = Do_Resetting(1);
			states = Resetting_X_Y_Finished;
			break;
		case Resetting_X_Y_Finished:
			Back_Num = Do_Resetting_Finished(1);
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
				states = Scanning_Finished;
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Resetting_X_Y_Finished;
			break;

		default:
			break;
		}

	}

	//Do_Scanning_Finished();
	return 1;
}


int Scanning_Capture::Do_Scanning_Model()
{
	Do_restting_Model();//现复位

	int Back_Num;
	int states = Move_To_Object;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******运动到本次扫描/聚焦需要用到的物镜******//
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished 表示查询到完成状态
				states = Move_To_Slide_Start_Postion;
			if (Back_Num == State_Freeze)//State_Freeze表示本次查询没有查询到完成状态
				states = Move_To_Object_Fininshed;
			break;
			//******运动到扫描的第一个点位置******//
		case Move_To_Slide_Start_Postion:
			states = Do_Slide_Start_Postion();
			break;
		case Move_To_Slide_Start_Postion_Finished:
			Back_Num = Do_Slide_Start_Postion_Finished();
			if (Back_Num == State_Finished)
				states = Z_Stepping;
			if (Back_Num == State_Freeze)
				states = Move_To_Slide_Start_Postion_Finished;
			break;
			//*************自动扫描*************//
		case Capture_Image:
			Back_Num = Do_Capture_Image();
			if (Back_Num == State_Finished)
			{
				s_Scanning_Para.Current_Point_Num++;//表示当前点已经拍摄完成，扫描点序号自加1
				states = Is_Scanning_Finished;
			}

			break;
		case Z_Stepping:
			/******加一个焦距自动计算的函数******/

			/********************************/
			
			Back_Num = Do_Stepping(3, s_Scanning_Para.Current_Point_Num);//测试用
			//Back_Num = Do_Stepping(3, s_Scanning_Para.Scanning_Length[s_Scanning_Para.Point_Num]);
			states = Z_Stepping_Finished;
			break;
		case Z_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(3, m_X_Moveto);
			if (Back_Num == State_Finished)
				states = Capture_Image;
			if (Back_Num == State_Freeze)
				states = Z_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = Z_Stepping;
			break;
			//************判断*****************//
		case Is_Scanning_Finished:
			states = Do_Is_Scanning_Finished();
			break;
			//*************运动到下一个点*******************//
		case X_Stepping:
			Back_Num = Do_Stepping(1, m_Scanning_Step_X);
			states = X_Stepping_Finished;
			break;
		case X_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(1, m_X_Moveto);
			if (Back_Num == State_Finished)
				states = Z_Stepping;
			if (Back_Num == State_Freeze)
				states = X_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = X_Stepping;
			break;
		case Y_Stepping:
			Back_Num = Do_Stepping(2, m_Scanning_Step_Y);
			states = Y_Stepping_Finished;
			break;
		case Y_Stepping_Finished:
			Back_Num = Do_Stepping_Finished(2, m_X_Moveto);
			if (Back_Num == State_Finished)
				states = Z_Stepping;
			if (Back_Num == State_Freeze)
				states = Y_Stepping_Finished;
			if (Back_Num == Z_Stepping)
				states = Y_Stepping;
			break;
		default:
			break;
		}

	}

	Do_Scanning_Finished();
	return 1;
}

int Scanning_Capture::Do_PUSIRobot_Resetting(int Motor_ID)
{
	CString Dev_id_s;
	Dev_id_s.Format(_T("%02x"), Motor_ID);
	CString Dev_F_id = _T("000006") + Dev_id_s;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}

	s_Dev_Drivers->Send_command("2F20600000000000", F_id, 0, 0);//停止动作
	s_Dev_Drivers->Dev_PUSIRobot_ENA(Motor_ID, 0);//3号使能
	Sleep(100);
	s_Dev_Drivers->Dev_heart_beat(Motor_ID, FALSE);//关闭报文
	s_Dev_Drivers->Dev_heart_beat(5, FALSE);//关闭报文
	s_Dev_Drivers->Dev_PUSIRobot_ENA(Motor_ID, 1);//3号使能
	s_Dev_Drivers->Dev_PUSIRobot_ENA(5, 0);//3号使能
	Sleep(300);
	long lk;
	vector<long>Home_offset;
	//根据输入的ID转化为16进制的字符串can帧ID
	s_Dev_Drivers->Dev_PUSIRobot_Set_Zero_Postion(Motor_ID);
	s_Dev_Drivers->Send_command("2F0F600101000000", F_id, 0, 0);//启用外部停止1
	s_Dev_Drivers->Send_command("2F0F600101000000", F_id, 0, 0);//启用外部停止1
	s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(Motor_ID, 20000, -4000, false);

	vector<UINT> Dev_id = { 0 };
	Dev_id[0] = Motor_ID;
	vector<bool> status_arr = { 0 };
	bool Move_Finished = false;
	while (!Move_Finished)
	{
		Sleep(50);
		s_Dev_Drivers->Dev_PUSIRobot_Read_Status(Dev_id, status_arr, 0);
		Move_Finished =status_arr[0];
	}
	s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(Motor_ID, 20000, -4000, false);

	Move_Finished = false;
	while (!Move_Finished)
	{
		Sleep(50);
		s_Dev_Drivers->Dev_PUSIRobot_Read_Status(Dev_id, status_arr, 0);
		Move_Finished = status_arr[0];
	}
	s_Dev_Drivers->Send_command("2F0F600100000000", F_id, 0, 0);//禁用外部停止1
	s_Dev_Drivers->Send_command("2F0F600100000000", F_id, 0, 0);//禁用外部停止1
	s_Dev_Drivers->Dev_PUSIRobot_Set_Zero_Postion(Motor_ID);
	int M4_Offset = s_Scanning_System_Para.Offset_arr[3];
	s_Dev_Drivers->Dev_PUSIRobot_Single_Mov(Motor_ID, -M4_Offset, -1000, false);
	status_arr[0] = false;
	Move_Finished = false;
	while (!Move_Finished)
	{
		Sleep(50);
		s_Dev_Drivers->Dev_PUSIRobot_Read_Status(Dev_id, status_arr, 1);
		long Current_Pos = 0;
		bool Get_Pos_Move = s_Dev_Drivers->Dev_PUSIRobot_Read_Pos_Theory(Motor_ID, Current_Pos);

		bool Move_In_Deviation = (Current_Pos >= (M4_Offset - s_Scanning_System_Para.Move_Deviation)) &&
			(Current_Pos <= (M4_Offset + s_Scanning_System_Para.Move_Deviation));
		Move_Finished = Move_In_Deviation && status_arr[0];
	}
	s_Dev_Drivers->Dev_PUSIRobot_Set_Zero_Postion(Motor_ID);
	s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);//3号使能
	
	return State_Finished;

}