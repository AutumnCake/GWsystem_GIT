// ChromosomeResult.cpp : 实现文件
//

#include "stdafx.h"
#include "CV_picture.h"
#include "GWSystem1.0.h"
#include "ChromosomeResult.h"
#include "afxdialogex.h"
#include"GWSystem1.0Dlg.h"
#include"CchromosomeHandle.h"
#include "MarkImgCHRO.h"

//图片显示
#include <fstream>
#include <io.h>
#include "EditTrans.h"


CMarkImgCHRO MarkImgCHRO;
CChromosomeResult * pShow;
cv::Mat ImgMat;
extern CGWSystem10Dlg* pDlg;
extern CChromosomeHandle* pHandleDlg;
extern CChromosomeResult * pChromosomeResult;

// CChromosomeResult 对话框

IMPLEMENT_DYNAMIC(CChromosomeResult, CDialogEx)

CChromosomeResult::CChromosomeResult(CWnd* pParent /*=NULL*/)
: CDialogEx(CChromosomeResult::IDD, pParent), BStop(TRUE)
{
	Page = 0;
	checked = true;
	EditOrNot = false;
}

CChromosomeResult::~CChromosomeResult()
{
	//SortDataToAccess();
	KillTimer(1);
}

void CChromosomeResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHFIGUREFRAME1, m_picchro1);
	DDX_Control(pDX, IDC_CHFIGUREFRAME2, m_picchro2);
	DDX_Control(pDX, IDC_CHFIGUREFRAME3, m_picchro3);
	DDX_Control(pDX, IDC_CHFIGUREFRAME4, m_picchro4);
	DDX_Control(pDX, IDC_CHFIGUREFRAME5, m_picchro5);
	DDX_Control(pDX, IDC_CHFIGUREFRAME6, m_picchro6);
	DDX_Control(pDX, IDC_CHFIGUREFRAME7, m_picchro7);
	DDX_Control(pDX, IDC_CHFIGUREFRAME8, m_picchro8);
	DDX_Control(pDX, IDC_CHFIGUREFRAME9, m_picchro9);

	DDX_Control(pDX, IDC_EDIT_DIC, m_dic);
	DDX_Control(pDX, IDC_EDIT_T, m_t);
	DDX_Control(pDX, IDC_EDIT_ROUND, m_round);
	DDX_Control(pDX, IDC_EDIT_INV, m_inv);
	DDX_Control(pDX, IDC_EDIT_ACE, m_ace);

	DDX_Control(pDX, IDC_EDIT_DICRG, m_dicrg);
	DDX_Control(pDX, IDC_EDIT_TRG, m_trg);
	DDX_Control(pDX, IDC_EDIT_ROUNDRG, m_roundrg);
	DDX_Control(pDX, IDC_EDIT_INVRG, m_invrg);
	DDX_Control(pDX, IDC_EDIT_ACERG, m_acerg);
	DDX_Control(pDX, IDC_COMBO_PATIENTCH, m_comboxpatient);
	DDX_Control(pDX, IDC_EDIT_PAGECHRO, m_page);
	DDX_Control(pDX, IDC_EDIT_DICRGSUM, m_sumdic);
	DDX_Control(pDX, IDC_EDIT_ROUNDRGSUM, m_sumround);
	DDX_Control(pDX, IDC_EDIT_ACERGSUM, m_sumace);
	DDX_Control(pDX, IDC_EDIT_TRGSUM, m_sumtrg);
	DDX_Control(pDX, IDC_EDIT_INVRGSUM, m_suminv);
	DDX_Control(pDX, IDC_EDIT_TRIRGSUM, m_sumtri);
	DDX_Control(pDX, IDC_EDIT_TENRGSUM, m_sumten);
	DDX_Control(pDX, IDC_EDIT_DELRGSUM, m_sumdel);
	DDX_Control(pDX, IDC_EDIT_CTGRGSUM, m_sumctg);
	DDX_Control(pDX, IDC_EDIT_CSGRGSUM, m_sumcsg);
	DDX_Control(pDX, IDC_EDIT_CTBRGSUM, m_sumctb);
	DDX_Control(pDX, IDC_EDIT_CTERGSUM, m_sumcte);
	DDX_Control(pDX, IDC_EDIT_TRI, m_tri);
	DDX_Control(pDX, IDC_EDIT_TEN, m_ten);
	DDX_Control(pDX, IDC_EDIT_DEL, m_del);
	DDX_Control(pDX, IDC_EDIT_CTG, m_ctg);
	DDX_Control(pDX, IDC_EDIT_CSG, m_csg);
	DDX_Control(pDX, IDC_EDIT_CTB, m_ctb);
	DDX_Control(pDX, IDC_EDIT_CTE, m_cte);
	DDX_Control(pDX, IDC_EDIT_TRIRG, m_trirg);
	DDX_Control(pDX, IDC_EDIT_TENRG, m_tenrg);
	DDX_Control(pDX, IDC_EDIT_DELRG, m_delrg);
	DDX_Control(pDX, IDC_EDIT_CTGRG, m_ctgrg);
	DDX_Control(pDX, IDC_EDIT_CSGRG, m_csgrg);
	DDX_Control(pDX, IDC_EDIT_CTBRG, m_ctbrg);
	DDX_Control(pDX, IDC_EDIT_CTERG, m_cterg);
	DDX_Control(pDX, IDC_CHMAINFRAME, m_bigImgchro);
	DDX_Control(pDX, IDC_EDIT_CHRONUM, m_chronum);
	DDX_Control(pDX, IDC_EDIT_CHRONUMRG, m_chronumrg);
	//DDX_Control(pDX, IDC_EDIT_POINTSUM, m_pointsum);
	DDX_Control(pDX, IDC_STATIC_POINTSUM, m_point_sum);
	DDX_Control(pDX, IDC_STATIC_CHRONUM, m_chro_num);
	DDX_Control(pDX, IDC_EDIT_ABNORMALNUM, m_abnormalnum);
	DDX_Control(pDX, IDC_EDIT_QUANUM, m_quanum);
}


BEGIN_MESSAGE_MAP(CChromosomeResult, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PATIENTCH, &CChromosomeResult::OnCbnSelchangeComboPatientch)
	ON_WM_TIMER()
	//ON_COMMAND(ID_MENU_DELCHRO, &CChromosomeResult::OnMenuDelchro)
	//ON_COMMAND(ID_MENU_CANCELDELCHRO, &CChromosomeResult::OnMenuCanceldelchro)
	//ON_COMMAND(ID_MENU_QUACHRO, &CChromosomeResult::OnMenuQuachro)
	//ON_COMMAND(ID_MENU_CANCELQUACHRO, &CChromosomeResult::OnMenuCancelquachro)
	//ON_COMMAND(ID_MENU_UNQCHRO, &CChromosomeResult::OnMenuUnqchro)
