// SlideSet.cpp : 实现文件
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "SlideSet.h"
#include "afxdialogex.h"


using namespace::std;
// CSlideSet 对话框

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


// CSlideSet 消息处理程序


BOOL CSlideSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//连接数据库
	m_Conn.OnInitADOAccess();
	//改变表格中的字体
	LOGFONT   logfont;//最好弄成类成员,全局变量,静态成员  
	CFont   *pfont1 = m_slidelist.GetFont();
	pfont1->GetLogFont(&logfont);
	logfont.lfHeight = logfont.lfHeight * 2;   //这里可以修改字体的高比例
	logfont.lfWidth = logfont.lfWidth * 2;   //这里可以修改字体的宽比例
	static   CFont   font1;
	font1.CreateFontIndirect(&logfont);
	m_slidelist.SetFont(&font1);
	font1.Detach();
	//改变编辑框字体
	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(150, _T("新宋体"));
	m_edit.SetFont(&font);//设置字体
	m_range.SetFont(&font);

	//设置路径
	m_savepath = _T("C:\\Users\\wench\\Desktop");
	m_path.SetWindowText(m_savepath);

	//设置表头
	m_slidelist.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES);
	m_slidelist.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 60, 0);
	m_slidelist.InsertColumn(1, _T("序号"), LVCFMT_CENTER, 60, 0);
	m_slidelist.InsertColumn(2, _T("选择玻片"), LVCFMT_CENTER, 120, 0);
	m_slidelist.InsertColumn(3, _T("玻片名称"), LVCFMT_CENTER, 160, 1);
	m_slidelist.InsertColumn(4, _T("模式"), LVCFMT_CENTER, 90, 2);
	m_slidelist.InsertColumn(5, _T("扫描范围（%）"), LVCFMT_CENTER, 180, 3);
	m_slidelist.InsertColumn(6, _T("备注"), LVCFMT_CENTER, 330, 4);
	m_slidelist.DeleteColumn(0); //删除第0列

	//设置正文
	int j;
	for (size_t i = 1; i < ITEMLENGTH+1; i++)
	{
		j = 1;
		//添加序号
		CString  mystr;
		mystr.Format(_T("%d"), i);
		m_slidelist.InsertItem(i-1, mystr);
		//添加名称和模式
		mystr.Format(_T("玻片%d"), i);
		m_slidelist.SetItemText(i-1, ++j, mystr);
		mystr = _T("微核");
		m_slidelist.SetItemText(i-1, ++j, mystr);
		mystr = _T("100");
		m_slidelist.SetItemText(i-1, ++j, mystr);
	}

	m_range.SetWindowText(_T("100"));
	m_edit.ShowWindow(SW_HIDE);

	//读取数据库的内容进行修改
	ReadFromAccess();

	haveccomboboxcreate = false;//初始化标志位，表示还没有创建下拉列表框

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE

}

//切换染色体和微核模式
void CSlideSet::OnBnClickedBtnModechange()
{
	// TODO:  在此添加控件通知处理程序代码
	CString  mystr;
	mystr = m_slidelist.GetItemText(50, 3);
	if (mystr.Compare(_T("染色体")) == 0)
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			mystr = _T("微核");
			m_slidelist.SetItemText(i, 3, mystr);
		}
	}
	else
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			mystr = _T("染色体");
			m_slidelist.SetItemText(i, 3, mystr);
		}
	}

}

//获得路径
void CSlideSet::OnBnClickedBtnPath()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR servPath[MAX_PATH];//用来存放文件夹路径
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = servPath;
	bi.lpszTitle = _T("选择输出处理结果文件夹路径");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	bi.iImage = NULL;
	if ((pidl = SHBrowseForFolder(&bi)) != NULL)
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl, servPath))) //得到文件夹的全路径，不要的话，只得本文件夹名
		{

			m_path.SetWindowTextW(servPath);

		}
	}

}

//点击选中和取消选中某一行
void CSlideSet::OnNMClickListSlide(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
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
		//TODO:添加多选的操作。
		if (nIndex == -1)
		{
			//m_list.SetItemState(-1,LVNI_SELECTED,LVNI_SELECTED);
		}
		m_slidelist.SetItemState(nIndex, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		CString mystr;
		mystr = m_slidelist.GetItemText(nIndex, 1);
		if (mystr.IsEmpty())
			m_slidelist.SetItemText(nIndex, 1, _T("√"));
		else
			m_slidelist.SetItemText(nIndex, 1, NULL);
	}
	else if (col == 2 || col == 4 || col == 5)
	{
		CRect rc;
		m_Row = pNMListView->iItem;//获得选中的行  
		m_Col = pNMListView->iSubItem;//获得选中列  

		if (pNMListView->iSubItem != 0) //如果选择的是子项;  
		{
			m_slidelist.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；  
			m_edit.SetParent(&m_slidelist);//转换坐标为列表框中的坐标  
			m_edit.MoveWindow(rc);//移动Edit到RECT坐在的位置;  
			m_edit.SetWindowText(m_slidelist.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；  
			m_edit.ShowWindow(SW_SHOW);//显示Edit控件；  
			m_edit.SetFocus();//设置Edit焦点  
			m_edit.ShowCaret();//显示光标  
			//m_edit.SetCursor(-1);//将光标移动到最后  
		}

	}
	else if (col == 3)
	{


		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pNMListView->iItem && e_SubItem == pNMListView->iSubItem))//如果点中的单元格不是之前创建好的
			{
				distroyCcombobox(&m_slidelist, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pNMListView, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
				m_comBox.AddString(L"微核");
				m_comBox.AddString(L"染色体");
				m_comBox.ShowDropDown();//自动下拉
			}
			else//点中的单元格是之前创建好的
			{
				m_comBox.SetFocus();//设置为焦点
			}
		}
		else
		{
			e_Item = pNMListView->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pNMListView->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createCcombobox(pNMListView, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
			m_comBox.AddString(L"微核");
			m_comBox.AddString(L"染色体");
			m_comBox.ShowDropDown();//自动下拉
		}


	}

	*pResult = 0;
}

//修改某个选中的单元格内容
void CSlideSet::OnEnKillfocusEdit()
{
	// TODO:  在此添加控件通知处理程序代码
	CString tem;
	m_edit.GetWindowText(tem);    //得到用户输入的新的内容  
	m_slidelist.SetItemText(m_Row, m_Col, tem);   //设置编辑框的新内容  
	m_edit.ShowWindow(SW_HIDE);                //应藏编辑框 
}

//设置扫描范围
void CSlideSet::OnBnClickedBtnRange()
{
	// TODO:  在此添加控件通知处理程序代码
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

//双击选中某个单元格
void CSlideSet::OnNMDblclkListSlide(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	*pResult = 0;
}

//全选
void CSlideSet::OnBnClickedBtnSelectall()
{
	// TODO:  在此添加控件通知处理程序代码
	CString mystr;
	mystr = m_slidelist.GetItemText(1, 1);
	if (mystr.IsEmpty())
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			m_slidelist.SetItemText(i, 1, _T("√"));
		}
		GetDlgItem(IDC_BTN_SELECTALL)->SetWindowText(_T("取消全选"));
	}
	else
	{
		for (size_t i = 0; i < ITEMLENGTH; i++)
		{
			m_slidelist.SetItemText(i, 1, NULL);
		}
		GetDlgItem(IDC_BTN_SELECTALL)->SetWindowText(_T("全选"));
	}

}

//应用
void CSlideSet::OnBnClickedBtnOk()
{
	// TODO:  在此添加控件通知处理程序代码
	SLIDE *myslide = new SLIDE;

	/***************1.保存存储路径*********************/
	CString n_savepath;
	m_path.GetWindowText(n_savepath);
	/*cout << n_savepath << endl;*/
	if (n_savepath.IsEmpty())
	{
		AfxMessageBox(_T("请选择存储路径"));
	}
	else
	{
		myslide->path = n_savepath;
	}
	/***************2.保存玻片信息*********************/
	//找出已选中的玻片
	CString mystr;
	BOOL selectone = FALSE;
	int slidenum;
	for (size_t i = 0; i < ITEMLENGTH; i++)
	{
		mystr = m_slidelist.GetItemText(i, 1);
		if (mystr == _T("√"))
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
		AfxMessageBox(_T("没有玻片被选中！"));
	}

	CString windowname;
	AfxGetMainWnd()->GetWindowText(windowname);//修改主窗口标题
	HWND   hwnd = ::FindWindow(NULL, windowname);//调用消息处理函数刷新页面
	::SendMessage(hwnd, WM_MN_BTN_OK, (WPARAM)myslide, NULL);
	//将玻片设置信息存入数据库
	SaveToAccess(myslide);
	/*OnOK();*/
}


// 玻片设置信息存入数据库
void CSlideSet::SaveToAccess(SLIDE * myslide)
{
	_bstr_t sql;
	_variant_t var;
	//删除原数据表
	sql = _T("delete  * from 玻片信息表");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	sql = _T("select  * from 玻片信息表");
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
		for (size_t i = 0; i < myslide->slidebuild.size(); i++)
		{
			m_Conn.m_pRecordset->AddNew();
			m_Conn.m_pRecordset->PutCollect(_T("路径"), _variant_t(myslide->path));
			m_Conn.m_pRecordset->PutCollect(_T("病人名称"), _variant_t(myslide->slidebuild[i].name));
			m_Conn.m_pRecordset->PutCollect(_T("模式"), _variant_t(myslide->slidebuild[i].mode));
			m_Conn.m_pRecordset->PutCollect(_T("扫描范围"), _variant_t(myslide->slidebuild[i].range));
			m_Conn.m_pRecordset->PutCollect(_T("备注"), _variant_t(myslide->slidebuild[i].remark));
			m_Conn.m_pRecordset->PutCollect(_T("是否选中"), _variant_t(_T("√")));
			m_Conn.m_pRecordset->PutCollect(_T("玻片序号"), _variant_t(myslide->All_Slide_ID[i]));
			m_Conn.m_pRecordset->Update();
		}

		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}


// 读取数据库的内容进行修改
void CSlideSet::ReadFromAccess()
{
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from 玻片信息表");
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
		CString  ReadString = m_Conn.m_pRecordset->GetCollect(_T("路径"));
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(ReadString);
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString number = m_Conn.m_pRecordset->GetCollect(_T("玻片序号"));
			m_slidelist.SetItemText(_ttoi(number), 1, _T("√"));
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("病人名称"));
			m_slidelist.SetItemText(_ttoi(number), 2, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("模式"));
			m_slidelist.SetItemText(_ttoi(number), 3, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("扫描范围"));
			m_slidelist.SetItemText(_ttoi(number), 4, ReadString);
			ReadString = m_Conn.m_pRecordset->GetCollect(_T("备注"));

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
	}

}

void CSlideSet::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//创建单元格下拉列表框函数
//pEditCtrl为列表对象指针，createccombobox为下拉列表框指针对象，
//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	havecreat = true;
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 0, 0), this, IDC_COMBO_EG);
	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(150, _T("新宋体"));
	createccomboboxobj->SetFont(&font);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createccomboboxobj->SetParent(&m_slidelist);//将list control设置为父窗口,生成的Ccombobox才能正确定位,这个也很重要
	CRect  EditRect;
	m_slidelist.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_slidelist.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	CString strItem = m_slidelist.GetItemText(e_Item, e_SubItem);//获得相应单元格字符
	createccomboboxobj->SetWindowText(strItem);//将单元格字符显示在编辑框上
	createccomboboxobj->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上
	createccomboboxobj->ShowWindow(SW_SHOW);//显示编辑框在单元格上面
}

void CSlideSet::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowTextW(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//更新相应单元格字符
	distroyccomboboxobj->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void CSlideSet::OnCbnSelchangeComboEg()
{
	// TODO:  在此添加控件通知处理程序代码
	int index = m_comBox.GetCurSel();
	CString str;
	m_comBox.GetLBText(index, str);
	m_slidelist.SetItemText(e_Item, 3, str);
}


void CSlideSet::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类


}
