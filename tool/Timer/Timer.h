#pragma once

#include <atomic>
#include <thread>

class Timer
{
	std::atomic_int timeInterval;
	std::atomic_int accuracy;
	bool bClosed;
	std::function<void()> callback;
	std::thread thread;
public:
	static const int DEFAULT_TIME_INTERVAL = 100;
	static const int DEFAULT_ACCUCARY = 10;
	Timer(const int accuracy = DEFAULT_ACCUCARY)
		:bClosed(true)
	{
		this->accuracy = accuracy;
	}
	Timer(const std::function<void()> &callback, \
		const int timeInterval = DEFAULT_TIME_INTERVAL, const int accuracy = DEFAULT_ACCUCARY);
	Timer(const Timer&) = delete;
	~Timer()
	{
		if (!bClosed)
			killTimer();
	}
	Timer& operator=(const Timer&) = delete;
	bool setTimer(const std::function<void()> &callback, const int timeInterval);
	void startTimer();
	void killTimer();
	void setTimeInterval(int timeInterval)
	{
		this->timeInterval = timeInterval;
	}
	int getTimeInterval()
	{
		return timeInterval;
	}
	void setAccuracy(int accuracy)
	{
		this->accuracy = accuracy;
	}
	int getAccuracy()
	{
		return accuracy;
	}
protected:
	void execute();
};