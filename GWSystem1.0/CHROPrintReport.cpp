// CHROPrintReport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CHROPrintReport.h"
#include "afxdialogex.h"
#include"ReadAndWriteForAccess.h"
//����
#include "IllusionExcelFile.h"

// CCHROPrintReport �Ի���
CReadAndWriteForAccess Read;

IMPLEMENT_DYNAMIC(CCHROPrintReport, CDialogEx)

CCHROPrintReport::CCHROPrintReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCHROPrintReport::IDD, pParent)
{

}

CCHROPrintReport::~CCHROPrintReport()
{
}

void CCHROPrintReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REPORTCHRO, m_reportchro);
	DDX_Control(pDX, IDC_LIST_CHREPORT, m_listchro);
}


BEGIN_MESSAGE_MAP(CCHROPrintReport, CDialogEx)
	ON_MESSAGE(WM_WAITTOPRINTCHRO, &CCHROPrintReport::InitialList1) //�Զ�����Ϣ��������
	ON_BN_CLICKED(IDC_BTN_ADDCHRO, &CCHROPrintReport::OnBnClickedBtnAddchro)
	ON_BN_CLICKED(IDC_BTN_PAINTALLCHRO, &CCHROPrintReport::OnBnClickedBtnPaintallchro)
	ON_BN_CLICKED(IDC_BTN_CHROPRINT, &CCHROPrintReport::OnBnClickedBtnChroprint)
	ON_BN_CLICKED(IDC_BTN_DELCHRO, &CCHROPrintReport::OnBnClickedBtnDelchro)
	ON_BN_CLICKED(IDC_BTN_CHROCANCLE, &CCHROPrintReport::OnBnClickedBtnChrocancle)
END_MESSAGE_MAP()


// CCHROPrintReport ��Ϣ�������

LRESULT CCHROPrintReport::InitialList1(WPARAM wParam, LPARAM lParam)//���ݿɴ�ӡ����Ĳ���
{
	p = (vector<WaitToPrint>*)wParam;
	//ˢ�¿ɴ�ӡ����Ĳ���
	m_reportchro.ResetContent();
	for (size_t i = 0; i < p->size(); i++)
	{
		m_reportchro.InsertString(i, (*p)[i].patientname);
	}
	m_reportchro.SetCurSel(0);

	//�����
	//���ñ�ͷ
	m_listchro.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_listchro.InsertColumn(0, _T("���"), LVCFMT_CENTER, 60, 0);
	m_listchro.InsertColumn(1, _T("��������"), LVCFMT_CENTER, 90, 0);
	m_listchro.InsertColumn(2, _T("����洢·��"), LVCFMT_CENTER, 480, 0);
	m_listchro.DeleteColumn(0); //ɾ����0��
	//�����иߣ�
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listchro.SetImageList(&m_l, LVSIL_SMALL);

	//�������壺
	CFont m_font;
	m_font.CreatePointFont(120, _T("������"));
	m_listchro.SetFont(&m_font);

	return 0;
}


//��Ӳ���
void CCHROPrintReport::OnBnClickedBtnAddchro()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡcombox�еĲ���
	CString patient = (*p)[m_reportchro.GetCurSel()].patientname;
	string stringpatient = (CW2A)patient;
	string path = (*p)[m_reportchro.GetCurSel()].savepath + stringpatient + ".xlsx";
	WaitToPrint combox2list;
	combox2list.patientname = patient;
	combox2list.savepath = path;
	//��ѯPList���Ƿ��Ѿ��и���
	vector<CString>::iterator it = find(PatientInList.begin(), PatientInList.end(), patient);
	if (it == PatientInList.end())
	{
		PatientInList.push_back(patient);
		pList.push_back(combox2list);
	}

	//��ʾ����
	FreshPatient();

}

