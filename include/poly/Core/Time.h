#ifndef POLY_TIME_H
#define POLY_TIME_H

#include <poly/Core/DataTypes.h>

namespace poly
{

class Time
{
public:
	Time();
	Time(Uint64 microseconds);

	static Time fromSeconds(float seconds);
	static Time fromMilliseconds(Uint32 milliseconds);
	static Time fromMicroseconds(Uint64 microseconds);

	float toSeconds() const;
	Uint32 toMilliseconds() const;
	Uint64 toMicroseconds() const;

private:
	Uint64 m_microseconds;
};

Time operator-(Time a, Time b);
Time operator-=(Time& a, Time b);
Time operator+(Time a, Time b);
Time operator+=(Time& a, Time b);

Time operator*(Time a, Int64 b);
Time operator*(Int64 a, Time b);
Time operator*(Time a, double b);
Time operator*(double a, Time b);
Time& operator*=(Time& a, Int64 b);
Time& operator*=(Time& a, double b);

Time operator/(Time a, Int64 b);
Time operator/(Int64 a, Time b);
Time operator/(Time a, double b);
Time operator/(double a, Time b);
Time& operator/=(Time& a, Int64 b);
Time& operator/=(Time& a, double b);

bool operator==(Time a, Time b);
bool operator!=(Time a, Time b);
bool operator>(Time a, Time b);
bool operator<(Time a, Time b);
bool operator>=(Time a, Time b);
bool operator<=(Time a, Time b);

}

#endif