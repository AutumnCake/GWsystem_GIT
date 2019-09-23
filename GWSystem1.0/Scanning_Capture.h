#pragma once
#include "Control_Dev.h"
#include <string.h>


/******����Opencv��ؿ�***********/
#include <opencv2\opencv.hpp>   
using namespace cv;
/********************************/

#define Scanning_intial 0       //��ʼ��
#define Move_To_Slide_Start_Postion 1   //�˶���ĳ�Ų�Ƭ�ı��ο�ʼɨ���λ��
#define Move_To_Slide_Start_Postion_Finished 2      //�˶���ĳ�Ų�Ƭ�ı��ο�ʼɨ���λ��
//Check_01_Arrived,//Move_To_Start_Postion_0�˶���λ
#define Is_Scanning_Finished 3  //�����жϣ������ж�һ�¼������֣�1���ж�ɨ���Ƿ���ɣ����ǽ�����һ�Ų�Ƭ��
					          //2���ж���һ���Ǽ�������ɨ�裬������ɨ��λ�ƺͷ��򣬻������򲽽������������򲽽������λ��
#define X_Stepping 4  //X�Ჽ��һ������
#define X_Stepping_Finished 5//X�Ჽ�����
#define Y_Stepping 6  //Y�Ჽ��һ������
#define Y_Stepping_Finished 7  //Y�Ჽ�����
#define Z_Stepping 8  //Z�Ჽ��һ������
#define Z_Stepping_Finished 9//Z�Ჽ�����
#define Scanning_Finished 10   //ɨ�����
#define Resetting_X_Y 11   //��ʼX��Y��ĸ�λ����
#define Resetting_X_Y_Finished 12   //�ж��Ƿ�λ���
#define Resetting_Z 13    //��ʼZ��ĸ�λ����
#define Resetting_Z_Finished 14    //�ж�Z�Ƿ�λ���
#define Resetting_Table 15   //��ʼ�ﾵת�̵ĸ�λ����
#define Resetting_Table_Finished 16//�ж��ﾵת���Ƿ�λ���
#define Capture_Image 17   //ץͼ�ͱ���
#define Save_Image 18   //ץͼ�ͱ���
#define Is_sharp_Image 19   //ͼ�������ȼ��㣬�����㷨�ж���ѽ����
#define Stepping 20
#define Stepping_Finished 21
#define State_Freeze 22   //��ʾ��ǰ�����ظ���
#define State_Finished 23  //��ʾ��ǰ�����ظ���
#define Move_To_Object 24
#define Move_To_Object_Fininshed 25
#define Search_Optimal_Focal_Distance 26
#define Resetting_Z_Again 27    //��ʼZ��ĸ�λ����
#define Resetting_Z_Again_Finished 28   //�ж�Z�Ƿ�λ���
#define Z_MoveDown_Avoid 29   //ת����ͷǰԤ���½�
#define Z_MoveDown_Avoid_Finished 30   //ת����ͷǰԤ���½����
#define Z_MoveDown_AvoidBack 31   //ת����ͷǰԤ���½�����
#define Z_MoveDown_AvoidBack_Finished 32   //ת����ͷǰԤ���½��������

#define Objective_4X 0 //�ﾵ���
#define Objective_10X 1
#define Objective_20X 2
#define Objective_40X 3
#define Objective_60X 4
#define Objective_100X 5

//����ɨ��Ļ����̶������ṹ��

