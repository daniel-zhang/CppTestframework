#pragma once

#include "winroot.h"
#include "datamodel.h"

class ConfigWindow :public WinRoot<ConfigWindow>
{
public:
	virtual LRESULT handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	virtual PCWSTR getClassName() const
	{
		return L"BaseWindow";
	}
	
	
private:

	HWND mGroupBox;
	HWND mBtnAlgo1;
	HWND mBtnAlgo2;
	HWND mBtnAlgo3;
	HFONT mFont;
	void onCreate();
};