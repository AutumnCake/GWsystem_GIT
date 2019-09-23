#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CTChart.h"
#include "ADOAccess.h"
#include "afxdtctl.h"

//GridCtrl控件
//
//#include"GridCtrl.h"

// CChromosomeHandle 对话框

//DWORD WINAPI ThreadProcWaitCHRO(LPVOID pParam);

class CChromosomeHandle : public CDialogEx
{
	DECLARE_DYNAMIC(CChromosomeHandle)

public:
	CChromosomeHandle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChromosomeHandle();

// 对话框数据
	enum { IDD = IDD_CHROSOMEHANDLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	HANDLE ThreadWait;
	DWORD ThreadID;
	ADOAccess m_Conn;


	struct ChroTable
	{
		CString PatientName; //病人名称
		vector <CString>  ChroImgNames;//该病人的染色体照片
		CString  ChroProgress;//分析进度
		CString PrintReady;//报表是否可以打印
		CHRO_HandleResult *Result;
		CString ImgPath; //分析图片路径
		CString ResultPath; //结果存储路径
	};
	vector <ChroTable> AllPatients;
	vector <ChroTable> AllPatientsChose;
	vector<int>Row;//选择分析的病人所在行数
	vector<int>PatientSlideNum;//该病人对应的玻片号数组
	vector <CString>  AllChroImg;//所有病人的染色体照片
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[10000];
	WCHAR   wstr[MAX_PATH];
	WIN32_FIND_DATA file;
	CString OneImgName;
	vector<WaitToPrint> CHROWaitPrint;
	bool patientNewImg;

public:
	virtual BOOL OnInitDialog();
	CEdit m_handlepathchro;
	CEdit m_resultpathchro;
	CString ReadPath;
	CString WritePath;
	CString StrTime;
	CString StrName;
	string  strFileSolve;
	int ResultRow;
	int patientRow;
	CListCtrl m_listchrohandle;
	CTChart m_chartchrom;
	CDateTimeCtrl m_datetimebegin;
	CDateTimeCtrl m_datetimeend;
	CListCtrl m_listchroresult;
	//CGridCtrl m_pGridchro;


	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//传递处理文件夹路径与储存文件夹路径
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//传递表格的初始数据
	afx_msg void OnBnClickedBtnChrohandlepath();
	afx_msg void OnBnClickedBtnMchroresultpath();
	afx_msg void OnBnClickedBtnChrohandle();

	// 清空数据库中的一些表格
	int DeleteAccessTable();
	void ClearAllSeries(void);

	void static ThreadProcWaitCHRO();
	// 加入可生成报表的结构体
	void CHROWaitToPrint(CString patientname, string savepath);
	//// 数据库中读取一个病人的分析结果
	//CHRO_HandleResult ReadOneResultFromAccess(CString patientname);
	// 将一个病人的分析结果显示到图表中
	bool ShowOneResult(CString patientname);
	// 将一个病人的分析结果显示到图表中
	bool ShowOneResultInTable(CString patientname);

	afx_msg void OnBnClickedBtnChrosolve();
	afx_msg void OnBnClickedBtnChroprint();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtnFindchro();

	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	// 查找指定文件夹下病人的照片数量
	vector <CString> InquiryPatientImg(CString patientname, CString imgpath);
	// 从数据库中获取所有该病人所对应的玻片号
	vector<int> GetPatientSlideNumFromAccess(CString patientname);
	//初始化Grid控件
	//void GridCtrlInit();
	bool LoadCurrentToList();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult);
};
