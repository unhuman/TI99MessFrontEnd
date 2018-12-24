#if !defined(AFX_TI99MESSABOUTDLG_H__CA810AA6_7C6C_11D5_8725_00104B7474A2__INCLUDED_)
#define AFX_TI99MESSABOUTDLG_H__CA810AA6_7C6C_11D5_8725_00104B7474A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TI99MessAboutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTI99MessAboutDlg dialog

class CTI99MessAboutDlg : public CDialog
{
// Construction
public:
	CTI99MessAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTI99MessAboutDlg)
	enum { IDD = IDD_DIALOG_ABOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTI99MessAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTI99MessAboutDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TI99MESSABOUTDLG_H__CA810AA6_7C6C_11D5_8725_00104B7474A2__INCLUDED_)
