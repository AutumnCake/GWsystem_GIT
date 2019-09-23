// CChromosomeHandle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CChromosomeHandle.h"
#include"ChromosomeAlgorithm.h"
#include"ChromosomeResult.h"
#include "CHROPrintReport.h"
#include "afxdialogex.h"
#include "GWSystem1.0Dlg.h"
#include "ReadAndWriteForAccess.h"
#include "Scanning_Control.h"
#include<thread>

#include "CSeries.h"
#include <stdlib.h>
#include <direct.h>
#include<io.h>
#include"FileOperate.h"

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"



extern Scanning_Control *m_Scanning_Control;
extern CGWSystem10Dlg* pDlg;
extern vector <PatientWithSlide> tempAllPatients;
//ȫ�ֱ���
CChromosomeHandle * pHandleDlg;
CChromosomeResult * pChromosomeResult;
CReadAndWriteForAccess ReadAndWrite;
string StrFileSolveChro;


// CChromosomeHandle �Ի���

IMPLEMENT_DYNAMIC(CChromosomeHandle, CDialogEx)

CChromosomeHandle::CChromosomeHandle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChromosomeHandle::IDD, pParent)
{
	pChromosomeResult = NULL;
	patientNewImg = true;
}

CChromosomeHandle::~CChromosomeHandle()
{
	
}

void CChromosomeHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listchrohandle);
	DDX_Control(pDX, IDC_TCHARTCHRO, m_chartchrom);
	DDX_Control(pDX, IDC_DATESTARTCHRO, m_datetimebegin);
	DDX_Control(pDX, IDC_DATEENDCHRO, m_datetimeend);
	DDX_Control(pDX, IDC_LIST_RESULTCHRO, m_listchroresult);
	//DDX_Control(pDX, IDC_GRID_CHRO, m_pGridchro);
}


BEGIN_MESSAGE_MAP(CChromosomeHandle, CDialogEx)
	ON_MESSAGE(WM_HANDLEPATHCHRO, &CChromosomeHandle::InitialList1) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_INIT_TABLECHRO, &CChromosomeHandle::InitialList2) //�Զ�����Ϣ��������
	ON_BN_CLICKED(IDC_BTN_CHROHANDLE, &CChromosomeHandle::OnBnClickedBtnChrohandle)
	ON_BN_CLICKED(IDC_BTN_CHROSOLVE, &CChromosomeHandle::OnBnClickedBtnChrosolve)
	ON_BN_CLICKED(IDC_BTN_CHROPRINT, &CChromosomeHandle::OnBnClickedBtnChroprint)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CChromosomeHandle::OnNMClickList1)
	ON_BN_CLICKED(IDC_BTN_FINDCHRO, &CChromosomeHandle::OnBnClickedBtnFindchro)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CChromosomeHandle::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CChromosomeHandle::OnNMDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULTCHRO, &CChromosomeHandle::OnNMClickListResultchro)
END_MESSAGE_MAP()


// CChromosomeHandle ��Ϣ�������


BOOL CChromosomeHandle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	pHandleDlg = this;//ָ��öԻ����ȫ��ָ��

	//�������ݿ�
	m_Conn.OnInitADOAccess();
	GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���
	GetDlgItem(IDC_BTN_CHROPRINT)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���
	GetDlgItem(IDC_BTN_CHROSOLVE)->EnableWindow(FALSE);
	//ɾ��ԭ���ݱ�
	DeleteAccessTable();
	//���ñ�ͷ
	CRect rectCtrl;
	CStatic *p = (CStatic*)GetDlgItem(IDC_LIST1);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listchrohandle.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listchrohandle.InsertColumn(0, _T("�Ƿ����"), LVCFMT_CENTER, 80, 0);
	m_listchrohandle.InsertColumn(1, _T("���"), LVCFMT_CENTER, rectCtrl.Width()*0.107, 0);
	m_listchrohandle.InsertColumn(2, _T("��������"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(3, _T("ͼƬ����"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(4, _T("��������"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listchrohandle.InsertColumn(5, _T("��ͼʱ��"), LVCFMT_CENTER, rectCtrl.Width()*0.25);
	m_listchrohandle.InsertColumn(6, _T("�����������"), LVCFMT_CENTER, rectCtrl.Width()*0.214);
	m_listchrohandle.DeleteColumn(0); //ɾ����0��

	//���ý�����ͷ
	p = (CStatic*)GetDlgItem(IDC_LIST_RESULTCHRO);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listchroresult.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_listchroresult.InsertColumn(0, _T("�Ƿ����"), LVCFMT_CENTER, 80, 0);
	m_listchroresult.InsertColumn(1, _T("����"), LVCFMT_CENTER, rectCtrl.Width()*0.073);
	m_listchroresult.InsertColumn(2, _T("����ϸ������"), LVCFMT_CENTER, rectCtrl.Width()*0.085);
	m_listchroresult.InsertColumn(3, _T("����ϸ����"), LVCFMT_CENTER, rectCtrl.Width()*0.065);
	m_listchroresult.InsertColumn(4, _T("������"), LVCFMT_CENTER, rectCtrl.Width()*0.045);
	m_listchroresult.InsertColumn(5, _T("dic"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(6, _T("tri"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(7, _T("ten"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(8, _T("ace"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(9, _T("r"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(10, _T("t"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(11, _T("inv"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(12, _T("del"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(13, _T("Ⱦɫ���ͻ�����"), LVCFMT_CENTER, rectCtrl.Width()*0.096);
	m_listchroresult.InsertColumn(14, _T("Ⱦɫ���ͻ�����"), LVCFMT_CENTER, rectCtrl.Width()*0.096);
	m_listchroresult.InsertColumn(15, _T("ctg"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(16, _T("csg"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(17, _T("ctb"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(18, _T("cte"), LVCFMT_CENTER, rectCtrl.Width()*0.03);
	m_listchroresult.InsertColumn(19, _T("�����ͻ�����"), LVCFMT_CENTER, rectCtrl.Width()*0.090);
	m_listchroresult.InsertColumn(20, _T("�����ͻ�����"), LVCFMT_CENTER, rectCtrl.Width()*0.090);
	m_listchroresult.DeleteColumn(0); //ɾ����0��


	//�����иߣ�
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listchrohandle.SetImageList(&m_l, LVSIL_SMALL);
	//m_listchroresult.SetImageList(&m_l, LVSIL_SMALL);

	//�������壺
	CFont m_font;
	m_font.CreatePointFont(120, _T("������"));
	m_listchrohandle.SetFont(&m_font);
	GetDlgItem(IDC_LISTHEADCHRO)->SetFont(&m_font);
	/*m_listchroresult.SetFont(&m_font);*/

	////��ȡ·��
	//m_handlepathchro.GetWindowTextW(ReadPath);
	//m_resultpathchro.GetWindowTextW(WritePath);

	//����ʱ��ؼ��ĸ�ʽ
	m_datetimebegin.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	m_datetimeend.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	COleDateTime MinTime(COleDateTime::GetCurrentTime() - COleDateTimeSpan(30, 0, 0, 0));
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	m_datetimebegin.SetRange(&MinTime, &MaxTime);
	m_datetimeend.SetRange(&MinTime, &MaxTime);

	//�������������� ������
	if (pChromosomeResult == NULL){//�ж��Ի����Ƿ�����ָ��
		pChromosomeResult = new CChromosomeResult(this);//ָ��һ����ģ�Ի���ʾ��
		pChromosomeResult->Create(IDD_CHROMOSOMERESULT, this);//����
	}
	pChromosomeResult->ShowWindow(SW_HIDE);//����ʾ
	pChromosomeResult->m_comboxpatient.ResetContent();


	//������ɨ����Ϣ�����б� ��ѡ��
	LoadCurrentToList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


// ������ݿ��е�һЩ���
int CChromosomeHandle::DeleteAccessTable()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("delete  * from Ⱦɫ��ͼ�����������ݱ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete  * from Ⱦɫ��ͼ�����������ݱ���ɾ����");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}


	sql = _T("delete  * from Ⱦɫ�����������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	return 0;
}


//�Զ�����Ϣ����1----���ݴ����ļ���·���봢���ļ���·��
LRESULT CChromosomeHandle::InitialList1(WPARAM wParam, LPARAM lParam)
{
	MyFiles *pA = (MyFiles*)wParam;
	//CString  str;
	//str = pA->picsolve.c_str();
	//m_handlepathchro.SetWindowText(str);
	//str = pA->endsolve.c_str();
	//m_resultpathchro.SetWindowText(str);
	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����2----���ݴ����ļ���·���봢���ļ���·��
LRESULT CChromosomeHandle::InitialList2(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString mystr(b);
	SysFreeString(b);

	pHandleDlg->m_listchrohandle.SetItemText(lParam, 3, mystr);
	pHandleDlg->UpdateData(FALSE);

	return 0;//һ�����и�����ֵ

}

//��ʼ����
void CChromosomeHandle::OnBnClickedBtnChrohandle()
{
	GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���

	//1.��ȡ��ѡ�еĲ������飬����AllPatientsChose
	int nItem, i;
	nItem = m_listchrohandle.GetItemCount();
	for (i = 0; i < nItem; ++i)
	{
		if (m_listchrohandle.GetCheck(i))
		{
			AllPatientsChose.push_back(AllPatients[i]);
			Row.push_back(i);
		}
	}

	thread ChroAnalyse(ThreadProcWaitCHRO);
	ChroAnalyse.detach();

}

//�����߳�
void CChromosomeHandle::ThreadProcWaitCHRO()
{
	//��ʼ��python
	//��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
	//���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
	//����ģ������·���С��������û�з���ֵ�����ϵͳ
	//�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��
	Py_Initialize();
	// ����ʼ���Ƿ�ɹ�
	if (!Py_IsInitialized())
	{
		AfxMessageBox(_T("python��ʼ��ʧ��!\n"));
	}
	PyRun_SimpleString("print('come in python')");
	//��ʾpython�汾��Ϣ
	cout << "��ʾpython�汾��Ϣ��";
	cout << (Py_GetVersion()) << "\n";
	/*PyObject* sys = PyImport_ImportModule("sys");*/
	//PyRun_SimpleString("import sys"); // ִ�� python �еĶ����  
	//PyRun_SimpleString("sys.path.append('./')");
	//PyRun_SimpleString("import cv2");

	//���ɴ���ʱ���ļ���
	string strWrite = CT2A(pHandleDlg->WritePath.GetBuffer());
	string writeroad;
	writeroad = strWrite + "\\";

	CTime time = CTime::GetCurrentTime(); ///����CTime����
	int m_nYear = time.GetYear(); ///��
	int m_nMonth = time.GetMonth(); ///��
	int m_nDay = time.GetDay(); ///��
	int m_nHour = time.GetHour(); ///Сʱ
	int m_nMinute = time.GetMinute(); ///����
	int m_nSecond = time.GetSecond(); ///��
	CString strDate;
	strDate.Format(_T("%d��%d��%d��"), m_nYear, m_nMonth, m_nDay); //�������ڸ�ʽ
	CString strTime = time.Format(_T("%H-%M-%S")); //����ʱ���ʽ
	CString index(_T("�Ĵ�����"));
	pHandleDlg->StrTime = strDate + strTime + index;

	pHandleDlg->ResultRow = 0;
	for (size_t i = 0; i < pHandleDlg->AllPatientsChose.size(); i++)
	{
		//vector<bool>().swap(m_Scanning_Control->Scanning_Completed_Status);
		pHandleDlg->patientNewImg = true;//�տ�ʼ���� �϶�������Ƭ

		int row = pHandleDlg->Row[i];
		pHandleDlg->PatientSlideNum = pHandleDlg->GetPatientSlideNumFromAccess(pHandleDlg->AllPatientsChose[i].PatientName);
		//(1)���ɸò��˵Ĵ������ļ���
		CString name = pHandleDlg->AllPatientsChose[i].PatientName; //����ʱ���ʽ
		pHandleDlg->StrName = pHandleDlg->AllPatientsChose[i].ResultPath + name + "-" + pHandleDlg->StrTime;
		string strname = CT2A(pHandleDlg->StrName.GetBuffer());
		StrFileSolveChro = pHandleDlg->strFileSolve + strname + "\\";//���ɽ�����ļ��� 
		const char* c_filesolve = StrFileSolveChro.c_str();//��·��ת���� const char
		_mkdir(c_filesolve);

		//(2)��ʼ�Ըò��˵�ͼƬ���з���
		CChromosomeAlgorithm algorithm;
		CHRO_HandleResult *pB = new CHRO_HandleResult; //�ò��˷�������洢�ṹ��
		CString mystr;
		mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].ChroImgNames.size());

		PatientAndQuaImg thistime;
		thistime.patientname = name;
		USES_CONVERSION;
		thistime.QuestionImgPath = A2W((StrFileSolveChro).c_str());

		pChromosomeResult->m_comboxpatient.AddString(name);
		pChromosomeResult->AllQuestionImgPath.push_back(thistime);
		pChromosomeResult->PatientNames.push_back(name);

		size_t newStart = 0;
		while (pHandleDlg->patientNewImg)//�жϸò����Ƿ����µ�ͼƬ����
		{
			for (size_t j = newStart; j < pHandleDlg->AllPatientsChose[i].ChroImgNames.size(); j++)
			{
				string readpath = CT2A(pHandleDlg->AllPatientsChose[i].ImgPath.GetBuffer());
				string imgname = CT2A(pHandleDlg->AllPatientsChose[i].ChroImgNames[j].GetBuffer());
				pHandleDlg->AllPatientsChose[i].Result = algorithm.DicMain(readpath, imgname, StrFileSolveChro, pHandleDlg->AllPatientsChose[i].PatientName, pB);
				pHandleDlg->AllPatientsChose[i].Result->patientname = pHandleDlg->AllPatientsChose[i].PatientName;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].ChroImgNames.size() + 1;
				pHandleDlg->AllPatientsChose[i].Result->sourcefile = pHandleDlg->AllPatientsChose[i].ImgPath;
				pHandleDlg->AllPatientsChose[i].Result->resultfile = pHandleDlg->AllPatientsChose[i].ResultPath;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].ChroImgNames.size();

				CString mystr1, mystr2;
				mystr1.Format(_T("%d"), j + 1);
				mystr2 = mystr1 + _T("/") + mystr;

				HWND   hwnd = ::FindWindow(NULL, _T("Ⱦɫ�崦��"));//������Ϣ������ˢ��ҳ��
				::SendMessage(hwnd, WM_INIT_TABLECHRO, (WPARAM)mystr2.AllocSysString(), (WPARAM)row);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ
			}
			//��ѯ�ò��˵�ͼƬ�Ƿ�������
			vector<CString>patientNewImgPath = pHandleDlg->InquiryPatientImg(pHandleDlg->AllPatientsChose[i].PatientName, pHandleDlg->AllPatientsChose[i].ImgPath);
			int patientNewImgNum = patientNewImgPath.size();
			if (patientNewImgNum > pHandleDlg->AllPatientsChose[i].ChroImgNames.size())
			{
				pHandleDlg->patientNewImg = true;
				newStart = pHandleDlg->AllPatientsChose[i].ChroImgNames.size();
				pHandleDlg->AllPatientsChose[i].ChroImgNames = patientNewImgPath;
				mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].ChroImgNames.size());

			}
			else
			{

				CString strPath;//10������ɨ��ͼ��洢��·��
				CA2T szr2((pDlg->AllFiles.picsolve).c_str());
				strPath = (LPCTSTR)szr2;
				if (pHandleDlg->AllPatientsChose[i].ImgPath == strPath)
				{
					for (size_t j = 0; j < tempAllPatients.size(); j++)
					{
						if (pHandleDlg->AllPatientsChose[i].PatientName == tempAllPatients[j].PatientName)
						{
							int AllOver = 0;
							for (size_t k = 0; k < tempAllPatients[j].SlideID.size(); k++)
							{
								if (tempAllPatients[j].SlideID[k] == -1)
									AllOver += 0;
								else
									AllOver += 1;
							}
							if (AllOver>0)
								pHandleDlg->patientNewImg = true;
							else
								pHandleDlg->patientNewImg = false;
						}

					}
					
				
				}
				else
					pHandleDlg->patientNewImg = false;

				//bool Scanning_End = false;

				//if (m_Scanning_Control->Scanning_Completed_Status.size() >= m_Scanning_Control->s_Scanning_Para.Current_Slide_Num+1)
				//	Scanning_End = m_Scanning_Control->Scanning_Completed_Status[m_Scanning_Control->s_Scanning_Para.Current_Slide_Num];
				//for (int m = 0; m < pHandleDlg->PatientSlideNum.size(); m++)
				//{
				//	bool Scanning_Over = pHandleDlg->PatientSlideNum[m] == m_Scanning_Control->s_Scanning_Para.Current_Slide_Num;
				//	//if ((pHandleDlg->PatientSlideNum[m] == m_Scanning_Control->s_Scanning_Para.Current_Slide_Num))
				//	if (Scanning_End && Scanning_Over)
				//		pHandleDlg->patientNewImg = false;
				//	else
				//		pHandleDlg->patientNewImg = true;
				//}

			}
				
		}

		//����������������ݿ���
		ReadAndWrite.SaveToAccess(pHandleDlg->AllPatientsChose[i].Result);
		//�������ɸò��˵ı���
		pHandleDlg->CHROWaitToPrint(pHandleDlg->AllPatientsChose[i].Result->patientname, StrFileSolveChro);
		//������иĳɱ���������
		CString mystr3 = _T("��");
		pHandleDlg->m_listchrohandle.SetItemText(row, 5, mystr3);
		//ͼ������ʾ�ò��˵ķ������
		pHandleDlg->ShowOneResult(pHandleDlg->AllPatientsChose[i].Result->patientname);
		pHandleDlg->ShowOneResultInTable(pHandleDlg->AllPatientsChose[i].Result->patientname);

		pHandleDlg->ResultRow++;

		pHandleDlg->GetDlgItem(IDC_BTN_CHROPRINT)->EnableWindow(TRUE);//��lisrѡ��ѡ��ʹɾ����ť���
		pHandleDlg->GetDlgItem(IDC_BTN_CHROSOLVE)->EnableWindow(TRUE);//��lisrѡ��ѡ��ʹɾ����ť���

	}

	//�ر�python����
	//**************************************************************************************************
	Py_Finalize();
}


// ��������ɱ���Ľṹ��
void CChromosomeHandle::CHROWaitToPrint(CString patientname, string savepath)
{
	WaitToPrint chroWaitToPrint;
	chroWaitToPrint.patientname = patientname;
	chroWaitToPrint.savepath = savepath;
	CHROWaitPrint.push_back(chroWaitToPrint);
}

// ��һ�����˵ķ��������ʾ��ͼ��
bool CChromosomeHandle::ShowOneResult(CString patientname)
{
	//��ʾ��ͼ��
	CHRO_HandleResult pA;
	pA = ReadAndWrite.ReadOneResultFromAccess(patientname);
	ClearAllSeries();
	CSeries barSeries = (CSeries)m_chartchrom.Series(0);
	barSeries.AddXY((double)1, (double)pA.allcell, _T("����ϸ������"), RGB(255, 255, 0));
	barSeries.AddXY((double)2, (double)pA.dic, _T("˫��˿������"), RGB(255, 0, 255));
	barSeries.AddXY((double)3, (double)pA.round, _T("��˿������"), RGB(0, 255, 255));
	barSeries.AddXY((double)4, (double)pA.ace, _T("����˿������"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.t, _T("�໥��λ��"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.inv, _T("��λ��"), RGB(0, 255, 255));
	//UpdateData(false);

	return false;
}

// ��һ�����˵ķ��������ʾ������
bool CChromosomeHandle::ShowOneResultInTable(CString patientname)
{

	CHRO_HandleResult pA;
	pA = ReadAndWrite.ReadOneResultFromAccess(patientname);

	//��ʾ������
	int j = 0;
	CString  mystr;
	mystr = pA.patientname;
	m_listchroresult.InsertItem(ResultRow, mystr);
	mystr.Format(_T("%d"), pA.allcell);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.abnormal);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	float rate = (float)pA.abnormal / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	CString str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	mystr.Format(_T("%d"), pA.dic);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.tri);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ten);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ace);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.round);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.t);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.inv);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.del);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	int sum = pA.dic + pA.tri + pA.ten + pA.ace + pA.round + pA.t + pA.inv + pA.del;
	mystr.Format(_T("%d"), sum);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	rate = (float)sum / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	mystr.Format(_T("%d"), pA.ctg);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.csg);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.ctb);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.cte);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	sum = pA.ctg + pA.csg + pA.ctb + pA.cte;
	mystr.Format(_T("%d"), sum);
	m_listchroresult.SetItemText(ResultRow, ++j, mystr);

	rate = (float)sum / pA.allcell;
	mystr.Format(_T("%.2f"), rate * 100);
	str = mystr + _T("%");
	m_listchroresult.SetItemText(ResultRow, ++j, str);

	return false;
}


//���ͼ��
void CChromosomeHandle::ClearAllSeries(void)
{
	for (long i = 0; i<m_chartchrom.get_SeriesCount(); i++)
	{
		((CSeries)m_chartchrom.Series(i)).Clear();
	}

}

//�鿴������
void CChromosomeHandle::OnBnClickedBtnChrosolve()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	pChromosomeResult->ShowWindow(SW_SHOW);//����ʾ

}

//��ӡ���η����ı���
void CChromosomeHandle::OnBnClickedBtnChroprint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CCHROPrintReport *pCHPrintReport = new CCHROPrintReport;
	pCHPrintReport->Create(IDD_PRINTREPORTCHRO, this);
	//����ǰ�ɴ�ӡ�Ĳ��˴�����ӡ�����Գ�ʼ��
	HWND   hwnd = ::FindWindow(NULL, _T("��ӡ�������棨Ⱦɫ�壩"));//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_WAITTOPRINTCHRO, (WPARAM)&CHROWaitPrint, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ

	pCHPrintReport->ShowWindow(SW_SHOW);
}

//�����б��¼�
void CChromosomeHandle::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//int row = m_listchrohandle.GetSelectionMark();
	//CString name = m_listchrohandle.GetItemText(row, 1);
	//ShowOneResult(name);
	*pResult = 0;
}

//��ѯ
void CChromosomeHandle::OnBnClickedBtnFindchro()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ԭ���
	m_listchrohandle.DeleteAllItems();

	vector<ChroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//���ÿһ�ֲ��ҵ�ͼ

	//1.�����Ӧʱ����ڵĲ�ƬͼƬ
	COleDateTime TimeBegin, TimeEnd;
	m_datetimebegin.GetTime(TimeBegin);
	m_datetimeend.GetTime(TimeEnd);
	CReadAndWriteForAccess xx;
	vector<Table_SlideSet> ReadSlideset;
	ReadSlideset = xx.QuerySaveTimeCHRO(TimeBegin, TimeEnd);

	//2.����ÿ�Ų�Ƭ��·�����ϲ���ͬ·�����õ�·����������
	vector<CString>AllCHROImgPath;
	vector<CString>AllCHROResPath;
	vector<COleDateTime>AllCHROTime;
	for (auto iter : ReadSlideset)
	{
		vector<CString>::iterator ret;
		ret = std::find(AllCHROImgPath.begin(), AllCHROImgPath.end(), iter.path);
		if (ret == AllCHROImgPath.end())
		{
			AllCHROImgPath.push_back(iter.path);
			AllCHROResPath.push_back(iter.resultpath);
			AllCHROTime.push_back(iter.time);
		}
	}

	//3.��·���������������·���µĲ���ͼƬ���й��࣬��ʾ�����
	int patientRow = 0;
	for (size_t pathNum_i = 0; pathNum_i < AllCHROImgPath.size(); pathNum_i++)
	{
		int picNum = 0;
		vector<CString> vecEmpty;
		AllChroImg.swap(vecEmpty);//���ÿһ�ֲ��ҵ�ͼ


		//��1��ɸѡ���в��˵�΢��ͼƬ��������ṹ��
		USES_CONVERSION;
		char* charReadPath = T2A(AllCHROImgPath[pathNum_i]);
		sprintf_s(tempFilePath, "%s\\*.bmp", charReadPath);
		MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
		HANDLE handle = FindFirstFile(wstr, &file);
		if (handle != INVALID_HANDLE_VALUE)
		{
			// ѭ�������õ��ļ��е������ļ���    
			do
			{
				OneImgName = file.cFileName;
				int iPos = OneImgName.Find(_T("_"));
				CString str1 = OneImgName.Mid(iPos + 1, OneImgName.GetLength() - iPos - 1);
				iPos = str1.Find(_T("_"));
				str1 = str1.Left(iPos);
				if (str1 == _T("Ⱦɫ��"))
					AllChroImg.push_back(OneImgName);
			} while (FindNextFile(handle, &file));
		}
		FindClose(handle);

		//(2)��������������ͼƬ�Ķ�Ӧ�ṹ��
		BOOL havepatientornot;
		CString strDst;
		vector <ChroTable> tempAllPatients;

		for (auto iter : AllChroImg)
		{
			picNum = 0;
			havepatientornot = FALSE;

			//��ȡ��Ƭ���еĲ�����
			CString strDst1 = iter.Left(iter.Find(_T('��')));
			CString strDst2 = iter.Left(iter.Find(_T('(')));
			CString strDst3 = iter.Left(iter.Find(_T('_')));
			if (strDst1.IsEmpty() && !strDst2.IsEmpty())
				strDst = strDst2;
			if (!strDst1.IsEmpty() && strDst2.IsEmpty())
				strDst = strDst1;
			if (strDst1.IsEmpty() && strDst2.IsEmpty())
				strDst = strDst3;
			for (auto iterpatient : tempAllPatients)
			{
				if (iterpatient.PatientName == strDst)
				{
					havepatientornot = TRUE;
					tempAllPatients[picNum].ChroImgNames.push_back(iter);
				}
				picNum++;
			}
			if (!havepatientornot)//���û�иò���
			{
				ChroTable patient;
				patient.PatientName = strDst;
				patient.ChroImgNames.push_back(iter);
				patient.ImgPath = AllCHROImgPath[pathNum_i];
				patient.ResultPath = AllCHROResPath[pathNum_i];
				tempAllPatients.push_back(patient);
			}

		}

		//��3��������������ͼƬ������ʾ�����
		
		for (size_t k = 0; k < tempAllPatients.size(); k++)
		{
			int j = 0;
			CString  mystr;
			mystr.Format(_T("%d"), patientRow + 1);
			m_listchrohandle.InsertItem(patientRow, mystr);
			m_listchrohandle.SetItemText(patientRow, ++j, tempAllPatients[k].PatientName);
			mystr.Format(_T("%d"), tempAllPatients[k].ChroImgNames.size());
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("0/") + mystr;
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = AllCHROTime[pathNum_i].Format();
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("δ����");
			m_listchrohandle.SetItemText(patientRow, ++j, mystr);
			patientRow++;
			AllPatients.push_back(tempAllPatients[k]);
		}

	}


 	UpdateData(FALSE);



}

void CChromosomeHandle::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nItem, i;
	nItem = m_listchrohandle.GetItemCount();

	bool mark = false;
	for (i = 0; i < nItem && !mark; ++i)
	{
		if (m_listchrohandle.GetCheck(i))
		{
			mark = true;
		}
	}
	if (mark)
	{
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(TRUE);//��listѡ��ѡ�к�ʹɾ����ť����
	}
	else
	{
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���
	}


	*pResult = 0;
}

void CChromosomeHandle::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_listchrohandle.GetSelectionMark();
	CString name = m_listchrohandle.GetItemText(row, 1);
	ShowOneResult(name);
	*pResult = 0;
}

// ����ָ���ļ����²���ͼƬ������
vector <CString> CChromosomeHandle::InquiryPatientImg(CString patientname, CString imgpath)
{
	vector <CString>().swap(AllChroImg);
	//��1��ɸѡ���в��˵�΢��ͼƬ��������ṹ��
	USES_CONVERSION;
	const char* charReadPath = T2A(imgpath);
	sprintf_s(tempFilePath, "%s\\*.bmp", charReadPath);
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		// ѭ�������õ��ļ��е������ļ���    
		do
		{
			OneImgName = file.cFileName;
			int iPos = OneImgName.Find(_T("_"));
			CString str1 = OneImgName.Mid(iPos + 1, OneImgName.GetLength() - iPos - 1);
			iPos = str1.Find(_T("_"));
			str1 = str1.Left(iPos);
			if (str1 == _T("Ⱦɫ��"))
				AllChroImg.push_back(OneImgName);
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	//(2)��������������ͼƬ�Ķ�Ӧ�ṹ��
	CString strDst;
	ChroTable patient;

	for (auto iter : AllChroImg)
	{

		//��ȡ��Ƭ���еĲ�����
		CString strDst1 = iter.Left(iter.Find(_T('��')));
		CString strDst2 = iter.Left(iter.Find(_T('(')));
		CString strDst3 = iter.Left(iter.Find(_T('_')));
		if (strDst1.IsEmpty() && !strDst2.IsEmpty())
			strDst = strDst2;
		if (!strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = strDst1;
		if (strDst1.IsEmpty() && strDst2.IsEmpty())
			strDst = strDst3;
		if (strDst == patientname)
		{
			patient.PatientName = strDst;
			patient.ChroImgNames.push_back(iter);
		}

	}


	return patient.ChroImgNames;
}

// �����ݿ��л�ȡ���иò�������Ӧ�Ĳ�Ƭ��
vector<int> CChromosomeHandle::GetPatientSlideNumFromAccess(CString patientname)
{
	vector<int>patientslidenum;
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from ��Ƭ��Ϣ��");
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
			CString str = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			if (!(str.Find(patientname) == -1))
			{
				str = m_Conn.m_pRecordset->GetCollect(_T("��Ƭ���"));
				int num = _ttoi(str);
				patientslidenum.push_back(num);
			}

			m_Conn.m_pRecordset->MoveNext();
		}

		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return patientslidenum;
}

//������ɨ����Ϣ�����б� ��ѡ��
bool CChromosomeHandle::LoadCurrentToList()
{
	m_listchrohandle.DeleteAllItems();
	vector<ChroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//���ÿһ�ֲ��ҵ�ͼ

	for (size_t i = 0; i < tempAllPatients.size(); i++)
	{
		if (tempAllPatients[i].mode == _T("Ⱦɫ��"))
		{
			ChroTable patient;
			patient.PatientName = tempAllPatients[i].PatientName;
			//patient.MicroImgNames.push_back(iter);
			patient.ImgPath = tempAllPatients[i].path;
			patient.ResultPath = tempAllPatients[i].resultpath;
			AllPatients.push_back(patient);
		}

	}

	patientRow = 0;
	for (size_t k = 0; k < AllPatients.size(); k++)
	{
		int j = 0;
		CString  mystr;
		mystr.Format(_T("%d"), patientRow + 1);
		m_listchrohandle.InsertItem(patientRow, mystr);
		m_listchrohandle.SetItemText(patientRow, ++j, AllPatients[k].PatientName);
		mystr.Format(_T("%d"), AllPatients[k].ChroImgNames.size());
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("0/") + mystr;
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("��ǰɨ��");
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("δ����");
		m_listchrohandle.SetItemText(patientRow, ++j, mystr);
		patientRow++;



		//ѡ��
		int nItem, i;
		nItem = m_listchrohandle.GetItemCount();


		for (i = 0; i < nItem; i++)
		{
			m_listchrohandle.SetCheck(i);

		}
		GetDlgItem(IDC_BTN_CHROHANDLE)->EnableWindow(TRUE);//��listѡ��ѡ�к�ʹɾ����ť����

	}

	return true;
}

//��ʼ��Grid�ؼ�
//void CChromosomeHandle::GridCtrlInit()
//{
//	try{
//		m_pGridchro.SetRowCount(8);
//		m_pGridchro.SetColumnCount(21);
//		m_pGridchro.SetFixedRowCount(3);
//		m_pGridchro.SetFixedColumnCount(0);
//	}
//	catch (CMemoryException *e){
//		e->ReportError();
//		e->Delete();
//	}
//
//	for (int row = 0; row<m_pGridchro.GetRowCount(); row++) //��
//	for (int col = 0; col < m_pGridchro.GetColumnCount(); col++){    //��
//		GV_ITEM item;
//		item.mask = GVIF_TEXT | GVIF_FORMAT;
//		item.row = row;
//		item.col = col;
//	
//		if (row < 1){
//
//			item.nFormat = DT_LEFT | DT_WORDBREAK;
//			switch (col){
//			case 1:
//				item.szText.Format(_T("���"));
//				break;
//			case 2:
//				item.szText.Format(_T("����"));
//				break;
//			case 3:
//				item.szText.Format(_T("����ϸ������"));
//				break;
//			case 4:
//				item.szText.Format(_T("����ϸ����"));
//				break;
//			case 5:
//				item.szText.Format(_T("������"));
//				break;
//			case 6:
//				item.szText.Format(_T("Ⱦɫ�����"));
//				break;
//			case 16:
//				item.szText.Format(_T("�����ͻ���"));
//				break;
//			}//swtich����
//		}
//		else{ //else
//			item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//			item.szText.Format(_T(" "));
//
//		}//if����
//
//		if (row==1){
//			switch (col){
//			case 6:
//				item.szText.Format(_T("Cu"));
//				break;
//			case 11:
//				item.szText.Format(_T("Cs"));
//				break;
//			case 14:
//				item.szText.Format(_T("�ϼ�"));
//				break;
//			case 16:
//				item.szText.Format(_T("ctg"));
//				break;
//			case 17:
//				item.szText.Format(_T("csg"));
//				break;
//			case 18:
//				item.szText.Format(_T("ctb"));
//				break;
//			case 19:
//				item.szText.Format(_T("cte"));
//				break;
//			case 20:
//				item.szText.Format(_T("�ϼ�"));
//				break;
//
//			}
//		}
//
//		if (row == 1){
//			switch (col){
//			case 6:
//				item.szText.Format(_T("dic"));
//				break;
//			case 7:
//				item.szText.Format(_T("tri"));
//				break;
//			case 8:
//				item.szText.Format(_T("ten"));
//				break;
//			case 9:
//				item.szText.Format(_T("ace"));
//				break;
//			case 10:
//				item.szText.Format(_T("r"));
//				break;
//			case 11:
//				item.szText.Format(_T("t"));
//				break;
//			case 12:
//				item.szText.Format(_T("inv"));
//				break;
//			case 13:
//				item.szText.Format(_T("del"));
//				break;
//			case 14:
//				item.szText.Format(_T("����"));
//				break;
//			case 15:
//				item.szText.Format(_T("�ٷ���(%)"));
//				break;
//			case 20:
//				item.szText.Format(_T("����"));
//				break;
//			case 21:
//				item.szText.Format(_T("�ٷ���(%)"));
//				break;
//			}
//		}
//
//		m_pGridchro.SetItem(&item);
//
//	}
//
//	m_pGridchro.AutoSize();
//	m_pGridchro.SetRowHeight(0, 3 * m_pGridchro.GetRowHeight(0) / 2);
//
//	CDC *pDC = m_pGridchro.GetDC();
//	CSize  cellSize;
//	GV_ITEM item;
//	item.mask = GVIF_TEXT | GVIF_FORMAT;
//	item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
//	CRect tmpRect;
//	/*
//	����Ĵ����������ϲ���Ԫ���
//	*/
//
//	if (m_pGridchro.GetSafeHwnd()){
//		m_pGridchro.SetFixedCellCombine(0, 1, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 2, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 3, 2, 0);
//		m_pGridchro.SetFixedCellCombine(0, 4, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 10, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 16, 1, 5);
//		m_pGridchro.SetFixedCellCombine(0, 22, 1, 5);
//	}
//
//}


BOOL CChromosomeHandle::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO:  �ڴ����ר�ô����/����û���
	NMHEADER* pNMHeader = (NMHEADER*)lParam;
	if (((pNMHeader->hdr.code == HDN_BEGINTRACKW) |
		(pNMHeader->hdr.code == HDN_DIVIDERDBLCLICKW)))
	{
		*pResult = TRUE;
		return TRUE;
	}
	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CChromosomeHandle::OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_listchroresult.GetSelectionMark();
	CString name = m_listchroresult.GetItemText(row, 0);
	ShowOneResult(name);
	*pResult = 0;
}