//	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CHLOAD2LIB, &CChromosomeResult::OnBnClickedBtnChload2lib)
	ON_WM_RBUTTONDOWN()

	ON_BN_CLICKED(IDC_BTN_LASTCHRO, &CChromosomeResult::MainWindowLastpage)
	ON_BN_CLICKED(IDC_BTN_NEXTCHRO, &CChromosomeResult::MainWindowNextpage)
	ON_BN_CLICKED(IDC_BTN_CHECKOVERCHRO, &CChromosomeResult::OnBnClickedBtnCheckoverchro)
	ON_EN_CHANGE(IDC_EDIT_DICRG, &CChromosomeResult::OnEnChangeEditDicrg)
	ON_EN_CHANGE(IDC_EDIT_TRIRG, &CChromosomeResult::OnEnChangeEditTrirg)
	ON_EN_CHANGE(IDC_EDIT_ROUNDRG, &CChromosomeResult::OnEnChangeEditRoundrg)
	ON_EN_CHANGE(IDC_EDIT_ACERG, &CChromosomeResult::OnEnChangeEditAcerg)
	ON_EN_CHANGE(IDC_EDIT_TENRG, &CChromosomeResult::OnEnChangeEditTenrg)
	ON_EN_CHANGE(IDC_EDIT_TRG, &CChromosomeResult::OnEnChangeEditTrg)
	ON_EN_CHANGE(IDC_EDIT_INVRG, &CChromosomeResult::OnEnChangeEditInvrg)
	ON_EN_CHANGE(IDC_EDIT_DELRG, &CChromosomeResult::OnEnChangeEditDelrg)
	ON_EN_CHANGE(IDC_EDIT_CTGRG, &CChromosomeResult::OnEnChangeEditCtgrg)
	ON_EN_CHANGE(IDC_EDIT_CSGRG, &CChromosomeResult::OnEnChangeEditCsgrg)
	ON_EN_CHANGE(IDC_EDIT_CTBRG, &CChromosomeResult::OnEnChangeEditCtbrg)
	ON_EN_CHANGE(IDC_EDIT_CTERG, &CChromosomeResult::OnEnChangeEditCterg)

	ON_BN_CLICKED(IDC_BTN_EDIT, &CChromosomeResult::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CChromosomeResult::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CChromosomeResult::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_LAST, &CChromosomeResult::OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CChromosomeResult::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_QUA, &CChromosomeResult::OnBnClickedBtnQua)
	ON_BN_CLICKED(IDC_BTN_UNQ, &CChromosomeResult::OnBnClickedBtnUnq)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_EN_CHANGE(IDC_EDIT_CHRONUMRG, &CChromosomeResult::OnEnChangeEditChronumrg)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChromosomeResult 消息处理程序


BOOL CChromosomeResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pShow = this;
	m_bigImgchro.OnInit();

	GetDlgItem(IDC_CHFIGUREFRAME1)->GetWindowRect(&rectCHRO1); ScreenToClient(&rectCHRO1);  	rectCHRO.push_back(rectCHRO1);
	GetDlgItem(IDC_CHFIGUREFRAME2)->GetWindowRect(&rectCHRO2); ScreenToClient(&rectCHRO2);  	rectCHRO.push_back(rectCHRO2);
	GetDlgItem(IDC_CHFIGUREFRAME3)->GetWindowRect(&rectCHRO3); ScreenToClient(&rectCHRO3);  	rectCHRO.push_back(rectCHRO3);
	GetDlgItem(IDC_CHFIGUREFRAME4)->GetWindowRect(&rectCHRO4); ScreenToClient(&rectCHRO4);  	rectCHRO.push_back(rectCHRO4);
	GetDlgItem(IDC_CHFIGUREFRAME5)->GetWindowRect(&rectCHRO5); ScreenToClient(&rectCHRO5);  	rectCHRO.push_back(rectCHRO5);
	GetDlgItem(IDC_CHFIGUREFRAME6)->GetWindowRect(&rectCHRO6); ScreenToClient(&rectCHRO6);  	rectCHRO.push_back(rectCHRO6);
	GetDlgItem(IDC_CHFIGUREFRAME7)->GetWindowRect(&rectCHRO7); ScreenToClient(&rectCHRO7);  	rectCHRO.push_back(rectCHRO7);
	GetDlgItem(IDC_CHFIGUREFRAME8)->GetWindowRect(&rectCHRO8); ScreenToClient(&rectCHRO8);  	rectCHRO.push_back(rectCHRO8);
	GetDlgItem(IDC_CHFIGUREFRAME9)->GetWindowRect(&rectCHRO9); ScreenToClient(&rectCHRO9);  	rectCHRO.push_back(rectCHRO9);
	PutPicControlIntoVector();

	//IDC_NameCHRO_M.push_back(m_picchro1);
	//IDC_NameCHRO_M.push_back(m_picchro2);
	//IDC_NameCHRO_M.push_back(m_picchro3);
	//IDC_NameCHRO_M.push_back(m_picchro4);
	//IDC_NameCHRO_M.push_back(m_picchro5);
	//IDC_NameCHRO_M.push_back(m_picchro6);
	//IDC_NameCHRO_M.push_back(m_picchro7);
	//IDC_NameCHRO_M.push_back(m_picchro8);
	//IDC_NameCHRO_M.push_back(m_picchro9);

	//连接数据库
	m_Conn.OnInitADOAccess();
	//清空病人数组
	vector<CString>().swap(PatientNames);
	//从数据库中获取所有的病人名
	GetAllPatientNames();
	//病人名填入ComBox
	m_comboxpatient.ResetContent();
	for (size_t i = 0; i < PatientNames.size(); i++)
	{
		m_comboxpatient.InsertString(i, PatientNames[i]);
	}
	m_comboxpatient.SetCurSel(0);
	OnCbnSelchangeComboPatientch();

	greenPen.CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	redPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	whitePen.CreatePen(PS_SOLID, 5, RGB(240, 240, 240));

	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_QUA)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UNQ)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EDIT)->EnableWindow(FALSE);


	font_PointSum.CreatePointFont(200, _T("微软雅黑"));
	m_point_sum.SetFont(&font_PointSum, FALSE);
	m_chro_num.SetFont(&font_PointSum, FALSE);
	//m_point_sum.SetBackColor(TRANS_BACK);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//初始将picture控件存入数组
void CChromosomeResult::PutPicControlIntoVector()
{
	//初始将picture控件放入数组,画滚动条
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME1);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME2);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME3);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME4);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME5);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME6);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME7);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME8);
	IDC_NameCHRO.push_back(IDC_CHFIGUREFRAME9);
	IDC_NameCHRO.push_back(IDC_CHMAINFRAME);

}

// 数据库中获取所有的病人名
void CChromosomeResult::GetAllPatientNames()
{
	sql = _T("select * from 染色体图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
		PatientNames.push_back(mystr);
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			mystr = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			vector<CString>::iterator iter;
			iter = find(PatientNames.begin(), PatientNames.end(), mystr);
			if (iter == PatientNames.end())
				PatientNames.push_back(mystr);//没找到
			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
	}
}

//选择病人
void CChromosomeResult::OnCbnSelchangeComboPatientch()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空画框
	for (int v = 0; v < 9; v++)
	{
		PicClear(IDC_NameCHRO[v]);
	}
	int current = m_comboxpatient.GetCurSel();
	if (current >= 0)
	{
		CurrentPatientName = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].patientname;
		Page = 0;
		//显示选中病人的照片
		vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
		AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
		ShowPicToImgctrl(AllImgAndResult);
		SetTimer(1, 3000, NULL);
	}
		//将结果显示清空
		CString str = NULL;

		checked = false;
		m_dic.SetWindowTextW(str);
		m_t.SetWindowTextW(str);
		m_round.SetWindowTextW(str);
		m_inv.SetWindowTextW(str);
		m_ace.SetWindowTextW(str);

		m_dicrg.SetWindowTextW(str);
		m_trg.SetWindowTextW(str);
		m_roundrg.SetWindowTextW(str);
		m_invrg.SetWindowTextW(str);
		m_acerg.SetWindowTextW(str);

		m_sumdic.SetWindowTextW(str);
		m_sumround.SetWindowTextW(str);
		m_sumace.SetWindowTextW(str);
		m_sumtrg.SetWindowTextW(str);
		m_suminv.SetWindowTextW(str);

		m_tri.SetWindowTextW(str);
		m_ten.SetWindowTextW(str);
		m_del.SetWindowTextW(str);
		m_ctg.SetWindowTextW(str);
		m_csg.SetWindowTextW(str);
		m_ctb.SetWindowTextW(str);
		m_cte.SetWindowTextW(str);
		m_chronum.SetWindowTextW(str);
		m_chronumrg.SetWindowTextW(str);
		m_trirg.SetWindowTextW(str);

		m_tenrg.SetWindowTextW(str);
		m_delrg.SetWindowTextW(str);
		m_ctgrg.SetWindowTextW(str);
		m_csgrg.SetWindowTextW(str);
		m_ctbrg.SetWindowTextW(str);
		m_cterg.SetWindowTextW(str);
		checked = true;

}

