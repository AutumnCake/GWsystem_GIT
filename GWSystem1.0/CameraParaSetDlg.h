#pragma once
#include "afxwin.h"
#include "lucamapi.h"
#include "ADOAccess.h"
#include "Scanning_Control.h"


// CameraParaSetDlg �Ի���

class CameraParaSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CameraParaSetDlg)

public:
	CameraParaSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CameraParaSetDlg();

// �Ի�������
	enum { IDD = IDD_CAMERASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void PassHandleCameera(HANDLE hCamera, BOOL bPreviewing, int Table_Index);
	void Camera_Paras_Get(int Table_Index);
	void Camera_Paras_Set(int Table_Index);
	BOOL Camera_Paras_write();
	BOOL Camera_Paras_read();

private:
	CComboBox m_comboxtablenum;

public:
	//���ݿ�
	ADOAccess m_Camera_Database_Scanning;
	_bstr_t sql;
	_variant_t var;
	CString tableIndex;
	CBrush m_DlgbkBrush;			//�Ի�����ɫ
	int m_Table_Index;				//��ǰ�ﾵת��ʹ�õ��ﾵ���
	CString m_ParasFilePath;						//����ϵͳ��������·��

	//�������
	Scanning_Control *m_Scanning_Control;
	HANDLE m_hCamera;                       //������
	BOOL m_bConnected;						//�������ӣ�
	BOOL m_bPreviewing;						//Ԥ����
	LUCAM_FRAME_FORMAT m_lffFormat;			//�������
	FLOAT m_fFrameRate;						//���֡��
	ULONG m_ulCameraId;						//���ID
	BOOL m_bIsColor;						//�Ƿ�Ϊ��ɫ���
	FLOAT m_fZoom;							//Ԥ�����ű���

	afx_msg void OnCbnSelchangeComboTablenum();
	afx_msg void OnBnClickedBtnSavecamerapara();
	afx_msg void OnEnChangeEditExposuretime();
	afx_msg void OnEnChangeEditGain();
	afx_msg void OnEnChangeEditRedgain();
	afx_msg void OnEnChangeEditGreengain1();
	afx_msg void OnEnChangeEditGreengain2();
	afx_msg void OnEnChangeEditBluegain();
	afx_msg void OnEnChangeEditGamma();
	afx_msg void OnEnChangeEditContrast();
	afx_msg void OnEnChangeEditBrightness();
	afx_msg void OnEnChangeEditHue();
	afx_msg void OnEnChangeEditSaturation();
	afx_msg void OnEnChangeEditExposuremax();
	afx_msg void OnBnClickedRadioBit8();
	afx_msg void OnBnClickedRadioBit16();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnDefaultcamera();
};