struct Scanning_System_Para
{
	vector<long> Offset_arr; //��λ��ƫ������X���Y��
	char *CANET_ID; //CAN���ص�ID
	int CANET_Port_Num; //CANͨ��ת��ģ���ID
	double Focal_Domain_Radius; //ĳ����֪����λ��ֵ�ĵ㣬��ĳ��δ֪�����������֪��뾶Ϊ��������ķ�Χ��ʱ������Ϊ
								//���δ֪��Ľ���������֪��Ľ���һ���� ��λ�Ǻ��� mm
	int Focusing_Step_Z[3];//�۽�������0��1��2�ֱ�Ϊ��󲽳����еȲ�������С����
	                   //ע������ĵ�λ����������
	//double Focusing_Step_Z_Max; //�۽��󲽳���ע������ĵ�λ����������
	//double Focusing_Step_Z_Min; //�۽�С������ע������ĵ�λ����������
	double Slide_Size_X; //��Ƭ��ʵ�ʴ�С��
	double Slide_Size_y; //��Ƭ��ʵ�ʴ�С��
	double Space_Slides; //��Ƭ�벣Ƭ֮��ļ��
	vector<long> Nosepiece_Abs_Position; //�ﾵת�̵ľ���λ��
	int Move_Deviation;
	double View_Field_Width_10X;//40Xʱ�������������Ұ��С�Ŀ�ȣ���������
	double View_Field_Higth_10X;//40Xʱ�������������Ұ��С�ĸ߶ȣ���������

	int CA_XPos_Compensate;//Ⱦɫ�����ʱ����10X�¶�λ�õ�������������Ļ�������ͨ���趨�������������
	int CA_YPos_Compensate;//Y���򲹳�����
	int Switch_Objective_ZPos;//�л��ﾵʱ��Z���½�һ����ĸ߶ȣ�ƫ���л��ﾵ

	int Pump_Oil_Pulse;//ÿ�α��͵ĵ����������
	float Pump_Oil_Vel;//���͵��ٶȣ�
	float Pump_Oil_Dis;//ÿ���͵�֮��ļ�ࣨmm��
	int Focus_Max_Limit;//����ֵ��������ƣ���ʾ�������е��ﾵ���඼�������Χ��
	int Focus_Min_Limit;//����ֵ���������
	float Pulse_ratio;//X,Y�� ���������ϵ���ȣ�Ҳ����1mm��Ӧ����������pp/mm��

	int Handle_Zero_X;//�ֶ�ҡ��X�������ֵ��ͨ��Ϊ������
	int Handle_Zero_Y;//�ֶ�ҡ��X�������ֵ��ͨ��Ϊ������
	int Handle_Zero_Z;//�ֶ�ҡ��X�������ֵ��ͨ��Ϊ������
	int Handle_Error_Range;//�ֶ�ҡ�˵���Χ
	int Focus_Equal_Dis;//�����㽹�౻��Ϊһ����������

}; 
//{{ -87000, 231000, 2000, -33950 },  "192.168.1.5", 5000, 0.05, 204.8,40.96,25,55,3.4,50};
//�û������ṹ��
struct User_Para
{
	int Slide_Max_Num;//һ��ɨ��Ĳ�Ƭ�������
	double Scanning_Aero_X; //ɨ������X����ĳ���
	double Scanning_Aero_Y; //ɨ������Y�����ϵĳ���
	int Scanning_Num_X; //X�᷽����ɨ��ĵ���
	int Scanning_Num_Y; //Y�᷽����ɨ��ĵ���
	unsigned int m_Focus_Spacing_X;//X����۽����� ��λmm
	unsigned int m_Focus_Spacing_Y; //Y����۽����� ��λmm
	//int Focusing_Num_X; //X�᷽���Ͼ۽��ĵ���
	//int Focusing_Num_Y; //Y�᷽���Ͼ۽��ĵ���
	bool NewStart_OR_BreakPoint;//���¿�ʼ��ϵ㿪ʼ

	
};
//{8,15, 30, 50, 80, 5,8};
struct Scanning_Para
{
	int Current_Slide_Num;//��ǰ�����ڵ�I�Ų�Ƭ��
	int Current_Table_Num;//��ǰ�����ڵ�I�벣Ƭ��
	//int Current_Scanning_I_X;//��ǰɨ����ǵ�ǰ��Ƭ�ϵ�X�᷽���ϵĵ�I����
	//int Current_Scanning_J_Y;//��ǰɨ����ǵ�ǰ��Ƭ�ϵ�Y�᷽���ϵĵ�J����
	bool Forcuing_OR_Scanning;//�۽���true������ɨ��
	unsigned int Objective_Num;

