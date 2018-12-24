#ifndef TI99MESS_UTILITY
#define TI99MESS_UTILITY

BOOL GetDirectory(CWnd* pWnd, CString& strDirectory, CString strLookingFor, 
				  CString strLookForExtensions);
BOOL GetPathToFile(CWnd* pWnd, CString& strPathToFile, 
				   CString strLookingFor, CString strExtension,
				   CString strExtensionstrExtensionDesc);

#endif //TI99MESS_UTILITY