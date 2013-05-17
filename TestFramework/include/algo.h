#ifndef _ALGO_H
#define _ALGO_H

/**
A max heap.
Too many sub methods, so put them into a class.
*/
class Heap
{
public:
	Heap();
	~Heap();
	void maxHeapSort(int* array, int arraySize);
	unsigned int mCost;
private:
	void buildMaxHeap(int* array, int arraySize);
	void maxHeapify(int* heap, int heapSize, int pos);
	int _parent(int i );
	int _left(int i );
	int _right(int i);
};

class AdjTable
{
public:
	AdjTable();
	~AdjTable();

};

///N(LogN) sort.
unsigned int quickSort(int* array, int arraySize);
unsigned int mergeSort(int* array, int arraySize);

///N^2 sort.
unsigned int bubbleSort(int* array, int arraySize);
unsigned int insertSort(int* array, int arraySize);
unsigned int selectSort(int* array, int arraySize);

/**
@param [in|out] counter The counter that keeps record of internal costs.
@retval The index of target item, or -1 if not found.
*/
int naiveSearch(int* array, int arraySize, int target, int* counter);
int binarySearch(int* array, int left, int right, int target, int* counter);

#endif