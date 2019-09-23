// MicroNucleusHandle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include"resource.h"
#include <thread>

#include "GWSystem1.0.h"
#include "GWSystem1.0Dlg.h"
#include "MicroNucleusHandle.h"
#include"MicroNucleusAlgorithm.h"
#include "MicroNucleusResult.h"
#include "MNPrintReport.h"
#include "ReadAndWriteForAccess.h"
#include "Scanning_Control.h"

#include "afxdialogex.h"
#include <stdlib.h>
#include <direct.h>
#include"FileOperate.h"
#include<io.h>
#include "CSeries.h"

#include "python.h"
#include "numpy\core\include\numpy\arrayobject.h"
//����
//#include "CApplication.h" //word�������   
//#include "CDocuments.h" //�ĵ�������   
//#include "CDocument0.h" //docx���� 
//#include "CBookmarks.h" //
//#include "CBookmark0.h" //
//#include "CRange.h" //�ĵ��е�һ��������Χ   


extern Scanning_Control *m_Scanning_Control;
extern CGWSystem10Dlg* pDlg;
extern vector <PatientWithSlide> tempAllPatients;
//ȫ�ֱ���
CMicroNucleusHandle * pHandleDlg;
CMicroNucleusResult * pMicroNucleusResult;
CReadAndWriteForAccess ReadAndWriteMN;
string StrFileSolve;
vector <CString> WaitingDeleteImg;
CString WaitingDeletePath;


// CMicroNucleusHandle �Ի���


IMPLEMENT_DYNAMIC(CMicroNucleusHandle, CDialogEx)

CMicroNucleusHandle::CMicroNucleusHandle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMicroNucleusHandle::IDD, pParent)
{
	pMicroNucleusResult = NULL;
}

CMicroNucleusHandle::~CMicroNucleusHandle()
{
	delete pMicroHandleDlg;
	AfxMessageBox(_T("��ģ̬�Ի���������"));
}

void CMicroNucleusHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listmicrohandle);
	DDX_Control(pDX, IDC_TCHARTMN, m_chartmicro);
	DDX_Control(pDX, IDC_DATESTARTMN, m_datetimebegin);
	DDX_Control(pDX, IDC_DATEENDMN, m_datetimeend);
	DDX_Control(pDX, IDC_LIST_RESULTCHRO, m_resultchrolist);
}


BEGIN_MESSAGE_MAP(CMicroNucleusHandle, CDialogEx)
	ON_MESSAGE(WM_HANDLEPATH, &CMicroNucleusHandle::InitialList1) //�Զ�����Ϣ��������
	ON_MESSAGE(WM_INIT_TABLEMN, &CMicroNucleusHandle::InitialList2) //�Զ�����Ϣ��������
	ON_BN_CLICKED(IDC_BTN_MICROHANDLE, &CMicroNucleusHandle::OnBnClickedBtnMicrohandle)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMicroNucleusHandle::OnNMClickList1)
	ON_BN_CLICKED(IDC_BTN_MICROSOLVE, &CMicroNucleusHandle::OnBnClickedBtnMicrosolve)
	ON_BN_CLICKED(IDC_BTN_MICROPRINT, &CMicroNucleusHandle::OnBnClickedBtnMicroprint)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMicroNucleusHandle::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMicroNucleusHandle::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BTN_FINDMN, &CMicroNucleusHandle::OnBnClickedBtnFindmn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMicroNucleusHandle::OnNMCustomdrawList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULTCHRO, &CMicroNucleusHandle::OnNMClickListResultchro)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMicroNucleusHandle ��Ϣ�������

BOOL CMicroNucleusHandle::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	pHandleDlg = this;//ָ��öԻ����ȫ��ָ��
	//�������ݿ�
	m_Conn.OnInitADOAccess();
	GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���
	GetDlgItem(IDC_BTN_MICROSOLVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MICROPRINT)->EnableWindow(FALSE);

	//ɾ��ԭ���ݱ�
	DeleteAccessTable();
	//���ñ�ͷ
	CRect rectCtrl;
	CStatic *p = (CStatic*)GetDlgItem(IDC_LIST1);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_listmicrohandle.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listmicrohandle.InsertColumn(0, _T("�Ƿ����"), LVCFMT_CENTER, 80, 0);
	m_listmicrohandle.InsertColumn(1, _T("���"), LVCFMT_CENTER, rectCtrl.Width()*0.107, 0);
	m_listmicrohandle.InsertColumn(2, _T("��������"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(3, _T("ͼƬ����"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(4, _T("��������"), LVCFMT_CENTER, rectCtrl.Width()*0.143);
	m_listmicrohandle.InsertColumn(5, _T("��ͼʱ��"), LVCFMT_CENTER, rectCtrl.Width()*0.25);
	m_listmicrohandle.InsertColumn(6, _T("�����������"), LVCFMT_CENTER, rectCtrl.Width()*0.214);
	//m_listmicrohandle.InsertColumn(7, _T("�����������"), LVCFMT_CENTER, 100, 3);
	m_listmicrohandle.DeleteColumn(0); //ɾ����0��

	//���ý�����ͷ
	p = (CStatic*)GetDlgItem(IDC_LIST_RESULTCHRO);
	p->GetWindowRect(rectCtrl);
	this->ScreenToClient(rectCtrl);

	m_resultchrolist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_resultchrolist.InsertColumn(0, _T("�Ƿ����"), LVCFMT_CENTER, 80, 0);
	m_resultchrolist.InsertColumn(1, _T("����"), LVCFMT_CENTER, rectCtrl.Width()*0.091, 0);
	m_resultchrolist.InsertColumn(2, _T("����ϸ����"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(3, _T("����MNC(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(4, _T("����MNC��(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(5, _T("����MN(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(6, _T("����MN��(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);

	m_resultchrolist.InsertColumn(7, _T("˫��ϸ����"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(8, _T("˫��MNC(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(9, _T("˫��MNC��(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(10, _T("˫��MN(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.091);
	m_resultchrolist.InsertColumn(11, _T("˫��MN��(��)"), LVCFMT_CENTER, rectCtrl.Width()*0.09);


	m_resultchrolist.DeleteColumn(0); //ɾ����0��

	//�����иߣ�
	CImageList   m_l;
	m_l.Create(1, 18, TRUE | ILC_COLOR32, 1, 0);
	m_listmicrohandle.SetImageList(&m_l, LVSIL_SMALL);
	m_resultchrolist.SetImageList(&m_l, LVSIL_SMALL);

	//�������壺
	CFont m_font;
	m_font.CreatePointFont(120, _T("������"));
	m_listmicrohandle.SetFont(&m_font);
	//m_resultchrolist.SetFont(&m_font);
	GetDlgItem(IDC_LISTHEADMN)->SetFont(&m_font);
	//��ȡ·��
	//m_handlepath.GetWindowTextW(ReadPath);
	//m_resultpath.GetWindowTextW(WritePath);

	//����ʱ��ؼ��ĸ�ʽ
	m_datetimebegin.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	m_datetimeend.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	COleDateTime MinTime(COleDateTime::GetCurrentTime() - COleDateTimeSpan(30, 0, 0, 0));
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	m_datetimebegin.SetRange(&MinTime, &MaxTime);
	m_datetimeend.SetRange(&MinTime, &MaxTime);

	//�������������� ������
	if (pMicroNucleusResult == NULL){//�ж��Ի����Ƿ�����ָ��
		pMicroNucleusResult = new CMicroNucleusResult(this);//ָ��һ����ģ�Ի���ʾ��
		pMicroNucleusResult->Create(IDD_MICRONUCLEUSRESULT, this);//����
	}
	pMicroNucleusResult->ShowWindow(SW_HIDE);//����ʾ
	pMicroNucleusResult->m_comboxpatient.ResetContent();


	//������ɨ����Ϣ�����б� ��ѡ��
	LoadCurrentToList();

	CRect rect;
	GetClientRect(&rect); //ȡ�ͻ�����С
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

//�Զ�����Ϣ����1----���ݴ����ļ���·���봢���ļ���·��
LRESULT CMicroNucleusHandle::InitialList1(WPARAM wParam, LPARAM lParam)
{
	MyFiles *pA = (MyFiles*)wParam;
	CString  str;
	//str = pA->picsolve.c_str();
	//m_handlepath.SetWindowText(str);
	//str = pA->endsolve.c_str();
	//m_resultpath.SetWindowText(str);
	return 0;

	//һ�����и�����ֵ

}

//�Զ�����Ϣ����2----���ݴ����ļ���·���봢���ļ���·��
LRESULT CMicroNucleusHandle::InitialList2(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString mystr(b);
	SysFreeString(b);

	pHandleDlg->m_listmicrohandle.SetItemText(lParam, 3, mystr);
	pHandleDlg->UpdateData(FALSE);

	return 0;//һ�����и�����ֵ

}

//��ʼ����
void CMicroNucleusHandle::OnBnClickedBtnMicrohandle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ձ��η��������ݿ�
	GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���

	//1.��ȡ��ѡ�еĲ������飬����AllPatientsChose
	int nItem, i;
	nItem = m_listmicrohandle.GetItemCount();
	for (i = 0; i < nItem; ++i)
	{
		if (m_listmicrohandle.GetCheck(i))
		{
			AllPatientsChose.push_back(AllPatients[i]);
			Row.push_back(i);
		}
	}

	//2.��ʼ����
	thread MNAnalyse(ThreadProcWaitMN);
	MNAnalyse.detach();
	//ThreadWait = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcWaitMN, NULL, 0, &ThreadID);
}

void  CMicroNucleusHandle::ThreadProcWaitMN()
{

	// ��ʼ��Python
	//��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
	//���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
	//����ģ������·���С��������û�з���ֵ�����ϵͳ
	//�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��
	//Py_Initialize();
	//if (!Py_IsInitialized())
	//{
	//	return;
	//}

	//PyObject * pModule = NULL;
	//PyObject * pFunc = NULL;
	//
	////PyRun_SimpleString("import sys");
	////PyRun_SimpleString("sys.path.append('./')");

	//PyObject* pName = PyUnicode_FromString("test0719");
	//pModule = PyImport_Import(pName);
	//cout << "test0719:" << pModule;
	//pFunc = PyObject_GetAttrString(pModule, "classifier_pridictor_keras");
	//if (!pFunc || !PyCallable_Check(pFunc))
	//{
	//	printf("can't find function [classifier_pridictor]");
	//	getchar();
	//}
	//else
	//{
	//	cout << "open function [classifier_pridictor]" << endl;
	//}



	//init_numpy();
	// ����ʼ���Ƿ�ɹ�


	//��ʾpython�汾��Ϣ
	//cout << (Py_GetVersion()) << "\n";
	//PyObject* sys = PyImport_ImportModule("sys");
	//PyRun_SimpleString("import  sys");

	//PyRun_SimpleString("print('come in python')");
	//PyRun_SimpleString("import  sys"); // ִ�� python �еĶ����  
	//PyRun_SimpleString("sys.path.append('./')");
	//PyRun_SimpleString("import cv2");

	////PyRun_SimpleString("import  sys"); // ִ�� python �еĶ����  
	////PyRun_SimpleString("sys.path.append('./')");
	////PyRun_SimpleString("import cv2");

	//// load python script
	//PyObject* pName = PyUnicode_FromString("predictSX");
	//PyObject* pModule = PyImport_Import(pName);
	//if (!pModule) {
	//	//std::cout << "can't find predict.py" << std::endl;
	//	//getchar();
	//	return;
	//}
	//else
	//{
	//	//cout << "open Module" << endl;
	//}
	//PyObject* pDict = PyModule_GetDict(pModule);
	//if (!pDict)
	//{
	//	return;
	//}
	//pFunc = PyDict_GetItemString(pDict, "classifier_pridictor");
	//if (!pFunc || !PyCallable_Check(pFunc))
	//{
	//	//printf("can't find function [classifier_pridictor]");
	//	//getchar();
	//	return;
	//}
	//else
	//{
	//	//cout << "open function [classifier_pridictor]" << endl;
	//}

	//���ɴ���ʱ��

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
		int row = pHandleDlg->Row[i];
		pHandleDlg->patientNewImg = true;
		//(1)���ɸò��˵Ĵ������ļ���
		CString name = pHandleDlg->AllPatientsChose[i].PatientName; //����ʱ���ʽ
		pHandleDlg->StrName = pHandleDlg->AllPatientsChose[i].ResultPath + name + "-" + pHandleDlg->StrTime;
		string strname = CT2A(pHandleDlg->StrName.GetBuffer());
		StrFileSolve = pHandleDlg->strFileSolve + strname + "\\";//���ɽ�����ļ��� 
		const char* c_filesolve = StrFileSolve.c_str();//��·��ת���� const char
		_mkdir(c_filesolve);

		//(2)��ʼ�Ըò��˵�ͼƬ���з���
		CMicroNucleusAlgorithm algorithm;
		CString mystr;
		mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].MicroImgNames.size());
		MN_HandleResult *pB = new MN_HandleResult; //�ò��˷�������洢�ṹ��
		memset(pB->SingleCellsWithMN, 0, sizeof(pB->SingleCellsWithMN));
		memset(pB->DoubleCellsWithMN, 0, sizeof(pB->DoubleCellsWithMN));
		memset(pB->MultiCellsWithMN, 0, sizeof(pB->MultiCellsWithMN));
		memset(pB->TripleCellsWithMN, 0, sizeof(pB->TripleCellsWithMN));

		PatientAndQuaImg thistime;
		thistime.patientname = name;
		USES_CONVERSION;
		thistime.QuestionImgPath = A2W((StrFileSolve).c_str());

		pMicroNucleusResult->m_comboxpatient.AddString(name);
		//pMicroNucleusResult->AllQuestionImgPath.push_back(thistime);
		pMicroNucleusResult->PatientNames.push_back(name);

		size_t newStart = 0;
		int deletedPicNum = 0;

		while (pHandleDlg->patientNewImg)//�жϸò����Ƿ����µ�ͼƬ����
		{
			for (size_t j = newStart; j < pHandleDlg->AllPatientsChose[i].MicroImgNames.size(); j++)
			{
				string readpath = CT2A(pHandleDlg->AllPatientsChose[i].ImgPath.GetBuffer());
				string imgname = CT2A(pHandleDlg->AllPatientsChose[i].MicroImgNames[j].GetBuffer());
				

				string picpath = readpath + "\\" + imgname;
				Mat image = imread(picpath);//37 38 40 43 44 45
				if (image.empty())
				{
					j--;
					Sleep(100);
					continue;
				}

				//��������ļ��ѷŵ��ļ���
				pHandleDlg->strFileProcess = StrFileSolve + "�����ļ�\\";
				const char* c_fileprocess = pHandleDlg->strFileProcess.c_str();//��·��ת���� const char
				_mkdir(c_fileprocess);

				pHandleDlg->AllPatientsChose[i].Result = algorithm.handlemicronucleus(readpath, imgname, StrFileSolve, pHandleDlg->AllPatientsChose[i].PatientName, pB, pHandleDlg->strFileProcess);
				deletedPicNum = pHandleDlg->AllPatientsChose[i].Result->delPictureNum;
				pHandleDlg->AllPatientsChose[i].Result->patientname = pHandleDlg->AllPatientsChose[i].PatientName;
				pHandleDlg->AllPatientsChose[i].Result->picturesum = pHandleDlg->AllPatientsChose[i].MicroImgNames.size() + 1;
				pHandleDlg->AllPatientsChose[i].Result->sourcefile = pHandleDlg->AllPatientsChose[i].ImgPath;
				pHandleDlg->AllPatientsChose[i].Result->resultfile = pHandleDlg->AllPatientsChose[i].ResultPath;

				//ɾ����������ļ��ѷŵ��ļ���
				CFileOperate x;
				/*x.DeleteFolder(A2W((pHandleDlg->strFileProcess).c_str()));*/

				CString mystr1, mystr2;
				mystr1.Format(_T("%d"), j + 1);
				mystr2 = mystr1 + _T("/") + mystr;

				HWND   hwnd = ::FindWindow(NULL, _T("΢�˴���"));//������Ϣ������ˢ��ҳ��
				::SendMessage(hwnd, WM_INIT_TABLEMN, (WPARAM)mystr2.AllocSysString(), (WPARAM)row);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ
			}

			//��ѯ�ò��˵�ͼƬ�Ƿ�������
			vector<CString>patientNewImgPath = pHandleDlg->InquiryPatientImg(pHandleDlg->AllPatientsChose[i].PatientName, pHandleDlg->AllPatientsChose[i].ImgPath);
			int patientNewImgNum = patientNewImgPath.size();
			if (patientNewImgNum > pHandleDlg->AllPatientsChose[i].MicroImgNames.size())
			{
				pHandleDlg->patientNewImg = true;
				newStart = pHandleDlg->AllPatientsChose[i].MicroImgNames.size();
				pHandleDlg->AllPatientsChose[i].MicroImgNames = patientNewImgPath;
				mystr.Format(_T("%d"), pHandleDlg->AllPatientsChose[i].MicroImgNames.size());
			}
			else
			{
				//�ļ�����ͼƬ�������䣬���ͼƬ�ļ����Ƿ��ǵ�ǰɨ��  ����ǣ����Ƿ��Ѿ�ɨ�����
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


				//�жϵ�ǰ�������˵��ļ����Ƿ��ɨ���ļ�����ͬ

			}
		}

		if (pHandleDlg->AllPatientsChose[i].Result->patientname != _T("Ctrl+"))
		{
			//�����߳�ɾ���������ͼƬ
			//WaitingDeletePath = pHandleDlg->AllPatientsChose[i].ImgPath;
			//WaitingDeleteImg = pHandleDlg->AllPatientsChose[i].MicroImgNames;
			//thread DelImage(DeltePatientImage);
			//DelImage.detach();

			//��������Ҫ�Ĳ���
			pHandleDlg->CalculateMNParaInExcel(pB);

			//����������������ݿ���
			ReadAndWriteMN.SaveToAccessMNAnalysis(pHandleDlg->AllPatientsChose[i].Result);

			//���ݿ��У����Ƿ��Ѿ�������Ϊ��

			//������иĳɱ���������
			CString mystr3 = _T("��");
			pHandleDlg->m_listmicrohandle.SetItemText(row, 5, mystr3);
			//�������ɸò��˵ı���
			pHandleDlg->MNJoinWaitToPrint(pHandleDlg->AllPatientsChose[i].Result->patientname, StrFileSolve);
			//ͼ����ʾ�ò��˵ķ������
			pHandleDlg->ShowOneResult(pHandleDlg->AllPatientsChose[i].Result->patientname);
			//������ʾ�ò��˵ķ������
			pHandleDlg->ShowOneResultInTable(pHandleDlg->AllPatientsChose[i].Result->patientname);

			pHandleDlg->ResultRow++;

			pHandleDlg->GetDlgItem(IDC_BTN_MICROPRINT)->EnableWindow(TRUE);
		}


	}
	pHandleDlg->GetDlgItem(IDC_BTN_MICROSOLVE)->EnableWindow(TRUE);


}

////�����߳�
//DWORD WINAPI ThreadProcWaitMN(LPVOID pParam)
//{
//	//4.΢�˷�����ʼ
//
//
//	return 0;
//
//}




// ��һ�����˵ķ��������ʾ��ͼ��
bool CMicroNucleusHandle::ShowOneResult(CString patientname)
{
	MN_HandleResult pA;
	pA = ReadAndWriteMN.ReadOneMNResultFromAccess(patientname);

	ClearAllSeries();
	CSeries barSeries = (CSeries)m_chartmicro.Series(0);
	barSeries.AddXY((double)2, (double)pA.singlecell, _T("����ϸ����_У��"), RGB(255, 0, 255));
	barSeries.AddXY((double)3, (double)pA.doublecell, _T("˫��ϸ����_У��"), RGB(0, 255, 255));
	barSeries.AddXY((double)4, (double)pA.multiplecell, _T("���ϸ����_У��"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.singleMN, _T("����MN_У��"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.DoubleMN, _T("˫��MN_У��"), RGB(0, 255, 255));
	barSeries.AddXY((double)7, pA.MultiMN, _T("���MN_У��"), RGB(0, 255, 0));
	barSeries.AddXY((double)5, pA.singlecell_wh, _T("����MNC_У��"), RGB(255, 0, 255));
	barSeries.AddXY((double)6, pA.doublecell_wh, _T("˫��MNC_У��"), RGB(0, 255, 255));
	barSeries.AddXY((double)7, pA.multiplecell_wh, _T("���MNC_У��"), RGB(0, 255, 0));

	//UpdateData(false);
	return false;
}

// ��һ�����˵ķ��������ʾ������
bool CMicroNucleusHandle::ShowOneResultInTable(CString patientname)
{
	MN_HandleResult pA;
	pA = ReadAndWriteMN.ReadOneMNResultFromAccess(patientname);

	int j = 0;
	CString  mystr;
	mystr = pA.patientname;
	m_resultchrolist.InsertItem(ResultRow, mystr);
	mystr.Format(_T("%d"), pA.singlecell);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.SingleMNC_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.SingleMNC_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.SingleMN_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.SingleMN_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);

	mystr.Format(_T("%d"), pA.doublecell);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%d"), pA.DoubleMNC_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.DoubleMNC_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr );
	mystr.Format(_T("%d"), pA.DoubleMN_Num);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);
	mystr.Format(_T("%.2f"), pA.DoubleMN_Rate);
	m_resultchrolist.SetItemText(ResultRow, ++j, mystr);

	//UpdateData(false);
	return false;
}



//���ͼ��
void CMicroNucleusHandle::ClearAllSeries(void)
{
	for (long i = 0; i<m_chartmicro.get_SeriesCount(); i++)
	{
		((CSeries)m_chartmicro.Series(i)).Clear();
	}

}




//�����б��¼�
void CMicroNucleusHandle::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	*pResult = 0;
}

//�鿴������
void CMicroNucleusHandle::OnBnClickedBtnMicrosolve()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	pMicroNucleusResult->ShowWindow(SW_SHOW);//����ʾ

}

//������ݿ��е�һЩ���
void CMicroNucleusHandle::DeleteAccessTable()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("delete  * from ΢��ͼ�����������ݱ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	sql = _T("delete  * from ΢��ͼ�����������ݱ���ɾ����");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	//ɾ��ԭ���ݱ�
	sql = _T("delete  * from ΢�˷���������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

//��ӡ���η����ı���
void CMicroNucleusHandle::OnBnClickedBtnMicroprint()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMNPrintReport *pMNPrintReport = new CMNPrintReport;
	pMNPrintReport->Create(IDD_PRINTREPORTMN, this);
	//����ǰ�ɴ�ӡ�Ĳ��˴�����ӡ�����Գ�ʼ��
	HWND   hwnd = ::FindWindow(NULL, _T("��ӡ�������棨΢�ˣ�"));//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_WAITTOPRINTMN, (WPARAM)&MNWaitPrint, NULL);//�߳��д��ݶ�ʱ����Ϣ���Կ�����ʱ����ˢ����ʾ��Ƭ

	pMNPrintReport->ShowWindow(SW_SHOW);

}

// ��������ɱ���Ľṹ��
void CMicroNucleusHandle::MNJoinWaitToPrint(CString patientname, string savepath)
{
	WaitToPrint mnWaitToPrint;
	mnWaitToPrint.patientname = patientname;
	mnWaitToPrint.savepath = savepath;
	MNWaitPrint.push_back(mnWaitToPrint);
}


//ĳ�����Ѿ������仯
void CMicroNucleusHandle::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nItem, i;
	nItem = m_listmicrohandle.GetItemCount();

	bool mark = false;
	for (i = 0; i < nItem && !mark; ++i)
	{
		if (m_listmicrohandle.GetCheck(i))
		{
			mark = true;
		}
	}
	if (mark)
	{
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(TRUE);//��listѡ��ѡ�к�ʹɾ����ť����
	}
	else
	{
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���
	}

	*pResult = 0;
}

//˫���б��¼�
void CMicroNucleusHandle::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_listmicrohandle.GetSelectionMark();
	CString name = m_listmicrohandle.GetItemText(row, 1);
	ShowOneResult(name);

	*pResult = 0;
}

//��ѯʱ����ڵ�ͼƬ
void CMicroNucleusHandle::OnBnClickedBtnFindmn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_FINDMN)->EnableWindow(FALSE);//��lisrѡ��ѡ��ʹɾ����ť���

	//�Ƿ��б���ɨ���ͼƬ ����� ����
	LoadCurrentToList();


	//1.�����Ӧʱ����ڵĲ�ƬͼƬ
	COleDateTime TimeBegin, TimeEnd;
	m_datetimebegin.GetTime(TimeBegin);
	m_datetimeend.GetTime(TimeEnd);
	CReadAndWriteForAccess xx;
	vector<Table_SlideSet> ReadSlideset;
	ReadSlideset = xx.QuerySaveTimeMN(TimeBegin, TimeEnd);

	//2.����ÿ�Ų�Ƭ��·�����ϲ���ͬ·�����õ�·����������
	vector<CString>AllMNImgPath;
	vector<CString>AllMNResPath;
	vector<COleDateTime>AllMNTime;
	for (auto iter : ReadSlideset)
	{
		vector<CString>::iterator ret;
		ret = std::find(AllMNImgPath.begin(), AllMNImgPath.end(), iter.path);
		if (ret == AllMNImgPath.end())
		{
			AllMNImgPath.push_back(iter.path);
			AllMNResPath.push_back(iter.resultpath);
			AllMNTime.push_back(iter.time);
		}
	}

	//3.��·���������������·���µĲ���ͼƬ���й��࣬��ʾ�����
	for (size_t pathNum_i = 0; pathNum_i < AllMNImgPath.size(); pathNum_i++)
	{
		int picNum = 0;
		vector<CString> vecEmpty;
		AllMicroImg.swap(vecEmpty);//���ÿһ�ֲ��ҵ�ͼ

		//��1��ɸѡ���в��˵�΢��ͼƬ��������ṹ��
		USES_CONVERSION;
		m_Scanning_Control->critical_section.Lock();
		char* charReadPath = T2A(AllMNImgPath[pathNum_i]);
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
				if (str1 == _T("΢��"))
					AllMicroImg.push_back(OneImgName);
			} while (FindNextFile(handle, &file));
		}
		m_Scanning_Control->critical_section.Unlock();
		FindClose(handle);

		//(2)��������������ͼƬ�Ķ�Ӧ�ṹ��
		BOOL havepatientornot;
		CString strDst;
		vector <MicroTable> tempMicroAllPatients;

		for (auto iter : AllMicroImg)
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
			for (auto iterpatient : tempMicroAllPatients)
			{
				if (iterpatient.PatientName == strDst)
				{
					havepatientornot = TRUE;
					tempMicroAllPatients[picNum].MicroImgNames.push_back(iter);
				}
				picNum++;
			}
			if (!havepatientornot)//���û�иò���
			{
				MicroTable patient;
				patient.PatientName = strDst;
				patient.MicroImgNames.push_back(iter);
				patient.ImgPath = AllMNImgPath[pathNum_i];
				patient.ResultPath = AllMNResPath[pathNum_i];
				tempMicroAllPatients.push_back(patient);
			}

		}

		//��3��������������ͼƬ������ʾ�����

		for (size_t k = 0; k < tempMicroAllPatients.size(); k++)
		{
			int j = 0;
			CString  mystr;
			mystr.Format(_T("%d"), patientRow + 1);
			m_listmicrohandle.InsertItem(patientRow, mystr);
			m_listmicrohandle.SetItemText(patientRow, ++j, tempMicroAllPatients[k].PatientName);
			mystr.Format(_T("%d"), tempMicroAllPatients[k].MicroImgNames.size());
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("0/") + mystr;
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = AllMNTime[pathNum_i].Format();
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			mystr = _T("δ����");
			m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
			patientRow++;
			AllPatients.push_back(tempMicroAllPatients[k]);
		}



	}


	UpdateData(FALSE);


}


void CMicroNucleusHandle::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMicroNucleusHandle::DrawText(int nItem,
	int nSubItem,
	CDC *pDC,
	COLORREF crText,
	COLORREF crBkgnd,
	CRect &rect)
{
	ASSERT(pDC);
	pDC->FillSolidRect(&rect, crBkgnd);

	CListCtrl x;
	int nProcess = x.GetItemData(nItem);
	CRect procRect = rect;
	pDC->Rectangle(procRect);

	procRect.left += 1;
	procRect.bottom -= 1;
	procRect.top += 1;
	procRect.right = procRect.left + rect.Width() * nProcess / 100;
	CBrush brush(RGB(255, 0, 0));
	pDC->FillRect(&procRect, &brush);

	CString str;
	str.Format(_T("%d%%"), nProcess);

	if (!str.IsEmpty())
	{
		UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_CENTER;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, nFormat);
	}
}

// ����ָ���ļ����²���ͼƬ������
vector <CString> CMicroNucleusHandle::InquiryPatientImg(CString patientname, CString imgpath)
{
	vector <CString>().swap(AllMicroImg);
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
			if (str1 == _T("΢��"))
				AllMicroImg.push_back(OneImgName);
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);

	//(2)��������������ͼƬ�Ķ�Ӧ�ṹ��
	CString strDst;
	MicroTable patient;

	for (auto iter : AllMicroImg)
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
			patient.MicroImgNames.push_back(iter);
		}

	}


	return patient.MicroImgNames;
}


// �����ݿ��л�ȡ���иò�������Ӧ�Ĳ�Ƭ��
vector<int> CMicroNucleusHandle::GetPatientSlideNumFromAccess(CString patientname)
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

//��������Ҫ�Ĳ���
void CMicroNucleusHandle::CalculateMNParaInExcel(MN_HandleResult *pB)
{
	int SingleMNC_Num = 0;
	float SingleMNC_Rate = 0;
	int SingleMN_Num = 0;
	float SingleMN_Rate = 0;

	int DoubleMNC_Num = 0;
	float DoubleMNC_Rate = 0;
	int DoubleMN_Num = 0;
	float DoubleMN_Rate = 0;

	for (size_t i = 0; i < sizeof(pB->SingleCellsWithMN)/4; i++)
	{
		pB->singleMN += i*pB->SingleCellsWithMN[i];
	}
	pB->SingleMNC_Num = pB->singlecell_wh;
	pB->SingleMN_Num = pB->singleMN;
	if (pB->singlecell == 0)
	{
		pB->SingleMNC_Rate = 0;
		pB->SingleMN_Rate = 0;
	}
	else{
		pB->SingleMNC_Rate = ((float)pB->SingleMNC_Num / pB->singlecell)*1000;
		pB->SingleMN_Rate = ((float)pB->SingleMN_Num / pB->singlecell)*1000;
	}

	for (size_t i = 0; i < sizeof(pB->DoubleCellsWithMN)/4; i++)
	{
		pB->DoubleMN += i*pB->DoubleCellsWithMN[i];
	}
	pB->DoubleMNC_Num = pB->doublecell_wh;
	pB->DoubleMN_Num = pB->DoubleMN;

	if (pB->doublecell == 0)
	{
		pB->DoubleMNC_Rate = 0;
		pB->DoubleMN_Rate = 0;
	}
	else{
		pB->DoubleMNC_Rate = ((float)pB->DoubleMNC_Num / pB->doublecell)*1000;
		pB->DoubleMN_Rate = ((float)pB->DoubleMN_Num / pB->doublecell)*1000;
	}




}

//������ɨ����Ϣ�����б� ��ѡ��
bool CMicroNucleusHandle::LoadCurrentToList()
{
	m_listmicrohandle.DeleteAllItems();
	vector<MicroTable> vecEmpty1;
	AllPatients.swap(vecEmpty1);//���ÿһ�ֲ��ҵ�ͼ

	for (size_t i = 0; i < tempAllPatients.size(); i++)
	{
		if (tempAllPatients[i].mode == _T("΢��"))
		{
			MicroTable patient;
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
		m_listmicrohandle.InsertItem(patientRow, mystr);
		m_listmicrohandle.SetItemText(patientRow, ++j, AllPatients[k].PatientName);
		mystr.Format(_T("%d"), AllPatients[k].MicroImgNames.size());
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("0/") + mystr;
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("��ǰɨ��");
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		mystr = _T("δ����");
		m_listmicrohandle.SetItemText(patientRow, ++j, mystr);
		patientRow++;



		//ѡ��
		int nItem, i;
		nItem = m_listmicrohandle.GetItemCount();


		for (i = 0; i < nItem; i++)
		{
			m_listmicrohandle.SetCheck(i);

		}
		GetDlgItem(IDC_BTN_MICROHANDLE)->EnableWindow(TRUE);//��listѡ��ѡ�к�ʹɾ����ť����

	}

	return true;
}

//�����л����
void CMicroNucleusHandle::OnNMClickListResultchro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int row = m_resultchrolist.GetSelectionMark();
	CString name = m_resultchrolist.GetItemText(row, 0);
	ShowOneResult(name);
	*pResult = 0;
}


void CMicroNucleusHandle::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();
	}
	// TODO:  �ڴ˴������Ϣ����������
}

void CMicroNucleusHandle::ReSize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;
	GetClientRect(&recta); //ȡ�ͻ�����С
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�
	CPoint OldBRPoint, BRPoint; //���½�
	HWND hwndChild = ::GetWindow(m_hWnd, GW_CHILD); //�г����пؼ�
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;

}

BOOL CMicroNucleusHandle::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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

void CMicroNucleusHandle::DeltePatientImage()
{
	for (size_t i = 0; i < WaitingDeleteImg.size(); i++)
	{
		char sourcePicPath[MAX_PATH + 1];
		WCHAR   wstr_sourpic[MAX_PATH];
		String Path = CT2A((WaitingDeletePath + WaitingDeleteImg[i]).GetBuffer());
		sprintf_s(sourcePicPath, "%s", Path.c_str());
		MultiByteToWideChar(CP_ACP, 0, sourcePicPath, -1, wstr_sourpic, sizeof(wstr_sourpic));
		int del = DeleteFile(wstr_sourpic);

		if (!del)
			AfxMessageBox(_T("ɾ��ʧ��"));

	}

}
