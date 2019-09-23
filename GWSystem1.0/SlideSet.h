#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ADOAccess.h"


// CSlideSet �Ի���

class CSlideSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSlideSet)                                                                                                                                                                              

public:
	CSlideSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSlideSet();

// �Ի�������
	enum { IDD = IDD_SLIDESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// �����ļ��е�·��
	CEdit m_path;
	CString m_savepath;
	ADOAccess m_Conn;

private:
	int m_Row;
	int m_Col;
	CListCtrl m_slidelist;
	CEdit m_edit;
	CEdit m_range;
	int e_Item;    //�ձ༭����  
	int e_SubItem; //�ձ༭����  
	SlideBuild myslidebuild;


public:
	CComboBox m_comBox;//������Ԫ�������б����
	bool haveccomboboxcreate;//��־�����б���Ѿ�������
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//������Ԫ�������б����
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//���ٵ�Ԫ�������б��

	afx_msg void OnBnClickedBtnModechange();
	afx_msg void OnBnClickedBtnPath();
	afx_msg void OnNMClickListSlide(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit();
	afx_msg void OnBnClickedBtnRange();
	afx_msg void OnNMDblclkListSlide(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnOk();
	// ��Ƭ������Ϣ�������ݿ�
	void SaveToAccess(SLIDE * myslide);
	// ��ȡ���ݿ�����ݽ����޸�
	void ReadFromAccess();
	afx_msg void OnCbnSelchangeComboEg();
	virtual void OnOK();
};
