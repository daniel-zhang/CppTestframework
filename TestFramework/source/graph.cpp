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
	Graph graph;
	graph.addNode();
	graph.addNode();
	graph.addNode();
	graph.addNode();

	graph.addEdge(0, 1);
	graph.addEdge(1, 2);
	graph.addEdge(2, 3);
	graph.addEdge(3, 1);
	graph.addEdge(3, 0);
	graph.addEdge(2, 0);

	cout<<"Num of Nodes:"<<graph.numOfNode()<<endl;
	cout<<"Num of Edges:"<<graph.numOfEdge()<<endl;
}
