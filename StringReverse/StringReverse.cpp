#include "resource.h"
#include "StringReverse.h"

#include <afxdlgs.h>

#define CHARNUM 1000000

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
CMainDialog::CMainDialog(CWnd* pParentWnd)
	:CDialog(IDD_MAINDIALOG, pParentWnd)
{

}

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CMainDialog::OnCancel()
{
	CDialog::OnCancel();
}

BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTONGENERATE, OnGenrateClicked)
	ON_BN_CLICKED(IDC_BUTTONCHOOSE, OnCreateFileClicked)
	ON_BN_CLICKED(IDC_BUTTONREVERSE1,OnReverse1Clicked)
END_MESSAGE_MAP()

void CMainDialog::OnGenrateClicked()
{
	AfxBeginThread([&](LPVOID)->UINT
	{
		srand(time(NULL));
		HANDLE hFile = ::CreateFile(_T("string.dat"), GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return -1;
		}
		HANDLE hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE,
											  0, CHARNUM, NULL);
		if (hFileMap == INVALID_HANDLE_VALUE)
		{
			return -1;
		}
		PVOID pStr = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);

		char* pc = (char*)pStr;
		for(int i =0;i<CHARNUM;i++)
		{
			char c = rand() % 26 + 'a';

			pc[i] = c;
		}

		::UnmapViewOfFile(pStr);
		::CloseHandle(hFileMap);
		::CloseHandle(hFile);
		return 0;
	}, nullptr);
}

void CMainDialog::OnCreateFileClicked()
{
	CFileDialog dlg(TRUE);
	dlg.DoModal();
}

void CMainDialog::OnReverse1Clicked()
{
	AfxBeginThread([&](LPVOID)->UINT
	{
		srand(time(NULL));
		HANDLE hFile = ::CreateFile(_T("string.dat"), GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
									OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return -1;
		}
		HANDLE hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE,
											  0, CHARNUM, NULL);
		if (hFileMap == INVALID_HANDLE_VALUE)
		{
			return -1;
		}
		PVOID pStr = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		char* pc = (char*)pStr;
		for(int i =0;i<CHARNUM;i++)
		{
			pc[i] = pc[i] - 32;
		}

		::UnmapViewOfFile(pStr);
		::CloseHandle(hFileMap);
		::CloseHandle(hFile);
		return 0;
	}, nullptr);
}
