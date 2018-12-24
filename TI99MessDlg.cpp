// TI99MessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TI99Mess.h"
#include "TI99MessDlg.h"
#include "TI99MessUtility.h"
#include "TI99MessAboutDlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SHOW_ABOUT_BOX					128

#define MENU_BAR_CUTOFF					112
#define ROUNDED_TOP_CORNERS_SIZE		25
#define KEYBOARD_ANGLE_LOCATION			181
#define KEYBOARD_ANGLE_DIFFERENCE		8
#define NO_MOVE_IN_PROGRESS				CPoint(-1, -1)

#define SWAP_SPACE_CHAR _T(' ')
#define SWAP_TRICK_CHAR _T('\255')
#define TI_DRIVER		_T("ti99_4a")
#define MODULE_OPTION   _T("-cart")
#define DISK_OPTION     _T("-flop")
#define TAPE_OPTION     _T("-cass")

/////////////////////////////////////////////////////////////////////////////
// CTI99MessDlg dialog

CTI99MessDlg::CTI99MessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTI99MessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTI99MessDlg)
	m_strModule = _T("");
	m_strCS1 = _T("");
	m_strCS2 = _T("");
	m_strDSK1 = _T("");
	m_strDSK2 = _T("");
	m_strDSK3 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	bLaunchClicking = FALSE;
	bMinimizeClicking = FALSE;
	bCloseClicking = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pointMoveOffset = NO_MOVE_IN_PROGRESS;
}

void CTI99MessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTI99MessDlg)
	DDX_Control(pDX, IDC_BITMAP_MINIMIZE, m_btnMinimize);
	DDX_Control(pDX, IDC_BITMAP_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BITMAPLAUNCH99, m_btnBitmapLaunch99);
	DDX_Text(pDX, IDC_STATIC_MODULE, m_strModule);
	DDX_Text(pDX, IDC_STATIC_CS1, m_strCS1);
	DDX_Text(pDX, IDC_STATIC_CS2, m_strCS2);
	DDX_Text(pDX, IDC_STATIC_DSK1_SEL, m_strDSK1);
	DDX_Text(pDX, IDC_STATIC_DSK2_SEL, m_strDSK2);
	DDX_Text(pDX, IDC_STATIC_DSK3_SEL, m_strDSK3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTI99MessDlg, CDialog)
	//{{AFX_MSG_MAP(CTI99MessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_DEFAULTSETTINGS_EXIT, OnPopupMenuExit)
	ON_COMMAND(ID_DEFAULTSETTINGS_ABOUT, OnPopupMenuAbout)
	ON_COMMAND(ID_DEFAULTSETTINGS_CHANGEMODEL, OnPopupMenuChangeMode)
	ON_COMMAND(ID_DEFAULTSETTINGS_DEFAULTSETTINGS, OnPopupMenuSettings)
	ON_COMMAND(ID_DEFAULTSETTINGS_MINIMIZE, OnPopupMinimize)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_CART, OnButtonSelectCart)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DSK1, OnButtonSelectDsk1)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DSK2, OnButtonSelectDsk2)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DSK3, OnButtonSelectDsk3)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_CS1, OnButtonSelectCS1)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_CS2, OnButtonSelectCS2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTI99MessDlg popup menu handlers
void CTI99MessDlg::OnPopupMenuExit() 
{
	EndDialog(0);
}

void CTI99MessDlg::OnPopupMinimize() 
{
	WINDOWPLACEMENT wndplaceMinimize;	
	memset((void*)&wndplaceMinimize, 0, sizeof(WINDOWPLACEMENT));
	wndplaceMinimize.length = sizeof(WINDOWPLACEMENT);

	// Get the position to restore to
	GetWindowRect(&wndplaceMinimize.rcNormalPosition);	
	wndplaceMinimize.showCmd = SW_MINIMIZE;	 
	SetWindowPlacement(&wndplaceMinimize);
}