//��ӡ�ܱ�
void CCHROPrintReport::OnBnClickedBtnPaintallchro()
{
	IllusionExcelFile excl;
	bool bInit = excl.InitExcel();
	if (!bInit)
	{
		AfxMessageBox(_T("��ʼ��Excelʧ�ܣ���ȷ�ϱ����Ƿ�װExcel��"));
	}
	else
	{
		bool bRet = excl.OpenExcelFile(_T("C:\\model.xlsx"));
		if (!bRet)
		{
			AfxMessageBox(_T("��Excelģ��ʧ�ܣ���ȷ��ģ���Ƿ���ڣ�"));
		}
		else{
			CString strSheetName = excl.GetSheetName(2);
			bool bLoad = excl.LoadSheet(strSheetName);
			CString strValue;

			vector<CHRO_HandleResult>AllChosePatient = Read.ReadAllResultFromAccess();
			for (size_t i = 0; i < AllChosePatient.size(); i++)
			{
				CHRO_HandleResult pA;
				pA = AllChosePatient[i];

				strValue.Format(_T("%d"), i+1);
				excl.SetCellString(8 + i, 2, strValue); //����ϸ������

				strValue = pA.patientname;
				excl.SetCellString(8 + i, 3, strValue); //����
				strValue.Format(_T("%d"), pA.allcell);
				excl.SetCellString(8 + i, 5, strValue); //����ϸ������
				strValue.Format(_T("%d"), pA.abnormal);
				excl.SetCellString(8 + i, 6, strValue); //����ϸ����
				strValue.Format(_T("%f"), pA.Y*100);
				excl.SetCellString(8 + i, 7, strValue); //������
				strValue.Format(_T("%d"), pA.dic);
				excl.SetCellString(8 + i, 8, strValue); //˫����
				//strValue.Format(_T("%d"), pA.tri);
				//excl.SetCellString(8 + i, 9, strValue);
				//strValue.Format(_T("%d"), pA.ten);
				//excl.SetCellString(8 + i, 10, strValue);
				//strValue.Format(_T("%d"), pA.ace);
				//excl.SetCellString(8 + i, 11, strValue);
				//strValue.Format(_T("%d"), pA.round);
				//excl.SetCellString(8 + i, 12, strValue);
				//strValue.Format(_T("%d"), pA.t);
				//excl.SetCellString(8 + i, 13, strValue);
				//strValue.Format(_T("%d"), pA.inv);
				//excl.SetCellString(8 + i, 14, strValue);
				//strValue.Format(_T("%d"), pA.del);
				//excl.SetCellString(8 + i, 15, strValue);

				//int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
				int sum = pA.dic ;
				strValue.Format(_T("%d"), sum);
				excl.SetCellString(8 + i, 17, strValue);
				float rate = (float)sum / pA.allcell;
				strValue.Format(_T("%f"), rate * 100);
				CString str = strValue;
				excl.SetCellString(8 + i, 18, str);

				//strValue.Format(_T("%d"), pA.ctg);
				//excl.SetCellString(8 + i, 18, strValue);
				//strValue.Format(_T("%d"), pA.csg);
				//excl.SetCellString(8 + i, 19, strValue);
				//strValue.Format(_T("%d"), pA.ctb);
				//excl.SetCellString(8 + i, 20, strValue);
				//strValue.Format(_T("%d"), pA.cte);
				//excl.SetCellString(8 + i, 21, strValue);

				//sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
				//strValue.Format(_T("%d"), sum);
				//excl.SetCellString(8 + i, 22, strValue);
				//rate = (float)sum / pA.allcell;
				//strValue.Format(_T("%f"), rate * 100);
				//str = strValue + _T("%");
				//excl.SetCellString(8 + i, 23, strValue);
			}

			CString strFile;
			CFileDialog dlg(FALSE, _T("xlsx"), _T("���ܱ�"));//FALSE��ʾΪ�����Ϊ���Ի��򣬷���Ϊ���򿪡��Ի���
			if (dlg.DoModal() == IDOK)
			{
				strFile = dlg.GetPathName();//��ȡ����·��
				excl.SaveasXSLFile(strFile);
				string str12 = CT2A(strFile.GetBuffer());
				string command_rd = "start  excel " + str12;
				system(command_rd.c_str());
			}


		}

	}

}

//��ӡ����
void CCHROPrintReport::OnBnClickedBtnChroprint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (size_t i = 0; i < pList.size(); i++)
	{
		MakeOneReporte(pList[i].patientname, pList[i].savepath);
	}
	AfxMessageBox(_T("���ɳɹ���"));

}

