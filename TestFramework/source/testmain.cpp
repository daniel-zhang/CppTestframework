#include "testframework.h"
#include "testalgo.h"
#include "graph.h"

#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

//test vector slicing
class testBar
{
public:
	int i;
	testBar(){i = 10;}
};
class testFoo : public testBar
{
public:
	int j;
	testFoo()
	{
		j = 20;
	}
};

int main(int argc, char** argv)
{
 	TestFramework testfrmk;
//  
// 	testfrmk.addTest<TestAlgo>(true);
	testfrmk.addTest<TestGraph>(true);
 	testfrmk.startAll();



	system("pause");
	return 0;
}