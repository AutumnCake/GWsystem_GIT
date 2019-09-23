
// GWSystem1.0.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <vector>
#include<string>
#include "UserLoad.h"


using std::vector;
using std::string;
// CGWSystem10App: 
// �йش����ʵ�֣������ GWSystem1.0.cpp
//


#define ITEMLENGTH 100

#define WM_MN_BTN_OK  WM_USER +102
#define  WM_USER_UPDATE WM_USER+103
#define  WM_USER_LOAD WM_USER+104
#define  WM_INIT_TABLEMN WM_USER+105
#define  WM_INIT_TABLECHRO WM_USER+106
#define  WM_USER_ScanSet WM_USER+107
#define  WM_USER_FocusSet WM_USER+108
#define  WM_USER_MNParaSet WM_USER+109
#define  WM_USER_CHROParaSet WM_USER+110
#define  WM_SENDTIMER WM_USER+111
#define  WM_HANDLEPATHCHRO WM_USER+112
#define  WM_WAITTOPRINTMN WM_USER+113
#define  WM_HANDLEPATH WM_USER+114
#define  WM_WAITTOPRINTCHRO WM_USER+115
#define  WM_USER_AdminFocusSet WM_USER+116
#define  WM_USER_AdminMotorSet WM_USER+117
#define  WM_USER_AdminScanSet WM_USER+118
#define  WM_DISPLAY_DRAWING WM_USER+119
#define  WM_PROGRESS_DRAW WM_USER+120
#define  WM_START_HANDOPERATION WM_USER+121
#define  WM_CURRENTPOSITION WM_USER+122

class CGWSystem10App : public CWinApp
{
public:
	CGWSystem10App();

	//�û�����ģ��
	CString convert1;//��¼��½���û���
	CString convert2;//��¼��¼�û������
	CString convert3; //��¼�޸�����
	CUserLoad dlg_login;    //������½�Ի������


// ��д
public:
	virtual BOOL InitInstance();

	//����ͼƬʱ�õ��Ĺ�������
	string WChar2Ansi(LPCWSTR pwszSrc);
	CString GetFileTitleFromFileName(CString FileName, BOOL Ext);
	string CString2string(CString csStrData);
	CString GetForwardPath_N(CString path,int forwardN);//��ȡ·����ǰN��
// ʵ��

	DECLARE_MESSAGE_MAP()
};


/********************************************ȫ�ֱ���**************************************************/



//�û���¼���ݲ���
struct User_Load//�û���¼���ݲ����ṹ��
{
	CString username;
	CString rank;
};

//��Ƭ��Ϣ���ýṹ��
struct SlideBuild{
	CString name;
	CString mode;
	CString range;
	CString remark;
};

struct SLIDE{
	CString path;
	vector<SlideBuild> slidebuild;
	vector <int > All_Slide_ID;//��¼ѡ�еĲ�Ƭ��
};

//���ɵ��ļ��нṹ��
struct MyFiles{
	string endsolve;//������������ʱ�䣩
	string picsolve;//ץͼ���
	string filesolve;//����������ʱ�䣩
	string quasolve;//�ϸ��ͼƬ
	string unqsolve;//���ϸ��ͼƬ
	string delesolve;//ɾ����ͼƬ
	string focussolve;//�۽���·��
	string scan10Xsolve;//�۽���·��
	string CApossolve;//�۽���·��

};

//һ�����˵�΢�˷���������ݼ�¼�ṹ��
struct  MN_HandleResult//΢�˷���������ݼ�¼�ṹ��
{
	CString patientname;//��������
	int picturesum = 0;//�ò��˵���Ƭ����
	int singlecell = 0;//����ϸ����
	int singlecell_wh = 0;//����΢�˵ĵ���ϸ����
	int singleMN = 0;//����ϸ��΢����
	int doublecell = 0;//˫��ϸ����
	int doublecell_wh = 0;//����΢�˵�˫��ϸ����
	int DoubleMN = 0;//˫��ϸ��΢����

	int multiplecell = 0;//���ϸ����
	int multiplecell_wh = 0;//����΢�˵Ķ��ϸ����
	int MultiMN = 0;//���ϸ��΢����

	int sumwh = 0;//΢������
	int sumcell = 0;//ϸ������

	int SingleCellsWithMN[20];
	int DoubleCellsWithMN[20];
	int TripleCellsWithMN[20];
	int MultiCellsWithMN[20];

	//������
	int SingleMNC_Num = 0;
	float SingleMNC_Rate = 0;
	int SingleMN_Num = 0;
	float SingleMN_Rate = 0;

	int DoubleMNC_Num = 0;
	float DoubleMNC_Rate = 0;
	int DoubleMN_Num = 0;
	float DoubleMN_Rate = 0;