//清空图片
void CChromosomeResult::PicClear(UINT ID)//清空图片
{
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
	CDC *dc = pStatic->GetDC();
	dc->SelectObject(&whitePen);//在dc上放置画笔
	dc->SelectStockObject(NULL_BRUSH);//使画出的矩形区域为透明
	dc->Rectangle(lRect);
}

//显示选中病人的照片
void CChromosomeResult::ShowPatientImg(CString patientname)
{
	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();

	int find_pic = 0;//的图片的序列号
	vector <string>().swap(imgNamesCHRO);
	USES_CONVERSION;
	char*fileName = T2A(patientname);
	sprintf_s(tempFilePath, "%s\*.bmp", fileName);
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));

	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf(tempFileName, "%s", fileName);
			imgNamesCHRO.push_back(app->WChar2Ansi(file.cFileName));
			imgNamesCHRO[find_pic].insert(0, tempFileName);
			find_pic++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	SetTimer(1, 300, NULL);

}

// 定时器显示照片
void CChromosomeResult::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (BStop)
	{
		vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
		AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
		ShowPicToImgctrl(AllImgAndResult);
	}

	BStop = TRUE;
	CDialogEx::OnTimer(nIDEvent);
}

//显示图片过程函数
void CChromosomeResult::ShowImage2(IplImage *img, UINT ID)//显示图片过程函数
{

	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

// 上一页
void CChromosomeResult::MainWindowLastpage()
{
	for (int v = 0; v < 9; v++)
	{
		PicClear(IDC_NameCHRO[v]);
	}
	Page--;

	if (Page < 0)
		Page = 0;

	//刷新界面
	vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
	AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
	ShowPicToImgctrl(AllImgAndResult);
	
}

// 下一页
void CChromosomeResult::MainWindowNextpage()
{
	Page++;
	for (int v = 0; v < 9; v++)
	{
		PicClear(IDC_NameCHRO[v]);
	}

	if (Page > (_ttoi(sumpage)-1))
		Page = _ttoi(sumpage) - 1;

	//刷新界面
	vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
	AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
	ShowPicToImgctrl(AllImgAndResult);
}

//从数据库读取该张照片处理结果
void CChromosomeResult::ReadThePicResult(CString picpath)
{

	checked = false;
	//从数据库中获取病人照片路径 存入数组
	sql = _T("select * from 染色体图像分析结果数据表\
			  where 染色体图像分析结果数据表.[照片路径]='" + picpath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		CString str;
		str = m_Conn.m_pRecordset->GetCollect(_T("系统判定的双着体条数"));
		m_dic.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("系统判定的染色体环数"));
		m_round.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("系统判定的无着丝体数"));
		m_ace.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("系统判定的相互易位数"));
		m_t.SetWindowTextW(str);		
		str = m_Conn.m_pRecordset->GetCollect(_T("系统判定的倒位体个数"));
		m_inv.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("tri"));
		m_tri.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("ten"));
		m_ten.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("del"));
		m_del.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("ctg"));
		m_ctg.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("csg"));
		m_csg.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("ctb"));
		m_ctb.SetWindowTextW(str);
		str = m_Conn.m_pRecordset->GetCollect(_T("cte"));
		m_cte.SetWindowTextW(str);

		var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
		if (var.vt != VT_NULL)
			m_dicrg.SetWindowTextW((_bstr_t)var);
		else
			m_dicrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
		if (var.vt != VT_NULL)
			m_roundrg.SetWindowTextW((_bstr_t)var);
		else
			m_roundrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
		if (var.vt != VT_NULL)
			m_acerg.SetWindowTextW((_bstr_t)var);
		else
			m_acerg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
		if (var.vt != VT_NULL)
			m_trg.SetWindowTextW((_bstr_t)var);
		else
			m_trg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
		if (var.vt != VT_NULL)
			m_invrg.SetWindowTextW((_bstr_t)var);
		else
			m_invrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
		if (var.vt != VT_NULL)
			m_trirg.SetWindowTextW((_bstr_t)var);
		else
			m_trirg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
		if (var.vt != VT_NULL)
			m_tenrg.SetWindowTextW((_bstr_t)var);
		else
			m_tenrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
		if (var.vt != VT_NULL)
			m_delrg.SetWindowTextW((_bstr_t)var);
		else
			m_delrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
		if (var.vt != VT_NULL)
			m_ctgrg.SetWindowTextW((_bstr_t)var);
		else
			m_ctgrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
		if (var.vt != VT_NULL)
			m_csgrg.SetWindowTextW((_bstr_t)var);
		else
			m_csgrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
		if (var.vt != VT_NULL)
			m_ctbrg.SetWindowTextW((_bstr_t)var);
		else
			m_ctbrg.SetWindowTextW(NULL);

		var = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
		if (var.vt != VT_NULL)
			m_cterg.SetWindowTextW((_bstr_t)var);
		else
			m_cterg.SetWindowTextW(NULL);

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}

////删除
//void CChromosomeResult::OnMenuDelchro()
//{
//	// TODO:  在此添加命令处理程序代码
//	CGWSystem10App *app = (CGWSystem10App*)AfxGetApp();
//	CFileFind find;
//	CString rootdir;
//	BOOL result = find.FindFile(strpathgood);//判断子目录是否存在  
//	if (result)
//	{
//		find.FindNextFile();
//		rootdir = find.GetRoot();//父目录  
//	}
//	CString deletefile = rootdir + _T("删除的图片\\");
//	CString sourcefile2 = rootdir + _T("合格的图片\\");
//	USES_CONVERSION;
//	CString  myfilename = app->GetFileTitleFromFileName(A2W(imgNamesCHRO[9 * Page + Frame].c_str()), TRUE);
//	CString quafilename = sourcefile2 + myfilename;
//	MarkImgCHRO.DeleteImgCHRO(A2W(imgNamesCHRO[9 * Page + Frame].c_str()), deletefile);
//	MarkImgCHRO.DeleteImgInQuaCHRO(quafilename, deletefile);
//	//2.删除之后显示照片
//	//QuaImgCurrentPath中删除
//	for (vector<string>::iterator it = QuaImgCurrentPath.begin(); it != QuaImgCurrentPath.end();)
//	{
//		if (*it == imgNamesCHRO[9 * Page + Frame])
//		{
//			it = QuaImgCurrentPath.erase(it);
//		}
//		else
//		{
//			++it;
//		}
//	}
//	//QuaImgCurrentFile中删除
//	for (vector<PatientAndCurImg>::iterator it = QuaImgCurrentFile.begin(); it != QuaImgCurrentFile.end();)
//	{
//		if (it->currentImgPath == imgNamesCHRO[9 * Page + Frame])
//		{
//			it = QuaImgCurrentFile.erase(it);
//		}
//		else
//		{
//			++it;
//		}
//	}
//	for (int v = 0; v < 9; v++)
//	{
//		PicClear(IDC_NameCHRO[v]);
//	}
//	int current = m_comboxpatient.GetCurSel();
//	if (current >= 0)
//	{
//		currentpatientpath = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].QuestionImgPath;
//		//显示选中病人的照片
//		ShowPatientImg(currentpatientpath);
//	}
//	UpdateWindow();
//	CountTheSumFromAcces();
//	//将结果显示清空
//	CString str = NULL;
//
//	m_dic.SetWindowTextW(str);
//	m_t.SetWindowTextW(str);
//	m_round.SetWindowTextW(str);
//	m_inv.SetWindowTextW(str);
//	m_ace.SetWindowTextW(str);
//
//	m_dicrg.SetWindowTextW(str);
//	m_trg.SetWindowTextW(str);
//	m_roundrg.SetWindowTextW(str);
//	m_invrg.SetWindowTextW(str);
//	m_acerg.SetWindowTextW(str);
//}
//
////取消删除
//void CChromosomeResult::OnMenuCanceldelchro()
//{
//	// TODO:  在此添加命令处理程序代码
//	MarkImgCHRO.CancelDeleteCHRO();
//	CountTheSumFromAcces();
//	//将结果显示清空
//	CString str = NULL;
//	for (int v = 0; v < 9; v++)
//	{
//		PicClear(IDC_NameCHRO[v]);
//	}
//	m_dic.SetWindowTextW(str);
//	m_t.SetWindowTextW(str);
//	m_round.SetWindowTextW(str);
//	m_inv.SetWindowTextW(str);
//	m_ace.SetWindowTextW(str);
//
//	m_dicrg.SetWindowTextW(str);
//	m_trg.SetWindowTextW(str);
//	m_roundrg.SetWindowTextW(str);
//	m_invrg.SetWindowTextW(str);
//	m_acerg.SetWindowTextW(str);
//}

