#include <poly/Core/Time.h>

#include <chrono>

namespace poly
{


///////////////////////////////////////////////////////////
Time::Time() :
	m_microseconds(0)
{ }


///////////////////////////////////////////////////////////
Time::Time(Int64 micro) :
	m_microseconds(micro)
{ }


///////////////////////////////////////////////////////////
Time Time::fromMinutes(double minutes)
{
	return Time((Int64)(minutes * 60000000));
}


///////////////////////////////////////////////////////////
Time Time::fromSeconds(float seconds)
{
	return Time((Int64)((double)seconds * 1000000));
}


///////////////////////////////////////////////////////////
Time Time::fromMilliseconds(Int32 milliseconds)
{
	return Time((Int64)milliseconds * 1000);
}


///////////////////////////////////////////////////////////
Time Time::fromMicroseconds(Int64 microseconds)
{
	return Time(microseconds);
}


///////////////////////////////////////////////////////////
double Time::toMinutes() const
{
	return m_microseconds * 0.000001 / 60.0;
}


///////////////////////////////////////////////////////////
float Time::toSeconds() const
{
	return (float)(m_microseconds * 0.000001);
}


///////////////////////////////////////////////////////////
Int32 Time::toMilliseconds() const
{
	return (Int32)(m_microseconds * 0.001);
}


///////////////////////////////////////////////////////////
Int64 Time::toMicroseconds() const
{
	return m_microseconds;
}


///////////////////////////////////////////////////////////
Time operator-(Time a, Time b)
{
	return Time(a.toMicroseconds() - b.toMicroseconds());
}


///////////////////////////////////////////////////////////
Time operator-=(Time& a, Time b)
{
	a = Time(a.toMicroseconds() - b.toMicroseconds());
	return a;
}


///////////////////////////////////////////////////////////
Time operator+(Time a, Time b)
{
	return Time(a.toMicroseconds() + b.toMicroseconds());
}


///////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////
Time operator*(Int64 a, Time b)
{
	return Time(a * b.toMicroseconds());
}


///////////////////////////////////////////////////////////
Time& operator*=(Time& a, Int64 b)
{
	a = Time(a.toMicroseconds() * b);
	return a;
}


///////////////////////////////////////////////////////////
Time operator*(Time a, double b)
{
	return Time((Uint64)(a.toMicroseconds() * b));
}


///////////////////////////////////////////////////////////
Time operator*(double a, Time b)
{
	return Time((Uint64)(a * b.toMicroseconds()));
}


///////////////////////////////////////////////////////////
Time& operator*=(Time& a, double b)
{
	a = Time((Uint64)(a.toMicroseconds() * b));
	return a;
}


///////////////////////////////////////////////////////////
Time operator/(Time a, Int64 b)
{
	return Time(a.toMicroseconds() / b);
}


///////////////////////////////////////////////////////////
Time operator/(Int64 a, Time b)
{
	return Time(a / b.toMicroseconds());
}


///////////////////////////////////////////////////////////
Time& operator/=(Time& a, Int64 b)
{
	a = Time(a.toMicroseconds() / b);
	return a;
}


///////////////////////////////////////////////////////////
Time operator/(Time a, double b)
{
	return Time((Uint64)(a.toMicroseconds() / b));
}


///////////////////////////////////////////////////////////
Time operator/(double a, Time b)
{
	return Time((Uint64)(a / b.toMicroseconds()));
}


///////////////////////////////////////////////////////////
Time& operator/=(Time& a, double b)
{
	a = Time((Uint64)(a.toMicroseconds() / b));
	return a;
}


///////////////////////////////////////////////////////////
bool operator==(Time a, Time b)
{
	return a.toMicroseconds() == b.toMicroseconds();
}


///////////////////////////////////////////////////////////
bool operator!=(Time a, Time b)
{
	return a.toMicroseconds() != b.toMicroseconds();
}


///////////////////////////////////////////////////////////
bool operator>(Time a, Time b)
{
	return a.toMicroseconds() > b.toMicroseconds();
}


///////////////////////////////////////////////////////////
bool operator<(Time a, Time b)
{
	return a.toMicroseconds() < b.toMicroseconds();
}


///////////////////////////////////////////////////////////
bool operator>=(Time a, Time b)
{
	return a.toMicroseconds() >= b.toMicroseconds();
}


///////////////////////////////////////////////////////////
bool operator<=(Time a, Time b)
{
	return a.toMicroseconds() <= b.toMicroseconds();
}


}