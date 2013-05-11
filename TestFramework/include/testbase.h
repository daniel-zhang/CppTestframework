#pragma  once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class TestFramework;

struct TestAssertion
{
	string mSrcFile;
	unsigned int mLineNum;

	bool mStatus;
	string mExprString;

	TestAssertion():mStatus(false){}
};

class TestBase
{
public:	
	TestBase();
	~TestBase();
	virtual void run(bool mute) = 0;

protected:
	friend class TestFramework;
	void doAssertion(bool expr, string exprString, string srcFile, unsigned int lineNum, bool mute);

	bool isPassed();
	bool isTouched();
	
	bool mTouched;
	string mAlias;
	vector<TestAssertion*> mAssertions;
};

#define  __assert__(expr) doAssertion(expr, #expr, __FILE__, __LINE__, mute)

