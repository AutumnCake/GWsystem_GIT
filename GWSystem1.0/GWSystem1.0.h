
// GWSystem1.0.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <vector>
#include<string>
#include "UserLoad.h"


using std::vector;
using std::string;
// CGWSystem10App: 
// 有关此类的实现，请参阅 GWSystem1.0.cpp
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

	//用户管理模块
	CString convert1;//记录登陆的用户名
	CString convert2;//记录登录用户的类别
	CString convert3; //记录修改日期
	CUserLoad dlg_login;    //建立登陆对话框对象


// 重写
public:
	virtual BOOL InitInstance();

	//处理图片时用到的公共函数
	string WChar2Ansi(LPCWSTR pwszSrc);
	CString GetFileTitleFromFileName(CString FileName, BOOL Ext);
	string CString2string(CString csStrData);
	CString GetForwardPath_N(CString path,int forwardN);//获取路径的前N级
// 实现

	DECLARE_MESSAGE_MAP()
};


/********************************************全局变量**************************************************/



//用户登录传递参数
struct User_Load//用户登录传递参数结构体
{
	CString username;
	CString rank;
};

//玻片信息设置结构体
struct SlideBuild{
	CString name;
	CString mode;
	CString range;
	CString remark;
};

struct SLIDE{
	CString path;
	vector<SlideBuild> slidebuild;
	vector <int > All_Slide_ID;//记录选中的玻片号
};

//生成的文件夹结构体
struct MyFiles{
	string endsolve;//处理结果（不带时间）
	string picsolve;//抓图结果
	string filesolve;//处理结果（带时间）
	string quasolve;//合格的图片
	string unqsolve;//不合格的图片
	string delesolve;//删除的图片
	string focussolve;//聚焦的路径
	string scan10Xsolve;//聚焦的路径
	string CApossolve;//聚焦的路径

};

//一个病人的微核分析结果数据记录结构体
struct  MN_HandleResult//微核分析结果数据记录结构体
{
	CString patientname;//病人名称
	int picturesum = 0;//该病人的照片总数
	int singlecell = 0;//单核细胞数
	int singlecell_wh = 0;//含有微核的单核细胞数
	int singleMN = 0;//单核细胞微核数
	int doublecell = 0;//双核细胞数
	int doublecell_wh = 0;//含有微核的双核细胞数
	int DoubleMN = 0;//双核细胞微核数

	int multiplecell = 0;//多核细胞数
	int multiplecell_wh = 0;//含有微核的多核细胞数
	int MultiMN = 0;//多核细胞微核数

	int sumwh = 0;//微核总数
	int sumcell = 0;//细胞总数

	int SingleCellsWithMN[20];
	int DoubleCellsWithMN[20];
	int TripleCellsWithMN[20];
	int MultiCellsWithMN[20];

	//计算结果
	int SingleMNC_Num = 0;
	float SingleMNC_Rate = 0;
	int SingleMN_Num = 0;
	float SingleMN_Rate = 0;

	int DoubleMNC_Num = 0;
	float DoubleMNC_Rate = 0;
	int DoubleMN_Num = 0;
	float DoubleMN_Rate = 0;

	CString sourcefile;//图片处理文件夹
	CString resultfile;//结果存储文件夹

	int delPictureNum = 0;
};

//一张小照片的微核分析结果数据记录结构体
struct  MN_HandleResultOnePic//微核分析结果数据记录结构体
{
	CString patientname;//病人名称
	CString picpath;//照片路径
	int NumCells = 0;//细胞核数
	int NumMicro = 0;//细胞微核数
	int CheckOrNot_NumCells = 0;
	int CheckOrNot_NumMicro = 0;

};

//一个病人的染色体分析结果数据记录结构体
struct CHRO_HandleResult //染色体分析结果数据记录结构体
{
	CString patientname;//病人名称
	CString sourcefile;//图片处理文件夹
	CString resultfile;//结果存储文件夹
	int picturesum = 0;//该病人的照片总数
	int allcell = 0;//染色体分析总的细胞数
	int normalcell = 0;//正常细胞数
	int abnormal = 0;//不正常细胞数
	int dic = 0;//染色体分析的双着丝粒体数
	int round = 0;//染色体分析的染色体环数
	int ace = 0;//染色体分析的无着丝粒数
	int t = 0;//染色体分析的相互易位
	int inv = 0;//染色体分析的倒位数
	int tri = 0;//
	int ten = 0;//
	int del = 0;
	int ctg = 0;
	int csg = 0;
	int ctb = 0;
	int cte = 0;

	double Y = 0.00;//染色体分析的畸变率

};

//一张照片的染色体分析结果数据记录结构体
struct CHRO_HandleResultOnePic//染色体分析结果数据记录结构体
{
	CString patientname;//病人名称
	CString picpath;//照片路径
	int allcell = 0;//染色体分析总的细胞数
	int normalcell = 0;//正常细胞数
	int abnormal = 0;//不正常细胞数
	int dic = 0;//染色体分析的双着丝粒体数
	int round = 0;//染色体分析的染色体环数
	int ace = 0;//染色体分析的无着丝粒数
	int t = 0;//染色体分析的相互易位
	int inv = 0;//染色体分析的倒位数
	int tri = 0;//
	int ten = 0;//
	int del = 0;
	int ctg = 0;
	int csg = 0;
	int ctb = 0;
	int cte = 0;

