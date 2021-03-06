// Modifications copyright Amazon.com, Inc. or its affiliates.   

//////////////////////////////////////////////////////////////////////////////
//
// RollupCtrl.h
// 
// Code Johann Nadalutti
// Mail: jnadalutti@worldonline.fr
//
//////////////////////////////////////////////////////////////////////////////
//
// This code is free for personal and commercial use, providing this 
// notice remains intact in the source files and all eventual changes are
// clearly marked with comments.
//
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
//////////////////////////////////////////////////////////////////////////////
//
// History
// --------
// #v1.0
//	31/03/01:	Created
//
// #v1.01
//	13/04/01:	Added ScrollToPage() method
//				Added automatic page visibility to ExpandPage() method
//				Added Mousewheel support
//	15/04/01:	Added mouse capture checking on WM_MOUSEMOVE dialog msg
//				Added SetCursor() on Dialog WM_SETCURSOR
//				Added MovePageAt() method
//	17/04/01:	Fixed Group Boxes displayed over Buttons
//	20/04/01:	Added IsPageExpanded() and IsPageExpanded() methods
//				Added PopupMenu
//				Added Button subclassing (now button's focus not drawn)
//
// Note
// -----
//	Dialog box width is
//		RollupCtrlClientRect.Width() - RC_SCROLLBARWIDTH - (RC_GRPBOXINDENT*2)
//
//
// Thanks to
// ----------
// PJ Arends, Ramon Smits, Uwe Keim, Daniel Madden, Do Quyet Tien,
// Ravi Bhavnani, Masaaki Onishi, ...
// and all others users for their comments.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CRYINCLUDE_EDITOR_CONTROLS_ROLLUPCTRL_H
#define CRYINCLUDE_EDITOR_CONTROLS_ROLLUPCTRL_H
#pragma once


/////////////////////////////////////////////////////////////////////////////
// CRollupCtrl include
#include <afxtempl.h>

#include <Controls/ColorCtrl.h>

// This notification sent to parent after some page is expanded.
#define  ROLLUPCTRLN_EXPAND (0x0001)

struct CRollupCtrlNotify
{
	NMHDR hdr;
	int nPageId;
	bool bExpand;
};

/////////////////////////////////////////////////////////////////////////////
// CRollupCtrl structure and defines

	struct RC_PAGEINFO {
		CWnd*			pwndTemplate;
		CButton*  pwndButton;
		CColorCtrl<CButton> *pwndGroupBox;
		BOOL			bExpanded;
		BOOL			bEnable;
		BOOL			bAutoDestroyTpl;
		WNDPROC 	pOldDlgProc;	//Old wndTemplate(Dialog) window proc
		WNDPROC 	pOldButProc;	//Old wndTemplate(Dialog) window proc
		int				id;
	};

	#define	RC_PGBUTTONHEIGHT		18
	#define	RC_SCROLLBARWIDTH		12
	#define	RC_GRPBOXINDENT			6
	const COLORREF	RC_SCROLLBARCOLOR = ::GetSysColor(COLOR_BTNFACE);			//RGB(150,180,180)
	#define RC_ROLLCURSOR			MAKEINTRESOURCE(32649)	// see IDC_HAND (WINVER >= 0x0500)

	//Popup Menu Ids
	#define	RC_IDM_EXPANDALL		0x100
	#define	RC_IDM_COLLAPSEALL		0x101
	#define	RC_IDM_STARTPAGES		0x102

/////////////////////////////////////////////////////////////////////////////
// CRollupCtrl class definition

class SANDBOX_API CRollupCtrl : public CWnd
{
	DECLARE_DYNCREATE(CRollupCtrl)

public:

	// Constructor-Destructor
	CRollupCtrl();
	virtual ~CRollupCtrl();

	// Methods
	BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	int		InsertPage(LPCTSTR caption, CDialog* pwndTemplate, BOOL bAutoDestroyTpl=TRUE, int idx=-1,BOOL bAutoExpand=TRUE);	//Return page zero-based index
	int		InsertPage(LPCTSTR caption, UINT nIDTemplate, CRuntimeClass* rtc, int idx=-1);				//Return page zero-based index

	void	RemovePage(int idx);	//idx is a zero-based index
	void	RemoveAllPages();

	void	ExpandPage(int idx, BOOL bExpand=TRUE,BOOL bScrool=TRUE,BOOL bFromUI=FALSE);	//idx is a zero-based index
	void	ExpandAllPages(BOOL bExpand=TRUE,BOOL bFromUI=FALSE);

	void	EnablePage(int idx, BOOL bEnable=TRUE);	//idx is a zero-based index
	void	EnableAllPages(BOOL bEnable=TRUE);

	void RenamePage(int idx, const char *caption);

	int		GetPagesCount()		{ return m_PageList.size(); }

	RC_PAGEINFO*	GetPageInfo(int idx); //idx is a zero-based index

	// New v1.01 Methods
	void	ScrollToPage(int idx, BOOL bAtTheTop=TRUE);
	int		MovePageAt(int idx, int newidx);	//newidx can be equal to -1 (move at end)
	BOOL	IsPageExpanded(int idx);
	BOOL	IsPageEnabled(int idx);

	void  SetBkColor( COLORREF bkColor );

protected:

	// Internal methods
	void	RecalLayout();
	void  RecalcHeight();
	int		GetPageIdxFromButtonHWND(HWND hwnd);
	void	_RemovePage(int idx);
	void	_ExpandPage(RC_PAGEINFO* pi, BOOL bExpand,BOOL bFromUI );
	void	_EnablePage(RC_PAGEINFO* pi, BOOL bEnable);
	int		_InsertPage(LPCTSTR caption, CDialog* dlg, int idx, BOOL bAutoDestroyTpl,BOOL bAutoExpand=TRUE);
	void	_RenamePage(RC_PAGEINFO* pi, LPCTSTR caption);
	void DestroyAllPages();

	RC_PAGEINFO*	FindPage( int id );
	int FindPageIndex( int id );

	// Datas
	CString		m_strMyClass;
	std::vector<RC_PAGEINFO*>		m_PageList;
	int			m_nStartYPos, m_nPageHeight;
	int			m_nOldMouseYPos, m_nSBOffset;
	int m_lastId;
	bool m_bRecalcLayout;

	std::map<CString,bool> m_expandedMap;

	CBrush m_bkgBrush;
	COLORREF m_bkColor;

	// Window proc
	static LRESULT CALLBACK DlgWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ButWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRollupCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CRollupCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC*, CWnd* pWnd, UINT);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#endif // CRYINCLUDE_EDITOR_CONTROLS_ROLLUPCTRL_H
