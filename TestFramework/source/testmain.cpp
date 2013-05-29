#include "testframework.h"
#include "testalgo.h"
#include "testgraph.h"
#include "testreflect.h"

#include <vector>
//#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
 	TestFramework testfrmk;

//	testfrmk.addTest<TestReflection>(true);
 	testfrmk.addTest<TestAlgo>(true);
//	testfrmk.addTest<TestGraph>(true);

	testfrmk.startAll();
	system("pause");
	return 0;
}