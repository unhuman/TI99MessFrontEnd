// TI99Mess.h : main header file for the TI99MESS application
//

#if !defined(AFX_TI99MESS_H__07C994A5_7B02_11D5_8393_444553540000__INCLUDED_)
#define AFX_TI99MESS_H__07C994A5_7B02_11D5_8393_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTI99MessApp:
// See TI99Mess.cpp for the implementation of this class
//

class CTI99MessApp : public CWinApp
{
public:
	CTI99MessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTI99MessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTI99MessApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TI99MESS_H__07C994A5_7B02_11D5_8393_444553540000__INCLUDED_)
