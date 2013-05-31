#include "renderwindow.h"
#include <WindowsX.h>

/**Helper class to retrieve the desktop DPI settings*/
class DPIScale
{
private:
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

LRESULT RenderWindow::onCreate()
{
	//Create D2D factory
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpFactory)))
		return -1;
	
	//Create direct write factory
	if (FAILED(DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED, 
					__uuidof(mpDWriteFactory), 
					reinterpret_cast<IUnknown**>(&mpDWriteFactory)
				)))
		return -1;

	//Create a simple text format
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
	//Then set the text format as center aligned
	mpDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mpDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	//Create an arrow shape(a D2D1::PathGeometry)
	if (FAILED(mpFactory->CreatePathGeometry(&mpArrowShape)))
		return -1;
	ID2D1GeometrySink* pSink = NULL;
	if (FAILED(mpArrowShape->Open(&pSink)))
		return -1;
	pSink->BeginFigure(D2D1::Point2F(0,0), D2D1_FIGURE_BEGIN_FILLED);
	pSink->AddLine(D2D1::Point2F(20.f,10.f));
	pSink->AddLine(D2D1::Point2F(15.f,0.f));
	pSink->AddLine(D2D1::Point2F(20.f,-10.f));
	pSink->AddLine(D2D1::Point2F(0.f,0.f));
	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();
	safeRelease(&pSink);

	//Get DPI settings
	DPIScale::Initialize(mpFactory);

	//Now let's show
	ShowWindow( mHwnd, SW_SHOWNORMAL );
	return 0;
}

void RenderWindow::onDestroy()
{
	//Release device dependent D2D resources.
	discardGraphicsResources();

	//Release device independent D2D resources.
	safeRelease(&mpFactory);
	safeRelease(&mpDWriteFactory);
	safeRelease(&mpDWriteTextFormat);
	safeRelease(&mpArrowShape);
}

HRESULT RenderWindow::createGraphicsResources()
{
	HRESULT hr = S_OK;
	if (mpRenderTarget == NULL)
	{
		//Create render target
		RECT rc;
		GetClientRect(mHwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = mpFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mpRenderTarget
			);

		//Create a bunch of brushes
		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray), &mpGrayBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &mpCornFlowerBlueBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkCyan ), &mpCyanBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White ), &mpWhiteBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black ), &mpBlackBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue ), &mpBlueBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red ), &mpRedBrush);

		if (SUCCEEDED(hr))
			hr = mpRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green ), &mpGreenBrush);
	}
	return hr;
}

void RenderWindow::discardGraphicsResources()
{
	safeRelease(&mpRenderTarget);
	safeRelease(&mpGrayBrush);
	safeRelease(&mpCornFlowerBlueBrush);
	safeRelease(&mpCyanBrush);
	safeRelease(&mpWhiteBrush);
	safeRelease(&mpBlackBrush);
	safeRelease(&mpBlueBrush);
	safeRelease(&mpRedBrush);
	safeRelease(&mpGreenBrush);
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
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		mpRenderTarget->Resize(size);
		InvalidateRect(mHwnd, NULL, FALSE);//Inform the OS to post a WM_PAINT
	}
}

