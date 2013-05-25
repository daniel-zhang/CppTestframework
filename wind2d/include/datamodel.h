#pragma once
#include <d2d1.h>
#include <vector>
using namespace std;

class Node
{
public:
	Node(int id, D2D1_POINT_2F& pos);

	int mId;
	D2D1_POINT_2F mPosition;
};

class Edge
{
public:
	Edge(int srcId, int dstId, double cost);

	int mSrcId;
	int mDstId;
	double mCost;
};

class Graph
{
public:
	Graph(){}
	~Graph(){}
	bool test()
	{
		return true;
	}

	bool addNode(D2D1_POINT_2F& pos);

	double calEuclideanDistance(int srcId, int dstId);

	bool addUndirectedEdge(int srcId, int dstId);

	void clear();

	Node* queryNodeByPos(D2D1_POINT_2F& pos);
	Node* queryNodeById(int id);

	Edge* queryUndirectedEdge(int srcId, int dstId);

	unsigned int numOfNode();
	unsigned int numOfUndirectedEdge();

	//A back-door for the renderer. 
	vector<Edge*> mUndirectedEdges; 

	vector<Edge*> PrimMST();

private:	
	vector<Node*> mNodes;
	vector<vector<Edge*>> mAdjList;

};