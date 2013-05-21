#ifndef _D2D_SAMPLE_H
#define _D2D_SAMPLE_H

#include "winroot.h"
#include <d2d1.h>
//#pragma comment(lib, "d2d1")

template <typename T> 
void safeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		(*ppT) = NULL;
	}
}

class D2dSample : public WinRoot<D2dSample>
{
private:
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;
	ID2D1StrokeStyle* pStroke;
	D2D1_ELLIPSE ellipse;

	void calcLayout();
	HRESULT createGraphicsResources();
	void disgardGraphicsResources();
	void onPaint();
	void onResize();

public:
	D2dSample():pFactory(NULL),pRenderTarget(NULL),pBrush(NULL){}
	

	virtual LRESULT  handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual PCWSTR  getClassName() const
	{
		return L"D2d Sample";
	}

protected:
	void drawClockHand(float handLength, float angle, float strokWidth);
	void drawClock();
};

#endif