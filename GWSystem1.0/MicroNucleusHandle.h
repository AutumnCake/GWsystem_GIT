#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"
#include "CTChart.h"
#include "afxdtctl.h"


// CMicroNucleusHandle �Ի���

//DWORD WINAPI ThreadProcWaitMN(LPVOID pParam);

class CMicroNucleusHandle : public CDialogEx
{
	DECLARE_DYNAMIC(CMicroNucleusHandle)

public:
	CMicroNucleusHandle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMicroNucleusHandle();

// �Ի�������
	enum { IDD = IDD_MICRONUCLEUSHANDLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CMicroNucleusHandle *pMicroHandleDlg;//ָ���ģ̬�Ի���
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
		CString PatientName; //��������
		vector <CString>  MicroImgNames;//�ò��˵�΢����Ƭ
		CString  MicroProgress;//��������
		CString PrintReady;//�����Ƿ���Դ�ӡ
		MN_HandleResult *Result;
		CString ImgPath; //����ͼƬ·��
		CString ResultPath; //����洢·��

	};
	vector <MicroTable> AllPatients;
	vector <MicroTable> AllPatientsChose;
	vector<int>Row;//ѡ������Ĳ�����������
	vector <CString>  AllMicroImg;//���в��˵�΢����Ƭ
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[10000];
	WCHAR   wstr[MAX_PATH];
	WIN32_FIND_DATA file;
	CString OneImgName;
	vector<WaitToPrint> MNWaitPrint;
	bool patientNewImg;
	vector<int>PatientSlideNum;//�ò��˶�Ӧ�Ĳ�Ƭ������


	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//���ݴ����ļ���·���봢���ļ���·��
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//���ݱ��ĳ�ʼ����

public:
	//afx_msg void OnBnClickedBtnMicrohandlepath();
	//afx_msg void OnBnClickedBtnMicroresultpath();
	afx_msg void OnBnClickedBtnMicrohandle();
	virtual BOOL OnInitDialog();
	void ClearAllSeries(void);
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect);

	void static ThreadProcWaitMN();
	void static DeltePatientImage();

	// ��һ�����˵ķ��������ʾ��ͼ����
	bool ShowOneResult(CString patientname);	

	// ��һ�����˵ķ��������ʾ��ͼ����
	bool ShowOneResultInTable(CString patientname);

	// ��������ɱ���Ľṹ��
	void MNJoinWaitToPrint(CString patientname, string savepath);

	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMicrosolve();
	// ������ݿ��е�һЩ���
	void DeleteAccessTable();
	afx_msg void OnBnClickedBtnMicroprint();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CDateTimeCtrl m_datetimebegin;
	CDateTimeCtrl m_datetimeend;
	afx_msg void OnBnClickedBtnFindmn();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_resultchrolist;

	// ����ָ���ļ����²��˵���Ƭ����
	vector <CString> InquiryPatientImg(CString patientname, CString imgpath);
	// �����ݿ��л�ȡ���иò�������Ӧ�Ĳ�Ƭ��
	vector<int> GetPatientSlideNumFromAccess(CString patientname);
	//��������Ҫ�Ĳ���
	void CalculateMNParaInExcel(MN_HandleResult *pB);

	//������ɨ����Ϣ�����б� ��ѡ��
	bool LoadCurrentToList();
	afx_msg void OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