// ����һ�����˵ı���
bool CCHROPrintReport::MakeOneReporte(CString patientname, string path)
{
	//��ȡ����
	CHRO_HandleResult pA;
	pA = Read.ReadOneResultFromAccess(patientname);

	IllusionExcelFile excl;
	bool bInit = excl.InitExcel();
	bool bRet = excl.OpenExcelFile(_T("C:\\model.xlsx"));
	CString strSheetName = excl.GetSheetName(1);
	bool bLoad = excl.LoadSheet(strSheetName);

	CString strValue;
	strValue = pA.patientname;
	excl.SetCellString(3, 4, strValue); //����
	strValue.Format(_T("%d"),pA.allcell);
	excl.SetCellString(8, 2, strValue); //����ϸ������
	strValue.Format(_T("%d"), pA.abnormal);
	excl.SetCellString(8, 3, strValue); //����ϸ����
	strValue.Format(_T("%f"), pA.Y);
	excl.SetCellString(8, 4, strValue); //������
	strValue.Format(_T("%d"), pA.dic);
	excl.SetCellString(8, 5, strValue); //˫����
	//strValue.Format(_T("%d"), pA.tri);
	//excl.SetCellString(8, 6, strValue);
	//strValue.Format(_T("%d"), pA.ten);
	//excl.SetCellString(8, 7, strValue);
	//strValue.Format(_T("%d"), pA.ace);
	//excl.SetCellString(8, 8, strValue);
	//strValue.Format(_T("%d"), pA.round);
	//excl.SetCellString(8, 9, strValue);
	//strValue.Format(_T("%d"), pA.t);
	//excl.SetCellString(8, 10, strValue);
	//strValue.Format(_T("%d"), pA.inv);
	//excl.SetCellString(8, 11, strValue);
	//strValue.Format(_T("%d"), pA.del);
	//excl.SetCellString(8, 12, strValue);

	//int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
	int sum = pA.dic;
	strValue.Format(_T("%d"), sum);
	excl.SetCellString(8, 13, strValue);
	float rate = (float)sum / pA.allcell;
	strValue.Format(_T("%f"), rate*100);
	CString str = strValue;
	excl.SetCellString(8, 14, str);

	//strValue.Format(_T("%d"), pA.ctg);
	//excl.SetCellString(8, 15, strValue);
	//strValue.Format(_T("%d"), pA.csg);
	//excl.SetCellString(8, 16, strValue);
	//strValue.Format(_T("%d"), pA.ctb);
	//excl.SetCellString(8, 17, strValue);
	//strValue.Format(_T("%d"), pA.cte);
	//excl.SetCellString(8, 18, strValue);

	//sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
	//strValue.Format(_T("%d"), sum);
	//excl.SetCellString(8, 19, strValue);
	//rate = (float)sum / pA.allcell;
	//strValue.Format(_T("%f"), rate*100);
	//str = strValue + _T("%");
	//excl.SetCellString(8, 20, str);

	CString strFile(path.c_str());
	//CFileDialog dlg(FALSE, _T("xlsx"), patientname);//FALSE��ʾΪ�����Ϊ���Ի��򣬷���Ϊ���򿪡��Ի���
	//if (dlg.DoModal() == IDOK)
	//{
	//	strFile = dlg.GetPathName();//��ȡ����·��
	//}

	excl.SaveasXSLFile(strFile);
	string str12 = CT2A(strFile.GetBuffer());
	string command_rd = "start  excel " + str12;
	system(command_rd.c_str());
	return false;
}


//ɾ��һ��
void CCHROPrintReport::OnBnClickedBtnDelchro()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int sel = m_listchro.GetSelectionMark();
	if (sel >= 0)
	{
		//vector��ɾ��
		auto iter = pList.erase(std::begin(pList) + sel);
		auto iter1 = PatientInList.erase(std::begin(PatientInList) + sel);

		//List�ؼ���Ҳɾ��������¼
		m_listchro.DeleteItem(sel);
		FreshPatient();
	}
	else if (sel<0)
		MessageBox(_T("�б�����ѡ�м�¼��"));

}

// ˢ����ʾ����
void CCHROPrintReport::FreshPatient()
{
	CString strpath;
	m_listchro.DeleteAllItems();
	for (size_t i = 0; i < pList.size(); i++)
	{
		m_listchro.InsertItem(i, pList[i].patientname);//����һ��
		USES_CONVERSION;
		strpath = pList[i].savepath.c_str();
		m_listchro.SetItemText(i, 1, strpath);
	}
}

//ȡ��
void CCHROPrintReport::OnBnClickedBtnChrocancle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
