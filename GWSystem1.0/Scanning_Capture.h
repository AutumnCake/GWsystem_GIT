#pragma once
#include "Control_Dev.h"
#include <string.h>


/******包含Opencv相关库***********/
#include <opencv2\opencv.hpp>   
using namespace cv;
/********************************/

#define Scanning_intial 0       //初始化
#define Move_To_Slide_Start_Postion 1   //运动到某张玻片的本次开始扫面点位置
#define Move_To_Slide_Start_Postion_Finished 2      //运动到某张玻片的本次开始扫面点位置
//Check_01_Arrived,//Move_To_Start_Postion_0运动到位
#define Is_Scanning_Finished 3  //进行判断，包括判断一下几个部分：1、判断扫描是否完成，或是进入下一张玻片中
					          //2、判断下一次是继续横向扫描，并给出扫描位移和方向，还是纵向步进，并给出纵向步进方向和位移
#define X_Stepping 4  //X轴步进一个步长
#define X_Stepping_Finished 5//X轴步进完成
#define Y_Stepping 6  //Y轴步进一个步长
#define Y_Stepping_Finished 7  //Y轴步进完成
#define Z_Stepping 8  //Z轴步进一个步长
#define Z_Stepping_Finished 9//Z轴步进完成
#define Scanning_Finished 10   //扫描结束
#define Resetting_X_Y 11   //开始X和Y轴的复位操作
#define Resetting_X_Y_Finished 12   //判断是否复位完成
#define Resetting_Z 13    //开始Z轴的复位操作
#define Resetting_Z_Finished 14    //判断Z是否复位完成
#define Resetting_Table 15   //开始物镜转盘的复位操作
#define Resetting_Table_Finished 16//判断物镜转盘是否复位完成
#define Capture_Image 17   //抓图和保存
#define Save_Image 18   //抓图和保存
#define Is_sharp_Image 19   //图像清晰度计算，爬坡算法判断最佳焦距点
#define Stepping 20
#define Stepping_Finished 21
#define State_Freeze 22   //表示当前操作重复；
#define State_Finished 23  //表示当前操作重复；
#define Move_To_Object 24
#define Move_To_Object_Fininshed 25
#define Search_Optimal_Focal_Distance 26
#define Resetting_Z_Again 27    //开始Z轴的复位操作
#define Resetting_Z_Again_Finished 28   //判断Z是否复位完成
#define Z_MoveDown_Avoid 29   //转换镜头前预先下降
#define Z_MoveDown_Avoid_Finished 30   //转换镜头前预先下降完成
#define Z_MoveDown_AvoidBack 31   //转换镜头前预先下降回升
#define Z_MoveDown_AvoidBack_Finished 32   //转换镜头前预先下降回升完成

#define Objective_4X 0 //物镜编号
#define Objective_10X 1
#define Objective_20X 2
#define Objective_40X 3
#define Objective_60X 4
#define Objective_100X 5

//定义扫描的基本固定参数结构体

struct Scanning_System_Para
{
	vector<long> Offset_arr; //复位的偏移量，X轴和Y轴
	char *CANET_ID; //CAN网关的ID
	int CANET_Port_Num; //CAN通信转换模块的ID
	double Focal_Domain_Radius; //某个已知焦距位移值的点，当某个未知点落入这个已知点半径为这个参数的范围内时，就认为
								//这个未知点的焦距和这个已知点的焦距一样， 单位是毫米 mm
	int Focusing_Step_Z[3];//聚焦步长，0、1、2分别为最大步长，中等步长，最小步长
	                   //注意这里的单位是脉冲数，
	//double Focusing_Step_Z_Max; //聚焦大步长，注意这里的单位是脉冲数，
	//double Focusing_Step_Z_Min; //聚焦小步长，注意这里的单位是脉冲数，
	double Slide_Size_X; //玻片的实际大小宽
	double Slide_Size_y; //玻片的实际大小长
	double Space_Slides; //玻片与玻片之间的间隔
	vector<long> Nosepiece_Abs_Position; //物镜转盘的绝对位置
	int Move_Deviation;
	double View_Field_Width_10X;//40X时，相机看到的视野大小的宽度（脉冲数）
	double View_Field_Higth_10X;//40X时，相机看到的视野大小的高度（脉冲数）

	int CA_XPos_Compensate;//染色体分析时，从10X下定位得到的坐标存在误差的话，可以通过设定这个补偿来修正
	int CA_YPos_Compensate;//Y方向补偿修正
	int Switch_Objective_ZPos;//切换物镜时，Z轴下降一个大的高度，偏于切换物镜

