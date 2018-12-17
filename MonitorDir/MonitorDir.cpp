#include "resource.h"
#include "MonitorDir.h"
#include "DirDialog.h"


CMyApp theApp;


BOOL CMyApp::InitInstance()
{
	CMonitorDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

BEGIN_MESSAGE_MAP(CMonitorDialog, CDialog)
	ON_BN_CLICKED(IDC_QUIT, OnCancel)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
END_MESSAGE_MAP()

CMonitorDialog::CMonitorDialog(CWnd* pParentWnd /*= NULL*/)
	:CDialog(IDD_MAINDIALOG, pParentWnd)
{
	m_hEvent = ::CreateEvent(NULL, FALSE, 0, NULL);
}

CMonitorDialog::~CMonitorDialog()
{
	CloseHandle(m_hEvent);
}

BOOL CMonitorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(theApp.LoadIcon(IDI_MAIN), FALSE);
	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	int arWidth[] = {250,-1};
	m_bar.SetParts(2, arWidth);
	m_bar.SetText(_T("windows程序"), 1, 0);
	m_bar.SetText(_T("空闲"), 0, 0);

	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_SUBDIR))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->SetCheck(1);
	return TRUE;
}

VOID CMonitorDialog::OnCancel()
{
	OnStop();
	CDialog::OnCancel();
}

void CMonitorDialog::OnBrowser()
{
	CDirDialog dir;
	if (dir.DoBrowse(m_hWnd, _T("请选择要监视的目录")))
	{
		GetDlgItem(IDC_TARGETDIR)->SetWindowText(dir.GetPath());
	}
}

void CMonitorDialog::OnStart()
{
	CString strDir;
	GetDlgItem(IDC_TARGETDIR)->GetWindowText(strDir);
	if (strDir.IsEmpty())
	{
		MessageBox(_T("请选择一个要监视的目录！"));
		return;
	}

	for (int i = 0; i < 6; i++)
		m_arhChange[i] = m_hEvent;
	m_bExit = FALSE;

	BOOL bSubDir = ((CButton*)GetDlgItem(IDC_SUBDIR))->GetCheck();
	BOOL bNeedExecute = FALSE;

	if (((CButton*)GetDlgItem(IDC_DIRNAME_CHANGE))->GetCheck())
	{
		m_arhChange[0] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_DIR_NAME);
		bNeedExecute = TRUE;
	}

	if (((CButton*)GetDlgItem(IDC_FILENAME_CHANGE))->GetCheck())
	{
		m_arhChange[1] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_FILE_NAME);
		bNeedExecute = TRUE;
	}

	if (((CButton*)GetDlgItem(IDC_ATTRIBUTE_CHANGE))->GetCheck())
	{
		m_arhChange[2] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_ATTRIBUTES);
		bNeedExecute = TRUE;
	}

	if (((CButton*)GetDlgItem(IDC_FILESIZE_CHANGE))->GetCheck())
	{
		m_arhChange[3] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SIZE);
		bNeedExecute = TRUE;
	}

	if (((CButton*)GetDlgItem(IDC_LASTWRITE_CHANGE))->GetCheck())
	{
		m_arhChange[4] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_LAST_WRITE);
		bNeedExecute = TRUE;
	}

	if (((CButton*)GetDlgItem(IDC_SECURITY_CHANGE))->GetCheck())
	{
		m_arhChange[4] =
			::FindFirstChangeNotification(strDir, bSubDir, FILE_NOTIFY_CHANGE_SECURITY);
		bNeedExecute = TRUE;
	}

	if (!bNeedExecute)
	{
		MessageBox(_T("请选择一个监控类型"));
		return;
	}
	AfxBeginThread(MonitorThread, this);

	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
	m_bar.SetText(_T("正在监视.."), 0, 0);
}

void CMonitorDialog::OnStop()
{
	if (!m_bExit)
	{
		m_bExit = TRUE;
		for (int i = 0; i < 6; i++)
		{
			if (m_arhChange[i] != m_hEvent)
				::FindCloseChangeNotification(m_arhChange[i]);
		}
	}
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	m_bar.SetText(_T("空闲"), 0, 0);
}

void CMonitorDialog::OnClear()
{
	GetDlgItem(IDC_EDITCHANGES)->SetWindowText(_T(""));
}

void CMonitorDialog::AddStringToList(LPCTSTR lpszString)
{
	CString strEdit;
	GetDlgItem(IDC_EDITCHANGES)->GetWindowText(strEdit);
	strEdit += lpszString;
	GetDlgItem(IDC_EDITCHANGES)->SetWindowText(strEdit);
}

UINT MonitorThread(LPVOID lpParam)
{
	CMonitorDialog *pDlg = (CMonitorDialog*)lpParam;
	while (TRUE)
	{
		DWORD nObjectWait = ::WaitForMultipleObjects(6, pDlg->m_arhChange, FALSE, INFINITE);
		if (pDlg->m_bExit)
			break;

		int nIndex = nObjectWait - WAIT_OBJECT_0;
		switch (nIndex)
		{
		case 0:
			pDlg->AddStringToList(_T("Directory name changed.\r\n"));
			break;
		case 1:
			pDlg->AddStringToList(_T("File name changed.\r\n"));
			break;
		case 2:
			pDlg->AddStringToList(_T("File attribute changed.\r\n"));
			break;
		case 3:
			pDlg->AddStringToList(_T("File size changed.\r\n"));
			break;
		case 4:
			pDlg->AddStringToList(_T("Last write changed.\r\n"));
			break;
		case 5:
			pDlg->AddStringToList(_T("security changed.\r\n"));
			break;
		}
		::FindNextChangeNotification(pDlg->m_arhChange[nObjectWait]);
	}
	return 0;
}

