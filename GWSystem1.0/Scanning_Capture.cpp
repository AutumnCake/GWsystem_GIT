#include "stdafx.h"
#include "Scanning_Capture.h"

//�����߳�ͬ���� �ٽ���
CCriticalSection Scanning_Capture::critical_section;

Control_Dev *Scanning_Capture::s_Dev_Drivers = NULL;
Scanning_Para Scanning_Capture::s_Scanning_Para = {};// { 0, 0, true, 3, 0, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0, 0, 0, 0, 0};//{ cvPoint(0,0) }
Scanning_System_Para Scanning_Capture::s_Scanning_System_Para = { { -94000, 231000, 58000, -2550 }, "192.168.1.5", 5000, 
 0.05, { 2000, 300, 800 }, 25, 55, 3.4, { 0, 1250, 2150 * 2, 2150 * 3, 2150 * 4, 2150 * 5 },20,
 11235, 11235, -300, 500, 150000, 3500, 0.5, 10, 45000 ,-50000,10000,12170,12405,12220,400,3};
//�ֶ�ҡ�ˣ��ɶ���ҡ�ˣ�14030,12176,12284��200
//�����λƫ������������λ�ã���{ -87000, 231000, 58000, -33950 }
//����40X���о۽���ʱ����������Ϊ��{ 600, 300, 200 }�����㷨�У�ֻ�е������۽���ֻ�õ������һλ������200��
//����10X���о۽���ʱ����������Ϊ��{ 2000, 1000, 800 }�����㷨�У�ֻ�е������۽���ֻ�õ������һλ������800��
//17, 35
User_Para Scanning_Capture::s_User_Para = { 8, 20, 40, 10, 20, 4, 6, true };//{ 8, 15, 25, 50, 80, 6, 9, true };//{ 8, 9, 9, 30, 30, 4, 4, true }
Scanning_10X_Para Scanning_Capture::s_Scanning_10X_Para = { 5, 8,15, 200 ,5};

Scanning_Capture::Scanning_Capture()
{

	 InitializeCriticalSection(critical_section);//���ٽ��� �������г�ʼ��
	 s_Scanning_Para = { 0, 0, true, 3, 0, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0, 0, 0, 0, 0 };//{ cvPoint(0,0) }

	 m_Current_Slide_Start_X = ((s_Scanning_System_Para.Space_Slides + s_Scanning_System_Para.Slide_Size_X)*s_Scanning_Para.Current_Slide_Num
		 + s_User_Para.Scanning_Aero_X / 2) * 10000;
	 m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
	 m_Scanning_Num_X = s_User_Para.Scanning_Num_X;
	 m_Scanning_Num_Y = s_User_Para.Scanning_Num_Y;
	 m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (s_User_Para.Scanning_Num_X - 1)) * 10000;
	 m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];

	 //�۽��þֲ�����
	 m_Focusing_Num_X=2; //X�᷽���Ͼ۽��ĵ���
	 m_Focusing_Num_Y=2; //Y�᷽���Ͼ۽��ĵ���
	//ע������X�᷽���ɨ�貽���Ǹ���ɨ������X�᷽��ĵ��������ˣ����ɨ��������Ҫ������Χ
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


	//Ĭ�ϳ�ʼ��������Ž���λ�ƵĶ�ά��������
	
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
//������һ����ɨ��״̬
void Scanning_Capture::Set_States(Scanning_States State)
{
	return State;
}

//Ѱ����ѽ���ֵ
enum Scanning_States Scanning_Capture::Get_States()
{
	return m_Scanning_States;
}
*/

