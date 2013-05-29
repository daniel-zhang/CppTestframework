#include "testbase.h"
#include "testframework.h"

TestFramework::TestFramework()
{

}

TestFramework::~TestFramework()
{

}

void TestFramework::startAll()
{
	evaluate();
	vector<TestBase*>::const_iterator iter;
	for (iter = mTestRegistry.begin(); iter != mTestRegistry.end(); ++iter)
	{
		setConsoleColor(FOREGROUND_BLUE|FOREGROUND_GREEN);
		cout<<"--"<<(*iter)->mAlias<<": BEGIN--"<<endl;
		setConsoleDefaultColor();

		(*iter)->run();
		this->touch((*iter));

		setConsoleColor(FOREGROUND_BLUE|FOREGROUND_GREEN);
		cout<<"--"<<(*iter)->mAlias<<": END--"<<endl<<endl;
		setConsoleDefaultColor();
	}
	evaluate();
}

void TestFramework::evaluate()
{
	unsigned int total = mTestRegistry.size();
	unsigned int touched = 0;
	unsigned int passed = 0;

	vector<TestBase*>::const_iterator iter;
	for (iter = mTestRegistry.begin(); iter != mTestRegistry.end(); ++iter)
	{
		if ((*iter)->isPassed())
			++passed;
		if ((*iter)->isTouched())
			++touched;
	}
	setConsoleColor(FOREGROUND_GREEN);
	cout<<"Total("<<total<<") ";
	cout<<"Touched("<<touched<<") ";
	cout<<"PASSED("<<passed<<") ";

	if(touched - passed)
	{
		setConsoleColor(FOREGROUND_RED|FOREGROUND_INTENSITY);
		cout<<"FAILED("<<touched - passed<<")"<<endl<<endl;
		setConsoleDefaultColor();
	}
	else
	{
		cout<<endl<<endl;
		setConsoleDefaultColor();
	}
}

void TestFramework::touch( TestBase* testCase )
{
	testCase->mTouched = true;
}


