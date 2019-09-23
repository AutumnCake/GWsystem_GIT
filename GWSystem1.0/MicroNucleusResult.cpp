// MicroNucleusResult.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "MicroNucleusResult.h"
#include "afxdialogex.h"
#include "ReadAndWriteForAccess.h"

// CMicroNucleusResult 对话框

IMPLEMENT_DYNAMIC(CMicroNucleusResult, CDialogEx)

CMicroNucleusResult::CMicroNucleusResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMicroNucleusResult::IDD, pParent)
{
	Page = 0;
	m_Checked[25] = {0};
	m_MicroCheck[25] = {0};
}

CMicroNucleusResult::~CMicroNucleusResult()
{
}

void CMicroNucleusResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PATIENT, m_comboxpatient);
	DDX_Control(pDX, IDC_EDIT_PAGEMN, m_page);
}


BEGIN_MESSAGE_MAP(CMicroNucleusResult, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PATIENT, &CMicroNucleusResult::OnCbnSelchangeComboPatient)
	ON_BN_CLICKED(IDC_BTN_LASTMN, &CMicroNucleusResult::OnBnClickedBtnLastmn)
	ON_BN_CLICKED(IDC_BTN_NEXTMN, &CMicroNucleusResult::OnBnClickedBtnNextmn)
	ON_BN_CLICKED(IDC_BTN_NInc1, &CMicroNucleusResult::OnBnClickedBtnNinc1)
	ON_BN_CLICKED(IDC_BTN_NDec1, &CMicroNucleusResult::OnBnClickedBtnNdec1)
	ON_BN_CLICKED(IDC_BTN_MNInc1, &CMicroNucleusResult::OnBnClickedBtnMninc1)
	ON_BN_CLICKED(IDC_BTN_MNDec1, &CMicroNucleusResult::OnBnClickedBtnMndec1)
	ON_BN_CLICKED(IDOK, &CMicroNucleusResult::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_NInc2, &CMicroNucleusResult::OnBnClickedBtnNinc2)
	ON_BN_CLICKED(IDC_BTN_NDec2, &CMicroNucleusResult::OnBnClickedBtnNdec2)
	ON_BN_CLICKED(IDC_BTN_MNInc2, &CMicroNucleusResult::OnBnClickedBtnMninc2)
	ON_BN_CLICKED(IDC_BTN_MNDec2, &CMicroNucleusResult::OnBnClickedBtnMndec2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_NInc3, &CMicroNucleusResult::OnBnClickedBtnNinc3)
	ON_BN_CLICKED(IDC_BTN_NInc4, &CMicroNucleusResult::OnBnClickedBtnNinc4)
	ON_BN_CLICKED(IDC_BTN_NInc5, &CMicroNucleusResult::OnBnClickedBtnNinc5)
	ON_BN_CLICKED(IDC_BTN_NInc6, &CMicroNucleusResult::OnBnClickedBtnNinc6)
	ON_BN_CLICKED(IDC_BTN_NInc7, &CMicroNucleusResult::OnBnClickedBtnNinc7)
	ON_BN_CLICKED(IDC_BTN_NInc8, &CMicroNucleusResult::OnBnClickedBtnNinc8)
	ON_BN_CLICKED(IDC_BTN_NInc9, &CMicroNucleusResult::OnBnClickedBtnNinc9)
	ON_BN_CLICKED(IDC_BTN_NInc10, &CMicroNucleusResult::OnBnClickedBtnNinc10)
	ON_BN_CLICKED(IDC_BTN_NInc11, &CMicroNucleusResult::OnBnClickedBtnNinc11)
	ON_BN_CLICKED(IDC_BTN_NInc12, &CMicroNucleusResult::OnBnClickedBtnNinc12)
	ON_BN_CLICKED(IDC_BTN_NInc13, &CMicroNucleusResult::OnBnClickedBtnNinc13)
	ON_BN_CLICKED(IDC_BTN_NInc14, &CMicroNucleusResult::OnBnClickedBtnNinc14)
	ON_BN_CLICKED(IDC_BTN_NInc15, &CMicroNucleusResult::OnBnClickedBtnNinc15)
	ON_BN_CLICKED(IDC_BTN_NInc16, &CMicroNucleusResult::OnBnClickedBtnNinc16)
	ON_BN_CLICKED(IDC_BTN_NInc17, &CMicroNucleusResult::OnBnClickedBtnNinc17)
	ON_BN_CLICKED(IDC_BTN_NInc18, &CMicroNucleusResult::OnBnClickedBtnNinc18)
	ON_BN_CLICKED(IDC_BTN_NInc19, &CMicroNucleusResult::OnBnClickedBtnNinc19)
	ON_BN_CLICKED(IDC_BTN_NInc20, &CMicroNucleusResult::OnBnClickedBtnNinc20)
	ON_BN_CLICKED(IDC_BTN_NInc21, &CMicroNucleusResult::OnBnClickedBtnNinc21)
	ON_BN_CLICKED(IDC_BTN_NDec3, &CMicroNucleusResult::OnBnClickedBtnNdec3)
	ON_BN_CLICKED(IDC_BTN_NDec4, &CMicroNucleusResult::OnBnClickedBtnNdec4)
	ON_BN_CLICKED(IDC_BTN_NDec5, &CMicroNucleusResult::OnBnClickedBtnNdec5)
	ON_BN_CLICKED(IDC_BTN_NDec6, &CMicroNucleusResult::OnBnClickedBtnNdec6)
	ON_BN_CLICKED(IDC_BTN_NDec7, &CMicroNucleusResult::OnBnClickedBtnNdec7)
	ON_BN_CLICKED(IDC_BTN_NDec8, &CMicroNucleusResult::OnBnClickedBtnNdec8)
	ON_BN_CLICKED(IDC_BTN_NDec9, &CMicroNucleusResult::OnBnClickedBtnNdec9)
	ON_BN_CLICKED(IDC_BTN_NDec10, &CMicroNucleusResult::OnBnClickedBtnNdec10)
	ON_BN_CLICKED(IDC_BTN_NDec11, &CMicroNucleusResult::OnBnClickedBtnNdec11)
	ON_BN_CLICKED(IDC_BTN_NDec12, &CMicroNucleusResult::OnBnClickedBtnNdec12)
	ON_BN_CLICKED(IDC_BTN_NDec13, &CMicroNucleusResult::OnBnClickedBtnNdec13)
	ON_BN_CLICKED(IDC_BTN_NDec14, &CMicroNucleusResult::OnBnClickedBtnNdec14)
	ON_BN_CLICKED(IDC_BTN_NDec15, &CMicroNucleusResult::OnBnClickedBtnNdec15)
	ON_BN_CLICKED(IDC_BTN_NDec16, &CMicroNucleusResult::OnBnClickedBtnNdec16)
	ON_BN_CLICKED(IDC_BTN_NDec17, &CMicroNucleusResult::OnBnClickedBtnNdec17)
	ON_BN_CLICKED(IDC_BTN_NDec18, &CMicroNucleusResult::OnBnClickedBtnNdec18)
	ON_BN_CLICKED(IDC_BTN_NDec19, &CMicroNucleusResult::OnBnClickedBtnNdec19)
	ON_BN_CLICKED(IDC_BTN_NDec20, &CMicroNucleusResult::OnBnClickedBtnNdec20)
	ON_BN_CLICKED(IDC_BTN_NDec21, &CMicroNucleusResult::OnBnClickedBtnNdec21)
	ON_BN_CLICKED(IDC_BTN_MNInc3, &CMicroNucleusResult::OnBnClickedBtnMninc3)
	ON_BN_CLICKED(IDC_BTN_MNInc4, &CMicroNucleusResult::OnBnClickedBtnMninc4)
	ON_BN_CLICKED(IDC_BTN_MNInc5, &CMicroNucleusResult::OnBnClickedBtnMninc5)
	ON_BN_CLICKED(IDC_BTN_MNInc6, &CMicroNucleusResult::OnBnClickedBtnMninc6)
	ON_BN_CLICKED(IDC_BTN_MNInc7, &CMicroNucleusResult::OnBnClickedBtnMninc7)
	ON_BN_CLICKED(IDC_BTN_MNInc8, &CMicroNucleusResult::OnBnClickedBtnMninc8)
	ON_BN_CLICKED(IDC_BTN_MNInc9, &CMicroNucleusResult::OnBnClickedBtnMninc9)
	ON_BN_CLICKED(IDC_BTN_MNInc10, &CMicroNucleusResult::OnBnClickedBtnMninc10)
	ON_BN_CLICKED(IDC_BTN_MNInc11, &CMicroNucleusResult::OnBnClickedBtnMninc11)
	ON_BN_CLICKED(IDC_BTN_MNInc12, &CMicroNucleusResult::OnBnClickedBtnMninc12)
	ON_BN_CLICKED(IDC_BTN_MNInc13, &CMicroNucleusResult::OnBnClickedBtnMninc13)
	ON_BN_CLICKED(IDC_BTN_MNInc14, &CMicroNucleusResult::OnBnClickedBtnMninc14)
	ON_BN_CLICKED(IDC_BTN_MNInc15, &CMicroNucleusResult::OnBnClickedBtnMninc15)
	ON_BN_CLICKED(IDC_BTN_MNInc16, &CMicroNucleusResult::OnBnClickedBtnMninc16)
	ON_BN_CLICKED(IDC_BTN_MNInc17, &CMicroNucleusResult::OnBnClickedBtnMninc17)
	ON_BN_CLICKED(IDC_BTN_MNInc18, &CMicroNucleusResult::OnBnClickedBtnMninc18)
	ON_BN_CLICKED(IDC_BTN_MNInc19, &CMicroNucleusResult::OnBnClickedBtnMninc19)
	ON_BN_CLICKED(IDC_BTN_MNInc20, &CMicroNucleusResult::OnBnClickedBtnMninc20)
	ON_BN_CLICKED(IDC_BTN_MNInc21, &CMicroNucleusResult::OnBnClickedBtnMninc21)
	ON_BN_CLICKED(IDC_BTN_MNDec3, &CMicroNucleusResult::OnBnClickedBtnMndec3)
	ON_BN_CLICKED(IDC_BTN_MNDec4, &CMicroNucleusResult::OnBnClickedBtnMndec4)
	ON_BN_CLICKED(IDC_BTN_MNDec5, &CMicroNucleusResult::OnBnClickedBtnMndec5)
	ON_BN_CLICKED(IDC_BTN_MNDec6, &CMicroNucleusResult::OnBnClickedBtnMndec6)
	ON_BN_CLICKED(IDC_BTN_MNDec7, &CMicroNucleusResult::OnBnClickedBtnMndec7)
	ON_BN_CLICKED(IDC_BTN_MNDec8, &CMicroNucleusResult::OnBnClickedBtnMndec8)
	ON_BN_CLICKED(IDC_BTN_MNDec9, &CMicroNucleusResult::OnBnClickedBtnMndec9)
	ON_BN_CLICKED(IDC_BTN_MNDec10, &CMicroNucleusResult::OnBnClickedBtnMndec10)
	ON_BN_CLICKED(IDC_BTN_MNDec11, &CMicroNucleusResult::OnBnClickedBtnMndec11)
	ON_BN_CLICKED(IDC_BTN_MNDec12, &CMicroNucleusResult::OnBnClickedBtnMndec12)
	ON_BN_CLICKED(IDC_BTN_MNDec13, &CMicroNucleusResult::OnBnClickedBtnMndec13)
	ON_BN_CLICKED(IDC_BTN_MNDec14, &CMicroNucleusResult::OnBnClickedBtnMndec14)
	ON_BN_CLICKED(IDC_BTN_MNDec15, &CMicroNucleusResult::OnBnClickedBtnMndec15)
	ON_BN_CLICKED(IDC_BTN_MNDec16, &CMicroNucleusResult::OnBnClickedBtnMndec16)
	ON_BN_CLICKED(IDC_BTN_MNDec17, &CMicroNucleusResult::OnBnClickedBtnMndec17)
	ON_BN_CLICKED(IDC_BTN_MNDec18, &CMicroNucleusResult::OnBnClickedBtnMndec18)
	ON_BN_CLICKED(IDC_BTN_MNDec19, &CMicroNucleusResult::OnBnClickedBtnMndec19)
	ON_BN_CLICKED(IDC_BTN_MNDec20, &CMicroNucleusResult::OnBnClickedBtnMndec20)
	ON_BN_CLICKED(IDC_BTN_MNDec21, &CMicroNucleusResult::OnBnClickedBtnMndec21)
