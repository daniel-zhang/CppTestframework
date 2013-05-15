#ifndef _SORT_H
#define _SORT_H

#include "testbase.h"

#include <string>
#include <Windows.h> //Use the WIN timer, let's be dependent on this platform.
using namespace std;

/** A test class for common sorting algorithms
*/
class TestSort: public TestBase
{
public:
	TestSort();
	~TestSort();

	virtual void run();

	/**
	@retval The cost(i.e. how many moves are made)

	@Remarks
	Recursive sorting might cause stack overflow if the array is large enough(100,000)
	*/
	unsigned int quickSort(int* array, int arraySize);

	unsigned int bubbleSort(int* array, int arraySize);

	unsigned int insertSort(int* array, int arraySize);

	unsigned int selectSort(int* array, int arraySize);

	unsigned int mergeSort(int* array, int arraySize);

protected:
	///Recursive sub-routines for merge/quick/heap sorting
	void _quickSort(int* array, int start, int end, unsigned int* cost);
	void merge(int* array, int left, int mid, int right, int* tmp, unsigned int* cost);
	void divide(int* array, int left, int right, int* tmp, unsigned int* cost);

	///A simple profiler for CPU tick, to be re-factored into the test base
	void timerGo();
	double timerStop();
	double mFreq;
	__int64 mPerfValue;

	///Generate a random INT array
	void randArray(int aSize);
	void showArray();
	bool checkArray(int aSize);	
	int mRandStart;
	int mRandEnd;
	int mMaxArraySize;
	int* mPtrArray;
};

#endif