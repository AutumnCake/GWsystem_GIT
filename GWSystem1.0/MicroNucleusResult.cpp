// MicroNucleusResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "MicroNucleusResult.h"
#include "afxdialogex.h"
#include "ReadAndWriteForAccess.h"

// CMicroNucleusResult �Ի���

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


// CMicroNucleusResult ��Ϣ�������

CReadAndWriteForAccess ReadAndWriteMNResult;


//ѡ����
void CMicroNucleusResult::OnCbnSelchangeComboPatient()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//����е�ǰ���˵�У����Ϣ���򱣴�
	memset(m_Checked, 0, 25 * sizeof(BOOL));
	memset(m_MicroCheck, 0, 25 * sizeof(BOOL));
	//��ջ���
	for (int v = 0; v < 21; v++)
	{
		PicClear(IDC_NameMN[v]);
		GetDlgItem(ID_StaticN[v])->SetWindowTextW(_T("0"));
		GetDlgItem(ID_StaticMN[v])->SetWindowTextW(_T("0"));
	}
	current = m_comboxpatient.GetCurSel();
	if (current >= 0)
	{
		//��øò��˵Ĵ�����
		CurrentPatientResult = ReadAndWriteMNResult.FindCurrentPatientResult(PatientNames[current]);
		SystemResult = ReadAndWriteMNResult.ReadOneMNSystemResultFromAccess(PatientNames[current]);
		CheckResult = ReadAndWriteMNResult.ReadOneMNResultFromAccess(PatientNames[current]);

		////�鿴�ò��˵�У�����
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
		
		//��������ʾ���ؼ�
		ShowCurrentPatientResult(CurrentPatientResult,Page);
		//�ܽ����ʾ��Edit
		//ϵͳ����
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


        //�˹�У��
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

//��ʼ��picture�ؼ���������
void CMicroNucleusResult::PutPicControlIntoVector()
{
	//��ʼ��picture�ؼ���������,��������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	PutPicControlIntoVector();
	//�������ݿ�
	m_Conn.OnInitADOAccess();
	//��ղ�������
	vector<CString>().swap(PatientNames);

	//����������ComBox
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}



//��ʾͼƬ���̺���
void CMicroNucleusResult::ShowImage2(IplImage *img, UINT ID)//��ʾͼƬ���̺���
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

//���ͼƬ
void CMicroNucleusResult::PicClear(UINT ID)//���ͼƬ
{
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left, lRect.top, lRect.Width(), lRect.Height(), RGB(240, 240, 240));
	CDC *dc = pStatic->GetDC();
	dc->SelectObject(&whitePen);//��dc�Ϸ��û���
	dc->SelectStockObject(NULL_BRUSH);//ʹ�����ľ�������Ϊ͸��
	dc->Rectangle(lRect);
}

//��������ʾ���ؼ�
bool CMicroNucleusResult::ShowCurrentPatientResult(vector<MN_HandleResultOnePic> currentpatientresult, int page)
{
	for (size_t i = 0; i < currentpatientresult.size(); i++)
	{
		if (i >= 21 * Page && i < 21 * page + 21)
		{   
			//��ʾͼƬ
			string mystring = CT2A(currentpatientresult[i].picpath.GetBuffer());
			image = cv::imread(mystring, 1);//��ȡͼƬ
			IplImage *Cvimage1 = &IplImage(image);
			ShowImage2(Cvimage1, IDC_NameMN[i%21]);//��ʾͼƬ
			
			//��ʾ�Ƿ�У��
			if (currentpatientresult[i].CheckOrNot_NumCells == 1)
				m_Checked[i % 21] = 1;
			if (currentpatientresult[i].CheckOrNot_NumMicro == 1)
				m_MicroCheck[i % 21] = 1;

			//��ʾ���
			CString str;
			str.Format(_T("%d"), currentpatientresult[i].NumCells);
			GetDlgItem(ID_StaticN[i % 21])->SetWindowTextW(str);
			str.Format(_T("%d"), currentpatientresult[i].NumMicro);
			GetDlgItem(ID_StaticMN[i % 21])->SetWindowTextW(str);
			
		}

		//������ҳ��
		int sumpagenum = currentpatientresult.size() / 21 + 1;
		sumpage.Format(_T("%d"), sumpagenum);
		currentpage.Format(_T("%d"), Page + 1);
		pageshow = _T("��") + currentpage + _T("ҳ") + "/" + _T("��") + sumpage + _T("ҳ");
		m_page.SetWindowText(pageshow);


	}
	return false;
}