END_MESSAGE_MAP()


// CMicroNucleusResult 消息处理程序

CReadAndWriteForAccess ReadAndWriteMNResult;


//选择病人
void CMicroNucleusResult::OnCbnSelchangeComboPatient()
{
	// TODO:  在此添加控件通知处理程序代码
	//如果有当前病人的校正信息，则保存
	memset(m_Checked, 0, 25 * sizeof(BOOL));
	memset(m_MicroCheck, 0, 25 * sizeof(BOOL));
	//清空画框
	for (int v = 0; v < 21; v++)
	{
		PicClear(IDC_NameMN[v]);
		GetDlgItem(ID_StaticN[v])->SetWindowTextW(_T("0"));
		GetDlgItem(ID_StaticMN[v])->SetWindowTextW(_T("0"));
	}
	current = m_comboxpatient.GetCurSel();
	if (current >= 0)
	{
		//获得该病人的处理结果
		CurrentPatientResult = ReadAndWriteMNResult.FindCurrentPatientResult(PatientNames[current]);
		SystemResult = ReadAndWriteMNResult.ReadOneMNSystemResultFromAccess(PatientNames[current]);
		CheckResult = ReadAndWriteMNResult.ReadOneMNResultFromAccess(PatientNames[current]);

		////查看该病人的校正结果
		//for (size_t i = 0; i < AllCheckInformation.size(); i++)
		//{
		//	if (AllCheckInformation[i].PatientName == PatientNames[current])
		//	{
		//		CheckInformation = AllCheckInformation[i].CheckInformation;

		//		bool findcheck = false;
		//		for (size_t j = 0; j< CheckInformation.size(); j++)
		//		{
		//			if (CheckInformation[j].CheckPage == Page)
		//			{
		//				memcpy(m_Checked, CheckInformation[j].Checked, 25 * sizeof(BOOL));
		//				memcpy(m_MicroCheck, CheckInformation[j].MicroCheck, 25 * sizeof(BOOL));
		//				findcheck = true;
		//				break;
		//			}
		//		}
		//		if (!findcheck)
		//		{
		//			memset(m_Checked, 0, 25 * sizeof(BOOL));
		//			memset(m_MicroCheck, 0, 25 * sizeof(BOOL));
		//		}
		//	}
		//	else
		//	{
		//		m_Checked[25] = { 0 };
		//		m_MicroCheck[25] = { 0 };
		//	}
		//}
		
		//处理结果显示到控件
		ShowCurrentPatientResult(CurrentPatientResult,Page);
		//总结果显示到Edit
		//系统分析
		CString mystr;
		mystr.Format(_T("%d"), SystemResult.singlecell);
		GetDlgItem(IDC_EDIT_SINGLECELL)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.SingleMNC_Num);
		GetDlgItem(IDC_EDIT_SINGLEMNC)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.SingleMN_Num);
		GetDlgItem(IDC_EDIT_SINGLEMN)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.doublecell);
		GetDlgItem(IDC_EDIT_DOUBLECELL)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.DoubleMNC_Num);
		GetDlgItem(IDC_EDIT_DOUBLEMNC)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.DoubleMN);
		GetDlgItem(IDC_EDIT_DOUBLEMN)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.multiplecell);
		GetDlgItem(IDC_EDIT_MULTICELL)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.multiplecell_wh);
		GetDlgItem(IDC_EDIT_MULTIMNC)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), SystemResult.MultiMN);
		GetDlgItem(IDC_EDIT_MULTIMN)->SetWindowTextW(mystr);


        //人工校正
		mystr.Format(_T("%d"), CheckResult.singlecell);
		GetDlgItem(IDC_EDIT_SIGCELLRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.SingleMNC_Num);
		GetDlgItem(IDC_EDIT_SIGMNCRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.SingleMN_Num);
		GetDlgItem(IDC_EDIT_SIGMNRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.doublecell);
		GetDlgItem(IDC_EDIT_DOUCELLRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.DoubleMNC_Num);
		GetDlgItem(IDC_EDIT_DOUMNCRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.DoubleMN);
		GetDlgItem(IDC_EDIT_DOUMN)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.multiplecell);
		GetDlgItem(IDC_EDIT_MULCELLRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.multiplecell_wh);
		GetDlgItem(IDC_EDIT_MULMNCRG)->SetWindowTextW(mystr);
		mystr.Format(_T("%d"), CheckResult.MultiMN);
		GetDlgItem(IDC_EDIT_MULMNRG)->SetWindowTextW(mystr);
	}

}

