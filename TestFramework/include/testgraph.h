#ifndef _TEST_GRAPH_H
#define _TEST_GRAPH_H

#include "testbase.h"

class TestGraph : public TestBase
{
public:
	TestGraph();
	~TestGraph();
	virtual void run();

private:
	void treeWalker(int start);
	vector<vector<int>> mst;
};

#endif