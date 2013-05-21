#ifndef _GRAPH_H
#define _GRAPH_H

#include "testbase.h"
#include <vector>
#include <list>

using namespace std;

/**Fundamental Data Structures for Graph
*/
enum _NodeType
{
	Invalid_Node_Index = -1,
};

class GraphNode
{
public:
	GraphNode():mIndex(Invalid_Node_Index){}
	GraphNode(int idx):mIndex(idx){}
	virtual ~GraphNode(){}

	//Getters & setters
	int getIndex(){return mIndex;}
	void setIndex(int idx){mIndex = idx;}
protected:
	int mIndex;
};

class GraphEdge
{
public:
	GraphEdge() : mSrc(Invalid_Node_Index), mDst(Invalid_Node_Index), mCost(1.0){}
	GraphEdge(int from, int to) : mSrc(from),mDst(to),mCost(1.0){}
	GraphEdge(int from, int to, double cost) : mSrc(from), mDst(to), mCost(cost){}
	virtual ~GraphEdge(){}

	//Getters & setters
	int getSrc(){return mSrc;}
	void setSrc(int from){mSrc = from;}

	int getDst(){return mDst;}
	void setDst(int to){mDst = to;}

	double getCost(){return mCost;}
	void setCost(double cost){mCost = cost;}

protected:
	int mSrc;
	int mDst;
	double mCost;
};

/**A non-directed, connected sparse graph. We do not need a base class for the time being.
*/
template<typename Node_Type, typename Edge_Type>
class SparseGraph
{
public:
	SparseGraph() :  mNextNodeIndex(0){}
	SparseGraph(bool isDirected) : mIsDirected(isDirected), mNextNodeIndex(0){}
	~SparseGraph()
	{
		;
	}
	
	void addNode()
	{
		int pos = mNodes.size();
		mNodes.push_back(new Node_Type(pos));
		//Add an anonymous empty adjacent list
		mAdjEdges.push_back(AdjList());
	}

	void addEdge(int from, int to, double cost)
	{
		Edge_Type* pEdge1 = new Edge_Type(from, to, cost);
		mAdjEdges[from].push_back(pEdge1);

		Edge_Type* pEdge2 = new Edge_Type(to, from, cost);
		mAdjEdges[to].push_back(pEdge2);
	}	

	Node_Type* getNode(int idx)
	{
		if (idx < mNodes.size())
			return mNodes[idx];
		else 
			return NULL;
	}

	Edge_Type* getEdge(int from, int to)
	{
		for ( int i = mAdjEdges[from].begin(); i < mAdjEdges[from].end(); ++i )
		{
			if (mAdjEdges[from][i]->getDst() == to)
				return mAdjEdges[from][i];
		}
		return NULL;
	}

	int numNodes()
	{
		return mNodes.size();
	}

	int numEdges()
	{
		int counter = 0;
		for (int i = 0; i < mAdjEdges.size(); ++i)
			counter += mAdjEdges[i].size();
		//Edges are referenced twice
		return counter/2;
	}

	bool isEmpty()
	{
		return mNodes.empty();
	}

	void dump()
	{
		for (unsigned int i = 0; i < mNodes.size(); ++i)
		{
			cout<<"Node ID: "<<mNodes[i]->getIndex()<<endl;	
			for(unsigned int j = 0; j < mAdjEdges[i].size(); ++j)
				cout<<mAdjEdges[i][j]->getSrc()<<" -> "<<mAdjEdges[i][j]->getDst()<<" Cost:"<<mAdjEdges[i][j]->getCost()<<endl;
			cout<<endl;
		}
	}

	bool save(){}
	bool load(){}

public:
	typedef vector<Node_Type*> NodeContainer;
	typedef vector<Edge_Type*> AdjList;
	typedef vector<AdjList> AdjEdgesContainer;

private:
	NodeContainer mNodes;
	AdjEdgesContainer mAdjEdges;
	int mNextNodeIndex;
};


class TestGraph : public TestBase
{
public:
	TestGraph();
	~TestGraph();
	virtual void run();
};
#endif