/*ȡĳ�Ų�Ƭ���Ѿ���ȡ���Զ��۽�ʱ�Ľ���ֵ
1������ҵ���ѽ��࣬�򷵻�true�����򷵻� false

*/
int  Scanning_Capture::Do_Search_Optimal_Focal_Distance()
{

	//*******************���Ƚ���ץͼ����*****************************//



	//************************//
	cout << "Ѱ����ѽ���ֵ" << endl;

	/***************�������������ѽ�����㷨��������ɽ���㷨*********************/
	//˵��������ҵ�����ѽ��࣬��ôֱ�ӽ������ѽ��࣬��ֵ���Զ��۽���Ӧ�ľ�����ȥ
	bool Obtain_Best_focal=false;//���������ж��У��õ���ǰ��ͼ��ʱ�����µ�ͼ����ô���bool��������ֵΪ��



	Obtain_Best_focal = true;


	/******************�����ǻ�ȡ����ѽ���󣬽��е�ϵ�в�������ȡ��ǰ��λ�ã����浱ǰץȡ��ͼ��***********************/
	if (Obtain_Best_focal)
	{

		long Z_Real_Pos;//��ǰ��ѽ���ֵ��
		s_Dev_Drivers->Dev_Read_Pos({ 3 }, Z_Real_Pos);
		s_Scanning_Para.Focal_Length[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//���浱ǰ�Ľ���ֵ
		s_Dev_Drivers->Dev_Read_Pos({ 1 }, Z_Real_Pos);
		s_Scanning_Para.X_Focus_Location[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//���浱ǰ�������X����ֵ
		s_Dev_Drivers->Dev_Read_Pos({ 2 }, Z_Real_Pos);
		s_Scanning_Para.Y_Focus_Location[s_Scanning_Para.Current_Point_Num] = Z_Real_Pos;//���浱ǰ�������Y����ֵ


		//long gg = Z_Real_Pos[0];
		//int jj = 0;

		//****************���۽�Z�Ჽ����������Ϊ�ֲ���*******************//
		m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
		//*********************************//

		return State_Finished;
	}
	else
	{
		//****************���۽�Z����һ���������й�����Ԥ�� *******************//

		m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[2];

		//*********************************//

		return State_Freeze;
	}
		
}

//��ȡĳ�Ų�Ƭ���Ѿ���ȡ���Զ��۽�ʱ�Ľ���ֵ
long Scanning_Capture::Read_Focal_Distance(int Next_Slide_Num)
{

	cout << "��ȡһ�Ų�Ƭ���Զ��۽��Ľ���ֵ"<<endl;


	return 0;
}
//����ĳ�Ų�Ƭ�Ļ�ȡ�ľ۽�ֵ,�������ΪTrueʱ��ʾ�����Զ��۽�ʱ�Ľ���ֵ��������Ǳ���ɨ���Ľ���ֵ
int  Scanning_Capture::Save_Focal_Distance(bool Focusing_Or_Scanning)
{
	if (Focusing_Or_Scanning)
	{
		cout << "����һ�Ų�Ƭ�Զ��۽���õĽ���ֵ" << endl;

	}
	
	else
	{
		cout << "����һ�Ų�Ƭ�Զ�ɨ���Ľ���ֵ" << endl;


	}
	return 0;
}

/*�������޾۽�λ��ֵ������ɨ���Ľ���λ��ֵ**************/
void Scanning_Capture::Calculate_Focal_Distance(int Scan_Index_Row, int Scan_Index_Col)
{
	// ��ɨ�������µĵ�I���±����� ӳ�䵽��֪�ľ۽�λ�������µ��±귶Χ��
	double Ref_Index_Row = (double)(Scan_Index_Row * (m_Focusing_Num_Y - 1)) / (m_Focusing_Num_Y - 1);
	double Ref_Index_Col = (double)(Scan_Index_Col * (m_Focusing_Num_X - 1)) / (m_Focusing_Num_X - 1);
	int Focus_Index_Row_Low, Focus_Index_Row_High;//����۽�ֵ�����£�X����ģ�ӳ���±긽����ǰ��ʵ������ֵ
	int Focus_Index_Col_Low, Focus_Index_Col_High;//����۽�ֵ�����£�Y����ģ�ӳ���±긽����ǰ��ʵ������ֵ
	//��ȡ��Χ���ĸ�����±�ֵ����
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

	/*���Եõ�Ŀ��㣨s_Scanning_Para.Current_Scanning_I_X��s_Scanning_Para.Current_Scanning_J_Y��
	���ñ��±�Ϊ��Focus_Index_X_Low��Focus_Index_Y_Low������Focus_Index_X_Low��Focus_Index_Y_High����
				��Focus_Index_X_High��Focus_Index_Y_Low������Focus_Index_X_High��Focus_Index_Y_High����
				���ĸ����Χ��
	*/
	//Ȼ��������ĸ����Ӧ��ʵ������
	double Scan_Space_X = s_User_Para.Scanning_Aero_X / s_User_Para.Scanning_Num_X; //ɨ����X�᷽����
	double Scan_Space_Y = s_User_Para.Scanning_Aero_Y / s_User_Para.Scanning_Num_Y; //ɨ����X�᷽����
	double Focal_Space_X = s_User_Para.Scanning_Aero_X / m_Focusing_Num_X; //�۽����X�᷽����
	double Focal_Space_Y = s_User_Para.Scanning_Aero_Y / m_Focusing_Num_Y; //�۽����X�᷽����
	/*��������ϵ����Χ���ĸ����ʵ������Ϊ��X,Y��ƽ���ϣ���0��0������0��Focal_Space_Y��
	��Focal_Space_X��0������Focal_Space_X��Focal_Space_Y����
	*/
	//Ŀ�����������ϵ�е�ʵ������Ϊ
	double Cardiate_Coords_X = Scan_Index_Col* Scan_Space_X - Focus_Index_Col_Low * Focal_Space_X;
	double Cardiate_Coords_Y = Scan_Index_Row* Scan_Space_Y - Focus_Index_Row_Low * Focal_Space_Y;
	/*
	//�ж�Ŀ����Ƿ�������һ�������Χ�ڣ������������Χ�����ø���֪��Ľ���λ��ֵ��ΪĿ���Ľ���λ��ֵ
	if ((Cardiate_Coords_X <s_Scanning_System_Para.Focal_Domain_Radius)//����������һ��������������η�Χ�ڣ���ֱ���õ�һ�㽹��λ��ֵ
		
		& (Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << " ��һ    ����ɨ�轹�� " << m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low] << endl;
		return;
	}

	if ((Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//ͬ���ڵڶ��㷶Χ��
		& (Focal_Space_Y - Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  �ڶ�     ����ɨ�轹��" << m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High] << endl;
		return;
	}

	if ((Focal_Space_X - Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//ͬ���ڵ����㷶Χ��
		& (Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << " ����     ����ɨ�轹�� " << m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low] << endl;
		return;
	}

	if ((Focal_Space_X - Cardiate_Coords_X < s_Scanning_System_Para.Focal_Domain_Radius)//ͬ���ڵ��ĵ㷶Χ��
		& (Focal_Space_Y - Cardiate_Coords_Y < s_Scanning_System_Para.Focal_Domain_Radius))
	{
		m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] =
			m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High];
		cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  ����     ����ɨ�轹��" << m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High] << endl;
		return;
	}

	//���Ŀ��㲻�����ĸ���Χ����֪����κ�һ��������������η�Χ֮�ڣ���������ĸ������Ŀ���Ľ���λ��ֵ

	//����Ŀ����ڵ�һ����͵ڶ�������ɵ����ϵ�ͶӰ��Ľ���λ��ֵ
	long Shadow_1 = (m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_High] - m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low])
		*Cardiate_Coords_Y / Focal_Space_Y + m_Focusing_Focus_Distance[Focus_Index_Row_Low][Focus_Index_Col_Low];
	//����Ŀ����ڵ�������͵��ĸ�����ɵ����ϵ�ͶӰ��Ľ���λ��ֵ
	long Shadow_2 = (m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_High] - m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low])
		*Cardiate_Coords_Y / Focal_Space_Y + m_Focusing_Focus_Distance[Focus_Index_Row_High][Focus_Index_Col_Low];
	//�����Ŀ���ľ۽�λ��ֵ��С
	m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] = (Shadow_2 - Shadow_1)*Cardiate_Coords_X / Focal_Space_X + Shadow_1;
	cout << m_Scanning_Focus_Distance[Scan_Index_Row][Scan_Index_Col] << "  ����     ����ɨ�轹��" << endl;
	*/

}

/*ɨ���ʼ������

Control_Dev Dev_Drivers��һ���˶����ƶ���
ScanningPara������Ĳ�����ɨ������ṹ��
bool Communication_Successed������λ����ͨ���Ƿ����ɹ���ͨ���ڴ����ʱ���ͻὨ��һ���Զ�����ͨ�ţ����û�н���ͨ�ţ����
                              �������Զ����Խ���ͨ�š�
 ����ֵ���⣺�������ͨ��ʧ�ܣ��򷵻�Ϊ-1��ͨ�ųɹ�����Ϊ0��

*/
int Scanning_Capture::Do_Scanning_intial()
{
	m_Respond_Right = false;
	m_Respond_Check_Num = 0;
	//long m_X_Moveto = 0;//��ǰX������˶�λ��
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

	//���Ȼ��ڵ�ǰ�ﾵ���ͣ��Լ�ɨ�������С
	//ϵͳ�������Ѿ�������10X��һ����Ұ�Ŀ��������
	s_User_Para.Scanning_Num_Y = s_User_Para.Scanning_Aero_Y * s_Scanning_System_Para.Pulse_ratio / Real_View_Field_Higth + 1;
	s_User_Para.Scanning_Num_X = s_User_Para.Scanning_Aero_X * s_Scanning_System_Para.Pulse_ratio / Real_View_Field_Width + 1;


	m_Respond_Check_Num = 0;
	if (s_Scanning_Para.Forcuing_OR_Scanning)//������Զ��۽�����
	{
		m_Scanning_Num_X = m_Focusing_Num_X;
		m_Scanning_Num_Y = m_Focusing_Num_X;
	    m_Focus_Step = s_Scanning_System_Para.Focusing_Step_Z[0];
		m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (m_Scanning_Num_X - 1)) * 10000;
		//ע������X�᷽���ɨ�貽���Ǹ���ɨ������X�᷽��ĵ��������ˣ����ɨ��������Ҫ������Χ
		m_Scanning_Step_Y = -(s_User_Para.Scanning_Aero_Y / (m_Scanning_Num_Y - 1)) * 10000;
	}
	else
	{	//ʵ���ϣ�m_Scanning_Step_X��ɨ��������û�õ��ģ������ʾ���Ǿ۽�ʱ�á�
		m_Scanning_Num_X = s_User_Para.Scanning_Num_X;
		m_Scanning_Num_Y = s_User_Para.Scanning_Num_Y;
    	m_Scanning_Step_X = -(s_User_Para.Scanning_Aero_X / (m_Scanning_Num_X - 1)) * 10000;
		//ע������X�᷽���ɨ�貽���Ǹ���ɨ������X�᷽��ĵ��������ˣ����ɨ��������Ҫ������Χ
		m_Scanning_Step_Y = -(s_User_Para.Scanning_Aero_Y / (m_Scanning_Num_Y - 1)) * 10000;
		s_Scanning_Para.X_Scanning_Location.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
		s_Scanning_Para.Y_Scanning_Location.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
		s_Scanning_Para.Scanning_Length.resize(s_User_Para.Scanning_Num_Y*s_User_Para.Scanning_Num_X);
	}

	
	//���¼��
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
		int err = s_Dev_Drivers->CANET_Init(s_Scanning_System_Para.CANET_ID, s_Scanning_System_Para.CANET_Port_Num);  //����ͨ��
		if (err)
		{
			MessageBox(NULL, TEXT("��CAN���ؽ���ͨ��ʧ��!����"), TEXT("������ʾ"), 0);
			return Scanning_Finished;
		}
		else
		{
			//������һ�����˶�״̬�����и�λ����
			return State_Finished;
		}

	}
	else
	{
		//������һ�����˶�״̬�����и�λ����
		return State_Finished;
	}
	//����ر������г�ʼ������



	/*����Ϊ���Դ���***********************************************************************************************/
	/*
	s_Dev_Drivers.Dev_heart_beat(1, FALSE);//�رձ���
	s_Dev_Drivers.Dev_heart_beat(2, FALSE);//�رձ���
	s_Dev_Drivers.Dev_heart_beat(3, FALSE);//�رձ���
	s_Dev_Drivers.Dev_ENA(3, 1);//3��ʹ��
	s_Dev_Drivers.Dev_Set_Zero_Postion(3);
	vector<long> Real_Pos = {12};
	s_Dev_Drivers.Dev_Read_Pos({ 3 }, Real_Pos);
	long postion3 = Real_Pos[0];
	cout <<Real_Pos[0] << endl;
	s_Dev_Drivers.Dev_Single_Mov(3, 20000, 5, true, true);
	s_Dev_Drivers.Dev_Read_Pos({ 3 }, Real_Pos);
	postion3 = Real_Pos[0];
	cout << Real_Pos[0] << endl;
	/*����Ϊ���Դ���*********************************************************************************************/

}
/*��λ����
*/
int Scanning_Capture::Do_Resetting(int Motor_ID,bool fast_slow)
{
	s_Dev_Drivers->Dev_clear_alarm(1);//�������
	s_Dev_Drivers->Dev_clear_alarm(2);//�������
	s_Dev_Drivers->Dev_clear_alarm(3);//�������

	s_Dev_Drivers->Dev_clear_alarm(4);//�������
	s_Dev_Drivers->Dev_clear_alarm(5);//�������

	s_Dev_Drivers->Dev_heart_beat(1, FALSE);//�رձ���
	s_Dev_Drivers->Dev_heart_beat(2, FALSE);//�رձ���
	s_Dev_Drivers->Dev_heart_beat(3, FALSE);//�رձ���
	s_Dev_Drivers->Dev_heart_beat(4, FALSE);//�رձ���
	s_Dev_Drivers->Dev_heart_beat(5, FALSE);//�رձ���
	s_Dev_Drivers->Dev_ENA(1, 1);//1��ʹ��
	s_Dev_Drivers->Dev_ENA(2, 1);//2��ʹ��
	s_Dev_Drivers->Dev_ENA(3, 1);//3��ʹ��
	s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);//3��ʹ��
	s_Dev_Drivers->Dev_PUSIRobot_ENA(5, 0);//3��ʹ��
	//s_Dev_Drivers->Dev_ENA(4, 0);//3��ʹ��
	//s_Dev_Drivers->Dev_ENA(5, 1);//3��ʹ��

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
		
		s_Dev_Drivers->Dev_Home({ 1, 2 }, Home_offset);//���и�λ������ֻ��1�����2����ĸ�λ����
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
/*��ѯ��λ��ɺ���
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

	Dap_01 = (Dap_01 || (status_arr[0]));//20��ʾ������ɣ�4��ʾ����˶���λ
	Dap_02 = (Dap_02 || (status_arr[1]));//20��ʾ������ɣ�4��ʾ����˶���λ
	if (Dap_01 & Dap_02)
	{
		if (Motor_ID == 4) s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);
		return State_Finished;//��⵽��λ��ɣ�������һ����״̬�������жϷ�֧���ؼ���֧
	}
	else
	{
		return State_Freeze;//��⵽��λû����ɣ�������һ����״̬�����и�λ��ɲ�ѯ
	}
	
}

/*�˶���һ�Ų�Ƭ�ĳ�ʼɨ��㣬�����˶�����ʷɨ���,�˶���ɣ�
void Do_Slide_Start_Postion_Finished()
*/

