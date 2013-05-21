#include "graph.h"

TestGraph::TestGraph()
{
	mAlias = "TestGraph";
}

TestGraph::~TestGraph()
{

}

void TestGraph::run()
{
 	SparseGraph<GraphNode, GraphEdge> sparseGraph;
	
	for (int i = 0; i < 5; ++i)
	{
		sparseGraph.addNode();
	}
	sparseGraph.addEdge(0,4,4);
	sparseGraph.addEdge(0,2,5);
	sparseGraph.addEdge(1,2,7);
	sparseGraph.addEdge(1,4,3);
	sparseGraph.addEdge(1,3,6);
	sparseGraph.addEdge(2,3,9);
 
 	sparseGraph.dump();
}
