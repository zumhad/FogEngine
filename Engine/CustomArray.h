#pragma once

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Core.h"

#include <vector>

template <typename T>
class Array
{
public:
	Array() : mArr() {}

	void Add(T t);
	int Size();
	void Clear();
	void Delete(int i);
	void Resize(int size);
	T* Data();
	const T* Data() const;

	Array& operator=(const Array& arr);
	const T& operator[] (int i) const;
	T& operator[] (int i);

private:
	std::vector<T> mArr;
};

template <typename T>
T* Array<T>::Data()
{
	return mArr.data();
}

template <typename T>
const T* Array<T>::Data() const
{
	return mArr.data();
}

template <typename T>
void Array<T>::Add(T t)
{
	mArr.push_back(t);
}

template <typename T>
int Array<T>::Size()
{
	return (int)mArr.size();
}

template <typename T>
void Array<T>::Clear()
{
	mArr.clear();
}

template <typename T>
void Array<T>::Resize(int size)
{
	mArr.resize(size);
}

template <typename T>
void Array<T>::Delete(int i)
{
	mArr.erase(mArr.begin() + i);
}

template <typename T>
const T& Array<T>::operator[] (int i) const
{
	return mArr[i];
}

template <typename T>
T& Array<T>::operator[] (int i)
{
	return mArr[i];
}

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& arr)
{
	mArr = arr.mArr;
	return *this;
}

#pragma warning(pop)