//��һҳ
void CMicroNucleusResult::OnBnClickedBtnLastmn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���浱ǰҳУ����Ϣ
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

	bool findcheck = false; //���ҵ�ǰУ����Ϣ�Ƿ��и�ҳ������и�ֵ�������  ��0
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

//��һҳ
void CMicroNucleusResult::OnBnClickedBtnNextmn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���浱ǰҳУ����Ϣ
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

	bool findcheck = false; //���ҵ�ǰУ����Ϣ�Ƿ��и�ҳ������и�ֵ�������  ��0
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

// ���㵱ǰ���˵Ľ��  ˢ��
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
				pB->singlecell_wh++;//����΢�˵ĵ���ϸ����
			}
			break;
		case 2:
			pB->DoubleCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->doublecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->doublecell_wh++;//����΢�˵ĵ���ϸ����
			}
			break;
		case 3:
			pB->TripleCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->multiplecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->multiplecell_wh++;//����΢�˵ĵ���ϸ����
			}
			break;
		default:
			pB->MultiCellsWithMN[currentpatientresult[i].NumMicro]++;
			pB->multiplecell++;
			pB->sumcell++;
			if (currentpatientresult[i].NumMicro > 0)
			{
				pB->multiplecell_wh++;//����΢�˵ĵ���ϸ����
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

// ��ʾ�ܽ��
void CMicroNucleusResult::ShowSumResult(MN_HandleResult * result)
{
	//�˹�У��
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


//����
void CMicroNucleusResult::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�����ҳУ����Ϣ
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

	////����ò���У����Ϣ
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


//�ػ��ؼ�
HBRUSH CMicroNucleusResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
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

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}









//��1�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumCells++;
		CurrentPatientResult[21 * Page].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		//��ʾ���
		m_Checked[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumCells;
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumCells);
		GetDlgItem(ID_StaticN[0])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}


}

//��2�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumCells++;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumCells);
		GetDlgItem(ID_StaticN[1])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}

}

//��3�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumCells++;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumCells);
		GetDlgItem(ID_StaticN[2])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}
}

//��4�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumCells++;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumCells);
		GetDlgItem(ID_StaticN[3])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}
}

//��5�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumCells++;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[4] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumCells);
		GetDlgItem(ID_StaticN[4])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}
}

//��6�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumCells++;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumCells);
		GetDlgItem(ID_StaticN[5])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}
}

//��7�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumCells++;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumCells);
		GetDlgItem(ID_StaticN[6])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}
}

//��8�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumCells++;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumCells);
		GetDlgItem(ID_StaticN[7])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}
}

//��9�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumCells++;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumCells);
		GetDlgItem(ID_StaticN[8])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}
}

//��10�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumCells++;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumCells);
		GetDlgItem(ID_StaticN[9])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}
}

//��11�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumCells++;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumCells);
		GetDlgItem(ID_StaticN[10])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}
}

//��12�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc12()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumCells++;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumCells);
		GetDlgItem(ID_StaticN[11])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}
}

//��13�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc13()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumCells++;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumCells);
		GetDlgItem(ID_StaticN[12])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}
}

//��14�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc14()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumCells++;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumCells);
		GetDlgItem(ID_StaticN[13])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}
}

//��15�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc15()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumCells++;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumCells);
		GetDlgItem(ID_StaticN[14])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}
}

//��16�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumCells++;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumCells);
		GetDlgItem(ID_StaticN[15])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}
}

//��17�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc17()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumCells++;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumCells);
		GetDlgItem(ID_StaticN[16])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}
}

//��18�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc18()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumCells++;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumCells);
		GetDlgItem(ID_StaticN[17])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}
}

//��19�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc19()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumCells++;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumCells);
		GetDlgItem(ID_StaticN[18])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}
}

//��20�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc20()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumCells++;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumCells);
		GetDlgItem(ID_StaticN[19])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}
}

