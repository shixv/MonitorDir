#pragma once
#include <afxwin.h>

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};

class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParentWnd = NULL);
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
protected:
	afx_msg void OnGenrateClicked();
	afx_msg void OnCreateFileClicked();
	afx_msg void OnReverse1Clicked();
	DECLARE_MESSAGE_MAP()
};
