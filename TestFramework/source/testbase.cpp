#include "testbase.h"

TestBase::TestBase()
{
	mMute = true;
	mAlias = "default_test";
	mTouched = false;
}

TestBase::~TestBase()
{

}

void TestBase::doAssertion( bool expr, string exprString, string srcFile, unsigned int lineNum )
{
	TestAssertion* asst = new TestAssertion();
	asst->mStatus = expr;
	asst->mExprString = exprString;
	asst->mSrcFile = srcFile;
	asst->mLineNum = lineNum;
	mAssertions.push_back(asst);
	if (!asst->mStatus)
	{
		setConsoleColor(FOREGROUND_GREEN|FOREGROUND_RED);
		cout<<"Assertion:\"";
		cout<<exprString;
		cout<<"\"";
		
		setConsoleColor(FOREGROUND_RED);
		cout<<"FAILED"<<endl;
		
		setConsoleColor(FOREGROUND_GREEN|FOREGROUND_RED);
		cout<<"@Src:<"<<srcFile<<">"<<" Line<"<<lineNum<<">"<<endl;
		setConsoleDefaultColor();
	}
	else if (asst->mStatus && !mMute)
	{
		setConsoleColor(FOREGROUND_GREEN|FOREGROUND_RED);
		cout<<"Assertion:\"";
		cout<<exprString;
		cout<<"\"";
		setConsoleColor(FOREGROUND_GREEN);
		cout<<" PASSED"<<endl;
		setConsoleDefaultColor();
	}
}

bool TestBase::isPassed()
{
	if (!isTouched())
		return false;

	bool passed = true;
	vector<TestAssertion*>::const_iterator iter;
	for (iter = mAssertions.begin(); iter != mAssertions.end(); ++iter)
	{
		passed &= (*iter)->mStatus;
	}

	return passed;
}

bool TestBase::isTouched()
{
	return mTouched;
}

void TestBase::setMute( bool m )
{
	mMute = m;
}


BOOL setConsoleColor( WORD attributes )
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	
	return SetConsoleTextAttribute(hConsole, attributes);
}

BOOL setConsoleDefaultColor()
{
	return setConsoleColor(FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
}
