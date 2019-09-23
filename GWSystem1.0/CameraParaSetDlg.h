#pragma once
#include "afxwin.h"
#include "lucamapi.h"
#include "ADOAccess.h"
#include "Scanning_Control.h"


// CameraParaSetDlg 对话框

class CameraParaSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CameraParaSetDlg)

public:
	CameraParaSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CameraParaSetDlg();

// 对话框数据
	enum { IDD = IDD_CAMERASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	//数据库
	ADOAccess m_Camera_Database_Scanning;
	_bstr_t sql;
	_variant_t var;
	CString tableIndex;
	CBrush m_DlgbkBrush;			//对话框颜色
	int m_Table_Index;				//当前物镜转盘使用的物镜编号
	CString m_ParasFilePath;						//设置系统参数保存路径

	//相机函数
	Scanning_Control *m_Scanning_Control;
	HANDLE m_hCamera;                       //相机句柄
	BOOL m_bConnected;						//建立连接？
	BOOL m_bPreviewing;						//预览打开
	LUCAM_FRAME_FORMAT m_lffFormat;			//相机参数
	FLOAT m_fFrameRate;						//相机帧率
	ULONG m_ulCameraId;						//相机ID
	BOOL m_bIsColor;						//是否为彩色相机
	FLOAT m_fZoom;							//预览缩放比例

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
