#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>

#include <cmath>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Vector4<T>::Vector4() :
	x(0), y(0), z(0), w(0)
{ }

template <typename T>
inline Vector4<T>::Vector4(T s) :
	x(s), y(s), z(s), w(s)
{ }

template <typename T>
inline Vector4<T>::Vector4(T x, T y, T z, T w) :
	x(x), y(y), z(z), w(w)
{ }

template <typename T>
inline Vector4<T>::Vector4(const Vector4<T>& v) :
	x(v.x), y(v.y), z(v.z), w(v.w)
{ }

template <typename T>
template <typename U>
inline Vector4<T>::Vector4(U s) :
	x(static_cast<T>(s)),
	y(static_cast<T>(s)),
	z(static_cast<T>(s)),
	w(static_cast<T>(s))
{ }

template <typename T>
template <typename X, typename Y, typename Z, typename W>
inline Vector4<T>::Vector4(X x, Y y, Z z, W w) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)),
	z(static_cast<T>(z)),
	w(static_cast<T>(w))
{ }

template <typename T>
template <typename U>
inline Vector4<T>::Vector4(const Vector4<U>& v) :
	x(static_cast<T>(v.x)),
	y(static_cast<T>(v.y)),
	z(static_cast<T>(v.z)),
	w(static_cast<T>(v.w))
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Vector4<T>::Vector4(const Vector2<A>& xy, B z, C w) :
	x(static_cast<T>(xy.x)),
	y(static_cast<T>(xy.y)),
	z(static_cast<T>(z)),
	w(static_cast<T>(w))
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Vector4<T>::Vector4(A x, const Vector2<B>& yz, C w) :
	x(static_cast<T>(x)),
	y(static_cast<T>(yz.y)),
	z(static_cast<T>(yz.z)),
	w(static_cast<T>(w))
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Vector4<T>::Vector4(A x, B y, const Vector2<C>& zw) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)),
	z(static_cast<T>(zw.z)),
	w(static_cast<T>(zw.w))
{ }

template <typename T>
template <typename A, typename B>
inline Vector4<T>::Vector4(const Vector2<A>& xy, const Vector2<B>& zw) :
	x(static_cast<T>(xy.x)),
	y(static_cast<T>(xy.y)),
	z(static_cast<T>(zw.x)),
	w(static_cast<T>(zw.y))
{ }

template <typename T>
template <typename A, typename B>
inline Vector4<T>::Vector4(const Vector3<A> & xyz, B w) :
	x(static_cast<T>(xyz.x)),
	y(static_cast<T>(xyz.y)),
	z(static_cast<T>(xyz.z)),
	w(static_cast<T>(w))
{ }

