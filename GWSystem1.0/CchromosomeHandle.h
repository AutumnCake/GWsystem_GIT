#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CTChart.h"
#include "ADOAccess.h"
#include "afxdtctl.h"

//GridCtrl�ؼ�
//
//#include"GridCtrl.h"

// CChromosomeHandle �Ի���

//DWORD WINAPI ThreadProcWaitCHRO(LPVOID pParam);

class CChromosomeHandle : public CDialogEx
{
	DECLARE_DYNAMIC(CChromosomeHandle)

public:
	CChromosomeHandle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChromosomeHandle();

// �Ի�������
	enum { IDD = IDD_CHROSOMEHANDLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	HANDLE ThreadWait;
	DWORD ThreadID;
	ADOAccess m_Conn;


	struct ChroTable
	{
		CString PatientName; //��������
		vector <CString>  ChroImgNames;//�ò��˵�Ⱦɫ����Ƭ
		CString  ChroProgress;//��������
		CString PrintReady;//�����Ƿ���Դ�ӡ
		CHRO_HandleResult *Result;
		CString ImgPath; //����ͼƬ·��
		CString ResultPath; //����洢·��
	};
	vector <ChroTable> AllPatients;
	vector <ChroTable> AllPatientsChose;
	vector<int>Row;//ѡ������Ĳ�����������
	vector<int>PatientSlideNum;//�ò��˶�Ӧ�Ĳ�Ƭ������
	vector <CString>  AllChroImg;//���в��˵�Ⱦɫ����Ƭ
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


	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//���ݴ����ļ���·���봢���ļ���·��
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//���ݱ��ĳ�ʼ����
	afx_msg void OnBnClickedBtnChrohandlepath();
	afx_msg void OnBnClickedBtnMchroresultpath();
	afx_msg void OnBnClickedBtnChrohandle();

	// ������ݿ��е�һЩ���
	int DeleteAccessTable();
	void ClearAllSeries(void);

	void static ThreadProcWaitCHRO();
	// ��������ɱ���Ľṹ��
	void CHROWaitToPrint(CString patientname, string savepath);
	//// ���ݿ��ж�ȡһ�����˵ķ������
	//CHRO_HandleResult ReadOneResultFromAccess(CString patientname);
	// ��һ�����˵ķ��������ʾ��ͼ����
	bool ShowOneResult(CString patientname);
	// ��һ�����˵ķ��������ʾ��ͼ����
	bool ShowOneResultInTable(CString patientname);

	afx_msg void OnBnClickedBtnChrosolve();
	afx_msg void OnBnClickedBtnChroprint();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtnFindchro();

	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	// ����ָ���ļ����²��˵���Ƭ����
	vector <CString> InquiryPatientImg(CString patientname, CString imgpath);
	// �����ݿ��л�ȡ���иò�������Ӧ�Ĳ�Ƭ��
	vector<int> GetPatientSlideNumFromAccess(CString patientname);
	//��ʼ��Grid�ؼ�
	//void GridCtrlInit();
	bool LoadCurrentToList();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult);
};
