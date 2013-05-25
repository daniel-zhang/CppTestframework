#include "configwindow.h"

LRESULT ConfigWindow::handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;

 	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(mHwnd, &ps);
			RECT clientRect;
			// Fill the client area with a brush
			GetClientRect(mHwnd, &clientRect);
			HRGN bgRgn = CreateRectRgnIndirect(&clientRect);
			HBRUSH hBrush = CreateSolidBrush(RGB(240,240,240));
			FillRgn(hdc, bgRgn, hBrush);
			EndPaint(mHwnd, &ps);
			break;
		}
		break;

	case WM_COMMAND:
		{
			//Forward everything to parent window.
			switch(LOWORD(wParam))
			{
			case IDC_ADD_NODE_BUTTON:
				//MessageBox(mHwnd, L"Hello,World!", L"Message", MB_OK|MB_ICONINFORMATION);
				PostMessage(GetParent(mHwnd), IDC_ADD_NODE_BUTTON, NULL, NULL);
				break;

			case IDC_ADD_EDGE_BUTTON:
				PostMessage(GetParent(mHwnd), IDC_ADD_EDGE_BUTTON, NULL, NULL);
				break;

			case IDC_PRIM_CHECKBOX:
				PostMessage(GetParent(mHwnd), IDC_PRIM_CHECKBOX, NULL, NULL);
				break;

			case IDC_DIJ_CHECKBOX:
				PostMessage(GetParent(mHwnd), IDC_DIJ_CHECKBOX, NULL, NULL);
				break;

			case IDC_ASTAR_CHECKBOX:
				PostMessage(GetParent(mHwnd), IDC_ASTAR_CHECKBOX, NULL, NULL);
				break;

			case IDC_GENERATE_BUTTON:
				PostMessage(GetParent(mHwnd), IDC_GENERATE_BUTTON, NULL, NULL);
				break;

			case IDC_CLEAR_BUTTON:
				PostMessage(GetParent(mHwnd), IDC_CLEAR_BUTTON, NULL, NULL);
				break;

			case IDC_SET_START_BUTTON:
				PostMessage(GetParent(mHwnd), IDC_SET_START_BUTTON, NULL, NULL);
				break;
			
			case IDC_SET_END_BUTTON:
				PostMessage(GetParent(mHwnd), IDC_SET_END_BUTTON, NULL, NULL);
				break;
			}
			break;
		}
		
	case WM_CREATE:
		onCreate();
		return 0;

	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

void ConfigWindow::onCreate()
{
	ShowWindow(mHwnd, SW_SHOWNORMAL);
	mFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
		CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_MODERN, L"MS Shell Dlg");

	RECT rc = { 10, 10, 80, 20 };

	HWND hBtn = CreateWindowEx(0, L"BUTTON", L"Add Node", 
		WS_GROUP | BS_AUTORADIOBUTTON |BS_PUSHLIKE | WS_CHILD | WS_VISIBLE, 
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_ADD_NODE_BUTTON),  GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
		SendMessage(hBtn, BM_CLICK, NULL, NULL);
	}
	rc.top += 30;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Add Edge",
		BS_AUTORADIOBUTTON |BS_PUSHLIKE | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_ADD_EDGE_BUTTON), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}

	rc.top += 30;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Set Start Node",
		BS_AUTORADIOBUTTON |BS_PUSHLIKE | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_SET_START_BUTTON), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 30;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Set End Node",
		BS_AUTORADIOBUTTON |BS_PUSHLIKE | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_SET_END_BUTTON), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 30;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Prim MST",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE|WS_GROUP,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_PRIM_CHECKBOX), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
		SendMessage(hBtn, BM_CLICK, NULL, NULL);
	}
	rc.top += 20;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Dijkstra SPT",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_DIJ_CHECKBOX), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 20;

	hBtn = CreateWindowEx(0, L"BUTTON", L"A*",
		BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_ASTAR_CHECKBOX), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 30;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Generate",
		BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom + 20, 
		mHwnd, reinterpret_cast<HMENU>(IDC_GENERATE_BUTTON), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 50;

	hBtn = CreateWindowEx(0, L"BUTTON", L"Clear",
		BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
		rc.left, rc.top, rc.right, rc.bottom, 
		mHwnd, reinterpret_cast<HMENU>(IDC_CLEAR_BUTTON), GetModuleHandle(NULL), 0);
	if (hBtn)
	{
		SendMessage(hBtn, WM_SETFONT, reinterpret_cast<WPARAM>(mFont), TRUE);
	}
	rc.top += 30;
}

