#include "renderwindow.h"
#include <WindowsX.h>

/*Helper to make the app DPI aware*/
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
void RenderWindow::calcLayout()
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

LRESULT RenderWindow::init()
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

void RenderWindow::clearup()
{
	safeRelease(&mpFactory);
	safeRelease(&mpStroke);
	safeRelease(&mpDWriteFactory);
	safeRelease(&mpDWriteTextFormat);
}

HRESULT RenderWindow::createGraphicsResources()
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
		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue ), &mpGreenBrush);
		}
		if (SUCCEEDED(hr))
		{
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red ), &mpRedBrush);
		}
	}
	return hr;
}

void RenderWindow::discardGraphicsResources()
{
	safeRelease(&mpRenderTarget);
	safeRelease(&mpBrush);
	safeRelease(&mpGrayBrush);
	safeRelease(&mpBlueBrush);
	safeRelease(&mpCyanBrush);
	safeRelease(&mpWhiteBrush);
	safeRelease(&mpBlackBrush);
	safeRelease(&mpGreenBrush);
	safeRelease(&mpRedBrush);
}

void RenderWindow::onPaint()
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

void RenderWindow::onResize(UINT width, UINT height)
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

void RenderWindow::onLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	D2D1_POINT_2F gridPos = mousePos2GridPos(pixelX, pixelY);
	switch(mEditMode)
	{
	case AddNode:
		{
			//If the click point is within a valid node, do nothing.
			if (mGraph.queryNodeByPos(gridPos) != NULL)
				return;
			mGraph.addNode(gridPos);
		}
		break;

	case AddEdge:
		{
			//If the click point is not within a valid node, do nothing.
			if (mGraph.queryNodeByPos(gridPos) == NULL)
				return;
			mActiveData.activeEdgeStart.x = gridPos.x;
			mActiveData.activeEdgeStart.y = gridPos.y;
			mActiveData.isActive = true;
		}
		break;

	case SetSrc:
		{
			//If the click point is not within a valid node, do nothing.
			Node* pNode = mGraph.queryNodeByPos(gridPos);
			if ( pNode == NULL)
				return;
			mActiveData.srcNode = pNode->mId;
		}
		break;

	case SetDst:
		{
			//If the click point is not within a valid node, do nothing.
			Node* pNode = mGraph.queryNodeByPos(gridPos);
			if ( pNode == NULL)
				return;
			mActiveData.dstNode = pNode->mId;
		}
		break;

	default:
		break;
	}
	InvalidateRect(mHwnd, NULL, FALSE);
}

void RenderWindow::onLButtonUp( int pixelX, int pixelY, DWORD flags )
{
	if (mEditMode == AddEdge)
	{
		D2D1_POINT_2F gridPos = mousePos2GridPos(pixelX, pixelY);
	
		Node* end = mGraph.queryNodeByPos(gridPos);
		Node* start = mGraph.queryNodeByPos(mActiveData.activeEdgeStart);

		//If the end point is not within a valid node, or if the end point is the start point,
		//then clear the active edge and repaint the scene.
		if ( end == NULL || end == start)
		{
			mActiveData.isActive = false;
			mActiveData.clearActiveEdge();
			InvalidateRect(mHwnd, NULL, FALSE);
		}
		else if (end != NULL)
		{
			mGraph.addUndirectedEdge(start->mId, end->mId);

			mActiveData.isActive = false;
			mActiveData.clearActiveEdge();
			InvalidateRect(mHwnd, NULL, FALSE);
		}
	}
	InvalidateRect(mHwnd, NULL, FALSE);
}

void RenderWindow::onMouseMove( int pixelX, int pixelY, DWORD flags )
{
	//Update the edge's end point as user drags the mouse when AddEdge is enabled.
	if (mEditMode == AddEdge && mActiveData.isActive)
	{
		D2D1_POINT_2F gridPos = mousePos2GridPos(pixelX, pixelY);
		mActiveData.activeEdgeEnd.x = gridPos.x;
		mActiveData.activeEdgeEnd.y = gridPos.y;
		InvalidateRect(mHwnd, NULL, FALSE);
	}	
}

LRESULT RenderWindow::handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
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

	case WM_LBUTTONUP:
		onLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
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
HRESULT RenderWindow::render()
{
	mpRenderTarget->BeginDraw();

	drawAll();
	//drawClock();
	
	return mpRenderTarget->EndDraw();
}

