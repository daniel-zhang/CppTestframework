#pragma once
#include "testbase.h"
#include <string>
#include <Windows.h>
using namespace std;
//For integer sequence
class TestSort: public TestBase
{
public:
	TestSort();
	~TestSort();

	virtual void run(bool mute);

	void quickSort(int* array, int start, int end, unsigned int* cost);

	unsigned int bubbleSort(int* array, int arraySize);

	unsigned int insertSort(int* array, int arraySize);

	unsigned int selectSort(int* array, int arraySize);

	unsigned int mergeSort(int* array, int arraySize);
	void merge(int* array, int left, int mid, int right, int* tmp, unsigned int* cost);
	void divide(int* array, int left, int right, int* tmp, unsigned int* cost);

protected:

	void timerGo();
	void timerStop();
	double mFreq;
	__int64 mPerfValue;

	int mRandStart;
	int mRandEnd;
	int mArraySize;
	int* mPtrArray;
	void randArray();
	void showArray();
	bool checkArray();	
};