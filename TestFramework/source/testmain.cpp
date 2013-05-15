#include "testframework.h"

#include "sort.h"
#include "serialization.h"
#include "luabinding.h"

int main(int argc, char** argv)
{
	TestFramework tf;
// 	tf.addTest<TestSerialization>(true);
 	tf.addTest<TestSort>(true);
//	tf.addTest<TestLuaBinding>(true);

	tf.startAll();
	
	system("pause");
	return 0;
}