	CString sourcefile;//ͼƬ�����ļ���
	CString resultfile;//����洢�ļ���

	int delPictureNum = 0;
};

//һ��С��Ƭ��΢�˷���������ݼ�¼�ṹ��
struct  MN_HandleResultOnePic//΢�˷���������ݼ�¼�ṹ��
{
	CString patientname;//��������
	CString picpath;//��Ƭ·��
	int NumCells = 0;//ϸ������
	int NumMicro = 0;//ϸ��΢����
	int CheckOrNot_NumCells = 0;
	int CheckOrNot_NumMicro = 0;

};

//һ�����˵�Ⱦɫ�����������ݼ�¼�ṹ��
struct CHRO_HandleResult //Ⱦɫ�����������ݼ�¼�ṹ��
{
	CString patientname;//��������
	CString sourcefile;//ͼƬ�����ļ���
	CString resultfile;//����洢�ļ���
	int picturesum = 0;//�ò��˵���Ƭ����
	int allcell = 0;//Ⱦɫ������ܵ�ϸ����
	int normalcell = 0;//����ϸ����
	int abnormal = 0;//������ϸ����
	int dic = 0;//Ⱦɫ�������˫��˿������
	int round = 0;//Ⱦɫ�������Ⱦɫ�廷��
	int ace = 0;//Ⱦɫ�����������˿����
	int t = 0;//Ⱦɫ��������໥��λ
	int inv = 0;//Ⱦɫ������ĵ�λ��
	int tri = 0;//
	int ten = 0;//
	int del = 0;
	int ctg = 0;
	int csg = 0;
	int ctb = 0;
	int cte = 0;

	double Y = 0.00;//Ⱦɫ������Ļ�����

};

//һ����Ƭ��Ⱦɫ�����������ݼ�¼�ṹ��
struct CHRO_HandleResultOnePic//Ⱦɫ�����������ݼ�¼�ṹ��
{
	CString patientname;//��������
	CString picpath;//��Ƭ·��
	int allcell = 0;//Ⱦɫ������ܵ�ϸ����
	int normalcell = 0;//����ϸ����
	int abnormal = 0;//������ϸ����
	int dic = 0;//Ⱦɫ�������˫��˿������
	int round = 0;//Ⱦɫ�������Ⱦɫ�廷��
	int ace = 0;//Ⱦɫ�����������˿����
	int t = 0;//Ⱦɫ��������໥��λ
	int inv = 0;//Ⱦɫ������ĵ�λ��
	int tri = 0;//
	int ten = 0;//
	int del = 0;
	int ctg = 0;
	int csg = 0;
	int ctb = 0;
	int cte = 0;

	int dic_rg = 0;//Ⱦɫ�������˫��˿������
	int round_rg = 0;//Ⱦɫ�������Ⱦɫ�廷��
	int ace_rg = 0;//Ⱦɫ�����������˿����
	int t_rg = 0;//Ⱦɫ��������໥��λ
	int inv_rg = 0;//Ⱦɫ������ĵ�λ��
	int tri_rg = 0;//
	int ten_rg = 0;//
	int del_rg = 0;
	int ctg_rg = 0;
	int csg_rg = 0;
	int ctb_rg = 0;
	int cte_rg = 0;

	int chromosome_num = 0;
	int chromosome_num_check = 0;
	int QuaOrNot = 0;  //0 ΪδУ��  1Ϊ�ϸ� -1Ϊ���ϸ�

	/*int chromNum = 0;*/
	int CheckOrNot = 0;//����Ƭ�Ƿ��Ѿ�У��
	CString CheckPath;
};

//�ȴ���ӡ����Ĳ��˽ṹ��
struct WaitToPrint
{
	CString patientname;
	string savepath;
};

