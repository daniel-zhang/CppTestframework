#ifndef _D2D_SAMPLE_H
#define _D2D_SAMPLE_H

#include "winroot.h"
#include <d2d1.h>
#include <dwrite.h>
#include <vector>
using namespace std;
//#pragma comment(lib, "d2d1")
//#pragma comment(lib, "dwrite")

template <typename T> 
void safeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		(*ppT) = NULL;
	}
}

class Edge
{
public:
	Edge()
	{
		inUse = false;
		clear();
	}

	void clear()
	{
		start = D2D1::Point2F(0.f, 0.f);
		end = D2D1::Point2F(0.f, 0.f);
	}

	//Calculate the Euclidean length between two points.
	float getLength()
	{
		float deltaX = max(start.x, end.x) - min(start.x, end.x);
		float deltaY = max(start.y, end.y) - min(start.y, end.y);
		return sqrt(deltaX*deltaX + deltaY*deltaY);
	}

	//Calculate a proper region of the edge for text display.
	D2D1_RECT_F getTextRect()
	{
		float deltaX = max(start.x, end.x) - min(start.x, end.x);
		float deltaY = max(start.y, end.y) - min(start.y, end.y);

		float centerX = min(start.x, end.x) + deltaX/2;
		float centerY = min(start.y, end.y) + deltaY/2;
		
		float xAdjust = (int)(log10(getLength())+3)*15.f;

		return D2D1::RectF(centerX - xAdjust/2, centerY -10.f, centerX + xAdjust/2, centerY+30.f);
	}

	D2D1_POINT_2F start;
	D2D1_POINT_2F end;
	bool inUse;
};

class Graph
{
public:
};

class D2dSample : public WinRoot<D2dSample>
{
private:
	//Device Independent Resources
	ID2D1Factory* mpFactory;
	ID2D1StrokeStyle* mpStroke;
	IDWriteFactory* mpDWriteFactory;
	IDWriteTextFormat* mpDWriteTextFormat;	
	
	//Device Dependent Resources
	ID2D1HwndRenderTarget* mpRenderTarget;
	ID2D1SolidColorBrush* mpBrush;
	ID2D1SolidColorBrush* mpGrayBrush;
	ID2D1SolidColorBrush* mpBlueBrush;
	ID2D1SolidColorBrush* mpCyanBrush;
	ID2D1SolidColorBrush* mpWhiteBrush;
	ID2D1SolidColorBrush* mpBlackBrush;

	//Data and operations, should be re-factored into a standalone class
	D2D1_ELLIPSE mEllipse;//Deprecated
	vector<D2D1_RECT_F> mNodes;
	Edge mCurrentEdge;
	vector<Edge> mEdges;

	BOOL isPointInNode(D2D1_POINT_2F* pPoint);

	//Resource management
	void calcLayout();
	LRESULT init();
	void clearup();
	HRESULT createGraphicsResources();
	void discardGraphicsResources();

	//Msg handlers
	void onPaint();
	void onResize(UINT width, UINT height);
	void onLButtonDown(int pixelX, int pixelY, DWORD flags);
	void onRButtonDown(int pixelX, int pixelY, DWORD flags);
	void onRButtonUp( int pixelX, int pixelY, DWORD flags );
	void onMouseMove(int pixelX, int pixelY, DWORD flags);

private:
	HRESULT render();
	void drawGrid();

	//Deprecated
	void drawClockHand(float handLength, float angle, float strokWidth);
	void drawClock();

	enum tagGraphEditMode{AddLine, AddEdge, RemoveNode, RemoveEdge} mEditeMode;

public:
	D2dSample():mpFactory(NULL),mpRenderTarget(NULL),mpBrush(NULL){}
	
	virtual LRESULT  handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual PCWSTR  getClassName() const
	{
		return L"D2d Sample";
	}
};

#endif