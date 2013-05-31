#include "datamodel.h"
#include <limits>

Node::Node( int id, D2D1_POINT_2F& pos )
{
	mId = id;
	mPosition.x = pos.x;
	mPosition.y = pos.y;
}

Edge::Edge( int srcId, int dstId, double cost ) :mSrcId(srcId), mDstId(dstId), mCost(cost)
{

}

bool Graph::addNode( D2D1_POINT_2F& pos )
{
	//Make sure each node's position is unique.
	if (queryNodeByPos(pos) != NULL)
		return false;

	int nextPos = mNodes.size();
	mNodes.push_back(new Node(nextPos, pos));
	//Add anonymous adj list.
	mAdjList.push_back(vector<Edge*>());
	return true;
}

double Graph::calEuclideanDistance( int srcId, int dstId )
{
	Node* src = queryNodeById(srcId);
	Node* dst = queryNodeById(dstId);
	if (src != NULL && dst != NULL)
	{
		FLOAT deltaX = max(src->mPosition.x, dst->mPosition.x) - min(src->mPosition.x, dst->mPosition.x);
		FLOAT deltaY = max(src->mPosition.y, dst->mPosition.y) - min(src->mPosition.y, dst->mPosition.y);
		return sqrt((double)(deltaX*deltaX + deltaY*deltaY));
	}
	else
		return -1;
}

bool Graph::addUndirectedEdge( int srcId, int dstId )
{
	//Validate each node.
	if (queryNodeById(srcId) != NULL && queryNodeById(dstId) != NULL)
	{
		double cost = calEuclideanDistance(srcId, dstId);
		Edge* pEgde = new Edge(srcId, dstId, cost);
		mAdjList[srcId].push_back(pEgde);
		mAdjList[dstId].push_back(new Edge(dstId, srcId, cost));

		mUndirectedEdges.push_back(pEgde);
		return true;
	}
	return false;
}

void Graph::clear()
{
	//Clear mNodes
	unsigned int i, j;
	for (i = 0; i < mNodes.size(); ++i)
	{
		delete mNodes[i];
	}
	mNodes.clear();

	//Clear mAdjList;
	for (i = 0; i < mAdjList.size(); ++i)
	{
		for (j = 0; j < mAdjList[i].size(); ++j )
		{
			delete mAdjList[i][j];
		}

	}
	mAdjList.clear();
	mUndirectedEdges.clear();
	
	mst.clear();
	spt.clear();
}

Node* Graph::queryNodeByPos( D2D1_POINT_2F& pos )
{
	unsigned int i = 0;
	for (i; i < mNodes.size(); ++i)
	{
		bool xFlag = (mNodes[i]->mPosition.x == pos.x);
		bool yFlag = (mNodes[i]->mPosition.y == pos.y);
		if (xFlag && yFlag)
			return mNodes[i];
	}
	return NULL;
}

Node* Graph::queryNodeById( int id )
{
	if (static_cast<unsigned int>(id) < mNodes.size())
		return mNodes[id];
	else
		return NULL;
}

Edge* Graph::queryUndirectedEdge( int srcId, int dstId )
{
	unsigned int i ;
	for (i = 0; i < mAdjList[srcId].size(); ++i)
	{
		if (mAdjList[srcId][i]->mDstId == dstId)
			return mAdjList[srcId][i];
	}
	return NULL;
}

unsigned int Graph::numOfNode()
{
	return mNodes.size();
}

unsigned int Graph::numOfUndirectedEdge()
{
	unsigned int counter = 0;
	unsigned int i;
	for (i = 0; i < mAdjList.size(); ++i)
		counter += mAdjList[i].size();
	return counter/2;
}

void Graph::PrimMST( int startId )
{
	mst.clear();
	mst.assign(numOfNode(), NULL);

	vector<double> keys;
	keys.assign(numOfNode(), INT_MAX);

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

void Graph::DijkSPT( int startId, int endId )
{
	spt.clear();
	spt.assign(numOfNode(), NULL);

	vector<double> keys;
	keys.assign(numOfNode(), INT_MAX);

	IndexedPriorityQueue<double> iPQ;
	iPQ.init(&keys);
	iPQ.buildHeap();
	iPQ.decreaseKey(startId, 0);

	int curNode, frontierNode;
	double edgeCost;
	while(iPQ.getQueueSize() != 0)
	{
		curNode = iPQ.popMin();
		
		if (curNode == endId)
			return;

		for (unsigned int i = 0; i < mAdjList[curNode].size(); ++i)
		{
			frontierNode = mAdjList[curNode][i]->mDstId;
			edgeCost = mAdjList[curNode][i]->mCost;
			//Edge relax
			if (iPQ.isInQueue(frontierNode) && keys[frontierNode] > keys[curNode] + edgeCost)
			{
				iPQ.decreaseKey(frontierNode, keys[curNode] + edgeCost);
				spt[frontierNode] = mAdjList[curNode][i];
			}
		}

	}
}

void Graph::AStar( int startId, int endId )
{
	spt.clear();
	spt.assign(numOfNode(), NULL);

	vector<double> keys;
	keys.assign(numOfNode(), INT_MAX);

	IndexedPriorityQueue<double> iPQ;
	iPQ.init(&keys);
	iPQ.buildHeap();
	iPQ.decreaseKey(startId, calEuclideanDistance(startId, endId));

	int curNode, frontierNode;
	double edgeCost;
	while(iPQ.getQueueSize() != 0)
	{
		curNode = iPQ.popMin();

		if (curNode == endId)
			return;

		for (unsigned int i = 0; i < mAdjList[curNode].size(); ++i)
		{
			frontierNode = mAdjList[curNode][i]->mDstId;
			edgeCost = mAdjList[curNode][i]->mCost;
			double heuristic = calEuclideanDistance(frontierNode, endId);
			//Edge relax
			if (iPQ.isInQueue(frontierNode))
			{
				double costToCurNode = keys[curNode] - calEuclideanDistance(curNode, endId);
				if (keys[frontierNode] > costToCurNode + edgeCost + heuristic)
				{
					iPQ.decreaseKey(frontierNode, costToCurNode + edgeCost + heuristic);
					spt[frontierNode] = mAdjList[curNode][i];
				}
			}	
			
		}
	}
}
