#include "basewindow.h"

LRESULT BaseWindow::handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CREATE:
		onCreate();
		return 0;

	case WM_SIZE:
		onResize(LOWORD(lParam), HIWORD(lParam));
		return 0;

	//Handle messages from config window
	case IDC_ADD_EDGE_BUTTON:
		mRenderWindow.setAddEdgeMode();
		return 0;

	case IDC_ADD_NODE_BUTTON:
		mRenderWindow.setAddNodeMode();
		return 0;

	case IDC_CLEAR_BUTTON:
		mRenderWindow.clearGraph();
		return 0;

	case IDC_GENERATE_BUTTON:
		mRenderWindow.execAlgo();
		return 0;

	case IDC_PRIM_CHECKBOX:
		mRenderWindow.setPrim();
		return 0;

	case IDC_DIJ_CHECKBOX:
		mRenderWindow.setDijkstra();
		return 0;

	case IDC_ASTAR_CHECKBOX:
		mRenderWindow.setAStar();
		return 0;

	case IDC_SET_START_BUTTON:
		mRenderWindow.setSrcNode();
		return 0;

	case IDC_SET_END_BUTTON:
		mRenderWindow.setDstNode();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

void BaseWindow::onResize( UINT width, UINT height )
{
	RECT rc; 
	SetRect(&rc, 0, 0, width, height); 
	
	MoveWindow(mRenderWindow.getWinHandle(), 100, 0, width, height, TRUE);
	MoveWindow(mConfigWindow.getWinHandle(), 0, 0, width, height, TRUE);
	InvalidateRect(mHwnd, NULL, FALSE);
}

HRESULT BaseWindow::onCreate()
{
	mRenderWindow.create(L"Renderer", WS_CHILD|WS_CLIPSIBLINGS, 0, 100, 0, 800, 600, mHwnd);
	mConfigWindow.create(L"Config", WS_CHILD|WS_CLIPSIBLINGS, 0, 0, 0, 100, 600, mHwnd);
	ShowWindow( mHwnd, SW_SHOWNORMAL );
	return 0;
}
