#pragma once
#include <windef.h>

#define WM_CUTTERSTART WM_USER+100
#define WM_CUTTERSTOP WM_USER+101
#define WM_CUTTERSTATUS WM_USER+102

class CFileCutter
{
public:
	enum ExitCode{
		exitSuccess,
		exitUserForce,
		exitSourceErr,
		exitDestErr
	};
	CFileCutter(HWND hWndNotify);

	BOOL IsRunning()const
	{
		return m_bRunning;
	}

	BOOL StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFIleSize);
	BOOL StartMerge(LPCTSTR lpszDestFile, LPCTSTR lpszSourceDir);
	BOOL SuspendCutter();
	BOOL ResumeCutter();
	void StopCutter();

public:
	~CFileCutter();
protected:
	void Reset();
	void DoSplit();
	void DoMerge();
	UINT friend _CutterEntry(LPVOID lpParam);

	CString m_strSource;
	CString m_strDest;
	UINT m_uFileSize;
	BOOL m_bSplit;

	BOOL m_bContinue;
	BOOL m_bRunning;

	CRITICAL_SECTION m_cs;
private:
	HWND m_hWndNotify;
	HANDLE m_bWorkEvent;
	CWinThread* m_pThread;
	BOOL m_bSuspend;
	BOOL m_bExitThread;
};

