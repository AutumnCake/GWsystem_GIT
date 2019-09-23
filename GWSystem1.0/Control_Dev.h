#pragma once
#ifndef _CONTROL_DEV_
#define _CONTROL_DEV_

#include <stdio.h>
#include <Winsock2.h>
#include <iostream>
#include<vector>
#pragma comment(lib, "ws2_32.lib")
#include<sstream>//ת���ַ�����
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
	����֧���ڳ�ʼ��Socket����Canת��̫�������أ�TCP_Serverģʽ������ͨ�ţ�
	Dev_addr�� ���ص���ַ��Ĭ��Ϊ��192.168.1.5
	Dev_port�� ���صĶ˿ںţ�Ĭ��Ϊ��5000
	������ֵΪ 0����������ʧ��
	������ֵΪ -1��������������ʧ�� */
	int CANET_Init(char *Dev_addr="192.168.1.5",int Dev_port=5000);
	//int CANET_Init(char *Dev_addr = "192.168.1.5", int Dev_port = 5000);
	/***************************************
	Send_command(char*pData, char*F_id, int iStandorExtern, int iRemoteOrData)
	char*pData�����͵�can֡�������ַ������飬16���Ƹ�ʽ����8λ�����磺"2B40600006000000"
	char*F_id�����͵�can֡��ID�ţ�16���ƣ���6λ  ���磺 "00000601" */
	int Send_command(char*pData, char*F_id, int iStandorExtern = 0, int iRemoteOrData = 0);
	//int Send_command(char*pData, char*F_id, int iStandorExtern = 0, int iRemoteOrData = 0);
	/***************************************
	Recv_message(char *pRecv,int &iRLen)
	char *pRecv���յ��ı��ģ�����ֵ
	int &iRLen�����ܵı��ĳ��ȣ�ÿ������13���ַ�������ֵ�ɶ���������ɡ� */
	int Recv_message(char *pRecv, int &iRLen, int Timeout=5);
	//int Recv_message(char *pRecv, int &iRLen, int Timeout = 5);
	/***************************************
	Dev_ENA(UINT Dev_id, bool Dev_Switch)  ���ʹ�ܣ��Ͽ�ʹ�ܺ���
	UINT Dev_id�� �����ID�ţ�
	bool Dev_Switch�� ΪTrueʱ����ʾ����ʹ�ܣ�Ϊfalseʱ��ʾ�Ͽ�ʹ�ܣ� */
	int Dev_Reset_Communication();
	int Dev_ENA(UINT Dev_id, bool Dev_Switch); 
	//��˼������
	int Dev_PUSIRobot_ENA(UINT Dev_id, bool Dev_Switch); 
	/***************************************
	int Dev_ENA(UINT Dev_id);  �������
	UINT Dev_id�� �����ID�ţ� */
	int Dev_clear_alarm(UINT Dev_id);

	/***************************************
	���λ�ÿ���ģʽ��
	Dev_relatice_Pos(UINT Dev_id, long Relative_Pos, int Dev_Pro_Velocity = 5)
	UINT Dev_id�� �����ID�ţ�
	long Relative_Pos �����λ�ã���Ŀǰ��λ����������20000����/ת��
	int Dev_Pro_Velocity �������˶��ٶ� 
	bool abs_or_rel : ��΢Trueʱ��ʾΪ�����˶���Ϊfalseʱ��ʾΪ����˶�
	 Control_Model=false��Ϊ��ʱ��ʾ��ʱ�����  */
	int Dev_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity = 15, bool abs_or_rel = true);
	int Dev_PUSIRobot_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity = 15, bool abs_or_rel = true);
	//�����˶�ģʽ�Ͳ���
	//Control_Model=0,��ʾ����λ��ģʽ��Ϊ1��ʾ���λ��ģʽ��Ϊ2��ʾ�ٶ�ģʽ
	int Dev_Single_Mov_Para(UINT Dev_id, unsigned int Control_Model = 0);

	/***************************************
	���λ�ÿ���ģʽ��
	Dev_Velocity_Model(UINT Dev_id, float Dev_Pro_Velocity = 15, int START_STOP = Velocity_Model_START)
	UINT Dev_id�� �����ID�ţ�
	int Dev_Pro_Velocity �������˶��ٶ�
	int START_STOP = Velocity_Model_START  :��ʼ��ֹͣ*/
	int Dev_Velocity_Model(UINT Dev_id, int START_STOP = Velocity_Model_START, float Dev_Pro_Velocity = 15, bool Continue_Model=true);


	/***************************************
	ֹͣ�˶���
	Dev_stop(UINT Dev_id)
	UINT Dev_id�� �����ID�� */
	int Dev_stop(UINT Dev_id);

	/***************************************
	ֹͣ�˶���
	Dev_stop(UINT Dev_id)
	UINT Dev_id�� �����ID�� */
	int Dev_heart_beat(UINT Dev_id,bool open_close);

	/***************************************
	���üӼ��٣�
	Dev_stop(UINT Dev_id)
	UINT Dev_id�� �����ID�� */
	int Dev_acc_dec(UINT Dev_id, long Dev_Acc=50, long Dev_Dec =100);

	/***************************************
	��ѯ״̬�֡�λ�õȣ�
	Dev_Read_Status(UINT *Dev_id, BOOL *Dev_Status,int recv_len = 100);
	UINT *Dev_id�� �����ID������ 
	INT *Dev_Status : ��Ų�ѯ����״̬����id����һ����С����ĳһ��Ԫ��Ϊ0 ��ʾ��ǰ�˶�/����δ��ɣ�Ϊ1 ��ʾ�˶�/�������
					  Ϊ-1��ʾ��ǰ�˶�/������ִ���
	int recv_len = 100����������ʱ�Ļ����С��Ĭ��Ϊ100������ķ�ʽ�ǣ��Ƚ������б��ģ����������ٷ��Ͳ�ѯָ�
	                    Ȼ���ٽ��ղ�ѯ���ģ�ƥ����Ҫ�ı������
	int model=0 :�˶�ģʽ��������0ʱ��ʾ��ǰֵ�ǻ���ģʽ����ѯ�Ƿ���㵽λ��������1ʱ��ʾ��ǰ��λ�ÿ���ģʽ����ѯ�Ƿ��˶���λ*/
	int Dev_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 200);
	int Dev_Read_InputIO_Status(UINT Dev_id, int recv_len = 200);
	//��ȡ�����˶�����ָ��󷵻صı������
	int Dev_Read_PosControl_Response(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 200);
	//model 0��ʾ��ѯ�ⲿֹͣ1�Ƿ������Ϊ1��ʾ��ѯ�˶��Ƿ����
	int Dev_PUSIRobot_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model = 0, int recv_len = 65);
	/***************************************
	��ѯ�˶�ָ���Ƿ���ȷ�·����鿴���������ر������ͣ�
	bool Dev_Read_Respond_Message(UINT Dev_id);
	UINT Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
	*/
	bool Dev_Read_Respond_Message(UINT Dev_id,int recv_len = 50);

	/***************************************
	��ѯλ�õȣ�
	Dev_Read_Pos(vector<UINT> Dev_id, vector<long> &Real_Pos, int recv_len = 100);
	UINT *Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
	long *Real_Pos :��Ų�ѯ����λ�ã�
	int recv_len = 100����������ʱ�Ļ����С��Ĭ��Ϊ100������ķ�ʽ�ǣ��Ƚ������б��ģ����������ٷ��Ͳ�ѯָ�
	Ȼ���ٽ��ղ�ѯ���ģ�ƥ����Ҫ�ı������
	�ر�ע�⣺����Dev_id ��Real_pos��Ԫ�ظ�������һ��*/
	bool Dev_Read_Pos(UINT Dev_id, long &Real_Pos,int recv_len = 200);
	//��ȡ��ǰλ�õ�����ֵ��6064��
	bool Dev_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len = 200);
	bool Dev_PUSIRobot_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len = 200);
	//��ȡģ��������1��ֵ��700E��
	bool Dev_Read_Analog_Input1(UINT Dev_id, long &Analog_Input, int recv_len = 200);

	/***************************************
	�����˶���
	Dev_Home(UINT *Dev_id,bool Star_or_Stop);
	UINT *Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
	vector<UINT> Honme_method,: ��Ż���ƫ���������飬��Dev_id��Сһ��
	bool Star_or_Stop :������ֹͣ���㣬Ϊ1��ʾ�������㣬Ϊ0��ʾֹͣ����
	�ر�ע�⣺����Dev_id ��Home_offset��Ԫ�ظ�������һ��*/
	int Dev_Home(vector<UINT> Dev_id, vector<long> Home_offset, bool Star_or_Stop = TRUE, bool fast_slow=true);

	/***************************************
	�̶��������ã�
	Dev_Para_set(vector<UINT> Dev_id);
	vector<UINT> Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID������
     */
	int Dev_Para_set(vector<UINT> Dev_id);

	/***************************************
	Dev_Set_Status:����ĳ������ĸ�λ���״̬
	int Dev_ID�������ID
	bool Status����ǰ�ĵ����λ���״̬��ΪTRUEʱ��ʾ��λ��ɣ�
	*/
	void Dev_Set_Status(int Dev_Num,bool Status);

	/***************************************
	���λ�ã�Ҳ��������λ����㣺
	Dev_Set_Zero_Postion(UINT Dev_id)
	UINT Dev_id�� �����ID�� */
	int Dev_Set_Zero_Postion(UINT Dev_id);
	int Dev_PUSIRobot_Set_Zero_Postion(UINT Dev_id);

	//�ر�ͨѶ
	void closeSocket();

	~Control_Dev();

	static bool Status_Ref;//���ڼ�¼������ѯ�˶��Ƿ�λʱ���Ȳ�ѯ�·����˶�ָ���Ƿ���ȷ���գ�
	static bool Communication_Success;
	int recv_error_num;


private:
	static SOCKET m_sockClient;
	char recv_data[50];
	vector<bool>  ResetCompleteStatus;
	/*ʹ�õ�����������*/
	int m_CAN_num;//�ڼ�·can��
	int m_devtype;//�豸���ͣ����豸Ϊ
	DWORD m_devind;//�豸��ţ�����ֻ����һ���豸�����Ϊ0

};

#endif