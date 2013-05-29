#ifndef _IPQ_H
#define _IPQ_H

#include <vector>
#include <iostream>
using namespace std;

/**A straight forward implementation of an indexed priority queue */
template<typename KeyType>
class IndexedPriorityQueue
{
public:
	void init(vector<KeyType>* keys)
	{
		mpKeys = keys;
		mHeapSize = mpKeys->size();

		for (unsigned int i = 0; i < mHeapSize; ++i)
		{
			mIndexHeap.push_back(i);
			mInverseIndex.push_back(i);
		}
	}

	void buildHeap()
	{
		for (int i = (mHeapSize)/2; i >= 0; --i)
			minHeapify(i);
	}
	
	void dump()
	{
		for (unsigned int i = 0; i < mHeapSize; ++i)
			cout<<mpKeys->at(mIndexHeap[i])<<" ";
		cout<<endl;
	}

	bool isInQueue(unsigned int idx)
	{
		unsigned int heapPos = mInverseIndex[idx];
		if (heapPos >= mHeapSize)
			return false;
		return true;
	}

	bool decreaseKey(unsigned int idx, KeyType new_key)
	{
		//Make sure new key is smaller.
		if (mpKeys->at(idx) <= new_key)
			return false;

		//Make sure the item is in the heap.
		unsigned int heapPos = mInverseIndex[idx];
		if (heapPos >= mHeapSize)
			return false;

		mpKeys->at(idx) = new_key;
		while(heapPos > 0 && mpKeys->at(mIndexHeap[_parent(heapPos)]) > mpKeys->at(mIndexHeap[heapPos]))
		{
			swap(heapPos, _parent(heapPos));
			heapPos = _parent(heapPos);
		}
		return true;
	}

	unsigned int getMin()
	{
		return mIndexHeap[0];
	}

	unsigned int popMin()
	{
		if (mHeapSize == 0 )
			return -1;
		unsigned int min = mIndexHeap[0];
		swap(0, --mHeapSize);
		minHeapify(0);
		return min;
	}

	unsigned int getQueueSize()
	{
		return mHeapSize;
	}
private:
	void swap(unsigned int i, unsigned  int j)
	{
		unsigned int tmp = mIndexHeap[i];
		mIndexHeap[i] = mIndexHeap[j];
		mIndexHeap[j] = tmp;

		mInverseIndex[mIndexHeap[i]] = i;
		mInverseIndex[mIndexHeap[j]] = j;
	}

	void minHeapify(unsigned int i)
	{
		unsigned int left, right, smallest;
		while(1)
		{
			left =  _left(i);
			right = _right(i);
			if ( left < mHeapSize && mpKeys->at(mIndexHeap[left]) < mpKeys->at(mIndexHeap[i]) )
				smallest = left;
			else
				smallest = i;
			if (right < mHeapSize && mpKeys->at(mIndexHeap[right]) < mpKeys->at(mIndexHeap[smallest]) )
				smallest = right;
			if (smallest == i)
				break;
			else
			{
				swap(i, smallest);
				i = smallest;
			}
		}
	}

	unsigned int _parent(unsigned int i )
	{
		return i>0? (i-1)/2: 0;
	}
	unsigned int _left(unsigned int i )
	{
		return 2*i+1;
	}
	unsigned int _right(unsigned int i)
	{
		return 2*i+2;
	}

private:
	unsigned int mHeapSize;
	vector<KeyType>* mpKeys;
	vector<unsigned int> mIndexHeap;
	vector<unsigned int> mInverseIndex;
};

#endif