void CTI99MessDlg::OnPopupMenuAbout() 
{
	CTI99MessAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CTI99MessDlg::OnPopupMenuChangeMode() 
{
	if (m_pTIBitmap == &m_bitmapSilverTI)
		m_pTIBitmap = &m_bitmapBeigeTI;
	else
		m_pTIBitmap = &m_bitmapSilverTI;
	Invalidate();
}

void CTI99MessDlg::OnPopupMenuSettings() 
{	
	if (m_dlgSettings.DoModal() == IDOK)
	{
		// Ensure that the bitmap matches the dialog settings - this is a "feature"
		m_pTIBitmap = (m_dlgSettings.m_iTIModel == SILVER_TI) ? &m_bitmapSilverTI : &m_bitmapBeigeTI;
		Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTI99MessDlg message handlers
BOOL CTI99MessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Get the rectangle of the window before modifications
	CRect rectClient;
	GetClientRect(rectClient);

	// Modify the style to remove unwanted items from showing up
	ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);   

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Set the dialog text
	SetWindowText("TI99 MESS");	

	// Round the corners of the window
	CRect rectTotalWindow;
	GetWindowRect(&rectTotalWindow);
	int iWidthBar = (rectTotalWindow.Width() - rectClient.Width()) / 2;
	int iTopBar = rectTotalWindow.Height() - rectClient.Height() + 1;
	rectTotalWindow -= rectTotalWindow.TopLeft(); //normalize
	CRgn rgnRounded;
	rgnRounded.CreateRoundRectRgn(rectTotalWindow.left+iWidthBar,
								  rectTotalWindow.top,
								  rectTotalWindow.right-iWidthBar*2,
								  rectTotalWindow.bottom - iTopBar, 
								  ROUNDED_TOP_CORNERS_SIZE, ROUNDED_TOP_CORNERS_SIZE);

	int iIndex = 0;
	CPoint ptRegionVertex[50];
	// Top left
	ptRegionVertex[iIndex].x = rectTotalWindow.left+iWidthBar;
	ptRegionVertex[iIndex++].y = rectTotalWindow.top;
	// Mid left
	ptRegionVertex[iIndex].x = rectTotalWindow.left+iWidthBar;
	ptRegionVertex[iIndex++].y = KEYBOARD_ANGLE_LOCATION;
	// Bottom left part 1
	ptRegionVertex[iIndex].x = rectTotalWindow.left+iWidthBar+KEYBOARD_ANGLE_DIFFERENCE;
	ptRegionVertex[iIndex++].y = rectTotalWindow.bottom - iTopBar - KEYBOARD_ANGLE_DIFFERENCE*2;
	// Bottom left part 2
	ptRegionVertex[iIndex].x = rectTotalWindow.left+iWidthBar+KEYBOARD_ANGLE_DIFFERENCE*2;
	ptRegionVertex[iIndex++].y = (long)(rectTotalWindow.bottom - iTopBar - KEYBOARD_ANGLE_DIFFERENCE*.5);
	// Bottom left part 3
	ptRegionVertex[iIndex].x = rectTotalWindow.left+iWidthBar+KEYBOARD_ANGLE_DIFFERENCE*4;
	ptRegionVertex[iIndex++].y = rectTotalWindow.bottom - iTopBar;
	
	// Bottom right part 1
	ptRegionVertex[iIndex].x = rectTotalWindow.right-iWidthBar*2-KEYBOARD_ANGLE_DIFFERENCE*4;
	ptRegionVertex[iIndex++].y = rectTotalWindow.bottom - iTopBar;
	// Bottom right part 2
	ptRegionVertex[iIndex].x = rectTotalWindow.right-iWidthBar*2-KEYBOARD_ANGLE_DIFFERENCE*2;
	ptRegionVertex[iIndex++].y = (long)(rectTotalWindow.bottom - iTopBar - KEYBOARD_ANGLE_DIFFERENCE*.5);
	// Bottom right part 3
	ptRegionVertex[iIndex].x = rectTotalWindow.right-iWidthBar*2-KEYBOARD_ANGLE_DIFFERENCE;
	ptRegionVertex[iIndex++].y = rectTotalWindow.bottom - iTopBar - KEYBOARD_ANGLE_DIFFERENCE*2;

	// mid right
	ptRegionVertex[iIndex].x = rectTotalWindow.right-iWidthBar*2;
	ptRegionVertex[iIndex++].y = KEYBOARD_ANGLE_LOCATION;
	// top left
	ptRegionVertex[iIndex].x = rectTotalWindow.right-iWidthBar*2;
	ptRegionVertex[iIndex++].y = rectTotalWindow.top;	
	CRgn rgnAngled;
	rgnAngled.CreatePolygonRgn(ptRegionVertex, iIndex, ALTERNATE);

	m_rgn.CreateRectRgn(0, 0, 1, 1);
	m_rgn.CombineRgn(&rgnRounded, &rgnAngled, RGN_AND);

	SetWindowRgn(m_rgn,TRUE);

	// Load Bitmaps and default to appropriate model
	m_bitmapSilverTI.LoadBitmap(IDB_SilverTI);
	m_bitmapBeigeTI.LoadBitmap(IDB_BeigeTI);
	m_pTIBitmap = (m_dlgSettings.m_iTIModel == SILVER_TI) ? &m_bitmapSilverTI : &m_bitmapBeigeTI;

	// Update System Menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu(_T("About TI99 Mess"));
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDD_DIALOG_ABOUT, strAboutMenu);

		iIndex = 0;
		CString strMenuText;
		while (pSysMenu->GetMenuString(iIndex, strMenuText, MF_BYPOSITION))
		{
			if ((strMenuText == _T("&Size")) ||
				(strMenuText == _T("Ma&ximize")) ||
				(strMenuText == _T("&Move")))
			{				
				pSysMenu->RemoveMenu(iIndex, MF_BYPOSITION);
				iIndex = 0;
				continue;
			}
			iIndex++;
		}	
	}

	// Setup the bitmap buttons
	m_bitmapLaunch99.LoadBitmap(IDB_ClickToBegin);
	m_bitmapLaunch99Clicked.LoadBitmap(IDB_ClickToBeginClicked);
	m_bitmapMinimize.LoadBitmap(IDB_Minimize);
	m_bitmapMinimizeClicked.LoadBitmap(IDB_MinimizeClicked);
	m_bitmapClose.LoadBitmap(IDB_Close);
	m_bitmapCloseClicked.LoadBitmap(IDB_CloseClicked);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTI99MessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// Dunno why SHOW_ABOUT_BOX define, but this works
	if ((nID & 0xFFF0) == SHOW_ABOUT_BOX) 
	{
		OnPopupMenuAbout();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CTI99MessDlg::CheckPointInWindow(CPoint point, CWnd& wndCheck)
{
	RECT rectWindow;
	GetWindowRect(&rectWindow);
	RECT rectBitmapCheck;
	wndCheck.GetWindowRect(&rectBitmapCheck);
	CPoint ptCheck = point;
	ptCheck.x += rectWindow.left;
	ptCheck.y += rectWindow.top;
	return PtInRect(&rectBitmapCheck, ptCheck);

}

BOOL CTI99MessDlg::PreTranslateMessage(MSG* pMsg) 
{
	// Don't exit for return 
	if ((pMsg->message == WM_KEYDOWN) &&
		((pMsg->wParam == VK_RETURN))) // ||
		 //(pMsg->wParam == VK_ESCAPE)))
	{	
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTI99MessDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		// device context for painting
		CRect rect;	GetClientRect(&rect);
		DrawBackgroundTI(&dc, CPoint(0, 0));
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTI99MessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-----------------------------------------------------------------------------
// Does:   Draw a Bitmap Resource To The Client DC (Using Bitblt())
// ~~~~    It Will Use The Bitmap Resource Original Size (Width And Height)
//
// InPut:  BMPResource - Resource Name As Defined In The Resources
// ~~~~~   pDC - Given DC To Draw On
//         LeftTop - Opening Point To Start Drawing (Left,Top)
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
void CTI99MessDlg::DrawBackgroundTI(CDC *pDC, CPoint LeftTop)
//=============================================================================
{   
	if (pDC == NULL)
		return;

	// Get Bitmap Details
	BITMAP BMPInfo;
	m_pTIBitmap->GetBitmap(&BMPInfo);
	// Create An In-Memory DC Compatible With The Display DC We R Gonna Paint On
	CDC DCMemory;
	DCMemory.CreateCompatibleDC(pDC);
	// Select The Bitmap Into The In-Memory DC
	CBitmap* pOldBitmap = DCMemory.SelectObject(m_pTIBitmap);
	// Copy Bits From The In-Memory DC Into The On-Screen DC
	pDC->BitBlt(LeftTop.x, LeftTop.y, BMPInfo.bmWidth, BMPInfo.bmHeight, &DCMemory, 0, 0, SRCCOPY);
	DCMemory.SelectObject(pOldBitmap);
}

void CTI99MessDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (point.y >= MENU_BAR_CUTOFF)
	{
		if (!CheckPointInWindow(point, m_btnBitmapLaunch99))
		{
			OnPopupMenuChangeMode();
		}
	}
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CTI99MessDlg::SwitchLaunchButton(BOOL bDark)
{
	if (bDark)
		m_btnBitmapLaunch99.SetBitmap(m_bitmapLaunch99Clicked);
	else
		m_btnBitmapLaunch99.SetBitmap(m_bitmapLaunch99);
}

void CTI99MessDlg::SwitchMinimizeButton(BOOL bDark)
{
	if (bDark)
		m_btnMinimize.SetBitmap(m_bitmapMinimizeClicked);
	else
		m_btnMinimize.SetBitmap(m_bitmapMinimize);
}

void CTI99MessDlg::SwitchCloseButton(BOOL bDark)
{
	if (bDark)
		m_btnClose.SetBitmap(m_bitmapCloseClicked);
	else
		m_btnClose.SetBitmap(m_bitmapClose);
}

void CTI99MessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Default to not moving
	m_pointMoveOffset = NO_MOVE_IN_PROGRESS;

	if (CheckPointInWindow(point, m_btnBitmapLaunch99))
	{
		bLaunchClicking = TRUE;
		SwitchLaunchButton(TRUE);
	}
	else if (CheckPointInWindow(point, m_btnMinimize))
	{
		bMinimizeClicking = TRUE;
		SwitchMinimizeButton(TRUE);
	}
	else if (CheckPointInWindow(point, m_btnClose))
	{
		bCloseClicking = TRUE;
		SwitchCloseButton(TRUE);
	}
	else if (point.y < MENU_BAR_CUTOFF)
	{
		// Capture the mouse
		SetCapture();
		// Remember the offset to where we are - indicating move in progress
		m_pointMoveOffset = point;
	}

	
	CDialog::OnLButtonDown(nFlags, point);
}

void CTI99MessDlg::RelocateWindow(CPoint point)
{
	RECT rectPosition;
	GetWindowRect(&rectPosition);		
	SetWindowPos(&wndTop, rectPosition.left-m_pointMoveOffset.x+point.x,
				 rectPosition.top-m_pointMoveOffset.y+point.y,
				 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);

}

BOOL CTI99MessDlg::ParseFilename(CString strFullPath,
								 CString& strPath,
								 CString& strFilename)
{

	int iTailSlashLoc = strFullPath.ReverseFind(_T('\\'));
	if (iTailSlashLoc > 0)
	{
		strPath = strFullPath.Left(iTailSlashLoc+1);
		strFilename = strFullPath.Mid(iTailSlashLoc + 1);
		return TRUE;
	}
	return FALSE;
}

CString CTI99MessDlg::GetShortFilename(CString strFilename)
{
	CString strShortPath = _T("");
	int iShortPathBufSize = strFilename.GetLength() + 1;
	char* pszShortPath = new char[iShortPathBufSize + 1];
	::GetShortPathName(strFilename, pszShortPath, iShortPathBufSize);
	strShortPath = pszShortPath;
	delete [] pszShortPath;
	return strShortPath;
}

CString CTI99MessDlg::GetCartParams()
{
	CString strCartParams = _T("");
	int iCartFound = 0;

	if (!m_strModule.IsEmpty())
	{
		CString strShortModulePath = GetShortFilename(UnfixStringForUsage(m_strModule));
		strShortModulePath.MakeUpper();
		CString strPath;
		CString strModulePart;
		
		if (ParseFilename(strShortModulePath,
						  strPath,
						  strModulePart))
		{
			// setting the default cartridge dir doesn't seem to be honored :(
			//strCartParams = "-cartridge_dir " + strPath;
			CString strCartBase = _T("");
			int iExtFound = strModulePart.ReverseFind(_T('.'));
			if (iExtFound > 1)
			{
				// Find out the last char and ensure it's valid
				char chRomType = strModulePart.GetAt(iExtFound-1);
				if (chRomType == _T('C') || chRomType == _T('D') || chRomType == _T('G'))
				{
					CString strBaseRom = strModulePart.Left(iExtFound-1);
					CString strExtension = strModulePart.Mid(iExtFound);
					FILE *fp;
					if (fp = fopen(strPath + strBaseRom + _T('C') + strExtension, "rt")) 
					{
						fclose(fp);
						strCartParams = strCartParams + _T(" ") + MODULE_OPTION + _T(" ") + strPath + strBaseRom + _T('C') + strExtension;
						iCartFound++;
					} 

					if (fp = fopen(strPath + strBaseRom + _T('D') + strExtension, "rt")) 
					{
						fclose(fp);
						strCartParams = strCartParams + _T(" ") + MODULE_OPTION + _T(" ") + strPath + strBaseRom + _T('D') + strExtension;
						iCartFound++;
					} 
					
					if (fp = fopen(strPath + strBaseRom + _T('G') + strExtension, "rt")) 
					{
						fclose(fp);
						strCartParams = strCartParams + _T(" ") + MODULE_OPTION + _T(" ") + strPath + strBaseRom + _T('G') + strExtension;
						iCartFound++;
					} 
				}
			}
		}
	}

	// Clean up the params
	for (; iCartFound++<3;)
	{
		strCartParams = strCartParams + _T(" ") + MODULE_OPTION + _T(" \"\"");
	}

	return strCartParams;
}

CString CTI99MessDlg::GetMediaParam(CString strCommandLineOption, CString strPathToResource)
{
	CString strMediaParam(_T(""));
	strMediaParam = _T(" ") + strCommandLineOption + _T(" ") +
					((strPathToResource.IsEmpty()) ? _T("\"\"") 
												 : GetShortFilename(UnfixStringForUsage(strPathToResource)));
	return strMediaParam;
}

void CTI99MessDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Only move the window if we have a move in progress
	if (m_pointMoveOffset != NO_MOVE_IN_PROGRESS)
	{
		// Move window to final location
		RelocateWindow(point);
		// Indicate we are not moving
		m_pointMoveOffset = NO_MOVE_IN_PROGRESS;
		// Release the mouse
		ReleaseCapture();
	}

	if (bLaunchClicking) {
		bLaunchClicking = FALSE;
		if (CheckPointInWindow(point, m_btnBitmapLaunch99))
		{
			Sleep(150);
			SwitchLaunchButton(FALSE);
			
			if (m_dlgSettings.m_strMessExecutable == _T(""))
			{
				MessageBox(_T("MESS has not been configured in Default Settings.\n\n" \
					          "Default Settings will pop up now."),
						   _T("TI99 MESS Error"), MB_ICONSTOP | MB_OK);
				OnPopupMenuSettings();
				return;
			}
			CString strArgs = m_dlgSettings.m_strMessOptions + _T("");

			// Set the working directory to be where MESS is.
			CString strShortPathToMESS;
			CString strMESSPath(_T(""));
			CString strMESSFilename(_T(""));
			strShortPathToMESS = GetShortFilename(m_dlgSettings.m_strMessExecutable);
			if (ParseFilename(strShortPathToMESS, strMESSPath, strMESSFilename))
				::SetCurrentDirectory(strMESSPath);				

			// Build up the parameters
			UpdateData();
			CString strParams = TI_DRIVER;
			strParams += _T(" ") + m_dlgSettings.m_strMessOptions +
						 GetCartParams() +
						 GetMediaParam(DISK_OPTION, m_strDSK1) + 
						 GetMediaParam(DISK_OPTION, m_strDSK2) + 
						 GetMediaParam(DISK_OPTION, m_strDSK3) +
						 GetMediaParam(TAPE_OPTION, m_strCS1) + 
						 GetMediaParam(TAPE_OPTION, m_strCS2);
						 
			// Spawn MESS process - waiting until it's done
			_spawnl(_P_WAIT,
				    strMESSFilename, strMESSFilename,
					strParams,
				    NULL);
		}
	} 

	if (bMinimizeClicking) {
		bMinimizeClicking = FALSE;
		if (CheckPointInWindow(point, m_btnMinimize))
		{
			Sleep(150);
			SwitchMinimizeButton(FALSE);
			WINDOWPLACEMENT wndPlacement;
			GetWindowPlacement(&wndPlacement);
			wndPlacement.showCmd = SW_MINIMIZE;
			SetWindowPlacement(&wndPlacement);
		}
	}

	if (bCloseClicking) {
		bCloseClicking = FALSE;
		if (CheckPointInWindow(point, m_btnClose))
		{
			Sleep(150);
			EndDialog(0);
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CTI99MessDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Only move the window if we have a move in progress
	if (m_pointMoveOffset != NO_MOVE_IN_PROGRESS)
	{
		// Relocate the window to current location
		RelocateWindow(point);
	}

	// Track button highlight for launch if moving
	if (bLaunchClicking)
	{
		SwitchLaunchButton(CheckPointInWindow(point, m_btnBitmapLaunch99));
	}
	else if (bMinimizeClicking)
	{
		SwitchMinimizeButton(CheckPointInWindow(point, m_btnMinimize));
	}
	else if (bCloseClicking)
	{
		SwitchCloseButton(CheckPointInWindow(point, m_btnClose));
	}


	CDialog::OnMouseMove(nFlags, point);
}

void CTI99MessDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//setup the menu
	CMenu menu;
	if (menu.LoadMenu(IDR_TI_POPUP_MENU))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		//Show menu
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			point.x, point.y,
			AfxGetMainWnd()); // use main window for cmds
	}
}

