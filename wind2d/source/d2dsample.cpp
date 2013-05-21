#include "d2dsample.h"

void D2dSample::calcLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width/2;
		const float y = size.height/2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

HRESULT D2dSample::createGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(mHwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&pRenderTarget
			);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.f, 1.f, 0.f);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
				calcLayout();
		}
	}
	return hr;
}

void D2dSample::disgardGraphicsResources()
{
	safeRelease(&pRenderTarget);
	safeRelease(&pBrush);
}

void D2dSample::onPaint()
{
	HRESULT hr = createGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(mHwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		drawClock();

		hr = pRenderTarget->EndDraw();

		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
			disgardGraphicsResources();

		EndPaint(mHwnd, &ps);
	}
}

void D2dSample::onResize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(mHwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		calcLayout();
		InvalidateRect(mHwnd, NULL, FALSE);//Inform the OS to post a WM_PAINT
	}
}

LRESULT D2dSample::handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;//CreateWindowEx failed.
		}
		pFactory->CreateStrokeStyle(D2D1::StrokeStyleProperties(), NULL, 0, &pStroke);
		return 0;

	case WM_DESTROY:
		disgardGraphicsResources();
		safeRelease(&pFactory);
		safeRelease(&pStroke);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		onPaint();
		return 0;

	case WM_SIZE:
		onResize();
		return 0;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

void D2dSample::drawClockHand( float handLength, float angle, float strokeWidth )
{
	const D2D1_COLOR_F colorTemp = pBrush->GetColor();
	
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, ellipse.point));
	D2D_POINT_2F endPoint = D2D1::Point2F(ellipse.point.x, ellipse.point.y-(ellipse.radiusY*handLength));

	pBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
	pRenderTarget->DrawLine(ellipse.point, endPoint, pBrush, strokeWidth, pStroke);
	pBrush->SetColor(colorTemp);
}

void D2dSample::drawClock()
{
	
	SYSTEMTIME time;
	GetLocalTime(&time);

	const float hourAngle = (360.f/12)*(time.wHour) + time.wMinute*.5f;
	const float minuteAngle = (360.f/60)*(time.wMinute);
	const float secondAngle = (360.f/60)*(time.wSecond);

	drawClockHand(0.6f, hourAngle, 8);
	drawClockHand(0.8f, minuteAngle, 6);
	drawClockHand(0.95f, secondAngle, 2);
	pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
}
