#pragma once
#ifndef _CONTROL_DEV_
#define _CONTROL_DEV_

#include <stdio.h>
#include <Winsock2.h>
#include <iostream>
#include<vector>
#pragma comment(lib, "ws2_32.lib")
#include<sstream>//转化字符串用
//#include<string>
using namespace std;

#define  Velocity_Model_START  1
#define  Velocity_Model_STOP   2
#define  Motor_UpperEnd_STOP   3
#define  Motor_LowerEnd_STOP   4
#define  Motor_Move_In_Range   5
class Control_Dev
{
public:
	Control_Dev();

	/***************************************
	CANET_Init(char *Dev_addr, int Dev_port)
	本分支用于初始化Socket，与Can转以太网的网关（TCP_Server模式）建立通信；
	Dev_addr： 网关的网址，默认为：192.168.1.5
	Dev_port： 网关的端口号，默认为：5000
	当返回值为 0，表明连接失败
	当返回值为 -1，表明建立连接失败 */
	int CANET_Init(char *Dev_addr="192.168.1.5",int Dev_port=5000);
	//int CANET_Init(char *Dev_addr = "192.168.1.5", int Dev_port = 5000);
	/***************************************
	Send_command(char*pData, char*F_id, int iStandorExtern, int iRemoteOrData)
	char*pData：发送的can帧的数据字符串数组，16进制格式，共8位，例如："2B40600006000000"
	char*F_id：发送的can帧的ID号，16进制，共6位  例如： "00000601" */
	int Send_command(char*pData, char*F_id, int iStandorExtern = 0, int iRemoteOrData = 0);
	//int Send_command(char*pData, char*F_id, int iStandorExtern = 0, int iRemoteOrData = 0);
	/***************************************
	Recv_message(char *pRecv,int &iRLen)
	char *pRecv：收到的报文，返回值
	int &iRLen：接受的报文长度，每条报文13个字符，返回值由多条报文组成。 */
	int Recv_message(char *pRecv, int &iRLen, int Timeout=5);
	//int Recv_message(char *pRecv, int &iRLen, int Timeout = 5);
	/***************************************
	Dev_ENA(UINT Dev_id, bool Dev_Switch)  电机使能，断开使能函数
	UINT Dev_id： 电机的ID号，
	bool Dev_Switch： 为True时，表示启动使能，为false时表示断开使能； */
	int Dev_Reset_Communication();
	int Dev_ENA(UINT Dev_id, bool Dev_Switch); 
	//谱思驱动器
	int Dev_PUSIRobot_ENA(UINT Dev_id, bool Dev_Switch); 
	/***************************************
	int Dev_ENA(UINT Dev_id);  清除报警
	UINT Dev_id： 电机的ID号， */
	int Dev_clear_alarm(UINT Dev_id);

