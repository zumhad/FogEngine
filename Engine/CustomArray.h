#pragma once

#include "Core.h"
#include "Trace.h"

#pragma warning(disable : 4251)

template <typename T>
class Array
{
public:
	Array() {}
	~Array() { Clear(); }

	void Clear();
	void Add(T t);
	size_t Size() { return mSize; }

	T& operator[] (size_t i);

private:
	T* mArr = 0;
	size_t mSize = 0;
};



template <typename T>
void Array<T>::Clear()
{
	SAFE_DELETE_ARR(mArr);
	mSize = 0;
}

template <typename T>
void Array<T>::Add(T t)
{
	T* buffer = new T[++mSize];

	for (size_t i = 0; i < mSize - 1; i++)
	{
		buffer[i] = mArr[i];
	}

	buffer[mSize - 1] = t;

	SAFE_DELETE_ARR(mArr);
	mArr = buffer;
}

template <typename T>
T& Array<T>::operator[] (size_t i)
{
	return mArr[i];
}
