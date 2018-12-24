// TI99MessSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Registry.h"
#include "TI99Mess.h"
#include "TI99MessSettingsDlg.h"
#include "TI99MessUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REG_MESS_EXE		_T("MessExe")
#define REG_MESS_OPTIONS	_T("MessOptions")
#define REG_CART_PATH		_T("CartPath")
#define REG_DISK_PATH		_T("DiskPath")
#define REG_TAPE_PATH		_T("TapePath")

#define REG_TI_MODEL		_T("TIModel")


/////////////////////////////////////////////////////////////////////////////
// CTI99MessSettingsDlg dialog

CTI99MessSettingsDlg::CTI99MessSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTI99MessSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTI99MessSettingsDlg)
	m_strDefaultCartPath = _T("");
	m_strDefaultDiskPath = _T("");
	m_strMessExecutable = _T("");
	m_strMessOptions = _T("");
	m_iTIModel = SILVER_TI;
	m_strDefaultTapePath = _T("");
	//}}AFX_DATA_INIT

	LoadSaveRegistry(TRUE);
}


void CTI99MessSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTI99MessSettingsDlg)
	DDX_Text(pDX, IDC_EDIT_DEFAULT_CART_PATH, m_strDefaultCartPath);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_DISK_PATH, m_strDefaultDiskPath);
	DDX_Text(pDX, IDC_EDIT_MESS_PATH, m_strMessExecutable);
	DDX_Text(pDX, IDC_EDIT_MESS_OPTIONS, m_strMessOptions);
	DDX_Radio(pDX, IDC_RADIO_TI_MODEL, m_iTIModel);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_TAPE_PATH, m_strDefaultTapePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTI99MessSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CTI99MessSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP_MODULES_PATH, OnButtonLookupModulesPath)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP_DISK_PATH, OnButtonLookupDiskPath)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP_MESS, OnButtonLookupMess)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP_TAPE_PATH, OnButtonLookupTapePath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTI99MessSettingsDlg message handlers

void CTI99MessSettingsDlg::OnButtonLookupModulesPath() 
{
	UpdateData();
	if (GetDirectory(this, m_strDefaultCartPath, 
					 _T("Select default directory for your Command Modules:"), _T(".bin")))
		UpdateData(FALSE);
}

void CTI99MessSettingsDlg::OnButtonLookupDiskPath() 
{
	UpdateData();
	if (GetDirectory(this, m_strDefaultDiskPath, 
					 _T("Select default directory for your Disk Images:"), _T(".dsk")))
		UpdateData(FALSE);
}

void CTI99MessSettingsDlg::OnButtonLookupTapePath() 
{
	UpdateData();
	if (GetDirectory(this, m_strDefaultTapePath, 
					 _T("Select default directory for your Tape Images:"), _T(".wav")))
		UpdateData(FALSE);
}

void CTI99MessSettingsDlg::OnButtonLookupMess() 
{
	UpdateData();
	if (GetPathToFile(this, m_strMessExecutable, _T("Path To Mess Executable"), 
					  _T(".exe"), _T("Executable Files (*.exe)|*.exe||")))
		UpdateData(FALSE);	
}

void CTI99MessSettingsDlg::LoadSaveRegistry(BOOL bLoad)
{
	CRegistry reg;
	reg.SetRootKey(HKEY_LOCAL_MACHINE);
	reg.SetKey("SOFTWARE\\unHUman\\TI99Mess", TRUE);
	if (bLoad)
	{
		m_strMessExecutable = reg.ReadString(REG_MESS_EXE, "");
		m_strMessOptions = reg.ReadString(REG_MESS_OPTIONS, "");
		m_strDefaultCartPath = reg.ReadString(REG_CART_PATH, "");
		m_strDefaultDiskPath = reg.ReadString(REG_DISK_PATH, "");
		m_strDefaultTapePath = reg.ReadString(REG_TAPE_PATH, "");
		m_iTIModel = reg.ReadInt(REG_TI_MODEL, SILVER_TI);
	}
	else // save
	{
		reg.WriteString(REG_MESS_EXE, m_strMessExecutable);
		reg.WriteString(REG_MESS_OPTIONS, m_strMessOptions);
		reg.WriteString(REG_CART_PATH, m_strDefaultCartPath);
		reg.WriteString(REG_DISK_PATH, m_strDefaultDiskPath);
		reg.WriteString(REG_TAPE_PATH, m_strDefaultTapePath);
		reg.WriteInt(REG_TI_MODEL, m_iTIModel);
	}
}

void CTI99MessSettingsDlg::OnCancel() 
{
	LoadSaveRegistry(TRUE);
	
	CDialog::OnCancel();
}

void CTI99MessSettingsDlg::OnOK() 
{
	// Ensure settings are up to date
	UpdateData(TRUE);

	LoadSaveRegistry(FALSE);
	
	CDialog::OnOK();
}
