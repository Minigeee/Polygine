#include <poly/Math/Vector2.h>
#include <poly/Math/Vector4.h>

#include <cmath>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T>::Vector3() :
	x(0), y(0), z(0)
{ }

template <typename T>
inline Vector3<T>::Vector3(T s) :
	x(s), y(s), z(s)
{ }

template <typename T>
inline Vector3<T>::Vector3(T x, T y, T z) :
	x(x), y(y), z(z)
{ }

template <typename T>
inline Vector3<T>::Vector3(const Vector3<T>& v) :
	x(v.x), y(v.y), z(v.z)
{ }

template <typename T>
template <typename U>
inline Vector3<T>::Vector3(U s) :
	x(static_cast<T>(s)),
	y(static_cast<T>(s)),
	z(static_cast<T>(s))
{ }

template <typename T>
template <typename X, typename Y, typename Z>
inline Vector3<T>::Vector3(X x, Y y, Z z) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)),
	z(static_cast<T>(z))
{ }

template <typename T>
template <typename U>
inline Vector3<T>::Vector3(const Vector3<U>& v) :
	x(static_cast<T>(v.x)),
	y(static_cast<T>(v.y)),
	z(static_cast<T>(v.z))
{ }

template <typename T>
template <typename A, typename B>
inline Vector3<T>::Vector3(const Vector2<A>& xy, B z) :
	x(static_cast<T>(xy.x)),
	y(static_cast<T>(xy.y)),
	z(static_cast<T>(z))
{ }

template <typename T>
template <typename A, typename B>
inline Vector3<T>::Vector3(A x, const Vector2<B>& yz) :
	x(static_cast<T>(x)),
	y(static_cast<T>(yz.y)),
	z(static_cast<T>(yz.z))
{ }

