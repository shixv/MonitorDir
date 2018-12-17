#pragma once

#include <ShlObj.h>
#include <tchar.h>

class CDirDialog
{
public:
	CDirDialog();
	BOOL DoBrowse(HWND hWndParent, LPCTSTR pszTitle = NULL);
	LPCTSTR GetPath()
	{
		return m_szPath;
	}
protected:
	BROWSEINFO m_bi;
	TCHAR m_szDisplay[MAX_PATH];
	TCHAR m_szPath[MAX_PATH];
};

CDirDialog::CDirDialog()
{
	memset(&m_bi, 0, sizeof(m_bi));
	m_bi.hwndOwner = NULL;
	m_bi.pidlRoot = NULL;
	m_bi.pszDisplayName = m_szDisplay;
	m_bi.lpszTitle = NULL;
	m_bi.ulFlags = BIF_RETURNFSANCESTORS;

	m_szPath[0] = '\0';
}

BOOL CDirDialog::DoBrowse(HWND hWndParent, LPCTSTR pszTitle /*= NULL*/)
{
	if (pszTitle == NULL)
		m_bi.lpszTitle = _T("选择目标文件夹");
	else
		m_bi.lpszTitle = pszTitle;
	m_bi.hwndOwner = hWndParent;

	LPITEMIDLIST pItem = ::SHBrowseForFolder(&m_bi);
	if (pItem != 0)
	{
		::SHGetPathFromIDList(pItem, m_szPath);
		return TRUE;
	}
	return FALSE;
}



