#pragma once
#include"resource.h"
#include "afxwin.h"
#include "ADOAccess.h"
#include"GWSystem1.0.h"
//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "CvvImage.h"

// CMicroNucleusResult 对话框

class CMicroNucleusResult : public CDialogEx
{
	DECLARE_DYNAMIC(CMicroNucleusResult)

public:
	CMicroNucleusResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMicroNucleusResult();

// 对话框数据
	enum { IDD = IDD_MICRONUCLEUSRESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	ADOAccess m_Conn;
	_bstr_t sql;
	_variant_t var;
	vector<CString> PatientNames;
	vector<MN_HandleResultOnePic>CurrentPatientResult;
	MN_HandleResult  SystemResult;
	MN_HandleResult  CheckResult;

	CComboBox m_comboxpatient;
	vector<UINT> IDC_NameMN;//picture控件的数组
	vector<UINT>ID_StaticN;
	vector<UINT>ID_StaticMN;

	int Page;//当前显示图片的页码，从第0页开始
	CEdit m_page;
	CString sumpage, currentpage;
	CString pageshow;
	cv::Mat image;
	CPen greenPen;
	CPen redPen;
	CPen whitePen;
	int current;//下拉框选中的位置

	struct OnePageCheck{
		int CheckPage;
		BOOL Checked[25];
		BOOL MicroCheck[25];
	};
	vector<OnePageCheck> CheckInformation;
	BOOL m_Checked[25];
	BOOL m_MicroCheck[25];

	struct OnePatientCheck{
		CString PatientName;
		vector<OnePageCheck> CheckInformation;
	};
	vector<OnePatientCheck> AllCheckInformation;

	afx_msg void OnCbnSelchangeComboPatient();
	virtual BOOL OnInitDialog();

	// 初始将picture控件存入数组，并画出滚动条
	void PutPicControlIntoVector();

	//清空picture中的图片
	void PicClear(UINT ID);
	//显示图片到picture控件
	void ShowImage2(IplImage *img, UINT ID);
	bool ShowCurrentPatientResult(vector<MN_HandleResultOnePic> currentpatientresult,int page);
	
	afx_msg void OnBnClickedBtnLastmn();
	afx_msg void OnBnClickedBtnNextmn();
	afx_msg void OnBnClickedBtnNinc1();
	afx_msg void OnBnClickedBtnNdec1();
	afx_msg void OnBnClickedBtnMninc1();
	afx_msg void OnBnClickedBtnMndec1();
	// 计算当前病人的结果  刷新
	MN_HandleResult * CalculateMNResult(vector<MN_HandleResultOnePic> currentpatientresult);
	// 显示总结果
	void ShowSumResult(MN_HandleResult * result);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnNinc2();
	afx_msg void OnBnClickedBtnNdec2();
	afx_msg void OnBnClickedBtnMninc2();
	afx_msg void OnBnClickedBtnMndec2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnNinc3();
	afx_msg void OnBnClickedBtnNinc4();
	afx_msg void OnBnClickedBtnNinc5();
	afx_msg void OnBnClickedBtnNinc6();
	afx_msg void OnBnClickedBtnNinc7();
	afx_msg void OnBnClickedBtnNinc8();
	afx_msg void OnBnClickedBtnNinc9();
	afx_msg void OnBnClickedBtnNinc10();
	afx_msg void OnBnClickedBtnNinc11();
	afx_msg void OnBnClickedBtnNinc12();
	afx_msg void OnBnClickedBtnNinc13();
	afx_msg void OnBnClickedBtnNinc14();
	afx_msg void OnBnClickedBtnNinc15();
	afx_msg void OnBnClickedBtnNinc16();
	afx_msg void OnBnClickedBtnNinc17();
	afx_msg void OnBnClickedBtnNinc18();
	afx_msg void OnBnClickedBtnNinc19();
	afx_msg void OnBnClickedBtnNinc20();
	afx_msg void OnBnClickedBtnNinc21();
	afx_msg void OnBnClickedBtnNdec3();
	afx_msg void OnBnClickedBtnNdec4();
	afx_msg void OnBnClickedBtnNdec5();
	afx_msg void OnBnClickedBtnNdec6();
	afx_msg void OnBnClickedBtnNdec7();
	afx_msg void OnBnClickedBtnNdec8();
	afx_msg void OnBnClickedBtnNdec9();
	afx_msg void OnBnClickedBtnNdec10();
	afx_msg void OnBnClickedBtnNdec11();
	afx_msg void OnBnClickedBtnNdec12();
	afx_msg void OnBnClickedBtnNdec13();
	afx_msg void OnBnClickedBtnNdec14();
	afx_msg void OnBnClickedBtnNdec15();
	afx_msg void OnBnClickedBtnNdec16();
	afx_msg void OnBnClickedBtnNdec17();
	afx_msg void OnBnClickedBtnNdec18();
	afx_msg void OnBnClickedBtnNdec19();
	afx_msg void OnBnClickedBtnNdec20();
	afx_msg void OnBnClickedBtnNdec21();
	afx_msg void OnBnClickedBtnMninc3();
	afx_msg void OnBnClickedBtnMninc4();
	afx_msg void OnBnClickedBtnMninc5();
	afx_msg void OnBnClickedBtnMninc6();
	afx_msg void OnBnClickedBtnMninc7();
	afx_msg void OnBnClickedBtnMninc8();
	afx_msg void OnBnClickedBtnMninc9();
	afx_msg void OnBnClickedBtnMninc10();
	afx_msg void OnBnClickedBtnMninc11();
	afx_msg void OnBnClickedBtnMninc12();
	afx_msg void OnBnClickedBtnMninc13();
	afx_msg void OnBnClickedBtnMninc14();
	afx_msg void OnBnClickedBtnMninc15();
	afx_msg void OnBnClickedBtnMninc16();
	afx_msg void OnBnClickedBtnMninc17();
	afx_msg void OnBnClickedBtnMninc18();
	afx_msg void OnBnClickedBtnMninc19();
	afx_msg void OnBnClickedBtnMninc20();
	afx_msg void OnBnClickedBtnMninc21();
	afx_msg void OnBnClickedBtnMndec3();
	afx_msg void OnBnClickedBtnMndec4();
	afx_msg void OnBnClickedBtnMndec5();
	afx_msg void OnBnClickedBtnMndec6();
	afx_msg void OnBnClickedBtnMndec7();
	afx_msg void OnBnClickedBtnMndec8();
	afx_msg void OnBnClickedBtnMndec9();
	afx_msg void OnBnClickedBtnMndec10();
	afx_msg void OnBnClickedBtnMndec11();
	afx_msg void OnBnClickedBtnMndec12();
	afx_msg void OnBnClickedBtnMndec13();
	afx_msg void OnBnClickedBtnMndec14();
	afx_msg void OnBnClickedBtnMndec15();
	afx_msg void OnBnClickedBtnMndec16();
	afx_msg void OnBnClickedBtnMndec17();
	afx_msg void OnBnClickedBtnMndec18();
	afx_msg void OnBnClickedBtnMndec19();
	afx_msg void OnBnClickedBtnMndec20();
	afx_msg void OnBnClickedBtnMndec21();
};
