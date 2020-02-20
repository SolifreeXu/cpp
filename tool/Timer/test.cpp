#include <iostream>

#include "Timer.h"

int main()
{
	Timer timer;
	timer.setTimer([&timer]()
	{
		std::cout << timer.getTimeInterval() << ' ';
	}, 100);
	//std::this_thread::sleep_for(std::chrono::milliseconds(102));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return 0;
}