//��21�� ϸ������+
void CMicroNucleusResult::OnBnClickedBtnNinc21()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumCells++;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumCells);
		GetDlgItem(ID_StaticN[20])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}
}


//��1�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumCells--;
		if (CurrentPatientResult[21 * Page].NumCells < 0)
			CurrentPatientResult[21 * Page].NumCells = 0;
		CurrentPatientResult[21 * Page].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		//��ʾ���
		m_Checked[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumCells;
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumCells);
		GetDlgItem(ID_StaticN[0])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}
}

//��2�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumCells--;
		if (CurrentPatientResult[21 * Page + 1].NumCells < 0)
			CurrentPatientResult[21 * Page + 1].NumCells = 0;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumCells);
		GetDlgItem(ID_StaticN[1])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//��3�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumCells--;
		if (CurrentPatientResult[21 * Page + 2].NumCells < 0)
			CurrentPatientResult[21 * Page + 2].NumCells = 0;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumCells);
		GetDlgItem(ID_StaticN[2])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//��4�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumCells--;
		if (CurrentPatientResult[21 * Page + 3].NumCells < 0)
			CurrentPatientResult[21 * Page + 3].NumCells = 0;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumCells);
		GetDlgItem(ID_StaticN[3])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//��5�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumCells--;
		if (CurrentPatientResult[21 * Page + 4].NumCells < 0)
			CurrentPatientResult[21 * Page + 4].NumCells = 0;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumCells);
		GetDlgItem(ID_StaticN[4])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//��6�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumCells--;
		if (CurrentPatientResult[21 * Page + 5].NumCells < 0)
			CurrentPatientResult[21 * Page + 5].NumCells = 0;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumCells);
		GetDlgItem(ID_StaticN[5])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//��7�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumCells--;
		if (CurrentPatientResult[21 * Page + 6].NumCells < 0)
			CurrentPatientResult[21 * Page + 6].NumCells = 0;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumCells);
		GetDlgItem(ID_StaticN[6])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//��8�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumCells--;
		if (CurrentPatientResult[21 * Page + 7].NumCells < 0)
			CurrentPatientResult[21 * Page + 7].NumCells = 0;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumCells);
		GetDlgItem(ID_StaticN[7])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//��9�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumCells--;
		if (CurrentPatientResult[21 * Page + 8].NumCells < 0)
			CurrentPatientResult[21 * Page + 8].NumCells = 0;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumCells);
		GetDlgItem(ID_StaticN[8])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//��10�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumCells--;
		if (CurrentPatientResult[21 * Page + 9].NumCells < 0)
			CurrentPatientResult[21 * Page + 9].NumCells = 0;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumCells);
		GetDlgItem(ID_StaticN[9])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//��11�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumCells--;
		if (CurrentPatientResult[21 * Page + 10].NumCells < 0)
			CurrentPatientResult[21 * Page + 10].NumCells = 0;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumCells);
		GetDlgItem(ID_StaticN[10])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//��12�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec12()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumCells--;
		if (CurrentPatientResult[21 * Page + 11].NumCells < 0)
			CurrentPatientResult[21 * Page + 11].NumCells = 0;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumCells);
		GetDlgItem(ID_StaticN[11])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//��13�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec13()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumCells--;
		if (CurrentPatientResult[21 * Page + 12].NumCells < 0)
			CurrentPatientResult[21 * Page + 12].NumCells = 0;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumCells);
		GetDlgItem(ID_StaticN[12])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//��14�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec14()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumCells--;
		if (CurrentPatientResult[21 * Page + 13].NumCells < 0)
			CurrentPatientResult[21 * Page + 13].NumCells = 0;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumCells);
		GetDlgItem(ID_StaticN[13])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//��15�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec15()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumCells--;
		if (CurrentPatientResult[21 * Page + 14].NumCells < 0)
			CurrentPatientResult[21 * Page + 14].NumCells = 0;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumCells);
		GetDlgItem(ID_StaticN[14])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//��16�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumCells--;
		if (CurrentPatientResult[21 * Page + 15].NumCells < 0)
			CurrentPatientResult[21 * Page + 15].NumCells = 0;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumCells);
		GetDlgItem(ID_StaticN[15])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//��17�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec17()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumCells--;
		if (CurrentPatientResult[21 * Page + 16].NumCells < 0)
			CurrentPatientResult[21 * Page + 16].NumCells = 0;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumCells);
		GetDlgItem(ID_StaticN[16])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//��18�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec18()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumCells--;
		if (CurrentPatientResult[21 * Page + 17].NumCells < 0)
			CurrentPatientResult[21 * Page + 17].NumCells = 0;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumCells);
		GetDlgItem(ID_StaticN[17])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//��19�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec19()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumCells--;
		if (CurrentPatientResult[21 * Page + 18].NumCells < 0)
			CurrentPatientResult[21 * Page + 18].NumCells = 0;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumCells);
		GetDlgItem(ID_StaticN[18])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//��20�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec20()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumCells--;
		if (CurrentPatientResult[21 * Page + 19].NumCells < 0)
			CurrentPatientResult[21 * Page + 19].NumCells = 0;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumCells);
		GetDlgItem(ID_StaticN[19])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//��21�� ϸ������-
