// TI99MessDlg.h : header file
//

#if !defined(AFX_TI99MESSDLG_H__07C994A7_7B02_11D5_8393_444553540000__INCLUDED_)
#define AFX_TI99MESSDLG_H__07C994A7_7B02_11D5_8393_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TI99MessSettingsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTI99MessDlg dialog

class CTI99MessDlg : public CDialog
{
// Construction
public:
	CTI99MessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTI99MessDlg)
	enum { IDD = IDD_TI99MESS_DIALOG };
	CStatic	m_btnMinimize;
	CStatic	m_btnClose;
	CStatic	m_btnBitmapLaunch99;
	CString	m_strModule;
	CString	m_strCS1;
	CString	m_strCS2;
	CString	m_strDSK1;
	CString	m_strDSK2;
	CString	m_strDSK3;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTI99MessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTI99MessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPopupMenuExit();
	afx_msg void OnPopupMenuAbout();
	afx_msg void OnPopupMenuChangeMode();
	afx_msg void OnPopupMenuSettings();
	afx_msg void OnPopupMinimize();
	afx_msg void OnButtonSelectCart();
	afx_msg void OnButtonSelectDsk1();
	afx_msg void OnButtonSelectDsk2();
	afx_msg void OnButtonSelectDsk3();
	afx_msg void OnButtonSelectCS1();
	afx_msg void OnButtonSelectCS2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:	
	void DrawBackgroundTI(CDC *pDC, CPoint LeftTop);
	void RelocateWindow(CPoint point);
	BOOL CheckPointInWindow(CPoint point, CWnd& wndCheck);
	void SwitchLaunchButton(BOOL bDark);
	void SwitchMinimizeButton(BOOL bDark);
	void SwitchCloseButton(BOOL bDark);
	CString GetCartParams();
	CString GetMediaParam(CString strCommandLineOption, CString strPathToResource);
	BOOL ParseFilename(CString strFullPath, CString& strPath, CString& strFilename);
	CString GetShortFilename(CString strFilename);
	CString FixStringForDisplay(CString strStringToFix);
	CString UnfixStringForUsage(CString strStringToUnfix);

	// state for the buttons
	bool bLaunchClicking;
	bool bMinimizeClicking;
	bool bCloseClicking;

	CTI99MessSettingsDlg m_dlgSettings;
	CRgn m_rgn;
	CBitmap m_bitmapSilverTI;
	CBitmap m_bitmapBeigeTI;
	CBitmap* m_pTIBitmap;

	CBitmap m_bitmapLaunch99;
	CBitmap m_bitmapLaunch99Clicked;
	CBitmap m_bitmapMinimize;
	CBitmap m_bitmapMinimizeClicked;
	CBitmap m_bitmapClose;
	CBitmap m_bitmapCloseClicked;

	CPoint m_pointMoveOffset;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TI99MESSDLG_H__07C994A7_7B02_11D5_8393_444553540000__INCLUDED_)