	int Pump_Oil_Pulse;//每次蹦油的的体积脉冲数
	float Pump_Oil_Vel;//泵油的速度；
	float Pump_Oil_Dis;//每个油滴之间的间距（mm）
	int Focus_Max_Limit;//焦距值的最大限制，表示的是所有的物镜焦距都在这个范围内
	int Focus_Min_Limit;//焦距值的最大限制
	float Pulse_ratio;//X,Y轴 电机的脉冲系数比，也即是1mm对应的脉冲数（pp/mm）

	int Handle_Zero_X;//手动摇杆X的零点数值，通常为数字量
	int Handle_Zero_Y;//手动摇杆X的零点数值，通常为数字量
	int Handle_Zero_Z;//手动摇杆X的零点数值，通常为数字量
	int Handle_Error_Range;//手动摇杆的误差范围
	int Focus_Equal_Dis;//两个点焦距被认为一样的最大距离

}; 
//{{ -87000, 231000, 2000, -33950 },  "192.168.1.5", 5000, 0.05, 204.8,40.96,25,55,3.4,50};
//用户参数结构体
struct User_Para
{
	int Slide_Max_Num;//一次扫描的玻片最大数量
	double Scanning_Aero_X; //扫描区域X方向的长度
	double Scanning_Aero_Y; //扫描区域Y方向上的长度
	int Scanning_Num_X; //X轴方向上扫面的点数
	int Scanning_Num_Y; //Y轴方向上扫面的点数
	unsigned int m_Focus_Spacing_X;//X方向聚焦点间距 单位mm
	unsigned int m_Focus_Spacing_Y; //Y方向聚焦点间距 单位mm
	//int Focusing_Num_X; //X轴方向上聚焦的点数
	//int Focusing_Num_Y; //Y轴方向上聚焦的点数
	bool NewStart_OR_BreakPoint;//重新开始或断点开始

	
};
//{8,15, 30, 50, 80, 5,8};
struct Scanning_Para
{
	int Current_Slide_Num;//当前运行在第I张玻片上
	int Current_Table_Num;//当前运行在第I屉玻片架
	//int Current_Scanning_I_X;//当前扫描的是当前玻片上的X轴方向上的第I个点
	//int Current_Scanning_J_Y;//当前扫描的是当前玻片上的Y轴方向上的第J个点
	bool Forcuing_OR_Scanning;//聚焦（true）还是扫描
	unsigned int Objective_Num;

	//***********以下为扫描时用到的参数**********//
	int Current_Point_Num;//当前扫描点的序号
	double save[100] ;//存储每个图片的值
	int High[100] ;//储存每个图片的高度
	vector <int > Focal_Length;//储存各个聚焦点的焦距
	vector <int > Scanning_Length;//储存各个扫描点的焦距
	vector <int> X_Scanning_Location;
	vector <int> Y_Scanning_Location;
	vector <int > X_Focus_Location;//储存各个聚焦点的x坐标
	vector <int > Y_Focus_Location;//储存各个聚焦点的y坐标
	int Current_Scanning_Num_X;//当前扫描点X方向序号
	int Current_Scanning_Num_Y;//当前扫描点Y方向的序号
	int BreakPoint_Pos_X;//断点开始的X轴坐标
	int BreakPoint_Pos_Y;//断点开始的Y轴坐标
	int BreakPoint_Pos_Z;//断点开始的Z轴坐标
	vector <Point> CA_Pos_10X;//在10倍下扫描后，得到的染色体区域具体坐标
};

//{ 0 , 0, 0, 0，0，true,3,0,{ 0 }，{ 0 }，{ 0 }，{ 0 }，{ 0 }};

//10X下扫描时的参数。主要用于调整定位染色体位置的算法参数等
typedef struct Scanning_10X_Para
{
	unsigned int Erode_element;//对阀值后图像进行腐蚀的结构元大小，用于将染色体区域腐蚀掉，留下非染色体区域
	unsigned int Dilate_element;//对疑似的染色体区域图像进行膨胀的结构元，用于区别染色体区域和细小杂质区域。
	unsigned int ErodeDilate_element;//对疑似的染色体区域图像进行腐蚀膨胀，锁定染色体区域。
	unsigned int Min_CA_Aero_10X;//区别细小杂质区域和染色体区域的阀值
	unsigned int Min_CA_Num_10X;//染色体区域中染色体疑似染色体条数的最小值
}Scanning_10X_Para, pScanning_10X_Para;


