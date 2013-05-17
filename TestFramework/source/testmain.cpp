#include "testframework.h"
#include "testalgo.h"

int main(int argc, char** argv)
{
	TestFramework testfrmk;
// 	tf.addTest<TestSerialization>(true);
	testfrmk.addTest<TestAlgo>(true);
	testfrmk.startAll();

	system("pause");
	return 0;
}