////标记为合格
//void CChromosomeResult::OnMenuQuachro()
//{
//	// TODO:  在此添加命令处理程序代码
//	//找到存合格图片的文件夹
//	CFileFind find;
//	CString rootdir;
//	BOOL result = find.FindFile(strpathgood);//判断子目录是否存在  
//	if (result)
//	{
//		find.FindNextFile();
//		rootdir = find.GetRoot();//父目录  
//	}
//	CString quafile = rootdir + _T("合格的图片\\");
//	USES_CONVERSION;
//	MarkImgCHRO.QuaImgCHRO(A2W(imgNamesCHRO[9 * Page + Frame].c_str()), quafile);
//	int current = m_comboxpatient.GetCurSel();
//	if (current >= 0)
//	{
//		currentpatientpath = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].QuestionImgPath;
//		//显示选中病人的照片
//		ShowPatientImg(currentpatientpath);
//	}
//	UpdateWindow();
//
//}
//
////取消标记为合格
//void CChromosomeResult::OnMenuCancelquachro()
//{
//	// TODO:  在此添加命令处理程序代码
//	MarkImgCHRO.CancelQuaCHRO();
//
//}
//
////标记为不合格
//void CChromosomeResult::OnMenuUnqchro()
//{
//	// TODO:  在此添加命令处理程序代码
//	//找到存合格图片的文件夹
//	CFileFind find;
//	CString rootdir;
//	BOOL result = find.FindFile(strpathgood);//判断子目录是否存在  
//	if (result)
//	{
//		find.FindNextFile();
//		rootdir = find.GetRoot();//父目录  
//	}
//	CString unqfile = rootdir + _T("不合格的图片\\");
//	USES_CONVERSION;
//	MarkImgCHRO.UnqImgCHRO(A2W(imgNamesCHRO[9 * Page + Frame].c_str()), unqfile);
//	int current = m_comboxpatient.GetCurSel();
//	if (current >= 0)
//	{
//		currentpatientpath = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].QuestionImgPath;
//		//显示选中病人的照片
//		ShowPatientImg(currentpatientpath);
//	}
//	UpdateWindow();
//
//}



