#ifndef POLY_VECTOR3_H
#define POLY_VECTOR3_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector2;
template <typename T> struct Vector4;

template <typename T>
struct Vector3
{
	Vector3();

	explicit Vector3(T s);

	Vector3(T x, T y, T z);

	Vector3(const Vector3<T>& v);

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Vector3(U s);

	template <typename X, typename Y, typename Z>
	Vector3(X x, Y y, Z z);

	template <typename U>
	Vector3(const Vector3<U>& v);

	template <typename A, typename B>
	explicit Vector3(const Vector2<A>& xy, B z);

	template <typename A, typename B>
	explicit Vector3(A x, const Vector2<B>& yz);

	template <typename U>
	explicit Vector3(const Vector4<U>& v);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	union { T x, r; };
	union { T y, g; };
	union { T z, b; };
};

///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

template <typename T, typename U>
Vector3<T>& operator+=(Vector3<T>& a, U b);

template <typename T, typename U>
Vector3<T>& operator+=(Vector3<T>& a, const Vector3<U>& b);

template <typename T, typename U>
Vector3<T>& operator-=(Vector3<T>& a, U b);

template <typename T, typename U>
Vector3<T>& operator-=(Vector3<T>& a, const Vector3<U>& b);

template <typename T, typename U>
Vector3<T>& operator*=(Vector3<T>& a, U b);

template <typename T, typename U>
Vector3<T>& operator*=(Vector3<T>& a, const Vector3<U>& b);

template <typename T, typename U>
Vector3<T>& operator/=(Vector3<T>& a, U b);

template <typename T, typename U>
Vector3<T>& operator/=(Vector3<T>& a, const Vector3<U>& b);

template <typename T, typename U>
Vector3<T>& operator%=(Vector3<T>& a, U b);

template <typename U>
Vector3<float>& operator%=(Vector3<float>& a, U b);

template <typename U>
Vector3<double>& operator%=(Vector3<double>& a, U b);

template <typename T, typename U>
Vector3<T>& operator%=(Vector3<T>& a, const Vector3<U>& b);

template <typename U>
Vector3<float>& operator%=(Vector3<float>& a, const Vector3<U>& b);

template <typename U>
Vector3<double>& operator%=(Vector3<double>& a, const Vector3<U>& b);


template <typename T>
Vector3<T> operator-(const Vector3<T>& v);

template <typename T>
Vector3<T> operator+(const Vector3<T>& v, T s);

template <typename T>
Vector3<T> operator+(T s, const Vector3<T>& v);

template <typename T>
Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> operator-(const Vector3<T>& v, T s);

template <typename T>
Vector3<T> operator-(T s, const Vector3<T>& v);

template <typename T>
Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> operator*(const Vector3<T>& v, T s);

template <typename T>
Vector3<T> operator*(T s, const Vector3<T>& v);

template <typename T>
Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> operator/(const Vector3<T>& v, T s);

template <typename T>
Vector3<T> operator/(T s, const Vector3<T>& v);

template <typename T>
Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> operator%(const Vector3<T>& v, T s);

template <typename T>
Vector3<T> operator%(T s, const Vector3<T>& v);

template <typename T>
Vector3<T> operator%(const Vector3<T>& a, const Vector3<T>& b);

template <>
Vector3<float> operator%(const Vector3<float>& v, float s);

template <>
Vector3<float> operator%(float s, const Vector3<float>& v);

template <>
Vector3<float> operator%(const Vector3<float>& a, const Vector3<float>& b);

template <>
Vector3<double> operator%(const Vector3<double>& v, double s);

template <>
Vector3<double> operator%(double s, const Vector3<double>& v);

template <>
Vector3<double> operator%(const Vector3<double>& a, const Vector3<double>& b);

template <typename T>
bool operator==(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
bool operator!=(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

template <typename T>
Vector3<T> floor(const Vector3<T>& v);

template <typename T>
Vector3<T> ceil(const Vector3<T>& v);

template <typename T>
Vector3<T> round(const Vector3<T>& v);

template <typename T>
Vector3<int> lround(const Vector3<T>& v);

template <typename T>
Vector3<T> abs(const Vector3<T>& v);

template <typename T>
Vector3<T> pow(const Vector3<T>& b, T p);

template <typename T>
Vector3<T> pow(const Vector3<T>& b, const Vector3<T>& p);

template <typename T>
Vector3<T> sqrt(const Vector3<T>& v);

///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

template <typename T>
T length(const Vector3<T>& v);

template <typename T>
T dist(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
T distSquared(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> normalize(const Vector3<T>& v);

template <typename T>
T sum(const Vector3<T>& v);

template <typename T>
T dot(const Vector3<T>& a, const Vector3<T>& b);

template <typename T>
Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Vector3<float>		Vector3f;
typedef Vector3<double>		Vector3d;
typedef Vector3<Int32>		Vector3i;
typedef Vector3<Uint32>		Vector3u;
typedef Vector3<bool>		Vector3b;

}

#include <poly/Math/Vector3.inl>

#endif