#include <poly/Core/Time.h>

#include <chrono>

namespace poly
{

///////////////////////////////////////////////////////////

Time::Time() :
	m_microseconds(0)
{ }

Time::Time(Uint64 micro) :
	m_microseconds(micro)
{ }

///////////////////////////////////////////////////////////

Time Time::fromSeconds(float seconds)
{
	return Time((Uint64)((double)seconds * 1000000));
}

Time Time::fromMilliseconds(Uint32 milliseconds)
{
	return Time((Uint64)milliseconds * 1000);
}

Time Time::fromMicroseconds(Uint64 microseconds)
{
	return Time(microseconds);
}

///////////////////////////////////////////////////////////

float Time::toSeconds() const
{
	return (float)(m_microseconds * 0.000001);
}

Uint32 Time::toMilliseconds() const
{
	return (Uint32)(m_microseconds * 0.001);
}

Uint64 Time::toMicroseconds() const
{
	return m_microseconds;
}

///////////////////////////////////////////////////////////

Time operator-(Time a, Time b)
{
	return Time(a.toMicroseconds() - b.toMicroseconds());
}


Time operator-=(Time& a, Time b)
{
	a = Time(a.toMicroseconds() - b.toMicroseconds());
	return a;
}

Time operator+(Time a, Time b)
{
	return Time(a.toMicroseconds() + b.toMicroseconds());
}

Time operator+=(Time& a, Time b)
{
	a = Time(a.toMicroseconds() + b.toMicroseconds());
	return a;
}

///////////////////////////////////////////////////////////

Time operator*(Time a, Int64 b)
{
	return Time(a.toMicroseconds() * b);
}

Time operator*(Int64 a, Time b)
{
	return Time(a * b.toMicroseconds());
}

Time& operator*=(Time& a, Int64 b)
{
	a = Time(a.toMicroseconds() * b);
	return a;
}

Time operator*(Time a, double b)
{
	return Time(a.toMicroseconds() * b);
}

Time operator*(double a, Time b)
{
	return Time(a * b.toMicroseconds());
}

Time& operator*=(Time& a, double b)
{
	a = Time(a.toMicroseconds() * b);
	return a;
}

///////////////////////////////////////////////////////////

Time operator/(Time a, Int64 b)
{
	return Time(a.toMicroseconds() / b);
}

Time operator/(Int64 a, Time b)
{
	return Time(a / b.toMicroseconds());
}

Time& operator/=(Time& a, Int64 b)
{
	a = Time(a.toMicroseconds() / b);
	return a;
}

Time operator/(Time a, double b)
{
	return Time(a.toMicroseconds() / b);
}

Time operator/(double a, Time b)
{
	return Time(a / b.toMicroseconds());
}

Time& operator/=(Time& a, double b)
{
	a = Time(a.toMicroseconds() / b);
	return a;
}

///////////////////////////////////////////////////////////

bool operator==(Time a, Time b)
{
	return a.toMicroseconds() == b.toMicroseconds();
}

bool operator!=(Time a, Time b)
{
	return a.toMicroseconds() != b.toMicroseconds();
}

bool operator>(Time a, Time b)
{
	return a.toMicroseconds() > b.toMicroseconds();
}

bool operator<(Time a, Time b)
{
	return a.toMicroseconds() < b.toMicroseconds();
}

bool operator>=(Time a, Time b)
{
	return a.toMicroseconds() >= b.toMicroseconds();
}

bool operator<=(Time a, Time b)
{
	return a.toMicroseconds() <= b.toMicroseconds();
}

///////////////////////////////////////////////////////////

}