class Scanning_Capture
{
public:
	Scanning_Capture();
	~Scanning_Capture();
	//Scanning_Para Do_Get_Paras();
	//void Do_Set_paras(Scanning_Para Paras);
	int Do_Scanning_intial();//初始化
	//void Do_Is_Reset();//判断是否复位
	//void Do_Move_To_Start_Postion();//运动到某张玻片的本次开始扫面点位置
	//void Do_Check_01_Arrived();//Move_To_Start_Postion_0运动到位
	virtual int Do_Is_Scanning_Finished();/*进行判断，包括判断一下几个部分：1、判断扫描是否完成，或是进入下一张玻片中
                                   2、判断下一次是继续横向扫描，并给出扫描位移和方向，还是纵向步进，并给出纵向步进方向和位移
                                   */
	int Do_Stepping(int Motor_ID, long Motor_Postion);//X轴放心步进一个步长
	int Do_Stepping_Finished(int Motor_ID,long Move_to);//X轴步进完成)
	//int Do_Y_Stepping();//Y轴放心步进一个步长
	//int Do_Y_Stepping_Finished();//Y轴步进完成
	//int Do_Z_Stepping();//Z轴放心步进一个步长
	//int Do_Z_Stepping_Finished();//Z轴步进完成
	bool Do_Scanning_Finished();//扫描结束
	//void Set_States(enum Scanning_States State);//设置状态
	//enum Scanning_States Get_States();//获取当前运行的状态
	void Calculate_Focal_Distance(int Scan_Index_Row, int Scan_Index_Col);//根据有限聚焦值，计算扫描点的焦距值
	long Read_Focal_Distance(int Next_Slide_Num);//读取某张玻片的已经获取的自动聚焦时的焦距值
	int Save_Focal_Distance(bool Focusing_Or_Scanning);//保存某张玻片的获取的聚焦值,输入参数为True时表示保存自动聚焦时的焦距值，否则就是保存扫描点的焦距值
	int Do_Search_Optimal_Focal_Distance();//寻找最佳焦距值
	int Do_Resetting(int Motor_ID, bool fast_slow=true);//X,Y轴复位开始
	int Do_Resetting_Finished(int Motor_ID);//X,Y轴复位完成
	int Do_PUSIRobot_Resetting(int Motor_ID);
	int Do_Slide_Start_Postion();//运动到初始开始扫描点位置
	int Do_Slide_Start_Postion_Finished();//运动到初始开始扫描点位置完成

	int Do_Move_To_Object(int Object_Num, int Dir = 1, int Speed = 4000);//运动到对应的物镜
	int Do_Move_To_Object_Fininshed();//物镜运动完成
	int Do_Capture_Image();//抓取照片，这个函数需要根据相机专门编写
	int Do_Save_Image();//主要用于保存图片
	virtual int Do_Focusing_Model();
	virtual int Do_Scanning_Model();
	virtual int Do_restting_Model();

	static Control_Dev *s_Dev_Drivers;
	static Scanning_Para s_Scanning_Para;
	static Scanning_System_Para s_Scanning_System_Para;
	static User_Para s_User_Para;
	static CCriticalSection critical_section;
	static Scanning_10X_Para s_Scanning_10X_Para;//10X扫描的参数

	bool Obtain_Optimal_Focal;//获得最佳焦距后为true

protected:
	//enum Scanning_States m_Scanning_States;//
	double m_Focus_Step;
	long m_Current_Slide_Start_X;//当前玻片的开始扫描点的X方向坐标，绝对位置
	long m_Current_Slide_Start_Y;//当前玻片的开始扫描点的Y方向坐标，绝对位置
	long m_Scanning_Step_X;//X轴方向步进角度值
	long m_Scanning_Step_Y;//X轴方向步进角度值
	int m_Scanning_Num_X;//
	int m_Scanning_Num_Y;//
	long m_X_Moveto;//当前X轴绝对运动位移
	long m_Y_Moveto;
	long m_Z_Moveto;
	long m_T_Moveto;
	long m_5_Moveto;
	//聚焦用局部变量
	unsigned int m_Focusing_Num_X; //X轴方向上聚焦的点数
	unsigned int m_Focusing_Num_Y; //Y轴方向上聚焦的点数

	//bool Scanning_OR_Forcuing;//判断是进行扫描操作，还是进行聚焦操作
	//vector<vector<long> > m_Focusing_Focus_Distance;//存放有限个聚焦点自动聚焦获得的焦距值位移值,断电后将没有意义，
	//vector<vector<long> > m_Scanning_Focus_Distance;//存放每个扫描点的焦距位移值,断电后将没有意义，需要重新获取
	bool m_Respond_Right;//用于对发送指令后，通过返回的报文判断该指令是否被驱动器正确接收；
	int m_Respond_Check_Num;//返回的报文相应的查询次数记录，相当于一个时钟；
};

