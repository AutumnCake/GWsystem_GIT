// SlideSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "SlideSet.h"
#include "afxdialogex.h"


using namespace::std;
// CSlideSet �Ի���

IMPLEMENT_DYNAMIC(CSlideSet, CDialogEx)

CSlideSet::CSlideSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSlideSet::IDD, pParent)
{

}

CSlideSet::~CSlideSet()
{
}

void CSlideSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_path);
	DDX_Control(pDX, IDC_LIST_SLIDE, m_slidelist);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_EDIT_RANGE, m_range);
	//DDX_Control(pDX, IDC_COMBO_EG, m_combo);
}


BEGIN_MESSAGE_MAP(CSlideSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_MODECHANGE, &CSlideSet::OnBnClickedBtnModechange)
	ON_BN_CLICKED(IDC_BTN_PATH, &CSlideSet::OnBnClickedBtnPath)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SLIDE, &CSlideSet::OnNMClickListSlide)
	ON_EN_KILLFOCUS(IDC_EDIT, &CSlideSet::OnEnKillfocusEdit)
	ON_BN_CLICKED(IDC_BTN_RANGE, &CSlideSet::OnBnClickedBtnRange)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SLIDE, &CSlideSet::OnNMDblclkListSlide)
	ON_BN_CLICKED(IDC_BTN_SELECTALL, &CSlideSet::OnBnClickedBtnSelectall)
	ON_BN_CLICKED(IDC_BTN_OK, &CSlideSet::OnBnClickedBtnOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EG, &CSlideSet::OnCbnSelchangeComboEg)
END_MESSAGE_MAP()


// CSlideSet ��Ϣ�������


BOOL CSlideSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�������ݿ�
	m_Conn.OnInitADOAccess();
	//�ı����е�����
	LOGFONT   logfont;//���Ū�����Ա,ȫ�ֱ���,��̬��Ա  
	CFont   *pfont1 = m_slidelist.GetFont();
	pfont1->GetLogFont(&logfont);
	logfont.lfHeight = logfont.lfHeight * 2;   //��������޸�����ĸ߱���
	logfont.lfWidth = logfont.lfWidth * 2;   //��������޸�����Ŀ����
	static   CFont   font1;
	font1.CreateFontIndirect(&logfont);
	m_slidelist.SetFont(&font1);
	font1.Detach();
	//�ı�༭������
	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(150, _T("������"));
	m_edit.SetFont(&font);//��������
	m_range.SetFont(&font);

	//����·��
	m_savepath = _T("C:\\Users\\wench\\Desktop");
	m_path.SetWindowText(m_savepath);

	//���ñ�ͷ
	m_slidelist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_slidelist.InsertColumn(0, _T("���"), LVCFMT_CENTER, 60, 0);
	m_slidelist.InsertColumn(1, _T("���"), LVCFMT_CENTER, 60, 0);
	m_slidelist.InsertColumn(2, _T("ѡ��Ƭ"), LVCFMT_CENTER, 120, 0);
	m_slidelist.InsertColumn(3, _T("��Ƭ����"), LVCFMT_CENTER, 160, 1);
	m_slidelist.InsertColumn(4, _T("ģʽ"), LVCFMT_CENTER, 90, 2);
	m_slidelist.InsertColumn(5, _T("ɨ�跶Χ��%��"), LVCFMT_CENTER, 180, 3);
	m_slidelist.InsertColumn(6, _T("��ע"), LVCFMT_CENTER, 330, 4);
	m_slidelist.DeleteColumn(0); //ɾ����0��

	//��������
	int j;
	for (size_t i = 1; i < ITEMLENGTH+1; i++)
	{
		j = 1;
		//������
		CString  mystr;
		mystr.Format(_T("%d"), i);
		m_slidelist.InsertItem(i-1, mystr);
		//������ƺ�ģʽ
		mystr.Format(_T("��Ƭ%d"), i);
		m_slidelist.SetItemText(i-1, ++j, mystr);
		mystr = _T("΢��");
		m_slidelist.SetItemText(i-1, ++j, mystr);
		mystr = _T("100");
		m_slidelist.SetItemText(i-1, ++j, mystr);
	}

	m_range.SetWindowText(_T("100"));
	m_edit.ShowWindow(SW_HIDE);

	//��ȡ���ݿ�����ݽ����޸�
	ReadFromAccess();

	haveccomboboxcreate = false;//��ʼ����־λ����ʾ��û�д��������б��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE

}

//�л�Ⱦɫ���΢��ģʽ
void CSlideSet::OnBnClickedBtnModechange()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString  mystr;
	mystr = m_slidelist.GetItemText(50, 3);
	if (mystr.Compare(_T("Ⱦɫ��")) == 0)
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			mystr = _T("΢��");
			m_slidelist.SetItemText(i, 3, mystr);
		}
	}
	else
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			mystr = _T("Ⱦɫ��");
			m_slidelist.SetItemText(i, 3, mystr);
		}
	}

}

//���·��
void CSlideSet::OnBnClickedBtnPath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR servPath[MAX_PATH];//��������ļ���·��
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = servPath;
	bi.lpszTitle = _T("ѡ������������ļ���·��");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl, servPath))) //�õ��ļ��е�ȫ·������Ҫ�Ļ���ֻ�ñ��ļ�����
		{

			m_path.SetWindowTextW(servPath);

		}
	}

}

//���ѡ�к�ȡ��ѡ��ĳһ��
void CSlideSet::OnNMClickListSlide(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int col = 0;
	int row = 0;
	row = pNMListView->iItem;
	col = pNMListView->iSubItem;
	if (col ==1)
	{
		POSITION ps;
		int nIndex;
		ps = m_slidelist.GetFirstSelectedItemPosition();
		nIndex = m_slidelist.GetNextSelectedItem(ps);
		//TODO:��Ӷ�ѡ�Ĳ�����
		if (nIndex == -1)
		{
			//m_list.SetItemState(-1,LVNI_SELECTED,LVNI_SELECTED);
		}
		m_slidelist.SetItemState(nIndex, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		CString mystr;
		mystr = m_slidelist.GetItemText(nIndex, 1);
		if (mystr.IsEmpty())
			m_slidelist.SetItemText(nIndex, 1, _T("��"));
		else
			m_slidelist.SetItemText(nIndex, 1, NULL);
	}
	else if (col == 2 || col == 4 || col == 5)
	{
		CRect rc;
		m_Row = pNMListView->iItem;//���ѡ�е���  
		m_Col = pNMListView->iSubItem;//���ѡ����  

		if (pNMListView->iSubItem != 0) //���ѡ���������;  
		{
			m_slidelist.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��  
			m_edit.SetParent(&m_slidelist);//ת������Ϊ�б���е�����  
			m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;  
			m_edit.SetWindowText(m_slidelist.GetItemText(m_Row, m_Col));//���������е�ֵ����Edit�ؼ��У�  
			m_edit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���  
			m_edit.SetFocus();//����Edit����  
			m_edit.ShowCaret();//��ʾ���  
			//m_edit.SetCursor(-1);//������ƶ������  
		}

	}
	else if (col == 3)
	{


		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pNMListView->iItem && e_SubItem == pNMListView->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�
			{
				distroyCcombobox(&m_slidelist, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pNMListView, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��
				m_comBox.AddString(L"΢��");
				m_comBox.AddString(L"Ⱦɫ��");
				m_comBox.ShowDropDown();//�Զ�����
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�
			{
				m_comBox.SetFocus();//����Ϊ����
			}
		}
		else
		{
			e_Item = pNMListView->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			e_SubItem = pNMListView->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			createCcombobox(pNMListView, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��
			m_comBox.AddString(L"΢��");
			m_comBox.AddString(L"Ⱦɫ��");
			m_comBox.ShowDropDown();//�Զ�����
		}


	}

	*pResult = 0;
}

//�޸�ĳ��ѡ�еĵ�Ԫ������
void CSlideSet::OnEnKillfocusEdit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString tem;
	m_edit.GetWindowText(tem);    //�õ��û�������µ�����  
	m_slidelist.SetItemText(m_Row, m_Col, tem);   //���ñ༭���������  
	m_edit.ShowWindow(SW_HIDE);                //Ӧ�ر༭�� 
}

//����ɨ�跶Χ
void CSlideSet::OnBnClickedBtnRange()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_range.GetWindowText(str);
	if (!str.IsEmpty())
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			m_slidelist.SetItemText(i, 4, str);
		}
	}

}

//˫��ѡ��ĳ����Ԫ��
void CSlideSet::OnNMDblclkListSlide(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

//ȫѡ
void CSlideSet::OnBnClickedBtnSelectall()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString mystr;
	mystr = m_slidelist.GetItemText(1, 1);
	if (mystr.IsEmpty())
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			m_slidelist.SetItemText(i, 1, _T("��"));
		}
		GetDlgItem(IDC_BTN_SELECTALL)->SetWindowText(_T("ȡ��ȫѡ"));
	}
	else
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			m_slidelist.SetItemText(i, 1, NULL);
		}
		GetDlgItem(IDC_BTN_SELECTALL)->SetWindowText(_T("ȫѡ"));
	}

}

//Ӧ��
void CSlideSet::OnBnClickedBtnOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SLIDE *myslide = new SLIDE;

	/***************1.����洢·��*********************/
	CString n_savepath;
	m_path.GetWindowText(n_savepath);
	/*cout << n_savepath << endl;*/
	if (n_savepath.IsEmpty())
	{
		AfxMessageBox(_T("��ѡ��洢·��"));
	}
	else
	{
		myslide->path = n_savepath;
	}
	/***************2.���沣Ƭ��Ϣ*********************/
	//�ҳ���ѡ�еĲ�Ƭ
	CString mystr;
	BOOL selectone = FALSE;
	int slidenum;
	for (size_t i = 0; i < ITEMLENGTH; i++)
	{
		mystr = m_slidelist.GetItemText(i, 1);
		if (mystr == _T("��"))
		{
			slidenum = _ttoi(m_slidelist.GetItemText(i, 0))-1;
			myslidebuild.name = m_slidelist.GetItemText(i, 2);
			myslidebuild.mode = m_slidelist.GetItemText(i, 3);
			myslidebuild.range = m_slidelist.GetItemText(i, 4);
			myslidebuild.remark = m_slidelist.GetItemText(i, 5);
			myslide->slidebuild.push_back(myslidebuild);
			myslide->All_Slide_ID.push_back(slidenum);
			selectone = TRUE;
		}
	}
	if (!selectone)
	{
		AfxMessageBox(_T("û�в�Ƭ��ѡ�У�"));
	}

	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//�޸������ڱ���
	HWND   hwnd = ::FindWindow(NULL, windowname);//������Ϣ������ˢ��ҳ��
	::SendMessage(hwnd, WM_MN_BTN_OK, (WPARAM)myslide, NULL);
	//����Ƭ������Ϣ�������ݿ�
	SaveToAccess(myslide);
	/*OnOK();*/
}


// ��Ƭ������Ϣ�������ݿ�
void CSlideSet::SaveToAccess(SLIDE * myslide)
{
	_bstr_t sql;
	_variant_t var;
	//ɾ��ԭ���ݱ�
	sql = _T("delete  * from ��Ƭ��Ϣ��");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
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
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		for (size_t i = 0; i < myslide->slidebuild.size(); i++)
		{
			m_Conn.m_pRecordset->AddNew();
			m_Conn.m_pRecordset->PutCollect(_T("·��"), _variant_t(myslide->path));
			m_Conn.m_pRecordset->PutCollect(_T("��������"), _variant_t(myslide->slidebuild[i].name));
			m_Conn.m_pRecordset->PutCollect(_T("ģʽ"), _variant_t(myslide->slidebuild[i].mode));
			m_Conn.m_pRecordset->PutCollect(_T("ɨ�跶Χ"), _variant_t(myslide->slidebuild[i].range));
			m_Conn.m_pRecordset->PutCollect(_T("��ע"), _variant_t(myslide->slidebuild[i].remark));
			m_Conn.m_pRecordset->PutCollect(_T("�Ƿ�ѡ��"), _variant_t(_T("��")));
			m_Conn.m_pRecordset->PutCollect(_T("��Ƭ���"), _variant_t(myslide->All_Slide_ID[i]));
			m_Conn.m_pRecordset->Update();
		}

		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}


// ��ȡ���ݿ�����ݽ����޸�
void CSlideSet::ReadFromAccess()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from ��Ƭ��Ϣ��");
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
		CString  ReadString = m_Conn.m_pRecordset->GetCollect(_T("·��"));
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(ReadString);
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString number = m_Conn.m_pRecordset->GetCollect(_T("��Ƭ���"));
			m_slidelist.SetItemText(_ttoi(number), 1, _T("��"));
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			m_slidelist.SetItemText(_ttoi(number), 2, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("ģʽ"));
			m_slidelist.SetItemText(_ttoi(number), 3, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("ɨ�跶Χ"));
			m_slidelist.SetItemText(_ttoi(number), 4, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("��ע"));

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
	}

}

void CSlideSet::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//������Ԫ�������б����
//pEditCtrlΪ�б����ָ�룬createccomboboxΪ�����б��ָ�����
//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	havecreat = true;
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 0, 0), this, IDC_COMBO_EG);
	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(150, _T("������"));
	createccomboboxobj->SetFont(&font);//��������,����������Ļ�������ֻ��ͻأ�ĸо�
	createccomboboxobj->SetParent(&m_slidelist);//��list control����Ϊ������,���ɵ�Ccombobox������ȷ��λ,���Ҳ����Ҫ
	CRect  EditRect;
	m_slidelist.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_slidelist.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������
	CString strItem = m_slidelist.GetItemText(e_Item, e_SubItem);//�����Ӧ��Ԫ���ַ�
	createccomboboxobj->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����
	createccomboboxobj->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����
	createccomboboxobj->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������
}

void CSlideSet::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowTextW(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//������Ӧ��Ԫ���ַ�
	distroyccomboboxobj->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��
}

void CSlideSet::OnCbnSelchangeComboEg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int index = m_comBox.GetCurSel();
	CString str;
	m_comBox.GetLBText(index, str);
	m_slidelist.SetItemText(e_Item, 3, str);
}


void CSlideSet::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���


}
