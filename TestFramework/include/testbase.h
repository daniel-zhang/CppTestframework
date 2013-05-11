#pragma  once
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>

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
	virtual void run() = 0;

	void setMute(bool m);
protected:
	friend class TestFramework;
	void doAssertion(bool expr, string exprString, string srcFile, unsigned int lineNum);

	bool isPassed();
	bool isTouched();
	
	bool mMute;
	bool mTouched;
	string mAlias;
	vector<TestAssertion*> mAssertions;
};

#define  __assert__(expr) doAssertion(expr, #expr, __FILE__, __LINE__)

BOOL setConsoleColor(WORD attributes);
BOOL setConsoleDefaultColor();