//初始将picture控件存入数组
void CMicroNucleusResult::PutPicControlIntoVector()
{
	//初始将picture控件放入数组,画滚动条
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME1);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME2);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME3);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME4);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME5);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME6);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME7);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME8);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME9);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME10);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME11);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME12);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME13);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME14);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME15);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME16);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME17);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME18);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME19);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME20);
	IDC_NameMN.push_back(IDC_MNFIGUREFRAME21);

	ID_StaticN.push_back(IDC_STA_N1);
	ID_StaticN.push_back(IDC_STA_N2);
	ID_StaticN.push_back(IDC_STA_N3);
	ID_StaticN.push_back(IDC_STA_N4);
	ID_StaticN.push_back(IDC_STA_N5);
	ID_StaticN.push_back(IDC_STA_N6);
	ID_StaticN.push_back(IDC_STA_N7);
	ID_StaticN.push_back(IDC_STA_N8);
	ID_StaticN.push_back(IDC_STA_N9);
	ID_StaticN.push_back(IDC_STA_N10);
	ID_StaticN.push_back(IDC_STA_N11);
	ID_StaticN.push_back(IDC_STA_N12);
	ID_StaticN.push_back(IDC_STA_N13);
	ID_StaticN.push_back(IDC_STA_N14);
	ID_StaticN.push_back(IDC_STA_N15);
	ID_StaticN.push_back(IDC_STA_N16);
	ID_StaticN.push_back(IDC_STA_N17);
	ID_StaticN.push_back(IDC_STA_N18);
	ID_StaticN.push_back(IDC_STA_N19);
	ID_StaticN.push_back(IDC_STA_N20);
	ID_StaticN.push_back(IDC_STA_N21);

	ID_StaticMN.push_back(IDC_STA_MN1);
	ID_StaticMN.push_back(IDC_STA_MN2);
	ID_StaticMN.push_back(IDC_STA_MN3);
	ID_StaticMN.push_back(IDC_STA_MN4);
	ID_StaticMN.push_back(IDC_STA_MN5);
	ID_StaticMN.push_back(IDC_STA_MN6);
	ID_StaticMN.push_back(IDC_STA_MN7);
	ID_StaticMN.push_back(IDC_STA_MN8);
	ID_StaticMN.push_back(IDC_STA_MN9);
	ID_StaticMN.push_back(IDC_STA_MN10);
	ID_StaticMN.push_back(IDC_STA_MN11);
	ID_StaticMN.push_back(IDC_STA_MN12);
	ID_StaticMN.push_back(IDC_STA_MN13);
	ID_StaticMN.push_back(IDC_STA_MN14);
	ID_StaticMN.push_back(IDC_STA_MN15);
	ID_StaticMN.push_back(IDC_STA_MN16);
	ID_StaticMN.push_back(IDC_STA_MN17);
	ID_StaticMN.push_back(IDC_STA_MN18);
	ID_StaticMN.push_back(IDC_STA_MN19);
	ID_StaticMN.push_back(IDC_STA_MN20);
	ID_StaticMN.push_back(IDC_STA_MN21);

}


