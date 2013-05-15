#include "testframework.h"
#include "sort.h"

int main(int argc, char** argv)
{
	TestFramework tf;
// 	tf.addTest<TestSerialization>(true);
 	tf.addTest<TestSort>(true);

	tf.startAll();
	
	system("pause");
	return 0;
}