int Scanning_Capture::Do_Slide_Start_Postion()
{
	if (!s_User_Para.NewStart_OR_BreakPoint)
	{//����Ƕϵ㿪ʼ����X\Y\Z�ĳ�ʼ�˶�λ�ƣ���������ǰ�͸�����
		s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
		s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);
		s_User_Para.NewStart_OR_BreakPoint = true;
		return Move_To_Slide_Start_Postion_Finished;//ת�������ɵķ�֧
	}
	if (s_Scanning_Para.Current_Slide_Num == 0)//�˶�����һ�Ų�Ƭ�����ɨ��λ��
	{
		//m_Current_Slide_Start_X = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		//m_Current_Slide_Start_Y = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		m_X_Moveto = (s_User_Para.Scanning_Aero_X / 2) * 10000;
		m_Y_Moveto = (s_User_Para.Scanning_Aero_Y / 2) * 10000;
		s_Dev_Drivers->Dev_Single_Mov(1, m_X_Moveto, 5, true);
		s_Dev_Drivers->Dev_Single_Mov(2, m_Y_Moveto, 5, true);

		if (!s_Scanning_Para.Forcuing_OR_Scanning)
			Read_Focal_Distance(s_Scanning_Para.Current_Slide_Num);//������Զ�ɨ�裬���ȶ�ȡ��һ�Ų�Ƭ��һֱ����ֵ

	}
	else//�˶�����I�Ų�Ƭ���ߵ�ɨ��λ��
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

	return Move_To_Slide_Start_Postion_Finished;//ת�������ɵķ�֧

}