BOOL CMicroNucleusResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	PutPicControlIntoVector();
	//连接数据库
	m_Conn.OnInitADOAccess();
	//清空病人数组
	vector<CString>().swap(PatientNames);

	//病人名填入ComBox
	m_comboxpatient.ResetContent();
	for (size_t i = 0; i < PatientNames.size(); i++)
	{
		m_comboxpatient.InsertString(i, PatientNames[i]);
	}
	m_comboxpatient.SetCurSel(0);
	OnCbnSelchangeComboPatient();

	greenPen.CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	redPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	whitePen.CreatePen(PS_SOLID, 5, RGB(255, 255, 255));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}



//显示图片过程函数
void CMicroNucleusResult::ShowImage2(IplImage *img, UINT ID)//显示图片过程函数
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

//清空图片
void CMicroNucleusResult::PicClear(UINT ID)//清空图片
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

//处理结果显示到控件
bool CMicroNucleusResult::ShowCurrentPatientResult(vector<MN_HandleResultOnePic> currentpatientresult, int page)
{
	for (size_t i = 0; i < currentpatientresult.size(); i++)
	{
		if (i >= 21 * Page && i < 21 * page + 21)
		{   
			//显示图片
			string mystring = CT2A(currentpatientresult[i].picpath.GetBuffer());
			image = cv::imread(mystring, 1);//读取图片
			IplImage *Cvimage1 = &IplImage(image);
			ShowImage2(Cvimage1, IDC_NameMN[i%21]);//显示图片
			
			//显示是否校正
			if (currentpatientresult[i].CheckOrNot_NumCells == 1)
				m_Checked[i % 21] = 1;
			if (currentpatientresult[i].CheckOrNot_NumMicro == 1)
				m_MicroCheck[i % 21] = 1;

			//显示结果
			CString str;
			str.Format(_T("%d"), currentpatientresult[i].NumCells);
			GetDlgItem(ID_StaticN[i % 21])->SetWindowTextW(str);
			str.Format(_T("%d"), currentpatientresult[i].NumMicro);
			GetDlgItem(ID_StaticMN[i % 21])->SetWindowTextW(str);
			
		}

		//更新总页码
		int sumpagenum = currentpatientresult.size() / 21 + 1;
		sumpage.Format(_T("%d"), sumpagenum);
		currentpage.Format(_T("%d"), Page + 1);
		pageshow = _T("第") + currentpage + _T("页") + "/" + _T("共") + sumpage + _T("页");
		m_page.SetWindowText(pageshow);


	}
	return false;
}

//上一页
void CMicroNucleusResult::OnBnClickedBtnLastmn()
{
	// TODO:  在此添加控件通知处理程序代码
	//保存当前页校正信息
	OnePageCheck Current_Page;
	Current_Page.CheckPage = Page;
	memcpy(Current_Page.Checked, m_Checked, 25 * sizeof(BOOL));
	memcpy(Current_Page.MicroCheck, m_MicroCheck, 25 * sizeof(BOOL));
	bool findpage = false;
	for (size_t i = 0; i < CheckInformation.size(); i++)
	{
		if (CheckInformation[i].CheckPage == Page)
		{
			CheckInformation[i] = Current_Page;
			findpage = true;
			break;
		}
	}
	if (!findpage)
		CheckInformation.push_back(Current_Page);



	for (int v = 0; v < 21; v++)
	{
		PicClear(IDC_NameMN[v]);
	}
	Page--;

	if (Page < 0)
		Page = 0;

	bool findcheck = false; //查找当前校正信息是否有该页，如果有赋值，如果无  则赋0
	for (size_t i = 0; i < CheckInformation.size(); i++)
	{
		if (CheckInformation[i].CheckPage == Page)
		{
			memcpy(m_Checked, CheckInformation[i].Checked, 25 * sizeof(BOOL));
			memcpy(m_MicroCheck, CheckInformation[i].MicroCheck, 25 * sizeof(BOOL));
			findcheck = true;
			break;
		}
	}
	if (!findcheck)
	{
		memset(m_Checked, 0, 25 * sizeof(BOOL));
		memset(m_MicroCheck, 0, 25 * sizeof(BOOL));
	}
	ShowCurrentPatientResult(CurrentPatientResult, Page);

}

//下一页
void CMicroNucleusResult::OnBnClickedBtnNextmn()
{
	// TODO:  在此添加控件通知处理程序代码
	//保存当前页校正信息
	OnePageCheck Current_Page;
	Current_Page.CheckPage = Page;
	memcpy(Current_Page.Checked, m_Checked, 25 * sizeof(BOOL));
	memcpy(Current_Page.MicroCheck, m_MicroCheck, 25 * sizeof(BOOL));
	bool findpage = false;
	for (size_t i = 0; i < CheckInformation.size(); i++)
	{
		if (CheckInformation[i].CheckPage == Page)
		{
			CheckInformation[i] = Current_Page;
			findpage = true;
			break;
		}
	}
	if (!findpage)
		CheckInformation.push_back(Current_Page);

	Page++;
	for (int v = 0; v < 21; v++)
	{
		PicClear(IDC_NameMN[v]);
	}
	if (Page > (_ttoi(sumpage)-1))
		Page = _ttoi(sumpage) - 1;

	bool findcheck = false; //查找当前校正信息是否有该页，如果有赋值，如果无  则赋0
	for (size_t i = 0; i < CheckInformation.size(); i++)
	{
		if (CheckInformation[i].CheckPage == Page)
		{
			memcpy(m_Checked, CheckInformation[i].Checked, 25 * sizeof(BOOL));
			memcpy(m_MicroCheck, CheckInformation[i].MicroCheck, 25 * sizeof(BOOL));
			findcheck = true;
			break;
		}
	}
	if (!findcheck)
	{
		memset(m_Checked, 0, 25 * sizeof(BOOL));
		memset(m_MicroCheck, 0, 25 * sizeof(BOOL));
	}
	
	ShowCurrentPatientResult(CurrentPatientResult, Page);

}

