#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"
#include "CTChart.h"
#include "afxdtctl.h"


// CMicroNucleusHandle 对话框

//DWORD WINAPI ThreadProcWaitMN(LPVOID pParam);

class CMicroNucleusHandle : public CDialogEx
{
	DECLARE_DYNAMIC(CMicroNucleusHandle)

public:
	CMicroNucleusHandle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicroNucleusHandle();

// 对话框数据
	enum { IDD = IDD_MICRONUCLEUSHANDLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CMicroNucleusHandle *pMicroHandleDlg;//指向非模态对话框
	CString ReadPath;
	CString WritePath;
	CString StrTime;
	CString StrName;

	CListCtrl m_listmicrohandle;
	string  strFileSolve;
	string  strFileProcess;
	int ResultRow;
	int patientRow;

	HANDLE ThreadWait;
	DWORD ThreadID;
	ADOAccess m_Conn;
	CTChart m_chartmicro;


	void ReSize();
	POINT old;

	struct MicroTable
	{
		CString PatientName; //病人名称
		vector <CString>  MicroImgNames;//该病人的微核照片
		CString  MicroProgress;//分析进度
		CString PrintReady;//报表是否可以打印
		MN_HandleResult *Result;
		CString ImgPath; //分析图片路径
		CString ResultPath; //结果存储路径

	};
	vector <MicroTable> AllPatients;
	vector <MicroTable> AllPatientsChose;
	vector<int>Row;//选择分析的病人所在行数
	vector <CString>  AllMicroImg;//所有病人的微核照片
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[10000];
	WCHAR   wstr[MAX_PATH];
	WIN32_FIND_DATA file;
	CString OneImgName;
	vector<WaitToPrint> MNWaitPrint;
	bool patientNewImg;
	vector<int>PatientSlideNum;//该病人对应的玻片号数组


	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//传递处理文件夹路径与储存文件夹路径
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//传递表格的初始数据

public:
	//afx_msg void OnBnClickedBtnMicrohandlepath();
	//afx_msg void OnBnClickedBtnMicroresultpath();
	afx_msg void OnBnClickedBtnMicrohandle();
	virtual BOOL OnInitDialog();
	void ClearAllSeries(void);
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);

	void static ThreadProcWaitMN();
	void static DeltePatientImage();

	// 将一个病人的分析结果显示到图表中
	bool ShowOneResult(CString patientname);	

	// 将一个病人的分析结果显示到图表中
	bool ShowOneResultInTable(CString patientname);

	// 加入可生成报表的结构体
	void MNJoinWaitToPrint(CString patientname, string savepath);

	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMicrosolve();
	// 清空数据库中的一些表格
	void DeleteAccessTable();
	afx_msg void OnBnClickedBtnMicroprint();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_datetimebegin;
	CDateTimeCtrl m_datetimeend;
	afx_msg void OnBnClickedBtnFindmn();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_resultchrolist;

	// 查找指定文件夹下病人的照片数量
	vector <CString> InquiryPatientImg(CString patientname, CString imgpath);
	// 从数据库中获取所有该病人所对应的玻片号
	vector<int> GetPatientSlideNumFromAccess(CString patientname);
	//计算所需要的参数
	void CalculateMNParaInExcel(MN_HandleResult *pB);

	//将本次扫描信息导入列表 并选中
	bool LoadCurrentToList();
	afx_msg void OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