// Swap spaces for #255 so they display nicer in static controls
CString CTI99MessDlg::FixStringForDisplay(CString strStringToFix)
{
	CString strFixed(_T(""));
	for (int i=0; i<strStringToFix.GetLength(); i++)
		strFixed += (strStringToFix[i] != SWAP_SPACE_CHAR) ? strStringToFix[i] : SWAP_TRICK_CHAR;
	return strFixed;
}

// Swap back trick char to spaces
CString CTI99MessDlg::UnfixStringForUsage(CString strStringToUnfix)
{
	CString strUnfixed(_T(""));
	for (int i=0; i<strStringToUnfix.GetLength(); i++)
		strUnfixed += (strStringToUnfix[i] != SWAP_TRICK_CHAR) ? strStringToUnfix[i] : SWAP_SPACE_CHAR;
	return strUnfixed;
}

void CTI99MessDlg::OnButtonSelectCart() 
{
	UpdateData(TRUE);
	CString strModule = m_dlgSettings.m_strDefaultCartPath + _T("\\*");
	if (GetPathToFile(this, strModule, _T("TI Module Image"),
					  _T(".bin"), _T("TI Disk Image (*.bin)|*.bin")))
	{
		m_strModule = FixStringForDisplay(strModule);
		UpdateData(FALSE);
	}
}