// 计算当前病人的结果  刷新
MN_HandleResult * CMicroNucleusResult::CalculateMNResult(vector<MN_HandleResultOnePic> currentpatientresult)
{
	MN_HandleResult * pB = new MN_HandleResult;
	memset(pB->SingleCellsWithMN, 0, sizeof(pB->SingleCellsWithMN));
	memset(pB->DoubleCellsWithMN, 0, sizeof(pB->DoubleCellsWithMN));
	memset(pB->MultiCellsWithMN, 0, sizeof(pB->MultiCellsWithMN));
	memset(pB->TripleCellsWithMN, 0, sizeof(pB->TripleCellsWithMN));
	for (size_t i = 0; i < currentpatientresult.size(); i++)
	{
		switch (currentpatientresult[i].NumCells)
		{
		case 0:
			break;
		case 1:
			pB->SingleCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->singlecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->singlecell_wh++;//含有微核的单核细胞数
			}
			break;
		case 2:
			pB->DoubleCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->doublecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->doublecell_wh++;//含有微核的单核细胞数
			}
			break;
		case 3:
			pB->TripleCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->multiplecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->multiplecell_wh++;//含有微核的单核细胞数
			}
			break;
		default:
			pB->MultiCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->multiplecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->multiplecell_wh++;//含有微核的单核细胞数
			}
			break;
		}

	}
	for (size_t i = 0; i < sizeof(pB->DoubleCellsWithMN)/4; i++)
	{
		pB->singleMN += i*pB->SingleCellsWithMN[i];
		pB->DoubleMN += i*pB->DoubleCellsWithMN[i];
		pB->MultiMN += (i*pB->MultiCellsWithMN[i] + i*pB->TripleCellsWithMN[i]);
	}

	pB->SingleMNC_Num = pB->singlecell_wh;
	pB->SingleMN_Num = pB->singleMN;
	if (pB->singlecell == 0)
	{
		pB->SingleMNC_Rate = 0;
		pB->SingleMN_Rate = 0;
	}
	else{
		pB->SingleMNC_Rate = (float)pB->SingleMNC_Num / pB->singlecell;
		pB->SingleMN_Rate = (float)pB->SingleMN_Num / pB->singlecell;
	}


	pB->DoubleMNC_Num = pB->doublecell_wh;
	pB->DoubleMN_Num = pB->DoubleMN;

	if (pB->doublecell == 0)
	{
		pB->DoubleMNC_Rate = 0;
		pB->DoubleMN_Rate = 0;
	}
	else{
		pB->DoubleMNC_Rate = (float)pB->DoubleMNC_Num / pB->doublecell;
		pB->DoubleMN_Rate = (float)pB->DoubleMN_Num / pB->doublecell;
	}

	return pB;
	
}

// 显示总结果
void CMicroNucleusResult::ShowSumResult(MN_HandleResult * result)
{
	//人工校正
	CString mystr;
	mystr.Format(_T("%d"), result->singlecell);
	GetDlgItem(IDC_EDIT_SIGCELLRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->doublecell_wh);
	GetDlgItem(IDC_EDIT_SIGMNCRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->singleMN);
	GetDlgItem(IDC_EDIT_SIGMNRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->doublecell);
	GetDlgItem(IDC_EDIT_DOUCELLRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->doublecell_wh);
	GetDlgItem(IDC_EDIT_DOUMNCRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->DoubleMN);
	GetDlgItem(IDC_EDIT_DOUMN)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->multiplecell);
	GetDlgItem(IDC_EDIT_MULCELLRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->multiplecell_wh);
	GetDlgItem(IDC_EDIT_MULMNCRG)->SetWindowTextW(mystr);
	mystr.Format(_T("%d"), result->MultiMN);
	GetDlgItem(IDC_EDIT_MULMNRG)->SetWindowTextW(mystr);
}


//保存
void CMicroNucleusResult::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//保存该页校正信息
	//OnePageCheck Current_Page;
	//Current_Page.CheckPage = Page;
	//memcpy(Current_Page.Checked, m_Checked, 25 * sizeof(BOOL));
	//memcpy(Current_Page.MicroCheck, m_MicroCheck, 25 * sizeof(BOOL));
	//bool findpage = false;
	//for (size_t i = 0; i < CheckInformation.size(); i++)
	//{
	//	if (CheckInformation[i].CheckPage == Page)
	//	{
	//		CheckInformation[i] = Current_Page;
	//		findpage = true;
	//		break;
	//	}
	//}
	//if (!findpage)
	//	CheckInformation.push_back(Current_Page);

	////保存该病人校正信息
	//bool havecheckpatient = false;
	//for (size_t i = 0; i < AllCheckInformation.size(); i++)
	//{
	//	if (AllCheckInformation[i].PatientName == PatientNames[current])
	//	{
	//		AllCheckInformation[i].CheckInformation = CheckInformation;
	//		havecheckpatient = true;
	//		break;
	//	}
	//}
	//if (!havecheckpatient)
	//{
	//	OnePatientCheck CurrentCheck;
	//	CurrentCheck.PatientName = PatientNames[current];
	//	CurrentCheck.CheckInformation = CheckInformation;
	//	AllCheckInformation.push_back(CurrentCheck);
	//}

}


//重画控件
HBRUSH CMicroNucleusResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if (IDC_STA_N1 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[0] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N2 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[1] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N3 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[2] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N4 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[3] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N5 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[4] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N6 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[5] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N7 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[6] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N8 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[7] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N9 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[8] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N10 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[9] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N11 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[10] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N12 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[11] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N13 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[12] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N14 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[13] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N15 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[14] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N16 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[15] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N17 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[16] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N18 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[17] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N19 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[18] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N20 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[19] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_N21 == pWnd->GetDlgCtrlID())
	{
		if (m_Checked[20] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}


	//checke_Micro
	if (IDC_STA_MN1 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[0] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN2 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[1] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN3 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[2] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN4 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[3] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN5 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[4] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN6 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[5] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN7 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[6] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN8 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[7] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN9 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[8] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN10 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[9] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN11 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[10] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN12 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[11] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN13 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[12] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN14 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[13] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN15 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[14] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN16 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[15] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN17 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[16] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN18 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[17] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN19 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[18] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN20 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[19] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}
	if (IDC_STA_MN21 == pWnd->GetDlgCtrlID())
	{
		if (m_MicroCheck[20] == true)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}









//第1张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumCells++;
		CurrentPatientResult[21 * Page].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		//显示结果
		m_Checked[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumCells;
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumCells);
		GetDlgItem(ID_StaticN[0])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}


}

