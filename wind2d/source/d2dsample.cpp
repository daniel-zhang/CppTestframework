#include "d2dsample.h"
#include <WindowsX.h>

/*Helper to make the Window DPI aware*/
class DPIScale
{
	static float scaleX;
	static float scaleY;

public:
	static void Initialize(ID2D1Factory *pFactory)
	{
		FLOAT dpiX, dpiY;
		pFactory->GetDesktopDpi(&dpiX, &dpiY);
		scaleX = dpiX/96.0f;
		scaleY = dpiY/96.0f;
	}

	template <typename T>
	static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
	}
};

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

/**/
void D2dSample::calcLayout()
{
	if (mpRenderTarget != NULL)
	{
		D2D1_SIZE_F size = mpRenderTarget->GetSize();
		const float x = size.width/2;
		const float y = size.height/2;
		const float radius = min(x, y);
		mEllipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

LRESULT D2dSample::init()
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpFactory)))
		return -1;//CreateWindowEx failed.
	
	if (FAILED(mpFactory->CreateStrokeStyle(D2D1::StrokeStyleProperties(), NULL, 0, &mpStroke)))
		return -1;
	//Get DPI settings
	DPIScale::Initialize(mpFactory);

	if (FAILED(DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED, 
					__uuidof(mpDWriteFactory), 
					reinterpret_cast<IUnknown**>(&mpDWriteFactory)
				)))
		return -1;

	if (FAILED( mpDWriteFactory->CreateTextFormat(
					L"Verdana",
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					15,//msc_fontSize
					L"", //locale
					&mpDWriteTextFormat) 
				))
		return -1;

	mpDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mpDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	
	ShowWindow( mHwnd, SW_SHOWNORMAL );
	return 0;
}

void D2dSample::clearup()
{
	safeRelease(&mpFactory);
	safeRelease(&mpStroke);
	safeRelease(&mpDWriteFactory);
	safeRelease(&mpDWriteTextFormat);
}

HRESULT D2dSample::createGraphicsResources()
{
	HRESULT hr = S_OK;
	if (mpRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(mHwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = mpFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mpRenderTarget
			);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.f, 1.f, 0.f);
			hr = mpRenderTarget->CreateSolidColorBrush(color, &mpBrush);
		}

		if (SUCCEEDED(hr))
			calcLayout();

		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray), &mpGrayBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &mpBlueBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkCyan ), &mpCyanBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White ), &mpWhiteBrush);
		}
		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black ), &mpBlackBrush);
		}
	}
	return hr;
}

void D2dSample::discardGraphicsResources()
{
	safeRelease(&mpRenderTarget);
	safeRelease(&mpBrush);
	safeRelease(&mpGrayBrush);
	safeRelease(&mpBlueBrush);
	safeRelease(&mpCyanBrush);
	safeRelease(&mpWhiteBrush);
	safeRelease(&mpBlackBrush);
}

BOOL D2dSample::isPointInNode( D2D1_POINT_2F* pPoint )
{
	for (unsigned int i = 0; i < mNodes.size(); ++i)
	{
		//Boundary check among 2 degrees of freedom.
		bool hcheck = mNodes[i].left < pPoint->x && mNodes[i].right > pPoint->x;
		bool vcheck = mNodes[i].top < pPoint->y && mNodes[i].bottom > pPoint->y;
		if (hcheck && vcheck)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void D2dSample::onPaint()
{
	HRESULT hr = createGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(mHwnd, &ps);

		hr = render();

		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
			discardGraphicsResources();

		EndPaint(mHwnd, &ps);
	}
}

void D2dSample::onResize(UINT width, UINT height)
{
	if (mpRenderTarget != NULL)
	{
// 		RECT rc;
// 		GetClientRect(mHwnd, &rc);
//		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		mpRenderTarget->Resize(size);
		//calcLayout();
		InvalidateRect(mHwnd, NULL, FALSE);//Inform the OS to post a WM_PAINT
	}
}

void D2dSample::onLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	//Get the current mouse pos and transfer to grip pos.
	D2D1_POINT_2F mousePos = DPIScale::PixelsToDips(pixelX, pixelY);
	int nx= (int)mousePos.x/40;
	int ny = (int)mousePos.y/40;
	D2D1_POINT_2F gridPos;
	gridPos.x = nx*40.f + 20.f;
	gridPos.y = ny*40.f + 20.f;

	//If gridPos is within a valid node, do nothing.
	if (isPointInNode(&gridPos))
	{
		return;
	}

	mNodes.push_back(D2D1::RectF(gridPos.x-18.f, gridPos.y - 18.f, gridPos.x + 18.f, gridPos.y + 18.f));

	InvalidateRect(mHwnd, NULL, FALSE);
}

void D2dSample::onRButtonDown( int pixelX, int pixelY, DWORD flags )
{
	//Get the current mouse pos and transfer to grip pos.
	D2D1_POINT_2F mousePos = DPIScale::PixelsToDips(pixelX, pixelY);
	int nx= (int)mousePos.x/40;
	int ny = (int)mousePos.y/40;
	D2D1_POINT_2F gridPos;
	gridPos.x = nx*40.f + 20.f;
	gridPos.y = ny*40.f + 20.f;

	//If the start point is not within a valid node,
	//do nothing.
	if (!isPointInNode(&gridPos))
	{
		return;
	}

	mCurrentEdge.start = gridPos;
	mCurrentEdge.inUse = true;
	InvalidateRect(mHwnd, NULL, FALSE);
}