/*��ѯ�˶���һ�Ų�Ƭ��ɨ�����
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
	Dap_01 = (Dap_01 || (status_arr[0]));//20��ʾ������ɣ�4��ʾ����˶���λ
	Dap_02 = (Dap_02 || (status_arr[1]));//20��ʾ������ɣ�4��ʾ����˶���λ
	if (Dap_01 & Dap_02)
	{
		return State_Finished;

	}
	else
	{
		return State_Freeze;//����Ƿ�λ��û�е�λ���½��뱾��֧���������
	}*/
}

/*�ж�ɨ���Ƿ���ɣ�����
*/
int Scanning_Capture::Do_Is_Scanning_Finished()
{

	//int Num_X, Num_Y;

	s_Scanning_Para.Current_Scanning_Num_Y = s_Scanning_Para.Current_Point_Num / m_Scanning_Num_X;
	s_Scanning_Para.Current_Scanning_Num_X = s_Scanning_Para.Current_Point_Num % m_Scanning_Num_X;

		if (s_Scanning_Para.Current_Scanning_Num_Y < m_Scanning_Num_Y)//�жϵ�ǰ�Ĳ�Ƭ�Ƿ�ɨ�����
		{
			if (s_Scanning_Para.Current_Scanning_Num_X == 0)//�жϵ�ǰX�᷽��ɨ���Ƿ����
			{
				m_Scanning_Step_X = -m_Scanning_Step_X;
				return Y_Stepping;////X�᷽��ɨ���Ѿ���ɣ���ʼ����һ��Y����λ��
			}
			else
			{				
				return X_Stepping;//X�᷽��û����ɣ�����ת��X�᷽��ɨ����ȥ
			}
		}
		
		else//��ǰ��Ƭɨ�����
		{
			/*�˴���ӱ���Ͷ�ȡ�۽�ֵ�ĺ���*/
			Save_Focal_Distance(s_Scanning_Para.Forcuing_OR_Scanning);//�����Զ��۽����Զ�ɨ���ľ۽�ֵ�����Ų�Ƭ����
			/*******************************/
			return Scanning_Finished;//�Զ��۽�ʱֻ��۽�һ�Ų�Ƭ
	
		}

}

