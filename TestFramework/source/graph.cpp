#include "graph.h"
#include "ipq.h"

void Graph::clearGraph()
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

void Graph::addNode()
{
	unsigned int nextId = mNodes.size();
	mNodes.push_back(new Node(nextId));
	mAdjList.push_back(vector<Edge*>());
}

void Graph::addEdge( unsigned int from, unsigned int to, double cost )
{
	if (from == to || from >= mNodes.size() || to >= mNodes.size())
		return;
	mAdjList[from].push_back(new Edge(from, to , cost));
	mAdjList[to].push_back(new Edge(to, from, cost));
}

unsigned int Graph::numOfNode()
{
	return mNodes.size();
}

unsigned int Graph::numOfEdge()
{
	unsigned int counter = 0;
	unsigned int i;
	for (i = 0; i < mAdjList.size(); ++i)
		counter += mAdjList[i].size();

	return counter/2;
}

void Graph::primMST( int startId, vector<vector<int>>& MST)
{
	vector<double> costs;
	costs.assign(numOfNode(), 1000);//Suppose 1000 is great enough.
	
	vector<int> flatTree;
	flatTree.assign(numOfNode(), -1);

	IndexedPriorityQueue<double> iPQ;
	iPQ.init(&costs);
	iPQ.buildHeap();
	iPQ.decreaseKey(startId, 0);

	unsigned int frontierNodeId, adjNodeId;
	double cost;
	while(iPQ.getQueueSize() != 0)
	{
		frontierNodeId = iPQ.popMin();
		for (unsigned int i = 0; i < mAdjList[frontierNodeId].size(); ++i)
		{
			adjNodeId = mAdjList[frontierNodeId][i]->mDstId;
			cost = mAdjList[frontierNodeId][i]->cost;
			if (iPQ.isInQueue(adjNodeId) && cost < costs[adjNodeId])
			{
				iPQ.decreaseKey(adjNodeId, cost);
				flatTree[adjNodeId] = frontierNodeId;
			}
		}
	}
	//dump
	for	(unsigned int i = 0; i < flatTree.size(); ++i)
		cout<<i<<"<<"<<flatTree[i]<<endl;

	//Turn the flat tree into a nice stand-up tree.
	for (unsigned int i = 0; i < flatTree.size(); ++i)
	{
		if (flatTree[i] == -1)
			continue;
		MST[flatTree[i]].push_back(i);
	}
}



