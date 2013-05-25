#ifndef UNICODE
#define UNICODE
#endif

#include "basewindow.h"

HANDLE g_hTimer = NULL;

BOOL InitializeTimer();

/*
How does the compiler know to invoke wWinMain instead of the standard main function? 
What actually happens is that the Microsoft C runtime library (CRT) provides an 
implementation of main that calls either WinMain or wWinMain.

The CRT does some additional work inside main. For example, any static initializers 
are called before wWinMain. 

Although you can tell the linker to use a different entry-point function, use the default 
if you link to the CRT. Otherwise, the CRT initialization code will be skipped.
*/
int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		return 0;
	}
	if (!InitializeTimer())
	{
		CoUninitialize();
		return 0;
	}

	BaseWindow bw;
	
	if (!bw.create(L"Graph Demo", WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN ))
	{
		return 0;
	}

	ShowWindow(bw.getWinHandle(), nShowCmd);

	MSG msg = {};
// 	while (msg.message != WM_QUIT)
// 	{
// 		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
// 		{
// 			TranslateMessage(&msg);
// 			DispatchMessage(&msg);
// 			continue;
// 		}
// 		// Wait until the timer expires or any message is posted.
// 		if (MsgWaitForMultipleObjects(1, &g_hTimer, FALSE, INFINITE, QS_ALLINPUT) 
// 			== WAIT_OBJECT_0)
// 		{
// 			InvalidateRect(sc.getWinHandle(), NULL, FALSE);
// 		}
// 	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CloseHandle(g_hTimer);
	CoUninitialize();
	return 0;
}

BOOL InitializeTimer()
{
	g_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (g_hTimer == NULL)
	{
		return FALSE;
	}

	LARGE_INTEGER li = {0};

	if (!SetWaitableTimer(g_hTimer, &li, (1000/60), NULL, NULL,FALSE))
	{
		CloseHandle(g_hTimer);
		g_hTimer = NULL;
		return FALSE;
	}

	return TRUE;
}