void CMicroNucleusResult::OnBnClickedBtnNdec21()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumCells--;
		if (CurrentPatientResult[21 * Page + 20].NumCells < 0)
			CurrentPatientResult[21 * Page + 20].NumCells = 0;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells = true;
		UpdateData(FALSE);
		m_Checked[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumCells;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumCells);
		GetDlgItem(ID_StaticN[20])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}

//��1�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumMicro++;
		if (CurrentPatientResult[21 * Page].NumMicro>20)
			CurrentPatientResult[21 * Page].NumMicro = 20;

		CurrentPatientResult[21 * Page].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumMicro);
		GetDlgItem(ID_StaticMN[0])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page ]);
	}
}

//��2�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumMicro++;
		if (CurrentPatientResult[21 * Page + 1].NumMicro>20)
			CurrentPatientResult[21 * Page + 1].NumMicro = 20;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumMicro);
		GetDlgItem(ID_StaticMN[1])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//��3�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumMicro++;
		if (CurrentPatientResult[21 * Page + 2].NumMicro>20)
			CurrentPatientResult[21 * Page + 2].NumMicro = 20;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumMicro);
		GetDlgItem(ID_StaticMN[2])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//��4�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumMicro++;
		if (CurrentPatientResult[21 * Page + 3].NumMicro>20)
			CurrentPatientResult[21 * Page + 3].NumMicro = 20;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumMicro);
		GetDlgItem(ID_StaticMN[3])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//��5�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumMicro++;
		if (CurrentPatientResult[21 * Page + 4].NumMicro>20)
			CurrentPatientResult[21 * Page + 4].NumMicro = 20;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumMicro);
		GetDlgItem(ID_StaticMN[4])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//��6�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumMicro++;
		if (CurrentPatientResult[21 * Page + 5].NumMicro>20)
			CurrentPatientResult[21 * Page + 5].NumMicro = 20;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumMicro);
		GetDlgItem(ID_StaticMN[5])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//��7�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumMicro++;
		if (CurrentPatientResult[21 * Page + 6].NumMicro>20)
			CurrentPatientResult[21 * Page + 6].NumMicro = 20;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumMicro);
		GetDlgItem(ID_StaticMN[6])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//��8�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumMicro++;
		if (CurrentPatientResult[21 * Page + 7].NumMicro>20)
			CurrentPatientResult[21 * Page + 7].NumMicro = 20;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumMicro);
		GetDlgItem(ID_StaticMN[7])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//��9�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumMicro++;
		if (CurrentPatientResult[21 * Page + 8].NumMicro>20)
			CurrentPatientResult[21 * Page + 8].NumMicro = 20;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumMicro);
		GetDlgItem(ID_StaticMN[8])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//��10�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumMicro++;
		if (CurrentPatientResult[21 * Page + 9].NumMicro>20)
			CurrentPatientResult[21 * Page + 9].NumMicro = 20;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumMicro);
		GetDlgItem(ID_StaticMN[9])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//��11�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumMicro++;
		if (CurrentPatientResult[21 * Page + 10].NumMicro>20)
			CurrentPatientResult[21 * Page + 10].NumMicro = 20;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumMicro);
		GetDlgItem(ID_StaticMN[10])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//��12�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc12()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumMicro++;
		if (CurrentPatientResult[21 * Page + 11].NumMicro>20)
			CurrentPatientResult[21 * Page + 11].NumMicro = 20;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumMicro);
		GetDlgItem(ID_StaticMN[11])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//��13�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc13()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumMicro++;
		if (CurrentPatientResult[21 * Page + 12].NumMicro>20)
			CurrentPatientResult[21 * Page + 12].NumMicro = 20;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumMicro);
		GetDlgItem(ID_StaticMN[12])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//��14�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc14()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumMicro++;
		if (CurrentPatientResult[21 * Page + 13].NumMicro>20)
			CurrentPatientResult[21 * Page + 13].NumMicro = 20;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumMicro);
		GetDlgItem(ID_StaticMN[13])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//��15�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc15()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumMicro++;
		if (CurrentPatientResult[21 * Page + 14].NumMicro>20)
			CurrentPatientResult[21 * Page + 14].NumMicro = 20;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumMicro);
		GetDlgItem(ID_StaticMN[14])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//��16�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumMicro++;
		if (CurrentPatientResult[21 * Page + 15].NumMicro>20)
			CurrentPatientResult[21 * Page + 15].NumMicro = 20;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumMicro);
		GetDlgItem(ID_StaticMN[15])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//��17�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc17()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumMicro++;
		if (CurrentPatientResult[21 * Page + 16].NumMicro>20)
			CurrentPatientResult[21 * Page + 16].NumMicro = 20;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumMicro);
		GetDlgItem(ID_StaticMN[16])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//��18�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc18()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumMicro++;
		if (CurrentPatientResult[21 * Page + 17].NumMicro>20)
			CurrentPatientResult[21 * Page + 17].NumMicro = 20;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumMicro);
		GetDlgItem(ID_StaticMN[17])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//��19�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc19()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumMicro++;
		if (CurrentPatientResult[21 * Page + 18].NumMicro>20)
			CurrentPatientResult[21 * Page + 18].NumMicro = 20;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumMicro);
		GetDlgItem(ID_StaticMN[18])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//��20�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc20()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumMicro++;
		if (CurrentPatientResult[21 * Page + 19].NumMicro>20)
			CurrentPatientResult[21 * Page + 19].NumMicro = 20;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumMicro);
		GetDlgItem(ID_StaticMN[19])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//��21�� ΢����+