	//***********����Ϊɨ��ʱ�õ��Ĳ���**********//
	int Current_Point_Num;//��ǰɨ�������
	double save[100] ;//�洢ÿ��ͼƬ��ֵ
	int High[100] ;//����ÿ��ͼƬ�ĸ߶�
	vector <int > Focal_Length;//��������۽���Ľ���
	vector <int > Scanning_Length;//�������ɨ���Ľ���
	vector <int> X_Scanning_Location;
	vector <int> Y_Scanning_Location;
	vector <int > X_Focus_Location;//��������۽����x����
	vector <int > Y_Focus_Location;//��������۽����y����
	int Current_Scanning_Num_X;//��ǰɨ���X�������
	int Current_Scanning_Num_Y;//��ǰɨ���Y��������
	int BreakPoint_Pos_X;//�ϵ㿪ʼ��X������
	int BreakPoint_Pos_Y;//�ϵ㿪ʼ��Y������
	int BreakPoint_Pos_Z;//�ϵ㿪ʼ��Z������
	vector <Point> CA_Pos_10X;//��10����ɨ��󣬵õ���Ⱦɫ�������������
};

//{ 0 , 0, 0, 0��0��true,3,0,{ 0 }��{ 0 }��{ 0 }��{ 0 }��{ 0 }};

//10X��ɨ��ʱ�Ĳ�������Ҫ���ڵ�����λȾɫ��λ�õ��㷨������
typedef struct Scanning_10X_Para
{
	unsigned int Erode_element;//�Է�ֵ��ͼ����и�ʴ�ĽṹԪ��С�����ڽ�Ⱦɫ������ʴ�������·�Ⱦɫ������
	unsigned int Dilate_element;//�����Ƶ�Ⱦɫ������ͼ��������͵ĽṹԪ����������Ⱦɫ�������ϸС��������
	unsigned int ErodeDilate_element;//�����Ƶ�Ⱦɫ������ͼ����и�ʴ���ͣ�����Ⱦɫ������
	unsigned int Min_CA_Aero_10X;//����ϸС���������Ⱦɫ������ķ�ֵ
	unsigned int Min_CA_Num_10X;//Ⱦɫ��������Ⱦɫ������Ⱦɫ����������Сֵ
}Scanning_10X_Para, pScanning_10X_Para;


class Scanning_Capture
{
public:
	Scanning_Capture();
	~Scanning_Capture();
	//Scanning_Para Do_Get_Paras();
	//void Do_Set_paras(Scanning_Para Paras);
	int Do_Scanning_intial();//��ʼ��
	//void Do_Is_Reset();//�ж��Ƿ�λ
	//void Do_Move_To_Start_Postion();//�˶���ĳ�Ų�Ƭ�ı��ο�ʼɨ���λ��
	//void Do_Check_01_Arrived();//Move_To_Start_Postion_0�˶���λ
	virtual int Do_Is_Scanning_Finished();/*�����жϣ������ж�һ�¼������֣�1���ж�ɨ���Ƿ���ɣ����ǽ�����һ�Ų�Ƭ��
                                   2���ж���һ���Ǽ�������ɨ�裬������ɨ��λ�ƺͷ��򣬻������򲽽������������򲽽������λ��
                                   */
	int Do_Stepping(int Motor_ID, long Motor_Postion);//X����Ĳ���һ������
	int Do_Stepping_Finished(int Motor_ID,long Move_to);//X�Ჽ�����)
	//int Do_Y_Stepping();//Y����Ĳ���һ������
	//int Do_Y_Stepping_Finished();//Y�Ჽ�����
	//int Do_Z_Stepping();//Z����Ĳ���һ������
	//int Do_Z_Stepping_Finished();//Z�Ჽ�����
	bool Do_Scanning_Finished();//ɨ�����
	//void Set_States(enum Scanning_States State);//����״̬
	//enum Scanning_States Get_States();//��ȡ��ǰ���е�״̬
	void Calculate_Focal_Distance(int Scan_Index_Row, int Scan_Index_Col);//�������޾۽�ֵ������ɨ���Ľ���ֵ
	long Read_Focal_Distance(int Next_Slide_Num);//��ȡĳ�Ų�Ƭ���Ѿ���ȡ���Զ��۽�ʱ�Ľ���ֵ
	int Save_Focal_Distance(bool Focusing_Or_Scanning);//����ĳ�Ų�Ƭ�Ļ�ȡ�ľ۽�ֵ,�������ΪTrueʱ��ʾ�����Զ��۽�ʱ�Ľ���ֵ��������Ǳ���ɨ���Ľ���ֵ
	int Do_Search_Optimal_Focal_Distance();//Ѱ����ѽ���ֵ
	int Do_Resetting(int Motor_ID, bool fast_slow=true);//X,Y�Ḵλ��ʼ
	int Do_Resetting_Finished(int Motor_ID);//X,Y�Ḵλ���
	int Do_PUSIRobot_Resetting(int Motor_ID);
	int Do_Slide_Start_Postion();//�˶�����ʼ��ʼɨ���λ��
	int Do_Slide_Start_Postion_Finished();//�˶�����ʼ��ʼɨ���λ�����

