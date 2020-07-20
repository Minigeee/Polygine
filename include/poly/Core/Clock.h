#ifndef POLY_CLOCK_H
#define POLY_CLOCK_H

#include <poly/Core/Time.h>

#include <chrono>

namespace poly
{

class Clock
{
public:
	Clock();

	Time restart();
	Time getElapsedTime() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

}

#endif