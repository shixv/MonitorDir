#pragma once
#include <afxwin.h>
#include <afxcmn.h>

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};


class CMonitorDialog : public CDialog
{
public:
	CMonitorDialog(CWnd* pParentWnd = NULL);
	~CMonitorDialog();
protected:
	void AddStringToList(LPCTSTR lpszString);

	CStatusBarCtrl m_bar;
	HANDLE m_hEvent;
	HANDLE m_arhChange[6];
	BOOL m_bExit;
protected:
	friend UINT MonitorThread(LPVOID lpParam);
	virtual BOOL OnInitDialog();
	virtual VOID OnCancel();

	afx_msg void OnBrowser();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnClear();

	DECLARE_MESSAGE_MAP()
};