/*X�Ჽ���˶�
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
		//s_Dev_Drivers->Dev_Set_Zero_Postion(5);//���Ƚ�������
		//Sleep(100);
		m_5_Moveto =  Motor_Postion;
		s_Dev_Drivers->Dev_Single_Mov(Motor_ID, m_5_Moveto, s_Scanning_System_Para.Pump_Oil_Vel, false);
		break;
	default:
		break;
	}
		
	return State_Finished;
	
}
/*X�Ჽ���˶�,����Ƿ�����˶�
����ֵΪ��State_Freeze����ʾ��ǰû��ѯ���˶���ɣ�
	State_Finished����ʾ��ѯ���˶����
	Stepping����ʾ��ѯ���󣬽������·���֮ǰ���˶�ָ������²�ѯ
*/
int Scanning_Capture::Do_Stepping_Finished(int Motor_ID,long Move_to)
{

	if (m_Respond_Check_Num < 200)//�������������ر���
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
			s_Dev_Drivers->Status_Ref = false;//******��ӵ��˶���ɱ�־λ//
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
	else//��������˱��Ĳ�ѯ����������û�в鵽�ϴ�ָ��ͺ�����������Ӧ���ģ���ô������ִ���ϴ�ָ��
	{
		m_Respond_Right = false;
		m_Respond_Check_Num = 0;
		//���½�������
		//s_Dev_Drivers->CANET_Init("192.168.1.5", 5000);
		//Sleep(20);

		return Stepping;
		//cout << "�ظ���һ��"<<endl;
	}

}

