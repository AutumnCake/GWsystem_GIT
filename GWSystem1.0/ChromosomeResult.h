#pragma once
#include "afxwin.h"
#include "ADOAccess.h"
#include"resource.h"
#include"GWSystem1.0.h"
//加载OpenCVAPI
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "CvvImage.h"
#include"CV_picture.h"
#include "EditTrans.h"
#include "StaticTrans.h"

using namespace cv;
// CChromosomeResult 对话框

class CChromosomeResult : public CDialogEx
{
	DECLARE_DYNAMIC(CChromosomeResult)

public:
	CChromosomeResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChromosomeResult();

// 对话框数据
	enum { IDD = IDD_CHROMOSOMERESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:

	CEdit m_dic;
	CEdit m_t;
	CEdit m_round;
	CEdit m_inv;
	CEdit m_ace;

	CEdit m_dicrg;
	CEdit m_trg;
	CEdit m_roundrg;
	CEdit m_invrg;
	CEdit m_acerg;

	CEdit m_page;
	CEdit m_sumdic;
	CEdit m_sumround;
	CEdit m_sumace;
	CEdit m_sumtrg;
	CEdit m_suminv;

	vector<CRect> rectCHRO; //定义9个picture control 区域

	CRect rectCHRO1;
	CRect rectCHRO2;
	CRect rectCHRO3;
	CRect rectCHRO4;
	CRect rectCHRO5;
	CRect rectCHRO6;
	CRect rectCHRO7;
	CRect rectCHRO8;
	CRect rectCHRO9;
	CRect rcClientCHRO;

	CV_picture m_picchro1;
	CV_picture m_picchro2;
	CV_picture m_picchro3;
	CV_picture m_picchro4;
	CV_picture m_picchro5;
	CV_picture m_picchro6;
	CV_picture m_picchro7;
	CV_picture m_picchro8;
	CV_picture m_picchro9;

	CEdit m_sumtri;
	CEdit m_sumten;
	CEdit m_sumdel;
	CEdit m_sumctg;
	CEdit m_sumcsg;
	CEdit m_sumctb;
	CEdit m_sumcte;
	CEdit m_tri;
	CEdit m_ten;
	CEdit m_del;
	CEdit m_ctg;
	CEdit m_csg;
	CEdit m_ctb;
	CEdit m_cte;
	CEdit m_chronum;
	CEdit m_chronumrg;
	CEdit m_trirg;
	CEdit m_tenrg;
	CEdit m_delrg;
	CEdit m_ctgrg;
	CEdit m_csgrg;
	CEdit m_ctbrg;
	CEdit m_cterg;

	CEdit m_abnormalnum;
	CEdit m_quanum;

public:
	ADOAccess m_Conn;
	_bstr_t sql;
	_variant_t var;
	vector<CString> PatientNames;
	vector<CString> PicturePath;
	vector<PatientAndQuaImg >AllQuestionImgPath;
	vector<PatientAndCurImg>QuaImgCurrentFile;
	vector<string>QuaImgCurrentPath;
	int Picctrl_Num;
	vector<UINT> IDC_NameCHRO;//picture控件的数组
	vector <string>  imgNamesCHRO;
	CString sumpage, currentpage;
	CString strpathgood;
	CString pageshow;
	CString CurrentPatientName;
	Mat image;
	unsigned int  Temp;//选中的图片号
	int Frame;
	BOOL BStop;//定时器显示图片停止的标志
	unsigned int Page;//当前显示图片的页码，从第0页开始
	CV_picture m_bigImgchro;
	string CurrentPicInFrame;
	//图片显示用到框号

	bool EditOrNot;
	vector<cv::Point>PointImg;
	int currentPointStep;
	cv::Mat srcROI;
	string savepathROI;

	char tempFilePath[MAX_PATH + 1];
	WCHAR   wstr[MAX_PATH];
	WIN32_FIND_DATA file;
	char tempFileName[10000];

	CPen greenPen;
	CPen redPen;
	CPen whitePen;
	CPen* pOldPen;
	CComboBox m_comboxpatient;

	CFont font_PointSum;
	/*CEditTrans m_pointsum;*/
	CStaticTrans m_point_sum;
	CStaticTrans m_chro_num;

	vector<CHRO_HandleResultOnePic> AllImgAndResult;

	bool checked;
public:
	// 初始将picture控件存入数组，并画出滚动条
	void PutPicControlIntoVector();
	// 数据库中获取所有的病人名
	void GetAllPatientNames();
	// 显示选中病人的照片
	void ShowPatientImg(CString patientname);
	//清空picture中的图片
	void PicClear(UINT ID);
	//显示图片到picture控件
	void ShowImage2(IplImage *img, UINT ID);
	// 从数据库读取该张照片处理结果
	void ReadThePicResult(CString picpath);
	//// 整理出人工校正结果，并存入数据库
	//void SortDataToAccess();
	// 从数据库中统计已分析的图片结果
	void CountTheSumFromAcces(CString patientname);

	void ShowPicToImgctrl(vector<CHRO_HandleResultOnePic> allpic);

	void MarkQua(CString picturepath);
	void MarkUnq(CString picturepath);

	vector<CHRO_HandleResultOnePic> GetOnePatientImgInformation(CString patientname);
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboPatientch();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnMenuDelchro();
	//afx_msg void OnMenuCanceldelchro();
	//afx_msg void OnMenuQuachro();
	//afx_msg void OnMenuCancelquachro();
	//afx_msg void OnMenuUnqchro();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnBnClickedBtnChload2lib();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void MainWindowLastpage();
	afx_msg void MainWindowNextpage();
	afx_msg void OnBnClickedBtnCheckoverchro();

	afx_msg void OnEnChangeEditDicrg();
	afx_msg void OnEnChangeEditTrirg();
	afx_msg void OnEnChangeEditRoundrg();
	afx_msg void OnEnChangeEditAcerg();
	afx_msg void OnEnChangeEditTenrg();
	afx_msg void OnEnChangeEditTrg();
	afx_msg void OnEnChangeEditInvrg();
	afx_msg void OnEnChangeEditDelrg();
	afx_msg void OnEnChangeEditCtgrg();
	afx_msg void OnEnChangeEditCsgrg();
	afx_msg void OnEnChangeEditCtbrg();
	afx_msg void OnEnChangeEditCterg();

	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnQua();
	afx_msg void OnBnClickedBtnUnq();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnEnChangeEditChronumrg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	

};
