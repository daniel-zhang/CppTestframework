#ifndef _TEST_SORT_H
#define _TEST_SORT_H

#include "testbase.h"
#include "algo.h"
#include <Windows.h> //Use WIN timer for higher accuracy.

/** A test class for common algorithms.
*/
class TestAlgo: public TestBase
{
public:
	TestAlgo();
	~TestAlgo();

	virtual void run();

protected:
	///A simple timer for CPU tick, to be re-factored into the test base.
	void timerGo();
	double timerStop();
	double mFrequency;
	__int64 mPerfValue;

	///Random array generator.
	void randArray(int aSize);
	void showArray(int size);
	bool checkArray(int aSize);	
	int mRangeStart;
	int mRangeEnd;
	int mStep;
	int mMaxArraySize;
	int* mPtrArray;
};

#endif