template <typename T>
template <typename U>
inline Vector3<T>::Vector3(const Vector4<U> & v) :
	x(static_cast<T>(v.x)),
	y(static_cast<T>(v.y)),
	z(static_cast<T>(v.z))
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector3<T>& operator+=(Vector3<T>& a, U b)
{
	a.x += static_cast<T>(b);
	a.y += static_cast<T>(b);
	a.z += static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator+=(Vector3<T>& a, const Vector3<U>& b)
{
	a.x += static_cast<T>(b.x);
	a.y += static_cast<T>(b.y);
	a.z += static_cast<T>(b.z);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator-=(Vector3<T>& a, U b)
{
	a.x -= static_cast<T>(b);
	a.y -= static_cast<T>(b);
	a.z -= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator-=(Vector3<T>& a, const Vector3<U>& b)
{
	a.x -= static_cast<T>(b.x);
	a.y -= static_cast<T>(b.y);
	a.z -= static_cast<T>(b.z);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator*=(Vector3<T>& a, U b)
{
	a.x *= static_cast<T>(b);
	a.y *= static_cast<T>(b);
	a.z *= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator*=(Vector3<T>& a, const Vector3<U>& b)
{
	a.x *= static_cast<T>(b.x);
	a.y *= static_cast<T>(b.y);
	a.z *= static_cast<T>(b.z);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator/=(Vector3<T>& a, U b)
{
	a.x /= static_cast<T>(b);
	a.y /= static_cast<T>(b);
	a.z /= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator/=(Vector3<T>& a, const Vector3<U>& b)
{
	a.x /= static_cast<T>(b.x);
	a.y /= static_cast<T>(b.y);
	a.z /= static_cast<T>(b.z);
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator%=(Vector3<T>& a, U b)
{
	a.x %= static_cast<T>(b);
	a.y %= static_cast<T>(b);
	a.z %= static_cast<T>(b);
	return a;
}

template <typename U>
inline Vector3<float>& operator%=(Vector3<float>& a, U b)
{
	a.x = fmod(a.x, static_cast<float>(b));
	a.y = fmod(a.y, static_cast<float>(b));
	a.z = fmod(a.z, static_cast<float>(b));
	return a;
}

template <typename U>
inline Vector3<double>& operator%=(Vector3<double>& a, U b)
{
	a.x = fmod(a.x, static_cast<double>(b));
	a.y = fmod(a.y, static_cast<double>(b));
	a.z = fmod(a.z, static_cast<double>(b));
	return a;
}

template <typename T, typename U>
inline Vector3<T>& operator%=(Vector3<T>& a, const Vector3<U>& b)
{
	a.x %= static_cast<T>(b.x);
	a.y %= static_cast<T>(b.y);
	a.z %= static_cast<T>(b.z);
	return a;
}

template <typename U>
inline Vector3<float>& operator%=(Vector3<float>& a, const Vector3<U>& b)
{
	a.x = fmod(a.x, static_cast<float>(b.x));
	a.y = fmod(a.y, static_cast<float>(b.y));
	a.z = fmod(a.z, static_cast<float>(b.z));
	return a;
}

template <typename U>
inline Vector3<double>& operator%=(Vector3<double>& a, const Vector3<U>& b)
{
	a.x = fmod(a.x, static_cast<double>(b.x));
	a.y = fmod(a.y, static_cast<double>(b.y));
	a.z = fmod(a.z, static_cast<double>(b.z));
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
Vector3<T> operator-(const Vector3<T>& v)
{
	return Vector3<T>(-v.x, -v.y, -v.z);
}

template <typename T, typename U>
inline Vector3<T> operator+(const Vector3<T>& v, U s)
{
	return Vector3<T>(v.x + s, v.y + s, v.z + s);
}

template <typename T, typename U>
inline Vector3<T> operator+(T s, const Vector3<U>& v)
{
	return Vector3<T>(s + v.x, s + v.y, s + v.z);
}

template <typename T, typename U>
inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<U>& b)
{
	return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T, typename U>
inline Vector3<T> operator-(const Vector3<T>& v, U s)
{
	return Vector3<T>(v.x - s, v.y - s, v.z - s);
}

template <typename T, typename U>
inline Vector3<T> operator-(T s, const Vector3<U>& v)
{
	return Vector3<T>(s - v.x, s - v.y, s - v.z);
}

template <typename T, typename U>
inline Vector3<T> operator-(const Vector3<T>& a, const Vector3<U>& b)
{
	return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T, typename U>
inline Vector3<T> operator*(const Vector3<T>& v, U s)
{
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T, typename U>
inline Vector3<T> operator*(T s, const Vector3<U>& v)
{
	return Vector3<T>(s * v.x, s * v.y, s * v.z);
}

template <typename T, typename U>
inline Vector3<T> operator*(const Vector3<T>& a, const Vector3<U>& b)
{
	return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T, typename U>
inline Vector3<T> operator/(const Vector3<T>& v, U s)
{
	return Vector3<T>(v.x / s, v.y / s, v.z / s);
}

template <typename T, typename U>
inline Vector3<T> operator/(T s, const Vector3<U>& v)
{
	return Vector3<T>(s / v.x, s / v.y, s / v.z);
}

template <typename T, typename U>
inline Vector3<T> operator/(const Vector3<T>& a, const Vector3<U>& b)
{
	return Vector3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template <typename T, typename U>
inline Vector3<T> operator%(const Vector3<T>& v, U s)
{
	return Vector3<T>(v.x % s, v.y % s, v.z % s);
}

template <typename T, typename U>
inline Vector3<T> operator%(T s, const Vector3<U>& v)
{
	return Vector3<T>(s % v.x, s % v.y, s % v.z);
}

template <typename T, typename U>
inline Vector3<T> operator%(const Vector3<T>& a, const Vector3<U>& b)
{
	return Vector3<T>(a.x % b.x, a.y % b.y, a.z % b.z);
}

template <>
inline Vector3<float> operator%(const Vector3<float>& v, float s)
{
	return Vector3<float>(fmod(v.x, s), fmod(v.y, s), fmod(v.z, s));
}

template <>
inline Vector3<float> operator%(float s, const Vector3<float>& v)
{
	return Vector3<float>(fmod(s, v.x), fmod(s, v.y), fmod(s, v.z));
}

template <>
inline Vector3<float> operator%(const Vector3<float>& a, const Vector3<float>& b)
{
	return Vector3<float>(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

template <>
inline Vector3<double> operator%(const Vector3<double>& v, double s)
{
	return Vector3<double>(fmod(v.x, s), fmod(v.y, s), fmod(v.z, s));
}

template <>
inline Vector3<double> operator%(double s, const Vector3<double>& v)
{
	return Vector3<double>(fmod(s, v.x), fmod(s, v.y), fmod(s, v.z));
}

template <>
inline Vector3<double> operator%(const Vector3<double>& a, const Vector3<double>& b)
{
	return Vector3<double>(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

template <typename T>
inline bool operator==(const Vector3<T>& a, const Vector3<T>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
inline bool operator!=(const Vector3<T>& a, const Vector3<T>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

///////////////////////////////////////////////////////////

template <typename T>
inline Vector3<T> floor(const Vector3<T>& v)
{
	return Vector3<T>(floor(v.x), floor(v.y), floor(v.z));
}

template <typename T>
inline Vector3<T> ceil(const Vector3<T>& v)
{
	return Vector3<T>(ceil(v.x), ceil(v.y), ceil(v.z));
}

template <typename T>
inline Vector3<T> round(const Vector3<T>& v)
{
	return Vector3<T>(round(v.x), round(v.y), round(v.z));
}

template <typename T>
inline Vector3<int> lround(const Vector3<T>& v)
{
	return Vector3<T>(lround(v.x), lround(v.y), lround(v.z));
}

template <typename T>
inline Vector3<T> abs(const Vector3<T>& v)
{
	return Vector3<T>(abs(v.x), abs(v.y), abs(v.z));
}

template <typename T>
inline Vector3<T> pow(const Vector3<T>& b, T p)
{
	return Vector3<T>(pow(b.x, p), pow(b.y, p), pow(b.z, p));
}

template <typename T>
inline Vector3<T> pow(const Vector3<T>& b, const Vector3<T>& p)
{
	return Vector3<T>(pow(b.x, p.x), pow(b.y, p.y), pow(b.z, p.z));
}

template <typename T>
inline Vector3<T> sqrt(const Vector3<T>& v)
{
	return Vector3<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z));
}

///////////////////////////////////////////////////////////

template <typename T>
inline T length(const Vector3<T>& v)
{
	return ::sqrt(sum(v * v));
}

template <typename T>
inline T dist(const Vector3<T>& a, const Vector3<T>& b)
{
	return length(a - b);
}

template <typename T>
inline T distSquared(const Vector3<T>& a, const Vector3<T>& b)
{
	Vector3<T> d(a - b);
	return sum(d * d);
}

template <typename T>
inline Vector3<T> normalize(const Vector3<T>& v)
{
	return v / length(v);
}

template <typename T>
inline T sum(const Vector3<T>& v)
{
	return v.x + v.y + v.z;
}

template <typename T>
inline T dot(const Vector3<T>& a, const Vector3<T>& b)
{
	return sum(a * b);
}

template <typename T>
inline Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
		);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Vector3<T> mix(const Vector3<T>& a, const Vector3<T>& b, float factor)
{
	return Vector3<T>(a + (b - a) * factor);
}


}