//第2张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumCells++;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumCells);
		GetDlgItem(ID_StaticN[1])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}

}

//第3张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumCells++;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumCells);
		GetDlgItem(ID_StaticN[2])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}
}

//第4张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumCells++;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumCells);
		GetDlgItem(ID_StaticN[3])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}
}

//第5张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc5()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumCells++;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[4] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumCells);
		GetDlgItem(ID_StaticN[4])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}
}

//第6张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc6()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumCells++;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumCells);
		GetDlgItem(ID_StaticN[5])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}
}

//第7张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc7()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumCells++;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumCells);
		GetDlgItem(ID_StaticN[6])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}
}

//第8张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc8()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumCells++;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumCells);
		GetDlgItem(ID_StaticN[7])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}
}

//第9张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc9()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumCells++;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumCells);
		GetDlgItem(ID_StaticN[8])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}
}

//第10张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc10()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumCells++;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumCells);
		GetDlgItem(ID_StaticN[9])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}
}

//第11张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc11()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumCells++;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumCells);
		GetDlgItem(ID_StaticN[10])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}
}

//第12张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc12()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumCells++;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumCells);
		GetDlgItem(ID_StaticN[11])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}
}

//第13张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc13()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumCells++;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumCells);
		GetDlgItem(ID_StaticN[12])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}
}

//第14张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc14()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumCells++;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumCells);
		GetDlgItem(ID_StaticN[13])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}
}

//第15张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc15()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumCells++;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumCells);
		GetDlgItem(ID_StaticN[14])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}
}

//第16张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc16()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumCells++;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumCells);
		GetDlgItem(ID_StaticN[15])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}
}

//第17张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc17()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumCells++;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumCells);
		GetDlgItem(ID_StaticN[16])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}
}

//第18张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc18()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumCells++;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumCells);
		GetDlgItem(ID_StaticN[17])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}
}

//第19张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc19()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumCells++;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumCells);
		GetDlgItem(ID_StaticN[18])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}
}

//第20张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc20()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumCells++;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumCells);
		GetDlgItem(ID_StaticN[19])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}
}

//第21张 细胞核数+
void CMicroNucleusResult::OnBnClickedBtnNinc21()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumCells++;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumCells);
		GetDlgItem(ID_StaticN[20])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}
}


//第1张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumCells--;
		if (CurrentPatientResult[21 * Page].NumCells < 0)
			CurrentPatientResult[21 * Page].NumCells = 0;
		CurrentPatientResult[21 * Page].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		//显示结果
		m_Checked[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumCells;
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumCells);
		GetDlgItem(ID_StaticN[0])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}
}

//第2张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumCells--;
		if (CurrentPatientResult[21 * Page + 1].NumCells < 0)
			CurrentPatientResult[21 * Page + 1].NumCells = 0;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumCells);
		GetDlgItem(ID_StaticN[1])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//第3张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumCells--;
		if (CurrentPatientResult[21 * Page + 2].NumCells < 0)
			CurrentPatientResult[21 * Page + 2].NumCells = 0;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumCells);
		GetDlgItem(ID_StaticN[2])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//第4张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumCells--;
		if (CurrentPatientResult[21 * Page + 3].NumCells < 0)
			CurrentPatientResult[21 * Page + 3].NumCells = 0;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumCells);
		GetDlgItem(ID_StaticN[3])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//第5张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec5()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumCells--;
		if (CurrentPatientResult[21 * Page + 4].NumCells < 0)
			CurrentPatientResult[21 * Page + 4].NumCells = 0;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumCells);
		GetDlgItem(ID_StaticN[4])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//第6张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec6()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumCells--;
		if (CurrentPatientResult[21 * Page + 5].NumCells < 0)
			CurrentPatientResult[21 * Page + 5].NumCells = 0;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumCells);
		GetDlgItem(ID_StaticN[5])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//第7张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec7()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumCells--;
		if (CurrentPatientResult[21 * Page + 6].NumCells < 0)
			CurrentPatientResult[21 * Page + 6].NumCells = 0;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumCells);
		GetDlgItem(ID_StaticN[6])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//第8张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec8()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumCells--;
		if (CurrentPatientResult[21 * Page + 7].NumCells < 0)
			CurrentPatientResult[21 * Page + 7].NumCells = 0;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumCells);
		GetDlgItem(ID_StaticN[7])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//第9张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec9()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumCells--;
		if (CurrentPatientResult[21 * Page + 8].NumCells < 0)
			CurrentPatientResult[21 * Page + 8].NumCells = 0;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumCells);
		GetDlgItem(ID_StaticN[8])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//第10张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec10()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumCells--;
		if (CurrentPatientResult[21 * Page + 9].NumCells < 0)
			CurrentPatientResult[21 * Page + 9].NumCells = 0;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumCells);
		GetDlgItem(ID_StaticN[9])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//第11张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec11()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumCells--;
		if (CurrentPatientResult[21 * Page + 10].NumCells < 0)
			CurrentPatientResult[21 * Page + 10].NumCells = 0;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumCells);
		GetDlgItem(ID_StaticN[10])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//第12张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec12()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumCells--;
		if (CurrentPatientResult[21 * Page + 11].NumCells < 0)
			CurrentPatientResult[21 * Page + 11].NumCells = 0;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumCells);
		GetDlgItem(ID_StaticN[11])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//第13张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec13()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumCells--;
		if (CurrentPatientResult[21 * Page + 12].NumCells < 0)
			CurrentPatientResult[21 * Page + 12].NumCells = 0;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumCells);
		GetDlgItem(ID_StaticN[12])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//第14张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec14()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumCells--;
		if (CurrentPatientResult[21 * Page + 13].NumCells < 0)
			CurrentPatientResult[21 * Page + 13].NumCells = 0;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumCells);
		GetDlgItem(ID_StaticN[13])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//第15张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec15()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumCells--;
		if (CurrentPatientResult[21 * Page + 14].NumCells < 0)
			CurrentPatientResult[21 * Page + 14].NumCells = 0;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumCells);
		GetDlgItem(ID_StaticN[14])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//第16张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec16()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumCells--;
		if (CurrentPatientResult[21 * Page + 15].NumCells < 0)
			CurrentPatientResult[21 * Page + 15].NumCells = 0;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumCells);
		GetDlgItem(ID_StaticN[15])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//第17张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec17()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumCells--;
		if (CurrentPatientResult[21 * Page + 16].NumCells < 0)
			CurrentPatientResult[21 * Page + 16].NumCells = 0;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumCells);
		GetDlgItem(ID_StaticN[16])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//第18张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec18()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumCells--;
		if (CurrentPatientResult[21 * Page + 17].NumCells < 0)
			CurrentPatientResult[21 * Page + 17].NumCells = 0;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumCells);
		GetDlgItem(ID_StaticN[17])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//第19张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec19()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumCells--;
		if (CurrentPatientResult[21 * Page + 18].NumCells < 0)
			CurrentPatientResult[21 * Page + 18].NumCells = 0;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumCells);
		GetDlgItem(ID_StaticN[18])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//第20张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec20()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumCells--;
		if (CurrentPatientResult[21 * Page + 19].NumCells < 0)
			CurrentPatientResult[21 * Page + 19].NumCells = 0;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumCells);
		GetDlgItem(ID_StaticN[19])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//第21张 细胞核数-
