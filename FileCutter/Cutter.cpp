#include "resource.h"
#include "Cutter.h"

CMyApp theApp;
BOOL CMyApp::InitInstance()
{
	CMainDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

CMainDialog::CMainDialog(CWnd* pParentWnd):CDialog(IDD_MAINDIALOG,pParentWnd)
{

}

BEGIN_MESSAGE_MAP(CMainDialog,CDialog)
END_MESSAGE_MAP()

BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

void CMainDialog::OnCancel()
{
	CDialog::OnCancel();
}
