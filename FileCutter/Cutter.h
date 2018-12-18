#pragma once
#include <afxwin.h>

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};

class CMainDialog :public CDialog
{
public:
	CMainDialog(CWnd* pParentWnd = NULL);
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};