//确定录入
void CChromosomeResult::OnBnClickedBtnChload2lib()
{
	// TODO:  在此添加控件通知处理程序代码
	CEdit *p;
	CString mystr;
	USES_CONVERSION;
	mystr = AllImgAndResult[Temp].picpath;
	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 			 where 染色体图像分析结果数据表.[照片路径]='" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		UpdateData(FALSE);
		p = (CEdit*)GetDlgItem(IDC_EDIT_DICRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的双着体条数"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_ROUNDRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的着丝粒环数"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_ACERG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的无着丝体数"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_TRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的相互易位数"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_INVRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("人工校对的倒位体个数"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_TRIRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_TENRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_DELRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_CTGRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_CTBRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_CSGRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_CTERG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(mystr));

		p = (CEdit*)GetDlgItem(IDC_EDIT_CHRONUMRG);
		p->GetWindowTextW(mystr);
		m_Conn.m_pRecordset->PutCollect(_T("chromosome_num_check"), _variant_t(mystr));

		//p = (CEdit*)GetDlgItem(IDC_EDIT_CHRONUMRG);
		//p->GetWindowTextW(mystr);
		//m_Conn.m_pRecordset->PutCollect(_T("chromosome_num_check"), _variant_t(mystr));

		//关键的两句
		m_Conn.m_pRecordset->Update();

		/*OnMenuUnqchro();*/
		//从数据库读取该张照片处理结果
		USES_CONVERSION;
		//ReadThePicResult(A2W(imgNamesCHRO[Temp].c_str()));
		CountTheSumFromAcces(CurrentPatientName);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

////整理出人工校正结果，并存入数据库
//void CChromosomeResult::SortDataToAccess()
//{
//	for (size_t i = 0; i < PatientNames.size(); i++)
//	{
//		//整理出该病人总的校正结果
//		CHRO_HandleResult amendpA;
//
//		sql = _T("select * from 染色体图像分析结果数据表\
//				 where 染色体图像分析结果数据表.[病人名称]='" + PatientNames[i] + "' ");
//		try
//		{
//			m_Conn.GetRecordSet(sql);
//		}
//		catch (_com_error *e)
//		{
//			AfxMessageBox(e->ErrorMessage());
//		}
//
//		try
//		{
//			m_Conn.m_pRecordset->MoveFirst();
//			CString mystr;
//			while (!m_Conn.m_pRecordset->adoEOF)
//			{
//				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
//				amendpA.dic += _ttoi(mystr);
//				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
//				amendpA.round += _ttoi(mystr);
//				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
//				amendpA.ace += _ttoi(mystr);
//				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
//				amendpA.t += _ttoi(mystr);
//				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
//				amendpA.inv += _ttoi(mystr);
//
//				m_Conn.m_pRecordset->MoveNext();
//			}
//		}
//		catch (_com_error e)
//		{
//			AfxMessageBox(e.Description());
//		}
//
//
//		//将该病人总的校正结果存入当前分析表
//		sql = _T("select * from 染色体分析结果数据表（本次）\
//				 	where 染色体分析结果数据表（本次）.[所属病人或代号]='" + PatientNames[i] + "' ");
//		try
//		{
//			m_Conn.GetRecordSet(sql);
//		}
//		catch (_com_error *e)
//		{
//			AfxMessageBox(e->ErrorMessage());
//		}
//
//		try
//		{
//			//将对应的玻片编号和照片序数存入数组，用于报表生成
//			m_Conn.m_pRecordset->PutCollect(_T("人工校对的双着体条数"), _variant_t(amendpA.dic));
//			m_Conn.m_pRecordset->PutCollect(_T("人工校对的着丝粒环数"), _variant_t(amendpA.round));
//			m_Conn.m_pRecordset->PutCollect(_T("人工校对的无着丝体数"), _variant_t(amendpA.ace));
//			m_Conn.m_pRecordset->PutCollect(_T("人工校对的相互易位数"), _variant_t(amendpA.t));
//			m_Conn.m_pRecordset->PutCollect(_T("人工校对的倒位体个数"), _variant_t(amendpA.inv));
//
//			m_Conn.m_pRecordset->Update();
//
//		}
//		catch (_com_error e)
//		{
//			AfxMessageBox(e.Description());
//		}
//
//
//	}
//}



// 从数据库中统计已分析的图片结果
void CChromosomeResult::CountTheSumFromAcces(CString patientname)
{
	int Sum_Dic = 0;
	int Sum_Round = 0;
	int Sum_Ace = 0;
	int Sum_T = 0;
	int Sum_Inv = 0;

	int Sum_Tri = 0;
	int Sum_Ten = 0;
	int Sum_Del = 0;
	int Sum_Ctg = 0;
	int Sum_Gsg = 0;
	int Sum_Ctb = 0;
	int Sum_Gte = 0;

	int Sum_Abnormal = 0;
	int Sum_Qua = 0;

	int allAbnormalNum = 0;
	CString name, qua_unq;
	string strname = CT2A(patientname.GetBuffer());
	string strName;
	bool equal;

	sql = _T("select * from 染色体图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		//m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			name = (CString)m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			qua_unq = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
			strName = CT2A(name.GetBuffer());

			if (strname == strName)
				equal = true;
			else
				equal = false;

			if (equal)
			{
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
				allAbnormalNum += _ttoi(mystr);

				mystr = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
				allAbnormalNum += _ttoi(mystr);

				if (allAbnormalNum != 0)
				{
					Sum_Abnormal++;
					cout << "Sum_Abnormal  " << Sum_Abnormal << endl;
				}

			}


			if (equal&& qua_unq == "1")
			{
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
				Sum_Dic = _ttoi(mystr) + Sum_Dic;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
				Sum_Round = _ttoi(mystr) + Sum_Round;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
				Sum_Ace = _ttoi(mystr) + Sum_Ace;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
				Sum_T = _ttoi(mystr) + Sum_T;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
				Sum_Inv = _ttoi(mystr) + Sum_Inv;
				allAbnormalNum += _ttoi(mystr);

				mystr = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
				Sum_Tri = _ttoi(mystr) + Sum_Tri;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
				Sum_Ten = _ttoi(mystr) + Sum_Ten;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
				Sum_Del = _ttoi(mystr) + Sum_Del;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
				Sum_Ctg = _ttoi(mystr) + Sum_Ctg;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
				Sum_Gsg = _ttoi(mystr) + Sum_Gsg;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
				Sum_Ctb = _ttoi(mystr) + Sum_Ctb;
				allAbnormalNum += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
				Sum_Gte = _ttoi(mystr) + Sum_Gte;
				allAbnormalNum += _ttoi(mystr);


				Sum_Qua++;
				cout << "Sum_Qua  " << Sum_Dic << endl;



			}



			m_Conn.m_pRecordset->MoveNext();
			}
		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());
		}

		CString mystring;
		mystring.Format(_T("%d"), Sum_Dic);
		m_sumdic.SetWindowText(mystring);
		cout << "Sum_Dic  " << Sum_Dic << endl;

		mystring.Format(_T("%d"), Sum_Round);
		m_sumround.SetWindowText(mystring);
		cout << "Sum_Round  " << Sum_Round << endl;


		mystring.Format(_T("%d"), Sum_Ace);
		m_sumace.SetWindowText(mystring);
		cout << "Sum_Ace  " << Sum_Ace << endl;


		mystring.Format(_T("%d"), Sum_T);
		m_sumtrg.SetWindowText(mystring);
		cout << "Sum_T  " << Sum_T << endl;


		mystring.Format(_T("%d"), Sum_Inv);
		m_suminv.SetWindowText(mystring);
		cout << "Sum_Inv  " << Sum_Inv << endl;


		mystring.Format(_T("%d"), Sum_Tri);
		m_sumtri.SetWindowText(mystring);
		cout << "Sum_Tri  " << Sum_Tri << endl;


		mystring.Format(_T("%d"), Sum_Ten);
		m_sumten.SetWindowText(mystring);
		cout << "Sum_Ten  " << Sum_Ten << endl;


		mystring.Format(_T("%d"), Sum_Del);
		m_sumdel.SetWindowText(mystring);

		mystring.Format(_T("%d"), Sum_Ctg);
		m_sumctg.SetWindowText(mystring);

		mystring.Format(_T("%d"), Sum_Gsg);
		m_sumcsg.SetWindowText(mystring);

		mystring.Format(_T("%d"), Sum_Ctb);
		m_sumctb.SetWindowText(mystring);

		mystring.Format(_T("%d"), Sum_Gte);
		m_sumcte.SetWindowText(mystring);

		mystring.Format(_T("%d"), Sum_Abnormal);
		m_abnormalnum.SetWindowText(mystring);
		cout << "Sum_Abnormal  " << Sum_Abnormal << endl;


		mystring.Format(_T("%d"), Sum_Qua);
		m_quanum.SetWindowText(mystring);
		cout << "Sum_Qua  " << Sum_Qua << endl;

}

//校正完成
void CChromosomeResult::OnBnClickedBtnCheckoverchro()
{
	// TODO:  在此添加控件通知处理程序代码
	CString name = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].patientname;
	//整理出该病人总的校正结果
	CHRO_HandleResult amendpA;

	//USES_CONVERSION;
	//CString mystr = A2W(QuaImgCurrentFile[i].currentImgPath.c_str());
	sql = _T("select * from 染色体图像分析结果数据表\
			 					 					 					 		where 染色体图像分析结果数据表.[所属病人或代号]='" + name + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			int quaornot = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
			if (quaornot == 1)
			{
				CString mystr;
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
				amendpA.dic += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
				amendpA.round += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
				amendpA.ace += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
				amendpA.t += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
				amendpA.inv += _ttoi(mystr);

				mystr = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
				amendpA.tri += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
				amendpA.ten += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
				amendpA.del += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
				amendpA.ctg += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
				amendpA.csg += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
				amendpA.ctb += _ttoi(mystr);
				mystr = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
				amendpA.cte += _ttoi(mystr);

			}
			
			m_Conn.m_pRecordset->MoveNext();

		}
		
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
		
	//将该病人总的校正结果存入当前分析表
	sql = _T("select * from 染色体分析结果数据表（本次）\
			 			 			where 染色体分析结果数据表（本次）.[所属病人或代号]='" + name + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		//将对应的玻片编号和照片序数存入数组，用于报表生成
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的双着丝粒体数"), _variant_t(amendpA.dic));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的着丝粒环数"), _variant_t(amendpA.round));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的无着丝粒体数"), _variant_t(amendpA.ace));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的相互易位数"), _variant_t(amendpA.t));
		m_Conn.m_pRecordset->PutCollect(_T("人工校正的倒位数"), _variant_t(amendpA.inv));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(amendpA.tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(amendpA.ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(amendpA.del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(amendpA.ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(amendpA.csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(amendpA.ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(amendpA.cte));
		m_Conn.m_pRecordset->Update();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}


void CChromosomeResult::OnEnChangeEditDicrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码

}


void CChromosomeResult::OnEnChangeEditTrirg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditRoundrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditAcerg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码

}


void CChromosomeResult::OnEnChangeEditTenrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditTrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditInvrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditDelrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditCtgrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditCsgrg()
{
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditCtbrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}


void CChromosomeResult::OnEnChangeEditCterg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	// TODO:  在此添加控件通知处理程序代码
}

void CChromosomeResult::OnEnChangeEditChronumrg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	if (checked)
	{
		OnBnClickedBtnChload2lib();
	}	

}

