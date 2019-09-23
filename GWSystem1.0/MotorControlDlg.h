#pragma once
#include "Scanning_Control.h"
#include "afxwin.h"


// MotorControlDlg �Ի���

class MotorControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MotorControlDlg)

public:
	MotorControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MotorControlDlg();

// �Ի�������
	enum { IDD = IDD_MOTORCONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	Scanning_Control *MotorDlg_Scanning_Control;
	BOOL m_MotorConnected;						//�������ӣ�


	void Pass_Motor_Paras(Scanning_Control *s_Scanning_Control, BOOL s_MotorConnected);


	afx_msg void OnBnClickedBtnXMoving();
	afx_msg void OnBnClickedBtnYMoving();
	afx_msg void OnBnClickedBtnStopall();
	afx_msg void OnBnClickedBtnTablereset();
	afx_msg void OnCbnSelchangeComboTablechose();
	afx_msg void OnBnClickedBtnXLeftmax();
	afx_msg void OnBnClickedBtnXRightmax();
	afx_msg void OnBnClickedBtnYFrontmax();
	afx_msg void OnBnClickedBtnYBehindmax();
	afx_msg void OnBnClickedBtnZUpmax();
	afx_msg void OnBnClickedBtnDownmax();
	afx_msg void OnBnClickedBtnXLeftmid();
	afx_msg void OnBnClickedBtnXRightmid();
	afx_msg void OnBnClickedBtnYFrontmid();
	afx_msg void OnBnClickedBtnYBehindmid();
	afx_msg void OnBnClickedBtnZUpmid();
	afx_msg void OnBnClickedBtnDownmid();
	afx_msg void OnBnClickedButton5idcBtnXLeftmin();
	afx_msg void OnBnClickedBtnXRightmin();
	afx_msg void OnBnClickedBtnYFrontmin();
	afx_msg void OnBnClickedBtnYBehindmin();
	afx_msg void OnBnClickedBtnZUpmin();
	afx_msg void OnBnClickedBtnDownmin();
	afx_msg void OnBnClickedBtnResetmotor();
	afx_msg void OnBnClickedBtnXMoveto();
	afx_msg void OnBnClickedBtnYMoveto();
	afx_msg void OnBnClickedBtnZMoveto();
	afx_msg void OnBnClickedBtnReadcurrentpos();

private:
	CComboBox m_combotablenum;
};