/*Y�Ჽ���˶�*/
/*
int Scanning_Capture::Do_Y_Stepping()
{
	s_Dev_Drivers->Dev_Single_Mov(2, m_Scanning_Step_Y);//
	return Y_Stepping_Finished;
}
/*Y�Ჽ���˶�,����Ƿ�����˶�
*/
/*
int Scanning_Capture::Do_Y_Stepping_Finished()
{
	bool Dap_02= FALSE;
	vector<bool> status_arr = { 0, 0 };
	s_Dev_Drivers->Dev_Read_Status({ 2 }, status_arr, 1);
	Dap_02 = (Dap_02 || (status_arr[0]));//20��ʾ������ɣ�4��ʾ����˶���λ
	if (Dap_02)
	{
		s_Scanning_Para.Current_Scanning_I_X = 0;
		m_Scanning_Step_X = -m_Scanning_Step_X;//����X����˶�λ�ƺͷ���
		return State_Finished;

	}
	else
	{
		return Y_Stepping_Finished;
	}
}

/*Z�Ჽ���˶�*/
/*
int Scanning_Capture::Do_Z_Stepping()
{
	if (s_Scanning_Para.Forcuing_OR_Scanning)
	{
		///*--------------------һ����Ϊ�Զ��۽�ʱ��Z���˶�------------------------------------
		//����Z��������λ���˶�����ʵ���𲽵��Զ��Խ���������Ҫע�⣬��Ҫ��Search_Optimal_Focal_Distance()�����У�Ԥ��ָ�������ķ���

		s_Dev_Drivers->Dev_Single_Mov(3, s_Scanning_Para.Focusing_Step_Z, 5, false, true);
		///*-----------------------------------------------------------------------------------------------
	}
	else
	{
		///*--------------------һ����Ϊ�Զ�ɨ���Z���˶�-------------------------------------//
		//���¿�ʼ����Z����Ҫ�˶���λ�ƴ�С�������Ƕ�ɨ��ʱ��Z���˶����������Ǿ����˶�������
		int Scanning_Dic = pow(-1, s_Scanning_Para.Current_Scanning_J_Y);//�жϷ���ɨ��״̬����ʱ�����±��ǵݼ���
		int j;
		if (Scanning_Dic<0)
			j = s_Scanning_Para.Scanning_Num_X + s_Scanning_Para.Current_Scanning_I_X * Scanning_Dic - 1;
		else
			j = s_Scanning_Para.Current_Scanning_I_X * Scanning_Dic;

		Calculate_Focal_Distance(s_Scanning_Para.Current_Scanning_J_Y, j);//�����˶����õ�Ľ���ֵ

		s_Dev_Drivers->Dev_Single_Mov(3, m_Scanning_Focus_Distance[s_Scanning_Para.Current_Scanning_J_Y][j], 5, true, true);
		///*-----------------------------------------------------------------------------------------------//
	}

	return Z_Stepping_Finished;

}
/*Z�Ჽ���˶����*/
/*
int Scanning_Capture::Do_Z_Stepping_Finished()
{
	bool Dap_03 = FALSE;
	vector<bool> status_arr = { 0 };
	s_Dev_Drivers->Dev_Read_Status({ 3 }, status_arr, 1);
	Dap_03 = (Dap_03 || (status_arr[0]));//20��ʾ������ɣ�4��ʾ����˶���λ
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
/*ץȡͼ��*/