template <typename T>
template <typename A, typename B>
inline Vector4<T>::Vector4(A x, const Vector3<B>& yzw) :
	x(static_cast<T>(x)),
	y(static_cast<T>(yzw.x)),
	z(static_cast<T>(yzw.y)),
	w(static_cast<T>(yzw.z))
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
inline Vector4<T>& operator+=(Vector4<T>& a, U b)
{
	a.x += static_cast<T>(b);
	a.y += static_cast<T>(b);
	a.z += static_cast<T>(b);
	a.w += static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator+=(Vector4<T>& a, const Vector4<U>& b)
{
	a.x += static_cast<T>(b.x);
	a.y += static_cast<T>(b.y);
	a.z += static_cast<T>(b.z);
	a.w += static_cast<T>(b.w);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator-=(Vector4<T>& a, U b)
{
	a.x -= static_cast<T>(b);
	a.y -= static_cast<T>(b);
	a.z -= static_cast<T>(b);
	a.w -= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator-=(Vector4<T>& a, const Vector4<U>& b)
{
	a.x -= static_cast<T>(b.x);
	a.y -= static_cast<T>(b.y);
	a.z -= static_cast<T>(b.z);
	a.w -= static_cast<T>(b.w);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator*=(Vector4<T>& a, U b)
{
	a.x *= static_cast<T>(b);
	a.y *= static_cast<T>(b);
	a.z *= static_cast<T>(b);
	a.w *= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator*=(Vector4<T>& a, const Vector4<U>& b)
{
	a.x *= static_cast<T>(b.x);
	a.y *= static_cast<T>(b.y);
	a.z *= static_cast<T>(b.z);
	a.w *= static_cast<T>(b.w);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator/=(Vector4<T>& a, U b)
{
	a.x /= static_cast<T>(b);
	a.y /= static_cast<T>(b);
	a.z /= static_cast<T>(b);
	a.w /= static_cast<T>(b);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator/=(Vector4<T>& a, const Vector4<U>& b)
{
	a.x /= static_cast<T>(b.x);
	a.y /= static_cast<T>(b.y);
	a.z /= static_cast<T>(b.z);
	a.w /= static_cast<T>(b.w);
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator%=(Vector4<T>& a, U b)
{
	a.x %= static_cast<T>(b);
	a.y %= static_cast<T>(b);
	a.z %= static_cast<T>(b);
	a.w %= static_cast<T>(b);
	return a;
}

template <typename U>
inline Vector4<float>& operator%=(Vector4<float>& a, U b)
{
	a.x = fmod(a.x, static_cast<float>(b));
	a.y = fmod(a.y, static_cast<float>(b));
	a.z = fmod(a.z, static_cast<float>(b));
	a.w = fmod(a.w, static_cast<float>(b));
	return a;
}

template <typename U>
inline Vector4<double>& operator%=(Vector4<double>& a, U b)
{
	a.x = fmod(a.x, static_cast<double>(b));
	a.y = fmod(a.y, static_cast<double>(b));
	a.z = fmod(a.z, static_cast<double>(b));
	a.w = fmod(a.w, static_cast<double>(b));
	return a;
}

template <typename T, typename U>
inline Vector4<T>& operator%=(Vector4<T>& a, const Vector4<U>& b)
{
	a.x %= static_cast<T>(b.x);
	a.y %= static_cast<T>(b.y);
	a.z %= static_cast<T>(b.z);
	a.w %= static_cast<T>(b.w);
	return a;
}

template <typename U>
inline Vector4<float>& operator%=(Vector4<float>& a, const Vector4<U>& b)
{
	a.x = fmod(a.x, static_cast<float>(b.x));
	a.y = fmod(a.y, static_cast<float>(b.y));
	a.z = fmod(a.z, static_cast<float>(b.z));
	a.w = fmod(a.w, static_cast<float>(b.w));
	return a;
}

template <typename U>
inline Vector4<double>& operator%=(Vector4<double>& a, const Vector4<U>& b)
{
	a.x = fmod(a.x, static_cast<double>(b.x));
	a.y = fmod(a.y, static_cast<double>(b.y));
	a.z = fmod(a.z, static_cast<double>(b.z));
	a.w = fmod(a.w, static_cast<double>(b.w));
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
Vector4<T> operator-(const Vector4<T>& v)
{
	return Vector4<T>(-v.x, -v.y, -v.z, -v.w);
}

template <typename T, typename U>
inline Vector4<T> operator+(const Vector4<T>& v, U s)
{
	return Vector4<T>(v.x + s, v.y + s, v.z + s, v.w + s);
}

template <typename T, typename U>
inline Vector4<T> operator+(T s, const Vector4<U>& v)
{
	return Vector4<T>(s + v.x, s + v.y, s + v.z, s + v.w);
}

template <typename T, typename U>
inline Vector4<T> operator+(const Vector4<T>& a, const Vector4<U>& b)
{
	return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T, typename U>
inline Vector4<T> operator-(const Vector4<T>& v, U s)
{
	return Vector4<T>(v.x - s, v.y - s, v.z - s, v.w - s);
}

template <typename T, typename U>
inline Vector4<T> operator-(T s, const Vector4<U>& v)
{
	return Vector4<T>(s - v.x, s - v.y, s - v.z, s - v.w);
}

template <typename T, typename U>
inline Vector4<T> operator-(const Vector4<T>& a, const Vector4<U>& b)
{
	return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T, typename U>
inline Vector4<T> operator*(const Vector4<T>& v, U s)
{
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T, typename U>
inline Vector4<T> operator*(T s, const Vector4<U>& v)
{
	return Vector4<T>(s * v.x, s * v.y, s * v.z, s * v.w);
}

template <typename T, typename U>
inline Vector4<T> operator*(const Vector4<T>& a, const Vector4<U>& b)
{
	return Vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T, typename U>
inline Vector4<T> operator/(const Vector4<T>& v, U s)
{
	return Vector4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}

template <typename T, typename U>
inline Vector4<T> operator/(T s, const Vector4<U>& v)
{
	return Vector4<T>(s / v.x, s / v.y, s / v.z, s / v.w);
}

template <typename T, typename U>
inline Vector4<T> operator/(const Vector4<T>& a, const Vector4<U>& b)
{
	return Vector4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T, typename U>
inline Vector4<T> operator%(const Vector4<T>& v, U s)
{
	return Vector4<T>(v.x % s, v.y % s, v.z % s, v.w % s);
}

template <typename T, typename U>
inline Vector4<T> operator%(T s, const Vector4<U>& v)
{
	return Vector4<T>(s % v.x, s % v.y, s % v.z, s % v.w);
}

template <typename T, typename U>
inline Vector4<T> operator%(const Vector4<T>& a, const Vector4<U>& b)
{
	return Vector4<T>(a.x % b.x, a.y % b.y, a.z % b.z, a.w % b.w);
}

template <>
inline Vector4<float> operator%(const Vector4<float>& v, float s)
{
	return Vector4<float>(fmod(v.x, s), fmod(v.y, s), fmod(v.z, s), fmod(v.w, s));
}

template <>
inline Vector4<float> operator%(float s, const Vector4<float>& v)
{
	return Vector4<float>(fmod(s, v.x), fmod(s, v.y), fmod(s, v.z), fmod(s, v.w));
}
template <>
inline Vector4<float> operator%(const Vector4<float>& a, const Vector4<float>& b)
{
	return Vector4<float>(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z), fmod(a.w, b.w));
}

template <>
inline Vector4<double> operator%(const Vector4<double>& v, double s)
{
	return Vector4<double>(fmod(v.x, s), fmod(v.y, s), fmod(v.z, s), fmod(v.w, s));
}

template <>
inline Vector4<double> operator%(double s, const Vector4<double>& v)
{
	return Vector4<double>(fmod(s, v.x), fmod(s, v.y), fmod(s, v.z), fmod(s, v.w));
}
template <>
inline Vector4<double> operator%(const Vector4<double>& a, const Vector4<double>& b)
{
	return Vector4<double>(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z), fmod(a.w, b.w));
}

template <typename T>
inline bool operator==(const Vector4<T>& a, const Vector4<T>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
inline bool operator!=(const Vector4<T>& a, const Vector4<T>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

///////////////////////////////////////////////////////////

template <typename T>
Vector4<T> floor(const Vector4<T>& v)
{
	return Vector4<T>(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
}

template <typename T>
Vector4<T> ceil(const Vector4<T>& v)
{
	return Vector4<T>(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
}

template <typename T>
Vector4<T> round(const Vector4<T>& v)
{
	return Vector4<T>(round(v.x), round(v.y), round(v.z), round(v.w));
}

template <typename T>
Vector4<int> lround(const Vector4<T>& v)
{
	return Vector4<T>(lround(v.x), lround(v.y), lround(v.z), lround(v.w));
}

template <typename T>
Vector4<T> abs(const Vector4<T>& v)
{
	return Vector4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
}

template <typename T>
Vector4<T> pow(const Vector4<T>& b, T p)
{
	return Vector4<T>(pow(b.x, p), pow(b.y, p), pow(b.z, p), pow(b.w, p));
}

template <typename T>
Vector4<T> pow(const Vector4<T>& b, const Vector4<T>& p)
{
	return Vector4<T>(pow(b.x, p.x), pow(b.y, p.y), pow(b.z, p.z), pow(b.w, p.w));
}

template <typename T>
Vector4<T> sqrt(const Vector4<T>& v)
{
	return Vector4<T>(sqrt(v.x), sqrt(v.y), sqrt(v.z), sqrt(v.w));
}

///////////////////////////////////////////////////////////

template <typename T>
T length(const Vector4<T>& v)
{
	return ::sqrt(sum(v * v));
}

template <typename T>
T dist(const Vector4<T>& a, const Vector4<T>& b)
{
	return length(a - b);
}

template <typename T>
T distSquared(const Vector4<T>& a, const Vector4<T>& b)
{
	Vector4<T> d(a - b);
	return sum(d * d);
}

template <typename T>
Vector4<T> normalize(const Vector4<T>& v)
{
	return v / length(v);
}

template <typename T>
T sum(const Vector4<T>& v)
{
	return v.x + v.y + v.z + v.w;
}

template <typename T>
T dot(const Vector4<T>& a, const Vector4<T>& b)
{
	return sum(a * b);
}

///////////////////////////////////////////////////////////

}