	int Do_Move_To_Object(int Object_Num, int Dir = 1, int Speed = 4000);//�˶�����Ӧ���ﾵ
	int Do_Move_To_Object_Fininshed();//�ﾵ�˶����
	int Do_Capture_Image();//ץȡ��Ƭ�����������Ҫ�������ר�ű�д
	int Do_Save_Image();//��Ҫ���ڱ���ͼƬ
	virtual int Do_Focusing_Model();
	virtual int Do_Scanning_Model();
	virtual int Do_restting_Model();

	static Control_Dev *s_Dev_Drivers;
	static Scanning_Para s_Scanning_Para;
	static Scanning_System_Para s_Scanning_System_Para;
	static User_Para s_User_Para;
	static CCriticalSection critical_section;
	static Scanning_10X_Para s_Scanning_10X_Para;//10Xɨ��Ĳ���

	bool Obtain_Optimal_Focal;//�����ѽ����Ϊtrue

protected:
	//enum Scanning_States m_Scanning_States;//
	double m_Focus_Step;
	long m_Current_Slide_Start_X;//��ǰ��Ƭ�Ŀ�ʼɨ����X�������꣬����λ��
	long m_Current_Slide_Start_Y;//��ǰ��Ƭ�Ŀ�ʼɨ����Y�������꣬����λ��
	long m_Scanning_Step_X;//X�᷽�򲽽��Ƕ�ֵ
	long m_Scanning_Step_Y;//X�᷽�򲽽��Ƕ�ֵ
	int m_Scanning_Num_X;//
	int m_Scanning_Num_Y;//
	long m_X_Moveto;//��ǰX������˶�λ��
	long m_Y_Moveto;
	long m_Z_Moveto;
	long m_T_Moveto;
	long m_5_Moveto;
	//�۽��þֲ�����
	unsigned int m_Focusing_Num_X; //X�᷽���Ͼ۽��ĵ���
	unsigned int m_Focusing_Num_Y; //Y�᷽���Ͼ۽��ĵ���

	//bool Scanning_OR_Forcuing;//�ж��ǽ���ɨ����������ǽ��о۽�����
	//vector<vector<long> > m_Focusing_Focus_Distance;//������޸��۽����Զ��۽���õĽ���ֵλ��ֵ,�ϵ��û�����壬
	//vector<vector<long> > m_Scanning_Focus_Distance;//���ÿ��ɨ���Ľ���λ��ֵ,�ϵ��û�����壬��Ҫ���»�ȡ
	bool m_Respond_Right;//���ڶԷ���ָ���ͨ�����صı����жϸ�ָ���Ƿ���������ȷ���գ�
	int m_Respond_Check_Num;//���صı�����Ӧ�Ĳ�ѯ������¼���൱��һ��ʱ�ӣ�
};

