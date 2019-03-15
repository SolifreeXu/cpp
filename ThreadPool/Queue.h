#pragma once

#include <list>
#include <mutex>
#include <atomic>

#define DEFAULT_UPPER_LIMIT 10000

template <typename Type>
class Queue
{
public:
	using size_type = typename std::list<Type>::size_type;
	Queue(size_type upperLimit = DEFAULT_UPPER_LIMIT);
	~Queue();
	const std::atomic<size_type> &size() const { return counter; }
	std::mutex &mutex() { return readMutex; }
	bool empty() const { return counter == 0; }
	bool push(Type &&data);
	bool push(std::list<Type> &data);
	Type &front();
	void pop();
protected:
	//size_type upperLimit;
	std::atomic<size_type> counter = 0;
	std::mutex writeMutex;
	std::mutex readMutex;
	std::list<Type> writeQueue;
	std::list<Type> readQueue;
};

template <typename Type>
Queue<Type>::Queue(size_type upperLimit)
{
	//if (upperLimit > 0)
	//	this->upperLimit = upperLimit;
}

template <typename Type>
Queue<Type>::~Queue()
{
	
}

template <typename Type>
bool Queue<Type>::push(Type &&data)
{
	std::lock_guard<std::mutex> writeLocker(writeMutex);
	//if (writeQueue.size() >= upperLimit)
	//	return false;
	writeQueue.push_back(std::forward<Type&&>(data));
	++counter;
	return true;
}

template <typename Type>
bool Queue<Type>::push(std::list<Type> &data)
{
	std::lock_guard<std::mutex> writeLocker(writeMutex);
	auto quantity = data.size();
	//if (writeQueue.size() + quantity >= upperLimit)
	//	return false;
	writeQueue.splice(writeQueue.cend(), data);
	counter += quantity;
	return true;
}

template <typename Type>
Type &Queue<Type>::front()
{
	if (readQueue.empty())
	{
		writeMutex.lock();
		readQueue.swap(writeQueue);
		writeMutex.unlock();
	}
	return readQueue.front();
}

template <typename Type>
void Queue<Type>::pop()
{
	--counter;
	readQueue.pop_front();
}