void CMicroNucleusResult::OnBnClickedBtnMninc21()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumMicro++;
		if (CurrentPatientResult[21 * Page + 20].NumMicro>20)
			CurrentPatientResult[21 * Page + 20].NumMicro = 20;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumMicro);
		GetDlgItem(ID_StaticMN[20])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}


//��1�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page)
	{
		CurrentPatientResult[21 * Page].NumMicro--;
		if (CurrentPatientResult[21 * Page].NumMicro<0)
			CurrentPatientResult[21 * Page].NumMicro = 0;

		CurrentPatientResult[21 * Page].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[0] = CurrentPatientResult[21 * Page].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page].NumMicro);
		GetDlgItem(ID_StaticMN[0])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page]);
	}
}

//��2�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 1)
	{
		CurrentPatientResult[21 * Page + 1].NumMicro--;
		if (CurrentPatientResult[21 * Page + 1].NumMicro<0)
			CurrentPatientResult[21 * Page + 1].NumMicro = 0;

		CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[1] = CurrentPatientResult[21 * Page + 1].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 1].NumMicro);
		GetDlgItem(ID_StaticMN[1])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 1]);
	}
}

//��3�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 2)
	{
		CurrentPatientResult[21 * Page + 2].NumMicro--;
		if (CurrentPatientResult[21 * Page + 2].NumMicro<0)
			CurrentPatientResult[21 * Page + 2].NumMicro = 0;

		CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[2] = CurrentPatientResult[21 * Page + 2].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 2].NumMicro);
		GetDlgItem(ID_StaticMN[2])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 2]);
	}

}

