#pragma once
#include <d2d1.h>
#include <vector>
#include "ipq.h"
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

	void PrimMST(int startId)
	{
		mst.clear();
		mst.assign(numOfNode(), NULL);

		vector<double> keys;
		keys.assign(numOfNode(), 10000);
		
		IndexedPriorityQueue<double> iPQ;
		iPQ.init(&keys);
		iPQ.buildHeap();
		iPQ.decreaseKey(startId, 0);

		int curNodeId, adjNodeId;
		double cost;
		while(iPQ.getQueueSize() != 0)
		{
			curNodeId = iPQ.popMin();
			for (unsigned int i = 0; i < mAdjList[curNodeId].size(); ++i)
			{
				adjNodeId = mAdjList[curNodeId][i]->mDstId;
				cost = mAdjList[curNodeId][i]->mCost;
				if (iPQ.isInQueue(adjNodeId) && cost < keys[adjNodeId])
				{
					iPQ.decreaseKey(adjNodeId, cost);
					mst[adjNodeId] = mAdjList[curNodeId][i];
				}
			}
		}
	}

	vector<Edge*> mst;
private:	
	vector<Node*> mNodes;
	vector<vector<Edge*>> mAdjList;


};