void CTI99MessDlg::OnButtonSelectDsk1() 
{
	UpdateData(TRUE);
	CString strDisk = m_dlgSettings.m_strDefaultDiskPath + _T("\\*.dsk");
	if (GetPathToFile(this, strDisk, _T("TI Disk Image"),
					  _T(".dsk"), _T("TI Disk Image (*.dsk)|*.dsk")))
	{
		m_strDSK1 = FixStringForDisplay(strDisk);
		UpdateData(FALSE);
	}
}

void CTI99MessDlg::OnButtonSelectDsk2() 
{
	UpdateData(TRUE);
	CString strDisk = m_dlgSettings.m_strDefaultDiskPath + _T("\\*.dsk");
	if (GetPathToFile(this, strDisk, _T("TI Disk Image"),
					  _T(".dsk"), _T("TI Disk Image (*.dsk)|*.dsk")))
	{
		m_strDSK2 = FixStringForDisplay(strDisk);
		UpdateData(FALSE);
	}
}

void CTI99MessDlg::OnButtonSelectDsk3() 
{
	UpdateData(TRUE);
	CString strDisk = m_dlgSettings.m_strDefaultDiskPath + _T("\\*.dsk");
	if (GetPathToFile(this, strDisk, _T("TI Disk Image"),
					  _T(".dsk"), _T("TI Disk Image (*.dsk)|*.dsk")))
	{
		m_strDSK3 = FixStringForDisplay(strDisk);
		UpdateData(FALSE);
	}
}

void CTI99MessDlg::OnButtonSelectCS1() 
{
	UpdateData(TRUE);
	CString strTape = m_dlgSettings.m_strDefaultTapePath + _T("\\*.wav");
	if( GetPathToFile(this, strTape, _T("TI Tape Image"),
					  _T(".wav"), _T("TI Tape Image (*.wav)|*.wav")))
	{
		m_strCS1 = FixStringForDisplay(strTape);
		UpdateData(FALSE);
	}
}

void CTI99MessDlg::OnButtonSelectCS2() 
{
	UpdateData(TRUE);
	CString strTape = m_dlgSettings.m_strDefaultTapePath + _T("\\*.wav");
	if( GetPathToFile(this, strTape, _T("TI Tape Image"),
					  _T(".wav"), _T("TI Tape Image (*.wav)|*.wav")))
	{
		m_strCS2 = FixStringForDisplay(strTape);
		UpdateData(FALSE);
	}
}
