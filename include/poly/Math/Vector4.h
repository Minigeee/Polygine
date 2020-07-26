#ifndef POLY_VECTOR4_H
#define POLY_VECTOR4_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector2;
template <typename T> struct Vector3;

template <typename T>
struct Vector4
{
	Vector4();

	explicit Vector4(T s);

	Vector4(T x, T y, T z, T w);

	Vector4(const Vector4<T>& v);

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Vector4(U s);

	template <typename X, typename Y, typename Z, typename W>
	Vector4(X x, Y y, Z z, W w);

	template <typename U>
	Vector4(const Vector4<U>& v);

	template <typename A, typename B, typename C>
	explicit Vector4(const Vector2<A>& xy, B z, C w);

	template <typename A, typename B, typename C>
	explicit Vector4(A x, const Vector2<B>& yz, C w);

	template <typename A, typename B, typename C>
	explicit Vector4(A x, B y, const Vector2<C>& zw);

	template <typename A, typename B>
	explicit Vector4(const Vector3<A>& xyz, B w);

	template <typename A, typename B>
	explicit Vector4(A x, const Vector3<B>& yzw);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	union { T x, r; };
	union { T y, g; };
	union { T z, b; };
	union { T w, a; };
};

///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

template <typename T, typename U>
Vector4<T>& operator+=(Vector4<T>& a, U b);

template <typename T, typename U>
Vector4<T>& operator+=(Vector4<T>& a, const Vector4<U>& b);

template <typename T, typename U>
Vector4<T>& operator-=(Vector4<T>& a, U b);

template <typename T, typename U>
Vector4<T>& operator-=(Vector4<T>& a, const Vector4<U>& b);

template <typename T, typename U>
Vector4<T>& operator*=(Vector4<T>& a, U b);

template <typename T, typename U>
Vector4<T>& operator*=(Vector4<T>& a, const Vector4<U>& b);

template <typename T, typename U>
Vector4<T>& operator/=(Vector4<T>& a, U b);

template <typename T, typename U>
Vector4<T>& operator/=(Vector4<T>& a, const Vector4<U>& b);

template <typename T, typename U>
Vector4<T>& operator%=(Vector4<T>& a, U b);

template <typename U>
Vector4<float>& operator%=(Vector4<float>& a, U b);

template <typename U>
Vector4<double>& operator%=(Vector4<double>& a, U b);

template <typename T, typename U>
Vector4<T>& operator%=(Vector4<T>& a, const Vector4<U>& b);

template <typename U>
Vector4<float>& operator%=(Vector4<float>& a, const Vector4<U>& b);

template <typename U>
Vector4<double>& operator%=(Vector4<double>& a, const Vector4<U>& b);


template <typename T>
Vector4<T> operator-(const Vector4<T>& v);

template <typename T>
Vector4<T> operator+(const Vector4<T>& v, T s);

template <typename T>
Vector4<T> operator+(T s, const Vector4<T>& v);

template <typename T>
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
Vector4<T> operator-(const Vector4<T>& v, T s);

template <typename T>
Vector4<T> operator-(T s, const Vector4<T>& v);

template <typename T>
Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
Vector4<T> operator*(const Vector4<T>& v, T s);

template <typename T>
Vector4<T> operator*(T s, const Vector4<T>& v);

template <typename T>
Vector4<T> operator*(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
Vector4<T> operator/(const Vector4<T>& v, T s);

template <typename T>
Vector4<T> operator/(T s, const Vector4<T>& v);

template <typename T>
Vector4<T> operator/(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
Vector4<T> operator%(const Vector4<T>& v, T s);

template <typename T>
Vector4<T> operator%(T s, const Vector4<T>& v);

template <typename T>
Vector4<T> operator%(const Vector4<T>& a, const Vector4<T>& b);

template <>
Vector4<float> operator%(const Vector4<float>& v, float s);

template <>
Vector4<float> operator%(float s, const Vector4<float>& v);

template <>
Vector4<float> operator%(const Vector4<float>& a, const Vector4<float>& b);

template <>
Vector4<double> operator%(const Vector4<double>& v, double s);

template <>
Vector4<double> operator%(double s, const Vector4<double>& v);

template <>
Vector4<double> operator%(const Vector4<double>& a, const Vector4<double>& b);

template <typename T>
bool operator==(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
bool operator!=(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

template <typename T>
Vector4<T> floor(const Vector4<T>& v);

template <typename T>
Vector4<T> ceil(const Vector4<T>& v);

template <typename T>
Vector4<T> round(const Vector4<T>& v);

template <typename T>
Vector4<int> lround(const Vector4<T>& v);

template <typename T>
Vector4<T> abs(const Vector4<T>& v);

template <typename T>
Vector4<T> pow(const Vector4<T>& b, T p);

template <typename T>
Vector4<T> pow(const Vector4<T>& b, const Vector4<T>& p);

template <typename T>
Vector4<T> sqrt(const Vector4<T>& v);

///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

template <typename T>
T length(const Vector4<T>& v);

template <typename T>
T dist(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
T distSquared(const Vector4<T>& a, const Vector4<T>& b);

template <typename T>
Vector4<T> normalize(const Vector4<T>& v);

template <typename T>
T sum(const Vector4<T>& v);

template <typename T>
T dot(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Vector4<float>		Vector4f;
typedef Vector4<double>		Vector4d;
typedef Vector4<Int32>		Vector4i;
typedef Vector4<Uint32>		Vector4u;
typedef Vector4<bool>		Vector4b;

}

#include <poly/Math/Vector4.inl>

#endif