#ifndef _BASE_WINDOW_H
#define _BASE_WINDOW_H
#include "winroot.h"
#include "renderwindow.h"
#include "configwindow.h"

class BaseWindow : public WinRoot<BaseWindow>
{
public:
	virtual LRESULT handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual PCWSTR getClassName() const
	{
		return L"BaseWindow";
	}

private:
	void onResize(UINT width, UINT height);
	HRESULT onCreate();

	RenderWindow mRenderWindow;
	ConfigWindow mConfigWindow;
};


#endif