int Scanning_Capture::Do_Capture_Image()
{

	cout << "��ǰ�����ͼƬ�ǵ� " << s_Scanning_Para.Current_Slide_Num << " ��Ƭ���� " << s_Scanning_Para.Current_Point_Num % s_User_Para.Scanning_Num_X
		<< " ��, �� " << s_Scanning_Para.Current_Point_Num / s_User_Para.Scanning_Num_X << " �е�ͼ��" << endl;

	/* ���ڴ˴������Ҫץͼ�Ĵ���*/
	
	
	
	
	
	
	/*+++++++++++++++++++++++++++++++*/

	return State_Finished;//���㡢����


}

/*����ͼ��*/
int Scanning_Capture::Do_Save_Image()
{

	cout << "���浱ǰͼƬ " << endl;

	/* ���ڴ˴������Ҫץͼ�ͱ���ͼƬ�Ĵ���
	/*********�����Զ��۽�ģʽ�£�һ��ֻ���ڻ�ȡ�����ͼ��ʱ�Ŷ�ͼ����б��棻*************





	*/

	if (s_Scanning_Para.Forcuing_OR_Scanning)   /*****������Զ��۽��������һ�²�����******/
	{


			/* ����Ӧ�����������ޣ�Z �˶��ķ�Χ���� */
			return Z_Stepping;


	}
	else    //************������Զ�ɨ�裬�����һ�²���**********//
	{

		return Is_Scanning_Finished;//���㡢������ɣ�ת�Ƶ��˶��жϷ�֧
	}

}


/***************************************
Move_To_Object:�˶�����Ӧ���ﾵ
int Object_Num:Ŀ���ﾵ��ţ�0�ű�ʾ4����
int Dir���˶��ķ���
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
Move_To_Object_Fininshed:�˶�����Ӧ���ﾵ

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

/*ɨ�����
*/
bool Scanning_Capture::Do_Scanning_Finished()
{
	s_Dev_Drivers->Dev_ENA(1,0);//1��ʹ��
	s_Dev_Drivers->Dev_ENA(2, 0);//2��ʹ��
	s_Dev_Drivers->Dev_ENA(3, 0);//3��ʹ��
	s_Dev_Drivers->Dev_ENA(4, 0);//2��ʹ��
	s_Dev_Drivers->Dev_ENA(5, 0);//3��ʹ��
	return TRUE;
}