	int dic_rg = 0;//染色体分析的双着丝粒体数
	int round_rg = 0;//染色体分析的染色体环数
	int ace_rg = 0;//染色体分析的无着丝粒数
	int t_rg = 0;//染色体分析的相互易位
	int inv_rg = 0;//染色体分析的倒位数
	int tri_rg = 0;//
	int ten_rg = 0;//
	int del_rg = 0;
	int ctg_rg = 0;
	int csg_rg = 0;
	int ctb_rg = 0;
	int cte_rg = 0;

	int chromosome_num = 0;
	int chromosome_num_check = 0;
	int QuaOrNot = 0;  //0 为未校正  1为合格 -1为不合格

	/*int chromNum = 0;*/
	int CheckOrNot = 0;//此照片是否已经校对
	CString CheckPath;
};

//等待打印报表的病人结构体
struct WaitToPrint
{
	CString patientname;
	string savepath;
};

typedef struct ALL_PAREMETER
{
	vector<long> Offset_arr; //复位的偏移量，X轴和Y轴() /// *2
	char *CANET_ID; //CAN网关的ID/ *2
	int CANET_Port_Num; //CAN通信转换模块的ID *2
	double Focal_Domain_Radius; //某个已知焦距位移值的点，当某个未知点落入这个已知点半径为这个参数的范围内时，就认为
	//这个未知点的焦距和这个已知点的焦距一样， 单位是毫米 mm  *2
	int Focusing_Step_Z[3];//聚焦步长，0、1、2分别为最大步长，中等步长，最小步长 *2
	//注意这里的单位是脉冲数，
	//double Focusing_Step_Z_Max; //聚焦大步长，注意这里的单位是脉冲数，
	//double Focusing_Step_Z_Min; //聚焦小步长，注意这里的单位是脉冲数，
	double Slide_Size_X; //玻片的实际大小宽 *1
	double Slide_Size_y; //玻片的实际大小长*1
	double Space_Slides; //玻片与玻片之间的间隔*1
	vector<long> Nosepiece_Abs_Position; //物镜转盘的绝对位置 *2
	int Move_Deviation; //*2
	double View_Field_Width_10X;//40X时，相机看到的视野大小的宽度（脉冲数）
	double View_Field_Higth_10X;//40X时，相机看到的视野大小的高度（脉冲数）


	int Slide_Max_Num;//一次扫描的玻片最大数量  * 0
	double Scanning_Aero_X; //扫描区域X方向的长度  * 0
	double Scanning_Aero_Y; //扫描区域Y方向上的长度  *0
	int Scanning_Num_X; //X轴方向上扫面的点数  *0
	int Scanning_Num_Y; //Y轴方向上扫面的点数  *0
	int Focusing_Num_X; //X轴方向上聚焦的点数  *0
	int Focusing_Num_Y; //Y轴方向上聚焦的点数  *0
	bool NewStart_OR_BreakPoint;//重新开始或断点开始---------

	unsigned int Erode_element;//对阀值后图像进行腐蚀的结构元大小，用于将染色体区域腐蚀掉，留下非染色体区域
	unsigned int Dilate_element;//对疑似的染色体区域图像进行膨胀的结构元，用于区别染色体区域和细小杂质区域。
	unsigned int ErodeDilate_element;//对疑似的染色体区域图像进行腐蚀膨胀，锁定染色体区域。
	unsigned int Min_CA_Aero_10X;//区别细小杂质区域和染色体区域的阀值
	unsigned int Min_CA_Num_10X;//染色体区域中染色体疑似染色体条数的最小值

	int CA_XPos_Compensate;//染色体分析时，从10X下定位得到的坐标存在误差的话，可以通过设定这个补偿来修正
	int CA_YPos_Compensate;//Y方向补偿修正
	int Switch_Objective_ZPos;//切换物镜时，如何补偿

	int Pump_Oil_Pulse;//每次蹦油的的体积脉冲数
	float Pump_Oil_Vel;//泵油的速度；
	float Pump_Oil_Dis;//每个油滴之间的间距（mm）
	int Focus_Max_Limit;//焦距值的最大限制，表示的是所有的物镜焦距都在这个范围内
	int Focus_Min_Limit;//焦距值的最大限制
	float Pulse_ratio;//电机的脉冲系数比，也即是1mm对应的脉冲数（pp/mm）
	int Focus_Equal_Dis;//两个点焦距被认为一样的最大距离


	CString csExtension;//图像的格式.bmp; .raw等
	CString savepath;//手动抓图存图地址
	CString focuspath;//聚焦图片的地址
	//CString csDiretory;//自动抓图图像存储的路径
	ULONG CorrectionMatrix;
	ULONG DemosaicMethod;


} ALL_PAREMETER;

//设置微核分析参数
struct MNPARASET{
	CString MIN;
	CString EI;
	CString DR;
	CString EIXBL;
	CString DRXBL;
};

//设置微核分析参数
struct CHROSOMEPARASET{
	CString MINWIDTH;
	CString MAXWIDTH;
	CString MIDDLE;
	CString SINGLEMIN;
};

//扫描点位置坐标
struct LOCATION{
	int x_all;//X方向总点数
	int y_all;//Y方向总点数
	int x_now;
	int y_now;
};

//病人以及此次分析有问题的图片文件夹路径的结构体
struct PatientAndQuaImg
{
	CString patientname;
	CString QuestionImgPath;
};

//病人以及当前分析有问题的图片本身路径的结构体
struct PatientAndCurImg
{
	CString patientname;
	string currentImgPath;
};  

/***************************************数据库表格相关结构体*************************************************/
//玻片设置
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

//病人与玻片号的对应信息
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