#ifndef _GRAPH_H
#define _GRAPH_H

#include "testbase.h"
#include <vector>
#include <list>

#include <time.h>
#include <stdlib.h>

using namespace std;

/**Fundamental Data Structures for Graph
*/
class MSTNode
{
public:
	MSTNode(unsigned int id, unsigned int parentId)
		:mId(id),mParentId(parentId), mKey(-1)
	{}
	unsigned int mId;
	unsigned int mParentId;
	double mKey;
};

class IndexedPriorityQueue
{
public:
	void buildQueue();
	bool isInQueue(unsigned int idx);

};

class Node
{
public:
	Node(unsigned int id):mId(id){}
	unsigned int mId;
};

class Edge
{
public:
	Edge(int from, int to, double cost)
	:mSrcId(from), mDstId(to), cost(cost)
	{}
	unsigned int mSrcId;
	unsigned int mDstId;
	double cost;
};

/**A non-directed, connected sparse graph. We do not need a base class for the time being.
*/

class Graph
{
public:
	Graph(){}
	void clearGraph()
	{
		unsigned int i, j;
		for (i = 0; i < mNodes.size(); ++i)
		{
			delete mNodes[i];
			for ( j = 0; j < mAdjList[i].size(); ++j )
			{
				delete mAdjList[i][j];
			}
			mAdjList[i].clear();
		}
		mNodes.clear();
		mAdjList.clear();
	}
	
	void addNode()
	{
		unsigned int nextId = mNodes.size();
		mNodes.push_back(new Node(nextId));
		mAdjList.push_back(vector<Edge*>());
	}

	double randCost()
	{
		srand(int(time(0)));
		return static_cast<double>(rand()%(10));
	}

	void addEdge(unsigned int from, unsigned int to)
	{
		if (from == to || from >= mNodes.size() || to >= mNodes.size())
			return;

		double cost = randCost();
		mAdjList[from].push_back(new Edge(from, to , cost));
		mAdjList[to].push_back(new Edge(to, from, cost));
	}

	unsigned int numOfNode()
	{
		return mNodes.size();
	}

	unsigned int numOfEdge()
	{
		unsigned int counter = 0;
		unsigned int i;
		for (i = 0; i < mAdjList.size(); ++i)
			counter += mAdjList[i].size();

		return counter/2;
	}
	
	void PrimMST(unsigned int root)
	{

	}
private:
	vector<Node*> mNodes;
	vector<vector<Edge*>> mAdjList;

};


class TestGraph : public TestBase
{
public:
	TestGraph();
	~TestGraph();
	virtual void run();
};
#endif