//��4�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 3)
	{
		CurrentPatientResult[21 * Page + 3].NumMicro--;
		if (CurrentPatientResult[21 * Page + 3].NumMicro<0)
			CurrentPatientResult[21 * Page + 3].NumMicro = 0;

		CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[3] = CurrentPatientResult[21 * Page + 3].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 3].NumMicro);
		GetDlgItem(ID_StaticMN[3])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 3]);
	}

}

//��5�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 4)
	{
		CurrentPatientResult[21 * Page + 4].NumMicro--;
		if (CurrentPatientResult[21 * Page + 4].NumMicro<0)
			CurrentPatientResult[21 * Page + 4].NumMicro = 0;

		CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[4] = CurrentPatientResult[21 * Page + 4].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 4].NumMicro);
		GetDlgItem(ID_StaticMN[4])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 4]);
	}

}

//��6�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 5)
	{
		CurrentPatientResult[21 * Page + 5].NumMicro--;
		if (CurrentPatientResult[21 * Page + 5].NumMicro<0)
			CurrentPatientResult[21 * Page + 5].NumMicro = 0;

		CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[5] = CurrentPatientResult[21 * Page + 5].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 5].NumMicro);
		GetDlgItem(ID_StaticMN[5])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 5]);
	}

}

//��7�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec7()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 6)
	{
		CurrentPatientResult[21 * Page + 6].NumMicro--;
		if (CurrentPatientResult[21 * Page + 6].NumMicro<0)
			CurrentPatientResult[21 * Page + 6].NumMicro = 0;

		CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[6] = CurrentPatientResult[21 * Page + 6].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 6].NumMicro);
		GetDlgItem(ID_StaticMN[6])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 6]);
	}

}

//��8�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 7)
	{
		CurrentPatientResult[21 * Page + 7].NumMicro--;
		if (CurrentPatientResult[21 * Page + 7].NumMicro<0)
			CurrentPatientResult[21 * Page + 7].NumMicro = 0;

		CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[7] = CurrentPatientResult[21 * Page + 7].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 7].NumMicro);
		GetDlgItem(ID_StaticMN[7])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 7]);
	}

}

//��9�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 8)
	{
		CurrentPatientResult[21 * Page + 8].NumMicro--;
		if (CurrentPatientResult[21 * Page + 8].NumMicro<0)
			CurrentPatientResult[21 * Page + 8].NumMicro = 0;

		CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[8] = CurrentPatientResult[21 * Page + 8].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 8].NumMicro);
		GetDlgItem(ID_StaticMN[8])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 8]);
	}

}

//��10�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec10()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 9)
	{
		CurrentPatientResult[21 * Page + 9].NumMicro--;
		if (CurrentPatientResult[21 * Page + 9].NumMicro<0)
			CurrentPatientResult[21 * Page + 9].NumMicro = 0;

		CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[9] = CurrentPatientResult[21 * Page + 9].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 9].NumMicro);
		GetDlgItem(ID_StaticMN[9])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 9]);
	}

}

//��11�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec11()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 10)
	{
		CurrentPatientResult[21 * Page + 10].NumMicro--;
		if (CurrentPatientResult[21 * Page + 10].NumMicro<0)
			CurrentPatientResult[21 * Page + 10].NumMicro = 0;

		CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[10] = CurrentPatientResult[21 * Page + 10].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 10].NumMicro);
		GetDlgItem(ID_StaticMN[10])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 10]);
	}

}

//��12�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec12()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 11)
	{
		CurrentPatientResult[21 * Page + 11].NumMicro--;
		if (CurrentPatientResult[21 * Page + 11].NumMicro<0)
			CurrentPatientResult[21 * Page + 11].NumMicro = 0;

		CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[11] = CurrentPatientResult[21 * Page + 11].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 11].NumMicro);
		GetDlgItem(ID_StaticMN[11])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 11]);
	}

}

//��13�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec13()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 12)
	{
		CurrentPatientResult[21 * Page + 12].NumMicro--;
		if (CurrentPatientResult[21 * Page + 12].NumMicro<0)
			CurrentPatientResult[21 * Page + 12].NumMicro = 0;

		CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[12] = CurrentPatientResult[21 * Page + 12].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 12].NumMicro);
		GetDlgItem(ID_StaticMN[12])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 12]);
	}

}

