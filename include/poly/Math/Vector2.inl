#include <cmath>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T>::Vector2() :
	x(0), y(0)
{ }

template <typename T>
inline Vector2<T>::Vector2(T s) :
	x(s), y(s)
{ }

template <typename T>
inline Vector2<T>::Vector2(T x, T y) :
	x(x), y(y)
{ }

template <typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v) :
	x(v.x), y(v.y)
{ }

template <typename T>
template <typename U>
inline Vector2<T>::Vector2(U s) :
	x(static_cast<T>(s)), y(static_cast<T>(s))
{ }

template <typename T>
template <typename A, typename B>
inline Vector2<T>::Vector2(A x, B y) :
	x(static_cast<T>(x)), y(static_cast<T>(y))
{ }

template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U>& v) :
	x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector2<T>& operator+=(Vector2<T>& a, U b)
{
	a.x += static_cast<T>(b);
	a.y += static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator+=(Vector2<T>& a, const Vector2<U>& b)
{
	a.x += static_cast<T>(b.x);
	a.y += static_cast<T>(b.y);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator-=(Vector2<T>& a, U b)
{
	a.x -= static_cast<T>(b);
	a.y -= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator-=(Vector2<T>& a, const Vector2<U>& b)
{
	a.x -= static_cast<T>(b.x);
	a.y -= static_cast<T>(b.y);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator*=(Vector2<T>& a, U b)
{
	a.x *= static_cast<T>(b);
	a.y *= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator*=(Vector2<T>& a, const Vector2<U>& b)
{
	a.x *= static_cast<T>(b.x);
	a.y *= static_cast<T>(b.y);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator/=(Vector2<T>& a, U b)
{
	a.x /= static_cast<T>(b);
	a.y /= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator/=(Vector2<T>& a, const Vector2<U>& b)
{
	a.x /= static_cast<T>(b.x);
	a.y /= static_cast<T>(b.y);
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator%=(Vector2<T>& a, U b)
{
	a.x %= static_cast<T>(b);
	a.y %= static_cast<T>(b);
	return a;
}

template <typename U>
inline Vector2<float>& operator%=(Vector2<float>& a, U b)
{
	a.x = fmod(a.x, static_cast<float>(b));
	a.y = fmod(a.y, static_cast<float>(b));
	return a;
}

template <typename U>
inline Vector2<double>& operator%=(Vector2<double>& a, U b)
{
	a.x = fmod(a.x, static_cast<double>(b));
	a.y = fmod(a.y, static_cast<double>(b));
	return a;
}

template <typename T, typename U>
inline Vector2<T>& operator%=(Vector2<T>& a, const Vector2<U>& b)
{
	a.x %= static_cast<T>(b.x);
	a.y %= static_cast<T>(b.y);
	return a;
}

template <typename U>
inline Vector2<float>& operator%=(Vector2<float>& a, const Vector2<U>& b)
{
	a.x = fmod(a.x, static_cast<float>(b.x));
	a.y = fmod(a.y, static_cast<float>(b.y));
	return a;
}

template <typename U>
inline Vector2<double>& operator%=(Vector2<double>& a, const Vector2<U>& b)
{
	a.x = fmod(a.x, static_cast<double>(b.x));
	a.y = fmod(a.y, static_cast<double>(b.y));
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& v, T s)
{
	return Vector2<T>(v.x + s, v.y + s);
}

template <typename T>
inline Vector2<T> operator+(T s, const Vector2<T>& v)
{
	return Vector2<T>(s + v.x, s + v.y);
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& v, T s)
{
	return Vector2<T>(v.x - s, v.y - s);
}

template <typename T>
inline Vector2<T> operator-(T s, const Vector2<T>& v)
{
	return Vector2<T>(s - v.x, s - v.y);
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& v, T s)
{
	return Vector2<T>(v.x * s, v.y * s);
}

template <typename T>
inline Vector2<T> operator*(T s, const Vector2<T>& v)
{
	return Vector2<T>(s * v.x, s * v.y);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T>& v, T s)
{
	return Vector2<T>(v.x / s, v.y / s);
}

template <typename T>
inline Vector2<T> operator/(T s, const Vector2<T>& v)
{
	return Vector2<T>(s / v.x, s / v.y);
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
inline Vector2<T> operator%(const Vector2<T>& v, T s)
{
	return Vector2<T>(v.x % s, v.y % s);
}

template <typename T>
inline Vector2<T> operator%(T s, const Vector2<T>& v)
{
	return Vector2<T>(s % v.x, s % v.y);
}

template <typename T>
inline Vector2<T> operator%(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>(a.x % b.x, a.y % b.y);
}

template <>
inline Vector2<float> operator%(const Vector2<float>& v, float s)
{
	return Vector2<float>(fmod(v.x, s), fmod(v.y, s));
}

template <>
inline Vector2<float> operator%(float s, const Vector2<float>& v)
{
	return Vector2<float>(fmod(s, v.x), fmod(s, v.y));
}

template <>
inline Vector2<float> operator%(const Vector2<float>& a, const Vector2<float>& b)
{
	return Vector2<float>(fmod(a.x, b.x), fmod(a.y, b.y));
}

template <>
inline Vector2<double> operator%(const Vector2<double>& v, double s)
{
	return Vector2<double>(fmod(v.x, s), fmod(v.y, s));
}

template <>
inline Vector2<double> operator%(double s, const Vector2<double>& v)
{
	return Vector2<double>(fmod(s, v.x), fmod(s, v.y));
}

template <>
inline Vector2<double> operator%(const Vector2<double>& a, const Vector2<double>& b)
{
	return Vector2<double>(fmod(a.x, b.x), fmod(a.y, b.y));
}

///////////////////////////////////////////////////////////

template <typename T>
Vector2<T> floor(const Vector2<T>& v)
{
	return Vector2<T>(floor(v.x), floor(v.y));
}

template <typename T>
Vector2<T> ceil(const Vector2<T>& v)
{
	return Vector2<T>(ceil(v.x), ceil(v.y));
}

template <typename T>
Vector2<T> round(const Vector2<T>& v)
{
	return Vector2<T>(round(v.x), round(v.y));
}

template <typename T>
Vector2<int> lround(const Vector2<T>& v)
{
	return Vector2<T>(lround(v.x), lround(v.y));
}

template <typename T>
Vector2<T> abs(const Vector2<T>& v)
{
	return Vector2<T>(abs(v.x), abs(v.y));
}

template <typename T>
Vector2<T> pow(const Vector2<T>& b, T p)
{
	return Vector2<T>(pow(b.x, p), pow(b.y, p));
}

template <typename T>
Vector2<T> pow(const Vector2<T>& b, const Vector2<T>& p)
{
	return Vector2<T>(pow(b.x, p.x), pow(b.y, p.y));
}

template <typename T>
Vector2<T> sqrt(const Vector2<T>& v)
{
	return Vector2<T>(sqrt(v.x), sqrt(v.y));
}

///////////////////////////////////////////////////////////

template <typename T>
T length(const Vector2<T>& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

template <typename T>
T dist(const Vector2<T>& a, const Vector2<T>& b)
{
	return length(a - b);
}

template <typename T>
T distSquared(const Vector2<T>& a, const Vector2<T>& b)
{
	Vector2<T> d(a - b);
	return d.x * d.x + d.y * d.y;
}

template <typename T>
Vector2<T> normalize(const Vector2<T>& v)
{
	T d = static_cast<T>(1) / length(v);
	return Vector2(
		v.x * d,
		v.y * d
	);
}

template <typename T>
T sum(const Vector2<T>& v)
{
	return v.x + v.y;
}

template <typename T>
T dot(const Vector2<T>& a, const Vector2<T>& b)
{
	return sum(a * b);
}

///////////////////////////////////////////////////////////

}