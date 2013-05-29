#include "datamodel.h"

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
	if (id < mNodes.size())
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