int Scanning_Capture::Do_Focusing_Model()
{
	Do_restting_Model();//�ָ�λ

	int Back_Num;
	int states = Move_To_Object;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******�˶�������ɨ��/�۽���Ҫ�õ����ﾵ******//
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished ��ʾ��ѯ�����״̬
				states = Move_To_Slide_Start_Postion;
			if (Back_Num == State_Freeze)//State_Freeze��ʾ���β�ѯû�в�ѯ�����״̬
				states = Move_To_Object_Fininshed;
			break;
			//******�˶���ɨ��ĵ�һ����λ��******//
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
			//*************�Զ��۽�*************//
		case Search_Optimal_Focal_Distance:
			Back_Num = Do_Search_Optimal_Focal_Distance();
			if (Back_Num == State_Finished)
			{
				s_Scanning_Para.Current_Point_Num++;//��ʾ��ǰ���Ѿ�������ɣ�ɨ�������Լ�1
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
			//************�ж�*****************//
		case Is_Scanning_Finished:
			states = Do_Is_Scanning_Finished();
			break;
			//*************�˶�����һ����*******************//
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
			//******��λ���̣�����Z�Ḵλ��Ȼ���ﾵת�̸�λ�����X,Yͬʱ��λ******//
		case Resetting_Z:
			Back_Num = Do_Resetting(3,true);//���ٸ�λ
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
			Back_Num = Do_Resetting(3,false);//���ٸ�λ
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
			if (Back_Num == State_Finished)//State_Finished ��ʾ��ѯ�����״̬
				states = Scanning_Finished;
			if (Back_Num == State_Freeze)//State_Freeze��ʾ���β�ѯû�в�ѯ�����״̬
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
	Do_restting_Model();//�ָ�λ

	int Back_Num;
	int states = Move_To_Object;
	while (states != Scanning_Finished)
	{
		switch (states)
		{
			//******�˶�������ɨ��/�۽���Ҫ�õ����ﾵ******//
		case Move_To_Object:
			states = Do_Move_To_Object(s_Scanning_Para.Objective_Num);
			break;
		case Move_To_Object_Fininshed:
			Back_Num = Do_Move_To_Object_Fininshed();
			if (Back_Num == State_Finished)//State_Finished ��ʾ��ѯ�����״̬
				states = Move_To_Slide_Start_Postion;
			if (Back_Num == State_Freeze)//State_Freeze��ʾ���β�ѯû�в�ѯ�����״̬
				states = Move_To_Object_Fininshed;
			break;
			//******�˶���ɨ��ĵ�һ����λ��******//
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
			//*************�Զ�ɨ��*************//
		case Capture_Image:
			Back_Num = Do_Capture_Image();
			if (Back_Num == State_Finished)
			{
				s_Scanning_Para.Current_Point_Num++;//��ʾ��ǰ���Ѿ�������ɣ�ɨ�������Լ�1
				states = Is_Scanning_Finished;
			}

			break;
		case Z_Stepping:
			/******��һ�������Զ�����ĺ���******/

			/********************************/
			
			Back_Num = Do_Stepping(3, s_Scanning_Para.Current_Point_Num);//������
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
			//************�ж�*****************//
		case Is_Scanning_Finished:
			states = Do_Is_Scanning_Finished();
			break;
			//*************�˶�����һ����*******************//
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

	s_Dev_Drivers->Send_command("2F20600000000000", F_id, 0, 0);//ֹͣ����
	s_Dev_Drivers->Dev_PUSIRobot_ENA(Motor_ID, 0);//3��ʹ��
	Sleep(100);
	s_Dev_Drivers->Dev_heart_beat(Motor_ID, FALSE);//�رձ���
	s_Dev_Drivers->Dev_heart_beat(5, FALSE);//�رձ���
	s_Dev_Drivers->Dev_PUSIRobot_ENA(Motor_ID, 1);//3��ʹ��
	s_Dev_Drivers->Dev_PUSIRobot_ENA(5, 0);//3��ʹ��
	Sleep(300);
	long lk;
	vector<long>Home_offset;
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	s_Dev_Drivers->Dev_PUSIRobot_Set_Zero_Postion(Motor_ID);
	s_Dev_Drivers->Send_command("2F0F600101000000", F_id, 0, 0);//�����ⲿֹͣ1
	s_Dev_Drivers->Send_command("2F0F600101000000", F_id, 0, 0);//�����ⲿֹͣ1
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
	s_Dev_Drivers->Send_command("2F0F600100000000", F_id, 0, 0);//�����ⲿֹͣ1
	s_Dev_Drivers->Send_command("2F0F600100000000", F_id, 0, 0);//�����ⲿֹͣ1
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
	s_Dev_Drivers->Dev_PUSIRobot_ENA(4, 0);//3��ʹ��
	
	return State_Finished;

}