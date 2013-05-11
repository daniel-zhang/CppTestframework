#include "testbase.h"
#include "testframework.h"


TestFramework::TestFramework():mMute(true)
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
		cout<<"--"<<(*iter)->mAlias<<": BEGIN--"<<endl<<endl;
		(*iter)->run(mMute);
		cout<<"--"<<(*iter)->mAlias<<": END--"<<endl<<endl;
		this->touch((*iter)->mTouched);
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

	cout<<"Total("<<total<<") ";
	cout<<"Touched("<<touched<<") ";
	cout<<"Passed("<<passed<<")"<<endl<<endl;
}

void TestFramework::touch( bool& isTouched )
{
	isTouched = true;
}

void TestFramework::setMute()
{
	mMute = true;
}

