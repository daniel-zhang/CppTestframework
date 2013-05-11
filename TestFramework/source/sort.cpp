#include "sort.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

TestSort::TestSort():mArraySize(20000),mRandStart(0),mRandEnd(30000)
{
	mAlias = "Sorting Comparison";
	mPtrArray = new int[mArraySize];
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	mFreq = tmp.QuadPart;
}

TestSort::~TestSort()
{
	delete []mPtrArray;
}

void TestSort::randArray()
{	
	srand(int(time(0)));
	for (int iter = 0; iter < mArraySize; ++iter)
	{
		mPtrArray[iter] = rand()%(mRandEnd - mRandStart);
	}
}

void TestSort::showArray()
{
	cout<<"\"";
	for (int iter = 0; iter < mArraySize; ++iter)
		cout<< mPtrArray[iter] << " ";
	cout<<"\"\n\n";
}

//TODO: make sure the sorted array does not damage its items
bool TestSort::checkArray()
{
	//Check if the array is ascendant
	if (mArraySize == 1)
		return true;

	bool ret = true;
	for (int i = 0; i < mArraySize -1; ++i)
	{
		ret &= (mPtrArray[i] <= mPtrArray[i+1]);
		if(!ret) break;
	}
	return ret;
}

void TestSort::timerGo()
{
	LARGE_INTEGER v;
	QueryPerformanceCounter(&v);
	mPerfValue = v.QuadPart;
}

void TestSort::timerStop()
{
	LARGE_INTEGER v;
	QueryPerformanceCounter(&v);
	__int64 cur = v.QuadPart;
	cout<<(cur-mPerfValue)*1000/mFreq<<" ms";
}

#define Run_Test(sort)\
	cout<<#sort<<": ";\
	timerGo(); \
	cost = sort(mPtrArray, mArraySize); \
	timerStop();\
	cout<<" Cost: "<<cost<<endl;\
	__assert__(checkArray());\
	randArray();

void TestSort::run()
{
	randArray();
	cout<<"Raw Array["<<mArraySize<<"]:\n";
	unsigned int cost = 0;

	Run_Test(quickSort);
	Run_Test(mergeSort);

	Run_Test(insertSort);

	Run_Test(bubbleSort);
	Run_Test(selectSort);
	
}
unsigned int TestSort::quickSort( int* array, int arraySize )
{
	unsigned int cost = 0;
	_quickSort(mPtrArray, 0, mArraySize-1, &cost);
	return cost;
}

void TestSort::_quickSort( int* array, int start, int end, unsigned int* cost )
{
	if (start >= end) 
		return;

	int left = start, right = end, val = array[start];  
	while (left < right)  
	{  
		while(left < right && array[right] >= val)  
			--right;    
		if(left < right)   
		{
			(*cost)++;
			array[left++] = array[right];  
		}

		while(left < right && array[left] < val)  
			++left;    
		if(left < right)	
		{
			(*cost)++;
			array[right--] = array[left]; 
		}
	}  
	array[right] = val;  
	(*cost)++;
	_quickSort(array, start, left-1, cost);   
	_quickSort(array, left + 1, end, cost); 
	return;
}

unsigned int TestSort::bubbleSort( int* array, int arraySize )
{
	unsigned int swapCounter = 0;

	for (int i = 0; i < arraySize; ++i)
		for (int j = 1; j < arraySize - i; ++j)
			if (array[j-1] > array[j])
			{
				int tmp = array[j];
				array[j] = array[j-1];
				array[j-1] = tmp;
				++swapCounter;
				++swapCounter;
			}
	return swapCounter;
}

unsigned int TestSort::insertSort( int* array, int arraySize )
{
	unsigned int costCounter = 0;
	//sorted range: 0 ... (i-1)
	//unsorted range: i ... n
	int i, j, k;
	for (i = 1; i < arraySize; ++i)
	{
		for( j = i-1; j >=0; --j)
		{
			if (array[i] > array[j])
				break;
		}
		if (j < i -1)
		{
			int tmp = array[i];
			for (k = i - 1; k > j; --k)
			{
				array[k+1] = array[k];
				++costCounter;
			}
			array[j+1] = tmp;
			++costCounter;
		}
	}
	return costCounter;
}

unsigned int TestSort::selectSort( int* array, int arraySize )
{
	unsigned int costCounter = 0;
	int i, j;
	for (i = 0; i < arraySize; ++i)
	{
		for (j = i; j < arraySize; ++j)
		{
			if (array[j] < array[i])
			{
				int tmp = array[j];
				array[j] = array[i];
				costCounter++;
				array[i] = tmp;
				costCounter++;
			}
		}
	}
	return costCounter;
}

unsigned int TestSort::mergeSort(int* array, int arraySize)
{
	unsigned int cost = 0;
	int* p = new int[arraySize];
	divide(array, 0, arraySize-1, p, &cost);
	delete[] p;
	return cost;

}

void TestSort::merge( int* array, int left, int mid, int right, int* tmp, unsigned int* cost )
{
	int i = left, j = mid + 1;  
	int m = mid,   n = right;  
	int k = 0;  
	
	while (i <= m && j <= n)  
	{  
		if (array[i] <= array[j])  
		{
			tmp[k++] = array[i++];  
			++(*cost);
		}
		else
		{
			tmp[k++] = array[j++];  
			++(*cost);
		}
	}  

	while (i <= m)  
	{
		tmp[k++] = array[i++];  
		++(*cost);
	}

	while (j <= n)  
	{
		tmp[k++] = array[j++];  
		++(*cost);
	}

	for (i = 0; i < k; i++)  
	{
		array[left + i] = tmp[i];  
		++(*cost);
	}
}

void TestSort::divide( int* array, int left, int right, int* tmp, unsigned int* cost )
{
	if (left < right)
	{
		int mid = (left + right)/2;
		divide(array, left, mid, tmp, cost);
		divide(array, mid+1, right, tmp, cost);
		merge(array, left, mid, right, tmp, cost);
	}
}