//��14�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec14()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 13)
	{
		CurrentPatientResult[21 * Page + 13].NumMicro--;
		if (CurrentPatientResult[21 * Page + 13].NumMicro<0)
			CurrentPatientResult[21 * Page + 13].NumMicro = 0;

		CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[13] = CurrentPatientResult[21 * Page + 13].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 13].NumMicro);
		GetDlgItem(ID_StaticMN[13])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 13]);
	}

}

//��15�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec15()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 14)
	{
		CurrentPatientResult[21 * Page + 14].NumMicro--;
		if (CurrentPatientResult[21 * Page + 14].NumMicro<0)
			CurrentPatientResult[21 * Page + 14].NumMicro = 0;

		CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[14] = CurrentPatientResult[21 * Page + 14].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 14].NumMicro);
		GetDlgItem(ID_StaticMN[14])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 14]);
	}

}

//��16�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 15)
	{
		CurrentPatientResult[21 * Page + 15].NumMicro--;
		if (CurrentPatientResult[21 * Page + 15].NumMicro<0)
			CurrentPatientResult[21 * Page + 15].NumMicro = 0;

		CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[15] = CurrentPatientResult[21 * Page + 15].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 15].NumMicro);
		GetDlgItem(ID_StaticMN[15])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 15]);
	}

}

//��17�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec17()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 16)
	{
		CurrentPatientResult[21 * Page + 16].NumMicro--;
		if (CurrentPatientResult[21 * Page + 16].NumMicro<0)
			CurrentPatientResult[21 * Page + 16].NumMicro = 0;

		CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[16] = CurrentPatientResult[21 * Page + 16].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 16].NumMicro);
		GetDlgItem(ID_StaticMN[16])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 16]);
	}

}

//��18�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec18()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 17)
	{
		CurrentPatientResult[21 * Page + 17].NumMicro--;
		if (CurrentPatientResult[21 * Page + 17].NumMicro<0)
			CurrentPatientResult[21 * Page + 17].NumMicro = 0;

		CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[17] = CurrentPatientResult[21 * Page + 17].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 17].NumMicro);
		GetDlgItem(ID_StaticMN[17])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 17]);
	}

}

//��19�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec19()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 18)
	{
		CurrentPatientResult[21 * Page + 18].NumMicro--;
		if (CurrentPatientResult[21 * Page + 18].NumMicro<0)
			CurrentPatientResult[21 * Page + 18].NumMicro = 0;

		CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[18] = CurrentPatientResult[21 * Page + 18].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 18].NumMicro);
		GetDlgItem(ID_StaticMN[18])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 18]);
	}

}

//��20�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec20()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 19)
	{
		CurrentPatientResult[21 * Page + 19].NumMicro--;
		if (CurrentPatientResult[21 * Page + 19].NumMicro<0)
			CurrentPatientResult[21 * Page + 19].NumMicro = 0;

		CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[19] = CurrentPatientResult[21 * Page + 19].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 19].NumMicro);
		GetDlgItem(ID_StaticMN[19])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 19]);
	}

}

//��21�� ΢����-
void CMicroNucleusResult::OnBnClickedBtnMndec21()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CurrentPatientResult.size() > 21 * Page + 20)
	{
		CurrentPatientResult[21 * Page + 20].NumMicro--;
		if (CurrentPatientResult[21 * Page + 20].NumMicro<0)
			CurrentPatientResult[21 * Page + 20].NumMicro = 0;

		CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro = true;
		UpdateData(FALSE);
		m_MicroCheck[20] = CurrentPatientResult[21 * Page + 20].CheckOrNot_NumMicro;

		//��ʾ���
		CString str;
		str.Format(_T("%d"), CurrentPatientResult[21 * Page + 20].NumMicro);
		GetDlgItem(ID_StaticMN[20])->SetWindowTextW(str);
		//�������
		MN_HandleResult * result = CalculateMNResult(CurrentPatientResult);
		//��ʾ�ܽ��
		ShowSumResult(result);
		//�����������ݿ�
		current = m_comboxpatient.GetCurSel();
		ReadAndWriteMNResult.SaveToAccessMN(result, PatientNames[current], CurrentPatientResult[21 * Page + 20]);
	}

}
