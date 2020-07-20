#include <poly/Core/Sleep.h>

#include <chrono>
#include <thread>

namespace poly
{

void sleep(Time time)
{
	// Use C++11 sleep_for
	std::this_thread::sleep_for(
		std::chrono::microseconds(time.toMicroseconds())
	);
}

void sleep(float seconds)
{
	// Use C++11 sleep_for
	std::this_thread::sleep_for(
		std::chrono::microseconds(Time::fromSeconds(seconds).toMicroseconds())
	);
}

void sleep(double seconds)
{
	// Use C++11 sleep_for
	std::this_thread::sleep_for(
		std::chrono::microseconds(Time::fromSeconds((float)seconds).toMicroseconds())
	);
}

void sleep(Int32 milliseconds)
{
	// Use C++11 sleep_for
	std::this_thread::sleep_for(
		std::chrono::milliseconds(milliseconds)
	);
}

void sleep(Uint32 milliseconds)
{
	// Use C++11 sleep_for
	std::this_thread::sleep_for(
		std::chrono::milliseconds(milliseconds)
	);
}

}