void CMicroNucleusResult::OnBnClickedBtnNdec21()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumCells--;
		if (CurrentPatientResult[21 * Page + 20].NumCells < 0)
			CurrentPatientResult[21 * Page + 20].NumCells = 0;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumCells);
		GetDlgItem(ID_StaticN[20])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}

//第1张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumMicro++;
		if (CurrentPatientResult[21 * Page].NumMicro>20)
			CurrentPatientResult[21 * Page].NumMicro = 20;

		CurrentPatientResult[21 * Page].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumMicro);
		GetDlgItem(ID_StaticMN[0])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page ]);
	}
}

//第2张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumMicro++;
		if (CurrentPatientResult[21 * Page + 1].NumMicro>20)
			CurrentPatientResult[21 * Page + 1].NumMicro = 20;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumMicro);
		GetDlgItem(ID_StaticMN[1])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//第3张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumMicro++;
		if (CurrentPatientResult[21 * Page + 2].NumMicro>20)
			CurrentPatientResult[21 * Page + 2].NumMicro = 20;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumMicro);
		GetDlgItem(ID_StaticMN[2])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//第4张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumMicro++;
		if (CurrentPatientResult[21 * Page + 3].NumMicro>20)
			CurrentPatientResult[21 * Page + 3].NumMicro = 20;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumMicro);
		GetDlgItem(ID_StaticMN[3])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//第5张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc5()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumMicro++;
		if (CurrentPatientResult[21 * Page + 4].NumMicro>20)
			CurrentPatientResult[21 * Page + 4].NumMicro = 20;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumMicro);
		GetDlgItem(ID_StaticMN[4])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//第6张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc6()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumMicro++;
		if (CurrentPatientResult[21 * Page + 5].NumMicro>20)
			CurrentPatientResult[21 * Page + 5].NumMicro = 20;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumMicro);
		GetDlgItem(ID_StaticMN[5])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//第7张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc7()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumMicro++;
		if (CurrentPatientResult[21 * Page + 6].NumMicro>20)
			CurrentPatientResult[21 * Page + 6].NumMicro = 20;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumMicro);
		GetDlgItem(ID_StaticMN[6])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//第8张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc8()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumMicro++;
		if (CurrentPatientResult[21 * Page + 7].NumMicro>20)
			CurrentPatientResult[21 * Page + 7].NumMicro = 20;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumMicro);
		GetDlgItem(ID_StaticMN[7])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//第9张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc9()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumMicro++;
		if (CurrentPatientResult[21 * Page + 8].NumMicro>20)
			CurrentPatientResult[21 * Page + 8].NumMicro = 20;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumMicro);
		GetDlgItem(ID_StaticMN[8])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//第10张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc10()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumMicro++;
		if (CurrentPatientResult[21 * Page + 9].NumMicro>20)
			CurrentPatientResult[21 * Page + 9].NumMicro = 20;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumMicro);
		GetDlgItem(ID_StaticMN[9])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//第11张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc11()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumMicro++;
		if (CurrentPatientResult[21 * Page + 10].NumMicro>20)
			CurrentPatientResult[21 * Page + 10].NumMicro = 20;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumMicro);
		GetDlgItem(ID_StaticMN[10])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//第12张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc12()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumMicro++;
		if (CurrentPatientResult[21 * Page + 11].NumMicro>20)
			CurrentPatientResult[21 * Page + 11].NumMicro = 20;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumMicro);
		GetDlgItem(ID_StaticMN[11])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//第13张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc13()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumMicro++;
		if (CurrentPatientResult[21 * Page + 12].NumMicro>20)
			CurrentPatientResult[21 * Page + 12].NumMicro = 20;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumMicro);
		GetDlgItem(ID_StaticMN[12])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//第14张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc14()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumMicro++;
		if (CurrentPatientResult[21 * Page + 13].NumMicro>20)
			CurrentPatientResult[21 * Page + 13].NumMicro = 20;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumMicro);
		GetDlgItem(ID_StaticMN[13])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//第15张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc15()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumMicro++;
		if (CurrentPatientResult[21 * Page + 14].NumMicro>20)
			CurrentPatientResult[21 * Page + 14].NumMicro = 20;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumMicro);
		GetDlgItem(ID_StaticMN[14])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//第16张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc16()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumMicro++;
		if (CurrentPatientResult[21 * Page + 15].NumMicro>20)
			CurrentPatientResult[21 * Page + 15].NumMicro = 20;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumMicro);
		GetDlgItem(ID_StaticMN[15])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//第17张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc17()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumMicro++;
		if (CurrentPatientResult[21 * Page + 16].NumMicro>20)
			CurrentPatientResult[21 * Page + 16].NumMicro = 20;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumMicro);
		GetDlgItem(ID_StaticMN[16])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//第18张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc18()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumMicro++;
		if (CurrentPatientResult[21 * Page + 17].NumMicro>20)
			CurrentPatientResult[21 * Page + 17].NumMicro = 20;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumMicro);
		GetDlgItem(ID_StaticMN[17])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//第19张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc19()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumMicro++;
		if (CurrentPatientResult[21 * Page + 18].NumMicro>20)
			CurrentPatientResult[21 * Page + 18].NumMicro = 20;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumMicro);
		GetDlgItem(ID_StaticMN[18])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//第20张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc20()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumMicro++;
		if (CurrentPatientResult[21 * Page + 19].NumMicro>20)
			CurrentPatientResult[21 * Page + 19].NumMicro = 20;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumMicro);
		GetDlgItem(ID_StaticMN[19])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//第21张 微核数+
