#ifndef _WIN_ROOT_H
#define _WIN_ROOT_H

#ifndef UNICODE
#define UNICODE
#endif

#define IDC_ADD_NODE_BUTTON		990
#define IDC_ADD_EDGE_BUTTON		991
#define IDC_PRIM_CHECKBOX		992
#define IDC_DIJ_CHECKBOX		993
#define IDC_ASTAR_CHECKBOX		994
#define IDC_GENERATE_BUTTON		995
#define IDC_CLEAR_BUTTON        996
#define IDC_SET_START_BUTTON    997
#define IDC_SET_END_BUTTON      998

#include <windows.h>

/**Base window class
*/
template<typename SubClass>
class WinRoot
{
public:
	WinRoot():mHwnd(NULL){}
	virtual ~WinRoot(){}
	BOOL create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,//We can achieve double-buffering by setting this
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0,
		HCURSOR hCursor = NULL)
	{
		WNDCLASS wc = {0};

		wc.lpfnWndProc   = SubClass::winProc;
		wc.hInstance     = GetModuleHandle(NULL);//Get the current handle(in a single-threaded environment)
		wc.lpszClassName = getClassName();
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//Set default background as white.

		RegisterClass(&wc);

		//On calling this API, a WM_NCCREATE message is sent to winProc(),
		//whose lParam holds a pointer to CREATESTRUCT where we can find "this".
		//A hidden fact that the winProc callback is called by system even
		//before the message loop. That's why mHwnd is actually assigned within
		//winProc() rather than here.
		CreateWindowEx(
			dwExStyle, getClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
			);
		return (mHwnd ? TRUE : FALSE);
	}

	HWND getWinHandle()
	{
		return mHwnd;
	}

	static LRESULT CALLBACK winProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		SubClass* pThis = NULL;
		if (uMsg == WM_NCCREATE)
		{
			//Extract "this" pointer. 
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = reinterpret_cast<SubClass*>(pCreate->lpCreateParams);

			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			//mHwnd is better assigned here, so our handleMsg() can process WM_NCCREATE
			//and WM_CREATE messages.
			pThis->mHwnd = hwnd;
		}
		else
		{
			pThis = (SubClass*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (pThis)
		{
			return pThis->handleMsg(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	
	virtual LRESULT  handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual PCWSTR  getClassName() const = 0;
protected:
	HWND mHwnd;
};

//A sample window class, just to demonstrate a typical usage.
class SampleClass : public WinRoot<SampleClass>
{
public:
	virtual LRESULT  handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE://User click close button
			if (MessageBox(mHwnd, L"Quit?", L"Sample App", MB_OKCANCEL) == IDOK)
				DestroyWindow(mHwnd);//Post a WM_Destory
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);//Post a WM_QUIT, which makes GetMessage() to return 0.
			return 0;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(mHwnd, &ps);

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(mHwnd, &ps);
			}
			return 0;
		}
		return DefWindowProc(mHwnd, uMsg, wParam, lParam);
	}

	virtual PCWSTR  getClassName() const
	{
		return L"Sample Win Class";
	}
};

#endif