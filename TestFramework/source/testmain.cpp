
#include <exception>
#include <iostream>
#include "testframework.h"

#include "sort.h"
#include "serialization.h"
using namespace std;

class TestBar : public TestBase
{
public:
	TestBar(){mAlias = "TestBar";}
	virtual void run(bool mute)
	{
		__assert__(true);
		__assert__("sss"==(char*)1);
	}
};

int main(int argc, char** argv)
{
	TestFramework tf;
//	tf.addTest<TestSerialization>();
	tf.addTest<TestSort>();
	tf.startAll();
	system("pause");
	return 0;
}