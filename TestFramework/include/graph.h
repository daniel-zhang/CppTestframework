#ifndef _GRAPH_H
#define _GRAPH_H
#include <vector>
using namespace std;

/**Fundamental Data Structures for Graph
*/
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

/**A non-directed/acyclic/connected graph, stored as an adjacent list.
*/
class Graph
{
public:
	Graph(){}
	void clearGraph();	
	void addNode();
	void addEdge(unsigned int from, unsigned int to, double cost);
	unsigned int numOfNode();
	unsigned int numOfEdge();
	void dump();
	void primMST(int startId, vector<vector<int>>& MST);

private:
	vector<Node*> mNodes;
	vector<vector<Edge*>> mAdjList;
};

#endif