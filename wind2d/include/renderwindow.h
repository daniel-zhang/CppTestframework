#ifndef _RENDER_WINDOW_H
#define _RENDER_WINDOW_H

#include "winroot.h"
#include <d2d1.h>
#include <dwrite.h>
#include <vector>

#include "datamodel.h"

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

/**A miscellaneous helper that do the messy stuff.
*/
class ActiveData
{
public:
	ActiveData()
	{
		isActive = false;
		srcNode = -1;
		dstNode = -1;
		clearActiveEdge();
	}

	void clearActiveEdge()
	{
		activeEdgeStart = D2D1::Point2F(0, 0);
		activeEdgeEnd = D2D1::Point2F(0, 0);
	}

	//Calculate the Euclidean length between two points.
	double calcDistance()
	{
		return calcDistance(activeEdgeStart, activeEdgeEnd);
	}

	//Calculate a proper region of the edge for text display.
	D2D1_RECT_F calcTextRegion()
	{
		return calcTextRegion(activeEdgeStart, activeEdgeEnd);
	}

	double calcDistance(D2D1_POINT_2F& src, D2D1_POINT_2F& dst)
	{
		FLOAT deltaX = max(src.x, dst.x) - min(src.x, dst.x);
		FLOAT deltaY = max(src.y, dst.y) - min(src.y, dst.y);
		return sqrt(static_cast<double>(deltaX*deltaX + deltaY*deltaY));
	}

	D2D1_RECT_F calcTextRegion(D2D1_POINT_2F& src, D2D1_POINT_2F& dst)
	{
		//Calculate the coordinates of the edge center
		FLOAT deltaX = max(src.x, dst.x) - min(src.x, dst.x);
		FLOAT deltaY = max(src.y, dst.y) - min(src.y, dst.y);
		D2D1_POINT_2F center;
		center.x = min(src.x, dst.x) + deltaX/2;
		center.y = min(src.y, dst.y) + deltaY/2;

		//Construct the text region
		UINT regionWidth =static_cast<UINT>(log10(calcDistance(src, dst)) + 3)*15;
		return D2D1::RectF(center.x - regionWidth/2, center.y - 10, center.x + regionWidth/2, center.y + 30 );
	}

	D2D1_POINT_2F activeEdgeStart;
	D2D1_POINT_2F activeEdgeEnd;
	
	int srcNode;
	int dstNode;

	bool isActive;
};

class RenderWindow : public WinRoot<RenderWindow>
{
private:
	//D2D: Device Independent Resources
	ID2D1Factory* mpFactory;
	IDWriteFactory* mpDWriteFactory;
	IDWriteTextFormat* mpDWriteTextFormat;	
	ID2D1PathGeometry* mpArrowShape;
	
	//D2D: Device Dependent Resources.
	//If render target becomes invalid, these resources need to be re-created.
	ID2D1HwndRenderTarget* mpRenderTarget;
	ID2D1SolidColorBrush* mpGrayBrush;
	ID2D1SolidColorBrush* mpCornFlowerBlueBrush;
	ID2D1SolidColorBrush* mpCyanBrush;
	ID2D1SolidColorBrush* mpWhiteBrush;
	ID2D1SolidColorBrush* mpBlackBrush;
	ID2D1SolidColorBrush* mpRedBrush;
	ID2D1SolidColorBrush* mpBlueBrush;
	ID2D1SolidColorBrush* mpGreenBrush;

	//Manage device dependent resources.
	HRESULT createGraphicsResources();
	void discardGraphicsResources();

private:
	//Miscellaneous data
	ActiveData mActiveData;
	UINT mGridSize;
	Graph mGraph;
	enum eGraphEditMode{AddNode, AddEdge, SetSrc, SetDst} mEditMode;
	enum eAlgoOption{INVALIDE, Prim, Dijkstra, AStar} mAlgo, mAlgoDone;

	//Regulate mouse click position to the center of a grid.
	D2D1_POINT_2F mousePos2GridPos(int pixelX, int pixelY);
	
	//Let's render!
	HRESULT render();
	void drawAll();
	//Draw a customized arrow-line.
	void drawArrow( D2D1_POINT_2F point0, D2D1_POINT_2F point1, ID2D1Brush *brush, FLOAT strokeWidth = 1.0f);

	//Msg handlers
	LRESULT onCreate();
	void onDestroy();
	void onPaint();
	void onResize(UINT width, UINT height);
	void onLButtonDown(int pixelX, int pixelY, DWORD flags);
	void onLButtonUp( int pixelX, int pixelY, DWORD flags );
	void onMouseMove(int pixelX, int pixelY, DWORD flags);

public:
	RenderWindow():
		mpFactory(NULL),mpRenderTarget(NULL),mGridSize(40), 
		mEditMode(AddNode), mAlgo(INVALIDE), mAlgoDone(INVALIDE){}
	
	//Control interface
	void setAddNodeMode(){mEditMode = AddNode;}
	void setAddEdgeMode(){mEditMode = AddEdge;}
	void setSrcNode(){mEditMode = SetSrc;}
	void setDstNode(){mEditMode = SetDst;}

	void setPrim(){mAlgo = Prim;}
	void setDijkstra(){mAlgo = Dijkstra;}
	void setAStar(){mAlgo = AStar;}

	void execAlgo();
	void clearGraph();

	//Framework interface
	virtual LRESULT  handleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual PCWSTR  getClassName() const{return L"GraphDemo";}
};

#endif