void D2dSample::onRButtonUp( int pixelX, int pixelY, DWORD flags )
{
	D2D1_POINT_2F mousePos = DPIScale::PixelsToDips(pixelX, pixelY);
	int nx= (int)mousePos.x/40;
	int ny = (int)mousePos.y/40;
	D2D1_POINT_2F gridPos;
	gridPos.x = nx*40.f + 20.f;
	gridPos.y = ny*40.f + 20.f;
	
	//If the end point is not within a valid node, clear the current
	//edge and repaint the scene.
	if (!isPointInNode(&gridPos))
	{
		mCurrentEdge.inUse = false;
		mCurrentEdge.clear();
		InvalidateRect(mHwnd, NULL, FALSE);
		return;
	}

	//TODO: if the end point is the same with the start point...
	//To compare 2 points, they better be D2D1_POINT_2U
	mCurrentEdge.end = gridPos;
	mCurrentEdge.inUse = false;
	mEdges.push_back(mCurrentEdge);
	mCurrentEdge.clear();

	InvalidateRect(mHwnd, NULL, FALSE);
}

void D2dSample::onMouseMove( int pixelX, int pixelY, DWORD flags )
{
	D2D1_POINT_2F mousePos = DPIScale::PixelsToDips(pixelX, pixelY);
	int nx= (int)mousePos.x/40;
	int ny = (int)mousePos.y/40;
	D2D1_POINT_2F gridPos;
	gridPos.x = nx*40.f + 20.f;
	gridPos.y = ny*40.f + 20.f;

	//Update the edge's end point as user drags the mouse.
	mCurrentEdge.end = gridPos;
	InvalidateRect(mHwnd, NULL, FALSE);
}

LRESULT D2dSample::handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CREATE:
		if (init() == -1)
		{
			return -1;
		}
		break;

	case WM_DESTROY:
		discardGraphicsResources();
		clearup();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		onPaint();
		return 0;

	case WM_SIZE:
		onResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	
	case WM_SETCURSOR:
		//Change the cursor if it is in client area.
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		break;

	case WM_LBUTTONDOWN:
		onLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_RBUTTONDOWN:
		onRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_RBUTTONUP:
		onRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	case WM_MOUSEMOVE:
		onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;

	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
HRESULT D2dSample::render()
{
	mpRenderTarget->BeginDraw();

	drawGrid();
	//drawClock();
	
	return mpRenderTarget->EndDraw();
}

void D2dSample::drawClockHand( float handLength, float angle, float strokeWidth )
{
	const D2D1_COLOR_F colorTemp = mpBrush->GetColor();
	
	mpRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, mEllipse.point));
	D2D_POINT_2F endPoint = D2D1::Point2F(mEllipse.point.x, mEllipse.point.y-(mEllipse.radiusY*handLength));

	mpBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
	mpRenderTarget->DrawLine(mEllipse.point, endPoint, mpBrush, strokeWidth, mpStroke);
	mpBrush->SetColor(colorTemp);
}

void D2dSample::drawClock()
{
	if (mpRenderTarget != NULL)
	{
		mpRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		mpRenderTarget->FillEllipse(mEllipse, mpBrush);

		SYSTEMTIME time;
		GetLocalTime(&time);

		const float hourAngle = (360.f/12)*(time.wHour) + time.wMinute*.5f;
		const float minuteAngle = (360.f/60)*(time.wMinute);
		const float secondAngle = (360.f/60)*(time.wSecond);

		drawClockHand(0.6f, hourAngle, 8);
		drawClockHand(0.8f, minuteAngle, 6);
		drawClockHand(0.95f, secondAngle, 2);

		mpRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );
	}
}

void D2dSample::drawGrid()
{
	if (mpRenderTarget != NULL)
	{
		mpRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//Draw grid
		D2D1_SIZE_F size = mpRenderTarget->GetSize();
		int width = static_cast<int>(size.width);
		int height = static_cast<int>(size.height);
		for (int x = 0; x < width; x += 40)
		{
			mpRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<float>(x), 0.f), 
				D2D1::Point2F(static_cast<float>(x), size.height),
				mpGrayBrush);
		}
		for (int y = 0; y < height; y += 40)
		{
			mpRenderTarget->DrawLine(
				D2D1::Point2F(0.f,  static_cast<float>(y)), 
				D2D1::Point2F(size.width, static_cast<float>(y)),
				mpGrayBrush);
		}

		//Draw current edge if any.
		if (mCurrentEdge.inUse && mCurrentEdge.getLength()>1.f)
		{
			mpRenderTarget->DrawLine(mCurrentEdge.start, mCurrentEdge.end, mpBlueBrush, 5.f );
			WCHAR text[20] = L"";
			_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mCurrentEdge.getLength());
			mpRenderTarget->DrawTextW(text, ARRAYSIZE(text) - 1, mpDWriteTextFormat, &(mCurrentEdge.getTextRect()),mpBlackBrush);
		}
		
		//Draw previously added edges.
		for (unsigned int i = 0; i < mEdges.size(); ++i)
		{
			mpRenderTarget->DrawLine(mEdges[i].start, mEdges[i].end, mpBlueBrush, 5.f );
			WCHAR text[20] = L"";
			_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mEdges[i].getLength());
			mpRenderTarget->DrawTextW(text, ARRAYSIZE(text) - 1, mpDWriteTextFormat, &(mEdges[i].getTextRect()),mpBlackBrush);
		}

		//Draw Rectangles
		for (unsigned int i = 0; i < mNodes.size(); ++i)
		{
			WCHAR text[20] = L"";
			mpRenderTarget->FillRoundedRectangle(
				&(D2D1::RoundedRect(mNodes[i], 5.f, 5.f)),
				mpCyanBrush);
			
			//Draw text
			_itow_s(i,text, ARRAYSIZE(text), 10);
			mpRenderTarget->DrawTextW(text, ARRAYSIZE(text) -1, mpDWriteTextFormat, mNodes[i], mpWhiteBrush);
		}
	}	
}




