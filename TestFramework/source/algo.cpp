#include "algo.h"

///Sub-routines for quick sort.
void _quickSort(int* array, int start, int end, unsigned int* cost);

///Sub-routines for merge sort.
void merge(int* array, int left, int mid, int right, int* tmp, unsigned int* cost);
void divide(int* array, int left, int right, int* tmp, unsigned int* cost);

HeapSort::HeapSort():mCost(0)
{

}

HeapSort::~HeapSort()
{

}

inline int HeapSort::_parent( int i )
{
	return i>0 ? (i-1)/2 : 0;
}

inline int HeapSort::_left( int i )
{
	return (i*2 + 1);
}

inline int HeapSort::_right( int i )
{
	return (i*2 + 2);
}

void HeapSort::maxHeapify( int* heap, int heapSize, int parent )
{
	int largest;
	if (_left(parent) < heapSize && heap[_left(parent)] > heap[parent])
		largest = _left(parent);
	else largest = parent;

	if (_right(parent) < heapSize && heap[_right(parent)] > heap[largest])
		largest = _right(parent);

	if (largest != parent)
	{
		int tmp = heap[parent];
		heap[parent] = heap[largest];
		heap[largest] = tmp;
		++mCost;++mCost;
		maxHeapify(heap, heapSize, largest);
	}
}

///Build a min heap from the bottom
void HeapSort::buildMaxHeap( int* array, int arraySize )
{
	for (int i = (arraySize)/2; i >= 0; --i)
	{
		maxHeapify(array, arraySize, i);
	}
}

void HeapSort::maxHeapSort( int* array, int arraySize )
{
	buildMaxHeap(array, arraySize);

	for (int i = arraySize - 1; i > 0; --i)
	{
		int tmp = array[i];
		array[i] = array[0];
		array[0] = tmp;
		++mCost;++mCost;
		maxHeapify(array, i , 0);
	}
}

unsigned int quickSort( int* array, int arraySize )
{
	if (arraySize < 0)
		return 0;

	unsigned int cost = 0;
	_quickSort(array, 0, arraySize-1, &cost);
	return cost;
}

void _quickSort( int* array, int start, int end, unsigned int* cost )
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

unsigned int bubbleSort( int* array, int arraySize )
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

unsigned int insertSort( int* array, int arraySize )
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

unsigned int selectSort( int* array, int arraySize )
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

unsigned int mergeSort(int* array, int arraySize)
{
	if (arraySize < 0)
		return 0;
	unsigned int cost = 0;
	int* p = new int[arraySize];
	divide(array, 0, arraySize-1, p, &cost);
	delete[] p;
	return cost;

}

void merge( int* array, int left, int mid, int right, int* tmp, unsigned int* cost )
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

void divide( int* array, int left, int right, int* tmp, unsigned int* cost )
{
	if (left < right)
	{
		int mid = (left + right)/2;
		divide(array, left, mid, tmp, cost);
		divide(array, mid+1, right, tmp, cost);
		merge(array, left, mid, right, tmp, cost);
	}
}

int binarySearch( int* array, int left, int right, int target, int* counter )
{
	if (left > right)
		return -1;
	else if (left == right)
	{
		if (target == array[left])
			return left;
		else	
			return -1;
	}
	else if(left < right)
	{
		++(*counter);
		int mid = left + (right - left)/2;
		if (target > array[mid])//Tail-recursion
			return binarySearch(array, mid+1, right, target, counter);
		else 
			return binarySearch(array, left, mid, target, counter);
	}

}

int naiveSearch( int* array, int arraySize, int target, int* counter )
{
	for (int i =0; i < arraySize; i++)
	{
		++(*counter);
		if (array[i] == target)
		{
			return i;
		}
	}
	return -1;
}
