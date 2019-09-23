#pragma once


// CButtonST

class CButtonST : public CButton
{
	DECLARE_DYNAMIC(CButtonST)

public:
	CButtonST();
	virtual ~CButtonST();
	//设置背景颜色
	void SetBkColor(COLORREF BkColor);
	//设置鼠标在按钮上的偏移颜色
	void SetShOffset(int nShOffset);
	//设置字体颜色
	void SetTextColor(COLORREF TextColor);
	//设置网页链接
	void SetURL(CString strURL);
	//设置背景图片
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

	//鼠标是否在按钮上面
	BOOL m_IsPressed;
	COLORREF m_BkColor;
	int      m_nShOffset;
	COLORREF m_TextColor;
	CString  m_strURL;
	CBitmap  *m_pBitMapBK;

	//背景颜色偏移
	COLORREF OffsetColor(COLORREF color, int nOffset);
	//鼠标离开
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};