typedef struct ALL_PAREMETER
{
	vector<long> Offset_arr; //��λ��ƫ������X���Y��() /// *2
	char *CANET_ID; //CAN���ص�ID/ *2
	int CANET_Port_Num; //CANͨ��ת��ģ���ID *2
	double Focal_Domain_Radius; //ĳ����֪����λ��ֵ�ĵ㣬��ĳ��δ֪�����������֪��뾶Ϊ��������ķ�Χ��ʱ������Ϊ
	//���δ֪��Ľ���������֪��Ľ���һ���� ��λ�Ǻ��� mm  *2
	int Focusing_Step_Z[3];//�۽�������0��1��2�ֱ�Ϊ��󲽳����еȲ�������С���� *2
	//ע������ĵ�λ����������
	//double Focusing_Step_Z_Max; //�۽��󲽳���ע������ĵ�λ����������
	//double Focusing_Step_Z_Min; //�۽�С������ע������ĵ�λ����������
	double Slide_Size_X; //��Ƭ��ʵ�ʴ�С�� *1
	double Slide_Size_y; //��Ƭ��ʵ�ʴ�С��*1
	double Space_Slides; //��Ƭ�벣Ƭ֮��ļ��*1
	vector<long> Nosepiece_Abs_Position; //�ﾵת�̵ľ���λ�� *2
	int Move_Deviation; //*2
	double View_Field_Width_10X;//40Xʱ�������������Ұ��С�Ŀ�ȣ���������
	double View_Field_Higth_10X;//40Xʱ�������������Ұ��С�ĸ߶ȣ���������


	int Slide_Max_Num;//һ��ɨ��Ĳ�Ƭ�������  * 0
	double Scanning_Aero_X; //ɨ������X����ĳ���  * 0
	double Scanning_Aero_Y; //ɨ������Y�����ϵĳ���  *0
	int Scanning_Num_X; //X�᷽����ɨ��ĵ���  *0
	int Scanning_Num_Y; //Y�᷽����ɨ��ĵ���  *0
	int Focusing_Num_X; //X�᷽���Ͼ۽��ĵ���  *0
	int Focusing_Num_Y; //Y�᷽���Ͼ۽��ĵ���  *0
	bool NewStart_OR_BreakPoint;//���¿�ʼ��ϵ㿪ʼ---------

	unsigned int Erode_element;//�Է�ֵ��ͼ����и�ʴ�ĽṹԪ��С�����ڽ�Ⱦɫ������ʴ�������·�Ⱦɫ������
	unsigned int Dilate_element;//�����Ƶ�Ⱦɫ������ͼ��������͵ĽṹԪ����������Ⱦɫ�������ϸС��������
	unsigned int ErodeDilate_element;//�����Ƶ�Ⱦɫ������ͼ����и�ʴ���ͣ�����Ⱦɫ������
	unsigned int Min_CA_Aero_10X;//����ϸС���������Ⱦɫ������ķ�ֵ
	unsigned int Min_CA_Num_10X;//Ⱦɫ��������Ⱦɫ������Ⱦɫ����������Сֵ

	int CA_XPos_Compensate;//Ⱦɫ�����ʱ����10X�¶�λ�õ�������������Ļ�������ͨ���趨�������������
	int CA_YPos_Compensate;//Y���򲹳�����
	int Switch_Objective_ZPos;//�л��ﾵʱ����β���

	int Pump_Oil_Pulse;//ÿ�α��͵ĵ����������
	float Pump_Oil_Vel;//���͵��ٶȣ�
	float Pump_Oil_Dis;//ÿ���͵�֮��ļ�ࣨmm��
	int Focus_Max_Limit;//����ֵ��������ƣ���ʾ�������е��ﾵ���඼�������Χ��
	int Focus_Min_Limit;//����ֵ���������
	float Pulse_ratio;//���������ϵ���ȣ�Ҳ����1mm��Ӧ����������pp/mm��
	int Focus_Equal_Dis;//�����㽹�౻��Ϊһ����������


	CString csExtension;//ͼ��ĸ�ʽ.bmp; .raw��
	CString savepath;//�ֶ�ץͼ��ͼ��ַ
	CString focuspath;//�۽�ͼƬ�ĵ�ַ
	//CString csDiretory;//�Զ�ץͼͼ��洢��·��
	ULONG CorrectionMatrix;
	ULONG DemosaicMethod;


} ALL_PAREMETER;

//����΢�˷�������
struct MNPARASET{
	CString MIN;
	CString EI;
	CString DR;
	CString EIXBL;
	CString DRXBL;
};

//����΢�˷�������
struct CHROSOMEPARASET{
	CString MINWIDTH;
	CString MAXWIDTH;
	CString MIDDLE;
	CString SINGLEMIN;
};

//ɨ���λ������
struct LOCATION{
	int x_all;//X�����ܵ���
	int y_all;//Y�����ܵ���
	int x_now;
	int y_now;
};

//�����Լ��˴η����������ͼƬ�ļ���·���Ľṹ��
struct PatientAndQuaImg
{
	CString patientname;
	CString QuestionImgPath;
};

//�����Լ���ǰ�����������ͼƬ����·���Ľṹ��
struct PatientAndCurImg
{
	CString patientname;
	string currentImgPath;
};  

/***************************************���ݿ�����ؽṹ��*************************************************/
//��Ƭ����
struct Table_SlideSet{
	CString path;
	CString name;
	CString mode;
	CString range;
	CString resultpath;
	BOOL HaveSolved;
	CString ImgSolvedNum;
	CString AllImgNum;
	COleDateTime time;
};

//�����벣Ƭ�ŵĶ�Ӧ��Ϣ
struct PatientWithSlide
{
	CString PatientName;
	vector<int> SlideID;
	vector<CString>ImgName;
	CString path;
	CString resultpath;
	CString mode;

};
extern CGWSystem10App theApp;