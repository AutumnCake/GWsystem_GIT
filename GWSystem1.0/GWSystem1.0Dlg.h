
// GWSystem1.0Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include"Resource.h"
#include "GWSystem1.0.h"
#include"Scanning_Control.h"
#include"ADOAccess.h"
#include<vector>

#include"ButtonST.h"

using std::vector;

// CGWSystem10Dlg �Ի���
class CGWSystem10Dlg : public CDialogEx 
{
// ����
public:
	CGWSystem10Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GWSYSTEM10_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

public:
	CString users, usertype; //�û����û�����

	HANDLE hThread;//���ٵ��߳̾��
	BOOL BStop;//��ʱ����ʾͼƬֹͣ�ı�־
	/*thread ScanThread;*/

	vector<UINT> IDC_SlideFrame;
	vector<UINT> IDC_SlideEdit;
	vector<UINT> IDC_ProgressEdit;

	vector<UINT> IDC_Name;//picture�ؼ�������

	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	CEdit m_edit4;
	CEdit m_edit5;
	CEdit m_edit6;
	CEdit m_edit7;
	CEdit m_edit8;

	CEdit m_edit11;
	CEdit m_edit22;
	CEdit m_edit33;
	CEdit m_edit44;
	CEdit m_edit55;
	CEdit m_edit66;
	CEdit m_edit77;
	CEdit m_edit88;

	CStatic m_frame1;
	CStatic m_frame2;
	CStatic m_frame3;
	CStatic m_frame4;
	CStatic m_frame5;
	CStatic m_frame6;
	CStatic m_frame7;
	CStatic m_frame8;

	CToolBar m_wndToolBar;
	CButtonST m_bbtn1;
	CButtonST m_bbtn2;
	CButtonST m_bbtn3;
	CButtonST m_bbtn4;


	CString ElementaryFile;
	MyFiles AllFiles;
	SLIDE *Slide = new SLIDE;
	
	//�Ի������
	void ReSize();
	POINT old;
	

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg LRESULT InitialList1(WPARAM wParam, LPARAM lParam);//�û���¼
	afx_msg LRESULT InitialList2(WPARAM wParam, LPARAM lParam);//��ʼ���ò���
	afx_msg LRESULT InitialList3(WPARAM wParam, LPARAM lParam);//������Ϣ����ʱ��
	afx_msg LRESULT InitialList4(WPARAM wParam, LPARAM lParam);//����ɨ�������������
	afx_msg LRESULT InitialList5(WPARAM wParam, LPARAM lParam);//��ͨ�û��۽���������
	afx_msg LRESULT InitialList6(WPARAM wParam, LPARAM lParam);//����Ա�۽���������
	afx_msg LRESULT InitialList7(WPARAM wParam, LPARAM lParam);//����Ա�����������
	afx_msg LRESULT InitialList8(WPARAM wParam, LPARAM lParam);//����Աɨ���������
	afx_msg LRESULT InitialList9(WPARAM wParam, LPARAM lParam);//������ǰλ�õ�
	afx_msg LRESULT InitialList10(WPARAM wParam, LPARAM lParam);//�����ֶ�����

	//afx_msg LRESULT Updata_Display_Drawing(WPARAM wParam, LPARAM lParam);//������ɨ��ͼ��





public:
	CStatic m_allframe;
	_bstr_t sql;
	_variant_t var;
	ADOAccess m_Conn;

	afx_msg void OnDiaryManagement();
	afx_msg void OnUserManagement();
	afx_msg void OnResetCode();
	afx_msg void OnBnClickedBtnScanbegin();
	afx_msg void OnBnClickedBtnSlideset();
	afx_msg void OnBnClickedBtnLastpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnIkaros();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnIkarosrst();
	afx_msg void OnMenuCamera();
	afx_msg void OnMenuPlatformset();

	afx_msg void OnMenuFocus();
	afx_msg void OnMenuAdmin();
	afx_msg void OnMenuAnalyse();
	CStatic m_progressDraw;
	afx_msg void OnBnClickedBtnStop();
	// ������ǰ���յ�
	int DrawCurrentPosition(LOCATION currrentLocation);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


	afx_msg void OnMenuuserexit();

	//���ٵ�ɨ���߳�ִ�к���
	void static ScanThreadProc();


	afx_msg void OnBnClickedBtnCaptureimg();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnParaset();
	afx_msg void OnBnClickedBtnHandle();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

DWORD WINAPI ThreadProc(PVOID pParam);//���ٵ�ɨ���߳�ִ�к���
