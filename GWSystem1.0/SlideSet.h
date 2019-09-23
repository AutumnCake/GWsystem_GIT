#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"


// CSlideSet 对话框

class CSlideSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSlideSet)                                                                                                                                                                              

public:
	CSlideSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSlideSet();

// 对话框数据
	enum { IDD = IDD_SLIDESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 生成文件夹的路径
	CEdit m_path;
	CString m_savepath;
	ADOAccess m_Conn;

private:
	int m_Row;
	int m_Col;
	CListCtrl m_slidelist;
	CEdit m_edit;
	CEdit m_range;
	int e_Item;    //刚编辑的行  
	int e_SubItem; //刚编辑的列  
	SlideBuild myslidebuild;


public:
	CComboBox m_comBox;//生产单元格下拉列表对象
	bool haveccomboboxcreate;//标志下拉列表框已经被创建
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//创建单元格下拉列表框函数
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//销毁单元格下拉列表框

	afx_msg void OnBnClickedBtnModechange();
	afx_msg void OnBnClickedBtnPath();
	afx_msg void OnNMClickListSlide(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit();
	afx_msg void OnBnClickedBtnRange();
	afx_msg void OnNMDblclkListSlide(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnOk();
	// 玻片设置信息存入数据库
	void SaveToAccess(SLIDE * myslide);
	// 读取数据库的内容进行修改
	void ReadFromAccess();
	afx_msg void OnCbnSelchangeComboEg();
	virtual void OnOK();
};
