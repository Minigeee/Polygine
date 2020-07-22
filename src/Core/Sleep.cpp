#include <poly/Core/Sleep.h>

#include <chrono>
#include <thread>

namespace poly
{

void sleep(Time time)
{
	Uint64 us = time.toMicroseconds();

	if (us > 0)
	{
		// Use C++11 sleep_for
		std::this_thread::sleep_for(
			std::chrono::microseconds(us)
		);
	}
}

void sleep(float seconds)
{
	if (seconds > 0.0f)
	{
		// Use C++11 sleep_for
		std::this_thread::sleep_for(
			std::chrono::microseconds(Time::fromSeconds(seconds).toMicroseconds())
		);
	}
}

void sleep(double seconds)
{
	sleep((float)seconds);
}

void sleep(Int32 milliseconds)
{
	if (milliseconds > 0)
	{
		// Use C++11 sleep_for
		std::this_thread::sleep_for(
			std::chrono::milliseconds(milliseconds)
		);
	}
}

void sleep(Uint32 milliseconds)
{
	sleep((Int32)milliseconds);
}

}