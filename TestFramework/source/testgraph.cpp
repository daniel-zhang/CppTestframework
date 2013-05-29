#include "testgraph.h"
#include "ipq.h"
#include "graph.h"

TestGraph::TestGraph()
{
	mAlias = "TestGraph";
}

TestGraph::~TestGraph(){}

void TestGraph::run()
{
	//A rough test on IPQ
	int data[] = {8,5,2,13,15,4,7,9};
	vector<int> costs;
	for (unsigned int i = 0; i < 8; ++i)
		costs.push_back(data[i]);

// 	IndexedPriorityQueue<int> iPQ;
// 	iPQ.init(&costs);
// 	iPQ.buildHeap();
// 	
// 	__assert__(iPQ.isInQueue(2));
// 	__assert__( 2 == costs[iPQ.popMin()] );
// 	__assert__(!iPQ.isInQueue(2));//data[2] is already popped out.
// 
// 	__assert__( iPQ.decreaseKey(1,3)); //data[1] = 5, decrease to 3.
// 	__assert__( 3 == costs[iPQ.popMin()] );//Heap top should be 3 now.
// 	__assert__(!iPQ.decreaseKey(1,1));//Deny to decreasing an item's key if it is out of the heap.

	//Test of Prim
	Graph graph;
	for (int i = 0; i < 6; ++i)
		graph.addNode();

// 	costs.clear();
// 	for (unsigned int i = 0; i < 8; ++i)
// 		costs.push_back(data[i]);

	graph.addEdge(0, 1, costs[0]);
	graph.addEdge(1, 2, costs[1]);
	graph.addEdge(2, 3, costs[2]);
	graph.addEdge(3, 4, costs[3]);
	graph.addEdge(4, 5, costs[4]);
	graph.addEdge(5, 0, costs[5]);
	graph.addEdge(4, 1, costs[6]);
	graph.addEdge(4, 2, costs[7]);

	//Init the mst
	for (unsigned int i = 0; i < graph.numOfNode(); ++i)
	{
		mst.push_back(vector<int>());
	}
	int startNodeId = 1;
	graph.primMST(startNodeId, mst);
	//dump
	treeWalker(startNodeId);
}

void TestGraph::treeWalker(int start)
{
	static int counter = 0;
	if (counter == 0)
		cout<<"+"<<start<<endl;
	counter +=2 ;
	for (unsigned int i = 0; i < mst[start].size(); ++i)
	{
		for (int j = 0; j < counter; ++j)
			cout<<" ";
		cout<<"+"<<mst[start][i]<<endl;
		treeWalker(mst[start][i]);
	}
	counter -= 2;
}