	/***************************************
	相对位置控制模式：
	Dev_relatice_Pos(UINT Dev_id, long Relative_Pos, int Dev_Pro_Velocity = 5)
	UINT Dev_id： 电机的ID号，
	long Relative_Pos ：相对位置，（目前单位是脉冲数，20000脉冲/转）
	int Dev_Pro_Velocity ：设置运动速度 
	bool abs_or_rel : 当微True时表示为绝对运动，为false时表示为相对运动
	 Control_Model=false：为假时表示当时的情况  */
	int Dev_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity = 15, bool abs_or_rel = true);
	int Dev_PUSIRobot_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity = 15, bool abs_or_rel = true);
	//配置运动模式和参数
	//Control_Model=0,表示绝对位置模式，为1表示相对位置模式，为2表示速度模式
	int Dev_Single_Mov_Para(UINT Dev_id, unsigned int Control_Model = 0);

	/***************************************
	相对位置控制模式：
	Dev_Velocity_Model(UINT Dev_id, float Dev_Pro_Velocity = 15, int START_STOP = Velocity_Model_START)
	UINT Dev_id： 电机的ID号，
	int Dev_Pro_Velocity ：设置运动速度
	int START_STOP = Velocity_Model_START  :开始或停止*/
	int Dev_Velocity_Model(UINT Dev_id, int START_STOP = Velocity_Model_START, float Dev_Pro_Velocity = 15, bool Continue_Model=true);


	/***************************************
	停止运动：
	Dev_stop(UINT Dev_id)
	UINT Dev_id： 电机的ID号 */
	int Dev_stop(UINT Dev_id);

	/***************************************
	停止运动：
	Dev_stop(UINT Dev_id)
	UINT Dev_id： 电机的ID号 */
	int Dev_heart_beat(UINT Dev_id,bool open_close);

	/***************************************
	设置加减速：
	Dev_stop(UINT Dev_id)
	UINT Dev_id： 电机的ID号 */
	int Dev_acc_dec(UINT Dev_id, long Dev_Acc=50, long Dev_Dec =100);

	/***************************************
	查询状态字、位置等：
	Dev_Read_Status(UINT *Dev_id, BOOL *Dev_Status,int recv_len = 100);
	UINT *Dev_id： 电机的ID号数组 
	INT *Dev_Status : 存放查询到的状态，与id数组一样大小，当某一个元素为0 表示当前运动/回零未完成，为1 表示运动/回零完成
					  为-1表示当前运动/回零出现错误
	int recv_len = 100：接收数据时的缓存大小，默认为100，这里的方式是，先接收所有报文（丢弃），再发送查询指令，
	                    然后再接收查询报文，匹配需要的报文类别
	int model=0 :运动模式，当等于0时表示当前值是回零模式，查询是否回零到位，当等于1时表示当前是位置控制模式，查询是否运动到位*/
	int Dev_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 200);
	int Dev_Read_InputIO_Status(UINT Dev_id, int recv_len = 200);
	//获取发送运动控制指令后返回的报文情况
	int Dev_Read_PosControl_Response(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 200);
	//model 0表示查询外部停止1是否出发，为1表示查询运动是否完成
	int Dev_PUSIRobot_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 65);
	/***************************************
	查询运动指令是否正确下发，查看驱动器返回报文类型：
	bool Dev_Read_Respond_Message(UINT Dev_id);
	UINT Dev_id： 需要查询电机对应的电机的ID号
	*/
	bool Dev_Read_Respond_Message(UINT Dev_id,int recv_len = 50);

	/***************************************
	查询位置等：
	Dev_Read_Pos(vector<UINT> Dev_id, vector<long> &Real_Pos, int recv_len = 100);
	UINT *Dev_id： 需要查询电机对应的电机的ID号
	long *Real_Pos :存放查询到的位置，
	int recv_len = 100：接收数据时的缓存大小，默认为100，这里的方式是，先接收所有报文（丢弃），再发送查询指令，
	然后再接收查询报文，匹配需要的报文类别
	特别注意：：：Dev_id 和Real_pos的元素个数必须一致*/
	bool Dev_Read_Pos(UINT Dev_id, long &Real_Pos,int recv_len = 200);
	//读取当前位置的理论值（6064）
	bool Dev_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len = 200);
	bool Dev_PUSIRobot_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len = 200);
	//读取模拟量输入1的值（700E）
	bool Dev_Read_Analog_Input1(UINT Dev_id, long &Analog_Input, int recv_len = 200);

	/***************************************
	回零运动：
	Dev_Home(UINT *Dev_id,bool Star_or_Stop);
	UINT *Dev_id： 需要查询电机对应的电机的ID号
	vector<UINT> Honme_method,: 存放回零偏移量的数组，和Dev_id大小一样
	bool Star_or_Stop :启动或停止回零，为1表示启动回零，为0表示停止回零
	特别注意：：：Dev_id 和Home_offset的元素个数必须一致*/
	int Dev_Home(vector<UINT> Dev_id, vector<long> Home_offset, bool Star_or_Stop = TRUE, bool fast_slow=true);

	/***************************************
	固定参数设置：
	Dev_Para_set(vector<UINT> Dev_id);
	vector<UINT> Dev_id： 需要查询电机对应的电机的ID号数组
     */
	int Dev_Para_set(vector<UINT> Dev_id);

	/***************************************
	Dev_Set_Status:设置某个电机的复位完成状态
	int Dev_ID：电机的ID
	bool Status：当前的电机复位完成状态，为TRUE时表示复位完成；
	*/
	void Dev_Set_Status(int Dev_Num,bool Status);

	/***************************************
	清除位置，也就是设置位置零点：
	Dev_Set_Zero_Postion(UINT Dev_id)
	UINT Dev_id： 电机的ID号 */
	int Dev_Set_Zero_Postion(UINT Dev_id);
	int Dev_PUSIRobot_Set_Zero_Postion(UINT Dev_id);

	//关闭通讯
	void closeSocket();

	~Control_Dev();

	static bool Status_Ref;//用于记录，当查询运动是否到位时，先查询下发的运动指令是否正确接收；
	static bool Communication_Success;
	int recv_error_num;


private:
	static SOCKET m_sockClient;
	char recv_data[50];
	vector<bool>  ResetCompleteStatus;
	/*使用的是周立功库*/
	int m_CAN_num;//第几路can口
	int m_devtype;//设备类型，本设备为
	DWORD m_devind;//设备编号，本处只用了一个设备，因此为0

};

#endif