void RenderWindow::drawClockHand( float handLength, float angle, float strokeWidth )
{
	const D2D1_COLOR_F colorTemp = mpBrush->GetColor();
	
	mpRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, mEllipse.point));
	D2D_POINT_2F endPoint = D2D1::Point2F(mEllipse.point.x, mEllipse.point.y-(mEllipse.radiusY*handLength));

	mpBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
	mpRenderTarget->DrawLine(mEllipse.point, endPoint, mpBrush, strokeWidth, mpStroke);
	mpBrush->SetColor(colorTemp);
}

void RenderWindow::drawClock()
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

void RenderWindow::drawAll()
{
	if (mpRenderTarget != NULL)
	{
		mpRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		
		//Draw grid
		D2D1_SIZE_F size = mpRenderTarget->GetSize();
		UINT width = static_cast<UINT>(size.width);
		UINT height = static_cast<UINT>(size.height);
		for (UINT x = 0; x < width; x += mGridSize)
		{
			mpRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<float>(x), 0.f), 
				D2D1::Point2F(static_cast<float>(x), size.height),
				mpGrayBrush);
		}
		for (UINT y = 0; y < height; y += mGridSize)
		{
			mpRenderTarget->DrawLine(
				D2D1::Point2F(0.f,  static_cast<float>(y)), 
				D2D1::Point2F(size.width, static_cast<float>(y)),
				mpGrayBrush);
		}

		//Draw active edge if any.
		if (mActiveData.isActive && mActiveData.calcDistance() > 1.f)
		{
			mpRenderTarget->DrawLine(mActiveData.activeEdgeStart, mActiveData.activeEdgeEnd, mpBlueBrush, 5.f );

			WCHAR text[20] = L"";
			_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mActiveData.calcDistance());
			mpRenderTarget->DrawTextW(text, ARRAYSIZE(text) - 1, mpDWriteTextFormat, &(mActiveData.calcTextRegion()),mpBlackBrush);
		}
		
		//Draw previously added edges.
		for (unsigned int i = 0; i < mGraph.mUndirectedEdges.size(); ++i)
		{
			Edge* pEdge = mGraph.mUndirectedEdges[i];
			Node* src = mGraph.queryNodeById(pEdge->mSrcId);
			Node* dst = mGraph.queryNodeById(pEdge->mDstId);

			mpRenderTarget->DrawLine(src->mPosition, dst->mPosition, mpBlueBrush, 5.f );

			WCHAR text[20] = L"";
			_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mActiveData.calcDistance(src->mPosition, dst->mPosition));
			mpRenderTarget->DrawTextW(
				text, ARRAYSIZE(text) - 1, mpDWriteTextFormat, 
				&(mActiveData.calcTextRegion(src->mPosition, dst->mPosition)),
				mpBlackBrush);
		}

		//Draw nodes
		UINT nodeSize = mGridSize/2 - 2;
		for (unsigned int i = 0; i < mGraph.numOfNode(); ++i)
		{
			D2D1_POINT_2F nodePos = mGraph.queryNodeById(i)->mPosition;
			D2D1_RECT_F nodeRect = 
				D2D1::RectF(
				nodePos.x-nodeSize, nodePos.y - nodeSize, 
				nodePos.x + nodeSize, nodePos.y + nodeSize);

			//Select brush for different node types
			ID2D1SolidColorBrush* pBrushSelector;
			if (i == mActiveData.srcNode)
				pBrushSelector = mpGreenBrush;
			else if(i == mActiveData.dstNode)
				pBrushSelector = mpRedBrush;
			else
				pBrushSelector = mpCyanBrush;

			//Draw node
			mpRenderTarget->FillRoundedRectangle(
				&(D2D1::RoundedRect(nodeRect, 5.f, 5.f)),
				pBrushSelector);
			
			//Draw text
			WCHAR text[20] = L"";
			ZeroMemory(text, ARRAYSIZE(text));
			_snwprintf(text, ARRAYSIZE(text), L"%d", i);
			mpRenderTarget->DrawTextW(text, ARRAYSIZE(text) -1, mpDWriteTextFormat, nodeRect, mpWhiteBrush);
		}
	}	
}

D2D1_POINT_2F RenderWindow::mousePos2GridPos( int pixelX, int pixelY )
{
	D2D1_POINT_2F mouseDPIs = DPIScale::PixelsToDips(pixelX, pixelY);
	int nx= (int)mouseDPIs.x / mGridSize;
	int ny = (int)mouseDPIs.y / mGridSize;

	D2D1_POINT_2F gridPos;
	gridPos.x = static_cast<FLOAT>(nx * mGridSize + mGridSize / 2);
	gridPos.y = static_cast<FLOAT>(ny * mGridSize + mGridSize / 2);
	return gridPos;
}




