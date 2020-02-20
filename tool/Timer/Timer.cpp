#include "stdafx.h"

#include "Timer.h"

Timer::Timer(const std::function<void()> &callback, \
	const int timeInterval, const int accuracy)
{
	// 使用标准库的placement new，在构造函数中，第二次调用自己的构造函数，\
	于是在已经分配好的内存上执行，而不是分配新的内存
	new (this)Timer(accuracy);
	setTimer(callback, timeInterval);
}

bool Timer::setTimer(const std::function<void()> &callback, const int timeInterval)
{
	if (!bClosed)
		return false;
	bClosed = false;
	this->timeInterval = timeInterval;
	this->callback = callback;
	thread = std::thread(&Timer::execute, this);
	return !(bClosed = false);
}

void Timer::killTimer()
{
	bClosed = true;
	thread.join();
}

void Timer::execute()
{
	while (!bClosed)
	{
		auto begin = std::chrono::steady_clock::now();
		while (std::chrono::steady_clock::now() - begin < std::chrono::milliseconds(timeInterval))
			std::this_thread::sleep_for(std::chrono::milliseconds(accuracy));
		if (!bClosed)
			callback();
	}
}