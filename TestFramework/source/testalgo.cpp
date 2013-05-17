#include "testalgo.h"
#include "luaplotter.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

///Comparison function for ANSI qsort()
int cmpInt(const void* i, const void* j)
{
	return *(int*)i - *(int*)j;
}
/**
mMaxArraySize and mStep determines how many iterations we are going to examine.
mRangeStart and mRangeEnd specify the range of random numbers.
Warning: do not set array size greater than 100,000, otherwise the stack might overflow.
*/
TestAlgo::TestAlgo():mMaxArraySize(50000),mStep(100),mRangeStart(0),mRangeEnd(10000)
{
	mAlias = "Sorting Comparison";
	mPtrArray = new int[mMaxArraySize];
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	mFrequency = tmp.QuadPart;
}

TestAlgo::~TestAlgo()
{
	delete []mPtrArray;
}

void TestAlgo::randArray(int aSize)
{	
	if (aSize> mMaxArraySize)
		return;

	srand(int(time(0)));
	for (int iter = 0; iter < aSize; ++iter)
	{
		mPtrArray[iter] = rand()%(mRangeEnd - mRangeStart);
	}
}

void TestAlgo::showArray(int size)
{
	cout<<"\"";
	for (int iter = 0; iter < size; ++iter)
		cout<< mPtrArray[iter] << " ";
	cout<<"\"\n\n";
}

//TODO: make sure the sorted array contains the same set of items with the original one.
bool TestAlgo::checkArray( int aSize )
{
	//Check if the array is ascendant
	if (aSize == 1)
		return true;

	bool ret = true;
	for (int i = 0; i < aSize -1; ++i)
	{
		ret &= (mPtrArray[i] <= mPtrArray[i+1]);
		if(!ret) break;
	}
	return ret;
}

void TestAlgo::timerGo()
{
	LARGE_INTEGER v;
	QueryPerformanceCounter(&v);
	mPerfValue = v.QuadPart;
}

double TestAlgo::timerStop()
{
	LARGE_INTEGER v;
	QueryPerformanceCounter(&v);
	__int64 cur = v.QuadPart;

	return (cur-mPerfValue)*1000.f/mFrequency;
}

void TestAlgo::run()
{
	LuaPlotter* plotter = new LuaPlotter("Simple Profiler");
	
	Plot* stepCmp = plotter->createPlot("数组大小", "Steps", "排序算法移动次数比较");
	Dataset* qsortStepData = stepCmp->createDataset("快速排序");
	Dataset* mergeStepData = stepCmp->createDataset("归并排序");
// 	Dataset* insertStepData = stepCmp->createDataset("Insert Sort");
// 	Dataset* bubbleStepData = stepCmp->createDataset("Bubble Sort");
	Dataset* heapStepData = stepCmp->createDataset("堆排序");

	Plot* timeCmp = plotter->createPlot("数组大小", "CPU Time(ms)", "排序算法耗时比较");
	Dataset* qsortTimeData = timeCmp->createDataset("快速排序");
	Dataset* mergeTimeData = timeCmp->createDataset("归并排序");
// 	Dataset* insertTimeData = timeCmp->createDataset("Insert Sort");
// 	Dataset* bubbleTimeData = timeCmp->createDataset("Bubble Sort");
	Dataset* ansi_qsortTimeData = timeCmp->createDataset("ANSI 快排");
	Dataset* heapTimeData = timeCmp->createDataset("堆排序");

	Plot* searchCmp = plotter->createPlot("数组大小", "搜索次数", "搜索算法比较");
	Dataset* bSearchData = searchCmp->createDataset("二分搜索");
	Dataset* nSearchData = searchCmp->createDataset("序列搜索");

	//Generate test data
	int times = mMaxArraySize/mStep;
	
	for (int i = 0; i <times; ++i)
	{
		int currentArraySize = mStep*(i);
		double runtime = 0;
		int steps = 0;

		//ANSI qsort
		randArray(currentArraySize);
		timerGo();
		qsort(mPtrArray, currentArraySize, sizeof(int), cmpInt);
		runtime = timerStop();
		ansi_qsortTimeData->addData(currentArraySize, runtime);
		__assert__(checkArray(currentArraySize));

		//Quick sort
		randArray(currentArraySize);
		timerGo();
		steps =  quickSort(mPtrArray, currentArraySize);
		runtime = timerStop();
		qsortStepData->addData(currentArraySize, steps);
		qsortTimeData->addData(currentArraySize, runtime);
		__assert__(checkArray(currentArraySize));

		//Merge Sort
		randArray(currentArraySize);
		timerGo();
		steps =  mergeSort(mPtrArray, currentArraySize);
		runtime = timerStop();
		mergeStepData->addData(currentArraySize, steps);
		mergeTimeData->addData(currentArraySize, runtime);
		__assert__(checkArray(currentArraySize));

		//(MAX)Heap sort
		randArray(currentArraySize);
		Heap heap;
		timerGo();
		heap.maxHeapSort(mPtrArray, currentArraySize);
		runtime = timerStop();
		heapTimeData->addData(currentArraySize, runtime);
		heapStepData->addData(currentArraySize, heap.mCost);
		heap.mCost = 0;
		__assert__(checkArray(currentArraySize));

		//Search comparison		
		if (currentArraySize > 0)
		{
			//Generate a random position as our target.
			int targetPos = rand()%currentArraySize;
			int counter = 0;
			int result = 0;
			result = naiveSearch(mPtrArray, currentArraySize, mPtrArray[targetPos], &counter);
			__assert__( mPtrArray[targetPos] ==  mPtrArray[result]);
			nSearchData->addData(currentArraySize, counter);
			
			counter = 0;
			result = binarySearch(mPtrArray, 0, currentArraySize - 1, mPtrArray[targetPos], &counter);
			__assert__( mPtrArray[targetPos]==  mPtrArray[result]);
 			bSearchData->addData(currentArraySize, counter);
		}
	 
		//O(N^2) Sorting algorithms quickly become slow as the array grows, so they are commented out by default.
		//Insert sort
// 		randArray(currentArraySize);
// 		timerGo();
// 		steps =  insertSort(mPtrArray, currentArraySize);
// 		runtime = timerStop();
// 		insertStepData->addData(currentArraySize, steps);
// 		insertTimeData->addData(currentArraySize, runtime);
// 		__assert__(checkArray(currentArraySize));
// 
		//Bubble sort
// 		randArray(currentArraySize);
// 		timerGo();
// 		steps =  bubbleSort(mPtrArray, currentArraySize);
// 		runtime = timerStop();
// 		bubbleStepData->addData(currentArraySize, steps);
// 		bubbleTimeData->addData(currentArraySize, runtime);
// 		__assert__(checkArray(currentArraySize));

		//Display a progress indicator
		double progress = ((double)(i+1)/(double)times)*100.0;
		printf("\r         %.lf%%\rRunning:", progress);
	}
	printf("\r         %.lf%%...OK\rRunning:\n", 100.0);
	
	plotter->display();
	delete plotter;
	plotter = NULL; 
}