//数据库下搜索并记录该病人的图片结果，输出结构体
vector<CHRO_HandleResultOnePic>CChromosomeResult::GetOnePatientImgInformation(CString patientname)
{
	CString name,mode;
	vector<CHRO_HandleResultOnePic> result;
	CHRO_HandleResultOnePic onepic;
	//从数据库中获取病人照片路径 存入数组
	sql = _T("select * from 染色体图像分析结果数据表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			name = m_Conn.m_pRecordset->GetCollect(_T("所属病人或代号"));
			//mode = m_Conn.m_pRecordset->GetCollect(_T("模式"));

			//if (name.Find(patientname) != -1 && mode == "染色体")
			if (name.Find(patientname) != -1 )
			{
				onepic.patientname = name;
				onepic.picpath = m_Conn.m_pRecordset->GetCollect(_T("照片路径"));

				onepic.dic = m_Conn.m_pRecordset->GetCollect(_T("系统判定的双着体条数"));
				onepic.round = m_Conn.m_pRecordset->GetCollect(_T("系统判定的染色体环数"));
				onepic.ace = m_Conn.m_pRecordset->GetCollect(_T("系统判定的无着丝体数"));
				onepic.t = m_Conn.m_pRecordset->GetCollect(_T("系统判定的相互易位数"));
				onepic.inv = m_Conn.m_pRecordset->GetCollect(_T("系统判定的倒位体个数"));
				onepic.tri = m_Conn.m_pRecordset->GetCollect(_T("tri"));
				onepic.ten = m_Conn.m_pRecordset->GetCollect(_T("ten"));
				onepic.del = m_Conn.m_pRecordset->GetCollect(_T("del"));
				onepic.ctg = m_Conn.m_pRecordset->GetCollect(_T("ctg"));
				onepic.csg = m_Conn.m_pRecordset->GetCollect(_T("csg"));
				onepic.ctb = m_Conn.m_pRecordset->GetCollect(_T("ctb"));
				onepic.cte = m_Conn.m_pRecordset->GetCollect(_T("cte"));

				CString str = m_Conn.m_pRecordset->GetCollect(_T("CheckPath"));
				onepic.CheckPath = str;

				var = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num"));
				if (var.vt != VT_NULL)
					onepic.chromosome_num = _ttoi((_bstr_t)var);
				else
					onepic.chromosome_num = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
				if (var.vt != VT_NULL)
					onepic.QuaOrNot = _ttoi((_bstr_t)var);
				else
					onepic.QuaOrNot = -1;

				//onepic.chromosome_num = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num"));
				//onepic.QuaOrNot = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));


				var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的双着体条数"));
				if (var.vt != VT_NULL)
					onepic.dic_rg = _ttoi((_bstr_t)var);
				else
					onepic.dic_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的着丝粒环数"));
				if (var.vt != VT_NULL)
					onepic.round_rg = _ttoi((_bstr_t)var);
				else
					onepic.round_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的无着丝体数"));
				if (var.vt != VT_NULL)
					onepic.ace_rg = _ttoi((_bstr_t)var);
				else
					onepic.ace_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的相互易位数"));
				if (var.vt != VT_NULL)
					onepic.t_rg = _ttoi((_bstr_t)var);
				else
					onepic.t_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("人工校对的倒位体个数"));
				if (var.vt != VT_NULL)
					onepic.inv_rg = _ttoi((_bstr_t)var);
				else
					onepic.inv_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
				if (var.vt != VT_NULL)
					onepic.tri_rg = _ttoi((_bstr_t)var);
				else
					onepic.tri_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
				if (var.vt != VT_NULL)
					onepic.ten_rg = _ttoi((_bstr_t)var);
				else
					onepic.ten_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
				if (var.vt != VT_NULL)
					onepic.del_rg = _ttoi((_bstr_t)var);
				else
					onepic.del_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
				if (var.vt != VT_NULL)
					onepic.ctg_rg = _ttoi((_bstr_t)var);
				else
					onepic.ctg_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
				if (var.vt != VT_NULL)
					onepic.csg_rg = _ttoi((_bstr_t)var);
				else
					onepic.csg_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
				if (var.vt != VT_NULL)
					onepic.ctb_rg = _ttoi((_bstr_t)var);
				else
					onepic.ctb_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
				if (var.vt != VT_NULL)
					onepic.cte_rg = _ttoi((_bstr_t)var);
				else
					onepic.cte_rg = 0;

				var = m_Conn.m_pRecordset->GetCollect(_T("chromosome_num_check"));
				if (var.vt != VT_NULL)
					onepic.chromosome_num_check = _ttoi((_bstr_t)var);
				else
					onepic.chromosome_num_check = 0;



				result.push_back(onepic);
			}


			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


	return result;

}

//将病人图片显示到控件
void CChromosomeResult::ShowPicToImgctrl(vector<CHRO_HandleResultOnePic> allpic)
{
	int j = 0;

	int sumpagenum = allpic.size() / 9 + 1;
	sumpage.Format(_T("%d"), sumpagenum);
	currentpage.Format(_T("%d"), Page + 1);
	pageshow = _T("第") + currentpage + _T("页") + "/" + _T("共") + sumpage + _T("页");
	m_page.SetWindowText(pageshow);

	if (9 * Page < allpic.size())
	{
		for (int i = 9 * Page; j < 9 && i < allpic.size(); i++)
		{
			//显示图片
			string currrentpath;
			if (allpic[i].CheckPath =="0")
				currrentpath = CT2A(allpic[i].picpath.GetBuffer());
			else
				currrentpath = CT2A(allpic[i].CheckPath.GetBuffer());

			Mat CurrentMat = cv::imread(currrentpath);

			switch (j)
			{
			case 0:
				m_picchro1.ShowImage(CurrentMat, 1);
				break;
			case 1:
				m_picchro2.ShowImage(CurrentMat, 1);
				break;
			case 2:
				m_picchro3.ShowImage(CurrentMat, 1);
				break;
			case 3:
				m_picchro4.ShowImage(CurrentMat, 1);
				break;
			case 4:
				m_picchro5.ShowImage(CurrentMat, 1);
				break;
			case 5:
				m_picchro6.ShowImage(CurrentMat, 1);
				break;
			case 6:
				m_picchro7.ShowImage(CurrentMat, 1);
				break;
			case 7:
				m_picchro8.ShowImage(CurrentMat, 1);
				break;
			case 8:
				m_picchro9.ShowImage(CurrentMat, 1);
				break;
			default:
				MessageBox(TEXT("出错了..."));
				break;
			}

			//合格还是不合格
			CDC *dc = new CDC;
			//判断图片是否合格，若合格，边框标为绿色
			if (allpic[i].QuaOrNot == 1)
			{
				CRect rtTop1;
				switch (j)
				{
				case 0:
					dc = m_picchro1.GetDC();
					m_picchro1.GetClientRect(&rtTop1);
					break;
				case 1:
					dc = m_picchro2.GetDC();
					m_picchro2.GetClientRect(&rtTop1);
					break;
				case 2:
					dc = m_picchro3.GetDC();
					m_picchro3.GetClientRect(&rtTop1);
					break;
				case 3:
					dc = m_picchro4.GetDC();
					m_picchro4.GetClientRect(&rtTop1);
					break;
				case 4:
					dc = m_picchro5.GetDC();
					m_picchro5.GetClientRect(&rtTop1);
					break;
				case 5:
					dc = m_picchro6.GetDC();
					m_picchro6.GetClientRect(&rtTop1);
					break;
				case 6:
					dc = m_picchro7.GetDC();
					m_picchro7.GetClientRect(&rtTop1);
					break;
				case 7:
					dc = m_picchro8.GetDC();
					m_picchro8.GetClientRect(&rtTop1);
					break;
				case 8:
					dc = m_picchro9.GetDC();
					m_picchro9.GetClientRect(&rtTop1);
					break;
				default:
					MessageBox(TEXT("出错了..."));
					break;
				}
				dc->SelectObject(&greenPen);//在dc上放置画笔
				dc->SelectStockObject(NULL_BRUSH);//使画出的矩形区域为透明
				dc->Rectangle(rtTop1);
			}
			if (allpic[i].QuaOrNot == -1)
			{
				CRect rtTop1;
				switch (j)
				{
				case 0:
					dc = m_picchro1.GetDC();
					m_picchro1.GetClientRect(&rtTop1);
					break;
				case 1:
					dc = m_picchro2.GetDC();
					m_picchro2.GetClientRect(&rtTop1);
					break;
				case 2:
					dc = m_picchro3.GetDC();
					m_picchro3.GetClientRect(&rtTop1);
					break;
				case 3:
					dc = m_picchro4.GetDC();
					m_picchro4.GetClientRect(&rtTop1);
					break;
				case 4:
					dc = m_picchro5.GetDC();
					m_picchro5.GetClientRect(&rtTop1);
					break;
				case 5:
					dc = m_picchro6.GetDC();
					m_picchro6.GetClientRect(&rtTop1);
					break;
				case 6:
					dc = m_picchro7.GetDC();
					m_picchro7.GetClientRect(&rtTop1);
					break;
				case 7:
					dc = m_picchro8.GetDC();
					m_picchro8.GetClientRect(&rtTop1);
					break;
				case 8:
					dc = m_picchro9.GetDC();
					m_picchro9.GetClientRect(&rtTop1);
					break;
				default:
					MessageBox(TEXT("出错了..."));
					break;
				}
				dc->SelectObject(&redPen);//在dc上放置画笔
				dc->SelectStockObject(NULL_BRUSH);//使画出的矩形区域为透明
				dc->Rectangle(rtTop1);
			}

			j++;
		}
	}
}

