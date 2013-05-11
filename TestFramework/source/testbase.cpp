#include "testbase.h"

TestBase::TestBase():mAlias("default_test"), mTouched(false)
{

}

TestBase::~TestBase()
{

}

void TestBase::doAssertion( bool expr, string exprString, string srcFile, unsigned int lineNum, bool mute )
{
	TestAssertion* asst = new TestAssertion();
	asst->mStatus = expr;
	asst->mExprString = exprString;
	asst->mSrcFile = srcFile;
	asst->mLineNum = lineNum;
	mAssertions.push_back(asst);
	if (!asst->mStatus)
	{
		cout<<mAlias<<"::Assertion \""<<exprString<<"\" FAILED"<<endl;
		cout<<"@Src:<"<<srcFile<<">"<<" Line<"<<lineNum<<">"<<endl<<endl;
	}
	else if (asst->mStatus && !mute)
	{
		cout<<mAlias<<"::Assertion \""<<exprString<<"\" PASSED"<<endl<<endl;
	}
}

bool TestBase::isPassed()
{
	if (mAssertions.empty())
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

