#if !defined(AFX_TI99MESSSETTINGSDLG_H__CA0E1921_7CA4_11D5_8393_444553540000__INCLUDED_)
#define AFX_TI99MESSSETTINGSDLG_H__CA0E1921_7CA4_11D5_8393_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TI99MessSettingsDlg.h : header file
//

#define SILVER_TI	0
#define BEIGE_TI	1

/////////////////////////////////////////////////////////////////////////////
// CTI99MessSettingsDlg dialog

class CTI99MessSettingsDlg : public CDialog
{
// Construction
public:
	CTI99MessSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTI99MessSettingsDlg)
	enum { IDD = IDD_DIALOG_SETTINGS };
	CString	m_strDefaultCartPath;
	CString	m_strDefaultDiskPath;
	CString	m_strMessExecutable;
	CString	m_strMessOptions;
	int		m_iTIModel;
	CString	m_strDefaultTapePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTI99MessSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTI99MessSettingsDlg)
	afx_msg void OnButtonLookupModulesPath();
	afx_msg void OnButtonLookupDiskPath();
	afx_msg void OnButtonLookupMess();
	afx_msg void OnButtonLookupTapePath();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void LoadSaveRegistry(BOOL bLoad);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TI99MESSSETTINGSDLG_H__CA0E1921_7CA4_11D5_8393_444553540000__INCLUDED_)
