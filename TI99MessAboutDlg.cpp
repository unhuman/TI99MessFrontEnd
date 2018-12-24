// TI99MessAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ti99mess.h"
#include "TI99MessAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTI99MessAboutDlg dialog


CTI99MessAboutDlg::CTI99MessAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTI99MessAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTI99MessAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTI99MessAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTI99MessAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTI99MessAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CTI99MessAboutDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTI99MessAboutDlg message handlers