//Handle the simple input states.
void RenderWindow::onLButtonDown(int pixelX, int pixelY, DWORD flags)
{
	//Regulate every user-click to a grid's center.
	D2D1_POINT_2F gridPos = mousePos2GridPos(pixelX, pixelY);

	switch(mEditMode)
	{
	case AddNode:
		{
			//If click point is within a valid node, do nothing.
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

			//Update the active data to record user input.
			mActiveData.activeEdgeStart.x = gridPos.x;
			mActiveData.activeEdgeStart.y = gridPos.y;

			mActiveData.activeEdgeEnd.x = gridPos.x;
			mActiveData.activeEdgeEnd.y = gridPos.y;

			//Inform the renderer that active edge should be rendered on the next draw.
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
	//LButtonUp message is only captured in "AddEdge" mode.
	if (mEditMode == AddEdge)
	{
		D2D1_POINT_2F gridPos = mousePos2GridPos(pixelX, pixelY);
	
		Node* end = mGraph.queryNodeByPos(gridPos);
		Node* start = mGraph.queryNodeByPos(mActiveData.activeEdgeStart);

		if (end != NULL && start != NULL && end != start)
		{
			mGraph.addUndirectedEdge(start->mId, end->mId);

			mActiveData.isActive = false;
			mActiveData.clearActiveEdge();
			InvalidateRect(mHwnd, NULL, FALSE);
		}
		else
		{
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
		if (onCreate() == -1)
		{
			return -1;
		}
		break;

	case WM_DESTROY:
		onDestroy();
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		onPaint();
		return 0;

	case WM_SIZE:
		onResize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	
	case WM_SETCURSOR:
		//Change the cursor into a cross if in client area.
		if (LOWORD(lParam) == HTCLIENT)
			SetCursor(LoadCursor(NULL, IDC_CROSS));
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

HRESULT RenderWindow::render()
{
	mpRenderTarget->BeginDraw();

	drawAll();
	
	return mpRenderTarget->EndDraw();
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
			mpRenderTarget->DrawLine(mActiveData.activeEdgeStart, mActiveData.activeEdgeEnd, mpCornFlowerBlueBrush, 5.f );

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

			mpRenderTarget->DrawLine(src->mPosition, dst->mPosition, mpCornFlowerBlueBrush, 5.f );
		}

		//Draw graph research results.
		switch(mAlgoDone)
		{
		case Prim:
			for (unsigned int i = 0; i < mGraph.mst.size(); ++i)
			{
				//NULL represents node who has no parent node and thus no edge to paint.
				if (mGraph.mst[i] == NULL)
					continue;
				Node* src = mGraph.queryNodeById(mGraph.mst[i]->mSrcId);
				Node* dst = mGraph.queryNodeById(mGraph.mst[i]->mDstId);
				
				drawArrow(src->mPosition, dst->mPosition, mpRedBrush, 5.f);
			}
			break;

		case Dijkstra:
		case AStar:
			for (unsigned int i = 0; i < mGraph.spt.size(); ++i)
			{
				if (mGraph.spt[i] == NULL)
					continue;
				Node* src = mGraph.queryNodeById(mGraph.spt[i]->mSrcId);
				Node* dst = mGraph.queryNodeById(mGraph.spt[i]->mDstId);
				
				drawArrow(src->mPosition, dst->mPosition, mpRedBrush, 5.f);
			}
			break;

		default:
			break;
		}
		
		//Draw previously added edges' costs.
		for (unsigned int i = 0; i < mGraph.mUndirectedEdges.size(); ++i)
		{
			Edge* pEdge = mGraph.mUndirectedEdges[i];
			Node* src = mGraph.queryNodeById(pEdge->mSrcId);
			Node* dst = mGraph.queryNodeById(pEdge->mDstId);

			WCHAR text[20] = L"";
			//_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mActiveData.calcDistance(src->mPosition, dst->mPosition));
			_snwprintf(text,  ARRAYSIZE(text), L"%.1f", mGraph.mUndirectedEdges[i]->mCost);
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
				pBrushSelector = mpRedBrush;
			else if(i == mActiveData.dstNode)
				pBrushSelector = mpBlueBrush;
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

//Draw arrow using D2D transformation matrices.
void RenderWindow::drawArrow( D2D1_POINT_2F point0, D2D1_POINT_2F point1, ID2D1Brush *brush, FLOAT strokeWidth /*= 1.0f*/ )
{
	FLOAT rectHalfLen = static_cast<FLOAT>(mGridSize)/2 - 2;
	FLOAT angle =atan2(point0.y - point1.y, point0.x - point1.x);
	//Calculate the length between a grid center and arrow head(contact point).
	FLOAT len;
	FLOAT absTan  = tan(angle)>0 ? tan(angle) : -tan(angle);
	if (  absTan> 1)
		len = rectHalfLen/sin(angle);
	else
		len = rectHalfLen/cos(angle);
	len = len > 0 ? len : -len;

	D2D1_POINT_2F contactPoint, arrowNeck;		
	contactPoint.x = point1.x + len*cos(angle);
	contactPoint.y = point1.y + len*sin(angle);
	arrowNeck.x = point1.x + (len + 15)*cos(angle);
	arrowNeck.y = point1.y + (len + 15)*sin(angle);

	mpRenderTarget->DrawLine(point0, arrowNeck, brush, strokeWidth);

	ID2D1TransformedGeometry* pArrow = NULL;
	HRESULT hr = mpFactory->CreateTransformedGeometry(mpArrowShape,
		D2D1::Matrix3x2F::Rotation(angle*180.f/3.141592653589793, D2D1::Point2F(0.f, 0.f))
		*D2D1::Matrix3x2F::Translation(contactPoint.x, contactPoint.y),
		&pArrow);

	mpRenderTarget->DrawGeometry(pArrow, brush, 1.f);
	mpRenderTarget->FillGeometry(pArrow, brush);
	safeRelease(&pArrow);
}

void RenderWindow::execAlgo()
{
	switch(mAlgo)
	{
	case Prim:
		if (mActiveData.srcNode == -1)
		{
			MessageBox(mHwnd, L"Start node is not set.", L"Notice", MB_OK);
			return;
		}
		mGraph.PrimMST(mActiveData.srcNode);

		//Inform the renderer which data set should be represented.
		mAlgoDone = Prim;
		
		InvalidateRect(mHwnd, NULL, FALSE);
		break;

	case Dijkstra:
		if (mActiveData.srcNode == -1 )
		{
			MessageBox(mHwnd, L"Start node is not set.", L"Notice", MB_OK);
			return;
		}
		mGraph.DijkSPT(mActiveData.srcNode, mActiveData.dstNode);
		mAlgoDone = Dijkstra;
		InvalidateRect(mHwnd, NULL, FALSE);
		break;

	case AStar:
		if (mActiveData.srcNode == -1 )
		{
			MessageBox(mHwnd, L"Start node is not set.", L"Notice", MB_OK);
			return;
		}
		mGraph.AStar(mActiveData.srcNode, mActiveData.dstNode);
		mAlgoDone = AStar;
		InvalidateRect(mHwnd, NULL, FALSE);
		break;

	case INVALIDE:
		break;

	default:
		break;
	}
}

//A little bit messy, but that's it.
void RenderWindow::clearGraph()
{
	mActiveData.clearActiveEdge();
	mActiveData.isActive = false;
	mActiveData.srcNode = -1;
	mActiveData.dstNode = -1;
	mGraph.clear();
	InvalidateRect(mHwnd, NULL, FALSE);
}