//标记为合格
void  CChromosomeResult::MarkQua(CString picturepath)
{
	//1.数据库中改为合格
	sql = _T("select * from 染色体图像分析结果数据表\
			 			 					 			 where 染色体图像分析结果数据表.[照片路径]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			int quaornot = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
			/*if (quaornot != -1)*/
			m_Conn.m_pRecordset->PutCollect(_T("QuaOrNot"), _variant_t("1"));
			m_Conn.m_pRecordset->MoveNext();

		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//刷新界面
	vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
	AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
	ShowPicToImgctrl(AllImgAndResult);

}

//标记为不合格
void  CChromosomeResult::MarkUnq(CString picturepath)
{
	//1.数据库中改为合格
	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 					 			 where 染色体图像分析结果数据表.[照片路径]='" + picturepath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			int quaornot = m_Conn.m_pRecordset->GetCollect(_T("QuaOrNot"));
			/*if (quaornot != -1)*/
			m_Conn.m_pRecordset->PutCollect(_T("QuaOrNot"), _variant_t("-1"));
			m_Conn.m_pRecordset->MoveNext();

		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//刷新界面
	vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
	AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
	ShowPicToImgctrl(AllImgAndResult);
}

//右击picture control 弹出菜单
void CChromosomeResult::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	for (unsigned int i = 0; i < 9; i++)
	{
		if (rectCHRO[i].PtInRect(point))
		{
			Picctrl_Num = i;
		}
	}

	//右键弹出菜单
	CMenu menu;
	menu.LoadMenuW(IDR_MENU_MARKCHRO);//加载菜单资源
	CMenu *pPopup = menu.GetSubMenu(0);
	ClientToScreen(&point);//将客户区坐标转换为屏幕坐标
	//显示右键菜单，由视类窗口拥有。
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	CDialogEx::OnRButtonDown(nFlags, point);
}

//左键单击图片显示到大窗口
//void CChromosomeResult::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	for (unsigned int i = 0; i < 9; i++)
//	{
//		if (rectCHRO[i].PtInRect(point))
//		{
//			//CString str;
//			//str.Format(TEXT("在 %d 号框内"), i);
//			//MessageBox(str);
//			Frame = i;
//			Temp = Page * 9 + Frame;
//			break;
//		}
//	}
//
//
//
//	//将选中的图片放大显示到主控件
//	if (Temp < AllImgAndResult.size())
//	{
//		string currrentpath;
//		if (AllImgAndResult[Temp].CheckPath == "0")
//			currrentpath = CT2A(AllImgAndResult[Temp].picpath.GetBuffer());
//		else
//			currrentpath = CT2A(AllImgAndResult[Temp].CheckPath.GetBuffer());
//
//		if (CurrentPicInFrame.empty() || CurrentPicInFrame != currrentpath)
//		{
//			ImgMat = cv::imread(currrentpath);
//			m_bigImgchro.ShowImage(ImgMat, 1);
//			CurrentPicInFrame = currrentpath;
//
//			GetDlgItem(IDC_BTN_QUA)->EnableWindow(TRUE);
//			GetDlgItem(IDC_BTN_UNQ)->EnableWindow(TRUE);
//			GetDlgItem(IDC_BTN_EDIT)->EnableWindow(TRUE);
//
//			//从数据库读取该张照片处理结果
//			CString str;
//			str.Format(_T("%d"), AllImgAndResult[Temp].dic);
//			m_dic.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].round);
//			m_round.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ace);
//			m_ace.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].t);
//			m_t.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].inv);
//			m_inv.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].tri);
//			m_tri.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ten);
//			m_ten.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].del);
//			m_del.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ctg);
//			m_ctg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].csg);
//			m_csg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ctb);
//			m_ctb.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].cte);
//			m_cte.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].dic_rg);
//			m_dicrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].round_rg);
//			m_roundrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ace_rg);
//			m_acerg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].t_rg);
//			m_trg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].inv_rg);
//			m_invrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].tri_rg);
//			m_trirg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ten_rg);
//			m_tenrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].del_rg);
//			m_delrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ctg_rg);
//			m_ctgrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].csg_rg);
//			m_csgrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].ctb_rg);
//			m_ctbrg.SetWindowTextW(str);
//
//			str.Format(_T("%d"), AllImgAndResult[Temp].cte_rg);
//			m_cterg.SetWindowTextW(str);
//
//
//			//标记为合格
//			/*MarkQua(AllImgAndResult[Temp].picpath);*/
//			//从数据库中统计所有合格的（已校正的）图片的总分析结果
//			CountTheSumFromAcces(CurrentPatientName);
//
//		}
//
//	}
//
//	m_bigImgchro.OnLButtonDown(nFlags, point);
//	CDialogEx::OnLButtonDown(nFlags, point);
//}



//编辑
void CChromosomeResult::OnBnClickedBtnEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str = AllImgAndResult[Temp].picpath;
	string str1 = CT2A(AllImgAndResult[Temp].picpath.GetBuffer());
	string strsavefile = str1.substr(0, str1.find_last_of('\\'));
	savepathROI = strsavefile + "\\src_roi.bmp";

	EditOrNot = true;
	vector<cv::Point>().swap(PointImg);
	currentPointStep = 0;
	srcROI = m_bigImgchro.m_dst_roi;
	//cv::imshow("原图.bmp", srcROI);
	cv::imwrite(savepathROI, srcROI);
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CANCEL)->EnableWindow(TRUE);
}

//保存
void CChromosomeResult::OnBnClickedBtnSave()
{
	// TODO:  在此添加控件通知处理程序代码
	EditOrNot = false;



	CString str = AllImgAndResult[Temp].picpath;
	string str1 = CT2A(AllImgAndResult[Temp].picpath.GetBuffer());
	string strsavefile = str1.substr(0, str1.find_last_of('\\'));
	stringstream ss;
	ss << Temp;
	string str2 = ss.str();

	string strsavepath = strsavefile + "\\check" + str2 + ".bmp";
	cv::imwrite(strsavepath, m_bigImgchro.m_dst_roi);

	USES_CONVERSION;
	CString savepath(strsavepath.c_str(), strsavepath.length());
	

	sql = _T("select * from 染色体图像分析结果数据表\
			 			 			 		where 染色体图像分析结果数据表.[照片路径]='" + AllImgAndResult[Temp].picpath + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			m_Conn.m_pRecordset->PutCollect(_T("CheckPath"), _variant_t(savepath));
			m_point_sum.GetWindowTextW(str);
			//p = (CEdit*)GetDlgItem(IDC_EDIT_CHRONUMRG);
			//p->GetWindowTextW(mystr);
			m_Conn.m_pRecordset->PutCollect(_T("chromosome_num_check"), _variant_t(str));
			m_Conn.m_pRecordset->MoveNext();

		}
	}



	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//刷新界面
	vector<CHRO_HandleResultOnePic>().swap(AllImgAndResult);
	AllImgAndResult = GetOnePatientImgInformation(CurrentPatientName);
	ShowPicToImgctrl(AllImgAndResult);

	/*cout << AllImgAndResult[Temp].chromosome_num_check << endl;*/
	str.Format(_T("%d"), AllImgAndResult[Temp].chromosome_num_check);
	m_chronumrg.SetWindowTextW(str);

	//删除编辑原图
	char DesTempFilePath[MAX_PATH + 1];
	WCHAR   wstr_des[MAX_PATH];
	sprintf_s(DesTempFilePath, "%s", savepathROI.c_str());
	MultiByteToWideChar(CP_ACP, 0, DesTempFilePath, -1, wstr_des, sizeof(wstr_des));
	int abc = DeleteFile(wstr_des);

	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CANCEL)->EnableWindow(FALSE);

}

