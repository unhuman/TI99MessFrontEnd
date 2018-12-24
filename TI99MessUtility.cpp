#include "stdafx.h"
#include "TI99MessUtility.h"

BOOL GetDirectory(CWnd* pWnd, CString& strDirectoryInOut, CString strLookingFor, CString strLookForExtensions)
{
	// Use the directory sent as input
	CString strDirectory = strDirectoryInOut;

	LPMALLOC pMalloc = NULL;
	/* Get's the Shell's default allocator */
	if (::SHGetMalloc(&pMalloc) != NOERROR)
	{		
		return FALSE;
	}

	BROWSEINFO browseInfo;
	LPITEMIDLIST pItemSelected = NULL;

	browseInfo.hwndOwner = pWnd->m_hWnd;
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = strDirectory.GetBuffer(MAX_PATH);
	browseInfo.lpszTitle = _T(strLookingFor);
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	browseInfo.lpfn = NULL;

	// If the user selected a folder
	if (pItemSelected = ::SHBrowseForFolder(&browseInfo))
	{
		// release buffer - done below also - since it's reused
		strDirectory.ReleaseBuffer();

		// Modify buffer in memory
		::SHGetPathFromIDList(pItemSelected, strDirectory.GetBuffer(MAX_PATH));
		strDirectory.ReleaseBuffer();
		
		pMalloc->Free(pItemSelected);

		// Set the directory for output
		strDirectoryInOut = strDirectory;

		return TRUE;
	}
	else //Cancel
	{
		//release buffer
		strDirectory.ReleaseBuffer();
		return FALSE;
	}	
}

BOOL GetPathToFile(CWnd* pWnd, CString& strPathToFile, 
				   CString strLookingFor, CString strExtension,
				   CString strExtensionDesc)
{
    CFileDialog dlgBrowse(TRUE, strExtension, strPathToFile,
						  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
						  strExtensionDesc, pWnd);
    if (dlgBrowse.DoModal() == IDOK)
    {
            strPathToFile = dlgBrowse.m_ofn.lpstrFile;    
			return TRUE;
    }

	return FALSE;
}
