#pragma once


// CButtonST

class CButtonST : public CButton
{
	DECLARE_DYNAMIC(CButtonST)

public:
	CButtonST();
	virtual ~CButtonST();
	//���ñ�����ɫ
	void SetBkColor(COLORREF BkColor);
	//��������ڰ�ť�ϵ�ƫ����ɫ
	void SetShOffset(int nShOffset);
	//����������ɫ
	void SetTextColor(COLORREF TextColor);
	//������ҳ����
	void SetURL(CString strURL);
	//���ñ���ͼƬ
	void SetBkPicture(CBitmap *pBitMap);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void DrawItem1(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//{{AFX_MSG(CButtonST)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	//����Ƿ��ڰ�ť����
	BOOL m_IsPressed;
	COLORREF m_BkColor;
	int      m_nShOffset;
	COLORREF m_TextColor;
	CString  m_strURL;
	CBitmap  *m_pBitMapBK;

	//������ɫƫ��
	COLORREF OffsetColor(COLORREF color, int nOffset);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};