//取消
void CChromosomeResult::OnBnClickedBtnCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	EditOrNot = false;
	vector<cv::Point>().swap(PointImg);
	string currrentpath = CT2A(AllImgAndResult[Temp].picpath.GetBuffer());
	ImgMat = cv::imread(currrentpath);	
	m_bigImgchro.ShowImage(ImgMat, 1);

	GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CANCEL)->EnableWindow(FALSE);
}

//上一步
void CChromosomeResult::OnBnClickedBtnLast()
{
	// TODO:  在此添加控件通知处理程序代码
	currentPointStep++;
	cv::Mat copySRC = cv::imread(savepathROI);
	//cv::imshow("原图.bmp", copySRC);
	if (PointImg.size() - currentPointStep == 0)
	{
		GetDlgItem(IDC_BTN_LAST)->EnableWindow(FALSE);
		m_bigImgchro.ShowImage(copySRC, 1);
	}
	else
	{
		int point_sum = 0;
		for (size_t i = 0; i < PointImg.size() - currentPointStep; i++)
		{
			cv::circle(copySRC, PointImg[i], 10, cv::Scalar(0, 255, 0), -1);  // 画半径为1的圆(画点）
			point_sum++;
		}
		if (!copySRC.empty())
		{
			m_bigImgchro.ShowImage(copySRC, 1);
			CString strnum;
			strnum.Format(_T("%d"), point_sum);
			m_point_sum.SetWindowTextW(strnum);
		}


		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(TRUE);
	}

}



//下一步
void CChromosomeResult::OnBnClickedBtnNext()
{
	// TODO:  在此添加控件通知处理程序代码
	currentPointStep--;
	GetDlgItem(IDC_BTN_LAST)->EnableWindow(TRUE);

	if (currentPointStep <0)
		GetDlgItem(IDC_BTN_NEXT)->EnableWindow(FALSE);
	else
	{
		cv::Mat copySRC = cv::imread(savepathROI);
		//cv::imshow("原图.bmp", copySRC);
		int point_sum = 0;
		for (size_t i = 0; i < PointImg.size() - currentPointStep; i++)
		{
			cv::circle(copySRC, PointImg[i], 10, cv::Scalar(0, 255, 0), -1);  // 画半径为1的圆(画点）
			point_sum++;
		}
		if (!copySRC.empty())
		{
			m_bigImgchro.ShowImage(copySRC, 1);
			CString strnum;
			strnum.Format(_T("%d"), point_sum);
			m_point_sum.SetWindowTextW(strnum);
		}

	}

}

//合格
void CChromosomeResult::OnBnClickedBtnQua()
{
	// TODO:  在此添加控件通知处理程序代码
	MarkQua(AllImgAndResult[Temp].picpath);
}


void CChromosomeResult::OnBnClickedBtnUnq()
{
	// TODO:  在此添加控件通知处理程序代码
	MarkUnq(AllImgAndResult[Temp].picpath);
}


void CChromosomeResult::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnKeyDown(nChar, nRepCnt, nFlags);

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChromosomeResult::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

		for (unsigned int i = 0; i < 9; i++)
		{
			if (rectCHRO[i].PtInRect(point))
			{
				//CString str;
				//str.Format(TEXT("在 %d 号框内"), i);
				//MessageBox(str);
				Frame = i;
				Temp = Page * 9 + Frame;
				break;
			}
		}



		//将选中的图片放大显示到主控件
		if (Temp < AllImgAndResult.size())
		{
			string currrentpath;
			if (AllImgAndResult[Temp].CheckPath == "0")
				currrentpath = CT2A(AllImgAndResult[Temp].picpath.GetBuffer());
			else
				currrentpath = CT2A(AllImgAndResult[Temp].CheckPath.GetBuffer());

			if (CurrentPicInFrame.empty() || CurrentPicInFrame != currrentpath)
			{
				ImgMat = cv::imread(currrentpath);
				m_bigImgchro.ShowImage(ImgMat, 1);
				CurrentPicInFrame = currrentpath;

				GetDlgItem(IDC_BTN_QUA)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_UNQ)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_EDIT)->EnableWindow(TRUE);

				//从数据库读取该张照片处理结果
				checked = false;
				CString str;
				str.Format(_T("%d"), AllImgAndResult[Temp].dic);
				m_dic.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].round);
				m_round.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ace);
				m_ace.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].t);
				m_t.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].inv);
				m_inv.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].tri);
				m_tri.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ten);
				m_ten.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].del);
				m_del.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ctg);
				m_ctg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].csg);
				m_csg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ctb);
				m_ctb.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].cte);
				m_cte.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].chromosome_num);
				m_chronum.SetWindowTextW(str);
				m_chro_num.SetWindowTextW(str);


				str.Format(_T("%d"), AllImgAndResult[Temp].dic_rg);
				m_dicrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].round_rg);
				m_roundrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ace_rg);
				m_acerg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].t_rg);
				m_trg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].inv_rg);
				m_invrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].tri_rg);
				m_trirg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ten_rg);
				m_tenrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].del_rg);
				m_delrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ctg_rg);
				m_ctgrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].csg_rg);
				m_csgrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].ctb_rg);
				m_ctbrg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].cte_rg);
				m_cterg.SetWindowTextW(str);

				str.Format(_T("%d"), AllImgAndResult[Temp].chromosome_num_check);
				m_chronumrg.SetWindowTextW(str);

				//标记为合格
				/*MarkQua(AllImgAndResult[Temp].picpath);*/
				//从数据库中统计所有合格的（已校正的）图片的总分析结果
				CString strq = pChromosomeResult->AllQuestionImgPath[m_comboxpatient.GetCurSel()].patientname;
				CountTheSumFromAcces(strq);
				checked = true;


			}

			//显示当前标记的点的数目
			CString strnum;
			strnum.Format(_T("%d"), 0);
			m_chro_num.SetWindowTextW(strnum);
			m_chro_num.UpdateCtrl();

		}
		//
		m_bigImgchro.OnLButtonDown(nFlags, point);

		m_point_sum.BringWindowToTop();
		m_chro_num.BringWindowToTop();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CChromosomeResult::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnMButtonDown(nFlags, point);

	CDialogEx::OnMButtonDown(nFlags, point);
}


void CChromosomeResult::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnMButtonUp(nFlags, point);

	CDialogEx::OnMButtonUp(nFlags, point);
}


void CChromosomeResult::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnLButtonUp(nFlags, point);

	CDialogEx::OnLButtonUp(nFlags, point);
}


//void CChromosomeResult::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
//}


void CChromosomeResult::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnMouseMove(nFlags, point);

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CChromosomeResult::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bigImgchro.OnMouseWheel(nFlags, zDelta, pt);

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}




//设置控件颜色
HBRUSH CChromosomeResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