void CMicroNucleusResult::OnBnClickedBtnMninc21()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumMicro++;
		if (CurrentPatientResult[21 * Page + 20].NumMicro>20)
			CurrentPatientResult[21 * Page + 20].NumMicro = 20;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumMicro);
		GetDlgItem(ID_StaticMN[20])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}


//第1张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumMicro--;
		if (CurrentPatientResult[21 * Page].NumMicro<0)
			CurrentPatientResult[21 * Page].NumMicro = 0;

		CurrentPatientResult[21 * Page].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumMicro);
		GetDlgItem(ID_StaticMN[0])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}
}

//第2张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumMicro--;
		if (CurrentPatientResult[21 * Page + 1].NumMicro<0)
			CurrentPatientResult[21 * Page + 1].NumMicro = 0;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumMicro);
		GetDlgItem(ID_StaticMN[1])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//第3张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec3()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumMicro--;
		if (CurrentPatientResult[21 * Page + 2].NumMicro<0)
			CurrentPatientResult[21 * Page + 2].NumMicro = 0;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumMicro);
		GetDlgItem(ID_StaticMN[2])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//第4张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec4()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumMicro--;
		if (CurrentPatientResult[21 * Page + 3].NumMicro<0)
			CurrentPatientResult[21 * Page + 3].NumMicro = 0;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumMicro);
		GetDlgItem(ID_StaticMN[3])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//第5张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec5()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumMicro--;
		if (CurrentPatientResult[21 * Page + 4].NumMicro<0)
			CurrentPatientResult[21 * Page + 4].NumMicro = 0;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumMicro);
		GetDlgItem(ID_StaticMN[4])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//第6张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec6()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumMicro--;
		if (CurrentPatientResult[21 * Page + 5].NumMicro<0)
			CurrentPatientResult[21 * Page + 5].NumMicro = 0;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumMicro);
		GetDlgItem(ID_StaticMN[5])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//第7张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec7()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumMicro--;
		if (CurrentPatientResult[21 * Page + 6].NumMicro<0)
			CurrentPatientResult[21 * Page + 6].NumMicro = 0;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumMicro);
		GetDlgItem(ID_StaticMN[6])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//第8张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec8()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumMicro--;
		if (CurrentPatientResult[21 * Page + 7].NumMicro<0)
			CurrentPatientResult[21 * Page + 7].NumMicro = 0;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumMicro);
		GetDlgItem(ID_StaticMN[7])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//第9张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec9()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumMicro--;
		if (CurrentPatientResult[21 * Page + 8].NumMicro<0)
			CurrentPatientResult[21 * Page + 8].NumMicro = 0;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumMicro);
		GetDlgItem(ID_StaticMN[8])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//第10张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec10()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumMicro--;
		if (CurrentPatientResult[21 * Page + 9].NumMicro<0)
			CurrentPatientResult[21 * Page + 9].NumMicro = 0;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumMicro);
		GetDlgItem(ID_StaticMN[9])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//第11张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec11()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumMicro--;
		if (CurrentPatientResult[21 * Page + 10].NumMicro<0)
			CurrentPatientResult[21 * Page + 10].NumMicro = 0;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumMicro);
		GetDlgItem(ID_StaticMN[10])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//第12张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec12()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumMicro--;
		if (CurrentPatientResult[21 * Page + 11].NumMicro<0)
			CurrentPatientResult[21 * Page + 11].NumMicro = 0;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumMicro);
		GetDlgItem(ID_StaticMN[11])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//第13张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec13()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumMicro--;
		if (CurrentPatientResult[21 * Page + 12].NumMicro<0)
			CurrentPatientResult[21 * Page + 12].NumMicro = 0;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumMicro);
		GetDlgItem(ID_StaticMN[12])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//第14张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec14()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumMicro--;
		if (CurrentPatientResult[21 * Page + 13].NumMicro<0)
			CurrentPatientResult[21 * Page + 13].NumMicro = 0;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumMicro);
		GetDlgItem(ID_StaticMN[13])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//第15张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec15()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumMicro--;
		if (CurrentPatientResult[21 * Page + 14].NumMicro<0)
			CurrentPatientResult[21 * Page + 14].NumMicro = 0;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumMicro);
		GetDlgItem(ID_StaticMN[14])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//第16张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec16()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumMicro--;
		if (CurrentPatientResult[21 * Page + 15].NumMicro<0)
			CurrentPatientResult[21 * Page + 15].NumMicro = 0;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumMicro);
		GetDlgItem(ID_StaticMN[15])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//第17张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec17()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumMicro--;
		if (CurrentPatientResult[21 * Page + 16].NumMicro<0)
			CurrentPatientResult[21 * Page + 16].NumMicro = 0;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumMicro);
		GetDlgItem(ID_StaticMN[16])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//第18张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec18()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumMicro--;
		if (CurrentPatientResult[21 * Page + 17].NumMicro<0)
			CurrentPatientResult[21 * Page + 17].NumMicro = 0;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumMicro);
		GetDlgItem(ID_StaticMN[17])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//第19张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec19()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumMicro--;
		if (CurrentPatientResult[21 * Page + 18].NumMicro<0)
			CurrentPatientResult[21 * Page + 18].NumMicro = 0;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumMicro);
		GetDlgItem(ID_StaticMN[18])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//第20张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec20()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumMicro--;
		if (CurrentPatientResult[21 * Page + 19].NumMicro<0)
			CurrentPatientResult[21 * Page + 19].NumMicro = 0;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumMicro);
		GetDlgItem(ID_StaticMN[19])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//第21张 微核数-
void CMicroNucleusResult::OnBnClickedBtnMndec21()
{
	// TODO:  在此添加控件通知处理程序代码
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumMicro--;
		if (CurrentPatientResult[21 * Page + 20].NumMicro<0)
			CurrentPatientResult[21 * Page + 20].NumMicro = 0;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro;

		//显示结果
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumMicro);
		GetDlgItem(ID_StaticMN[20])->SetWindowTextW(str);
		//计算求和
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//显示总结果
		ShowSumResult(result);
		//保存结果到数据库
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}
