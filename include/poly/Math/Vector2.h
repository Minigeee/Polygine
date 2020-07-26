#ifndef POLY_VECTOR_H
#define POLY_VECTOR_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector3;
template <typename T> struct Vector4;

template <typename T>
struct Vector2
{
	Vector2();

	explicit Vector2(T s);

	Vector2(T x, T y);

	Vector2(const Vector2<T>& v);

	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Vector2(U s);

	template <typename A, typename B>
	Vector2(A x, B y);

	template <typename U>
	Vector2(const Vector2<U>& v);

	template <typename U>
	explicit Vector2(const Vector3<U>& v);

	template <typename U>
	explicit Vector2(const Vector4<U>& v);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	union { T x, r; };
	union { T y, g; };
};

///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

template <typename T, typename U>
Vector2<T>& operator+=(Vector2<T>& a, U b);

template <typename T, typename U>
Vector2<T>& operator+=(Vector2<T>& a, const Vector2<U>& b);

template <typename T, typename U>
Vector2<T>& operator-=(Vector2<T>& a, U b);

template <typename T, typename U>
Vector2<T>& operator-=(Vector2<T>& a, const Vector2<U>& b);

template <typename T, typename U>
Vector2<T>& operator*=(Vector2<T>& a, U b);

template <typename T, typename U>
Vector2<T>& operator*=(Vector2<T>& a, const Vector2<U>& b);

template <typename T, typename U>
Vector2<T>& operator/=(Vector2<T>& a, U b);

template <typename T, typename U>
Vector2<T>& operator/=(Vector2<T>& a, const Vector2<U>& b);

template <typename T, typename U>
Vector2<T>& operator%=(Vector2<T>& a, U b);

template <typename U>
Vector2<float>& operator%=(Vector2<float>& a, U b);

template <typename U>
Vector2<double>& operator%=(Vector2<double>& a, U b);

template <typename T, typename U>
Vector2<T>& operator%=(Vector2<T>& a, const Vector2<U>& b);

template <typename U>
Vector2<float>& operator%=(Vector2<float>& a, const Vector2<U>& b);

template <typename U>
Vector2<double>& operator%=(Vector2<double>& a, const Vector2<U>& b);


template <typename T>
Vector2<T> operator+(const Vector2<T>& v, T s);

template <typename T>
Vector2<T> operator+(T s, const Vector2<T>& v);

template <typename T>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> operator-(const Vector2<T>& v, T s);

template <typename T>
Vector2<T> operator-(T s, const Vector2<T>& v);

template <typename T>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> operator*(const Vector2<T>& v, T s);

template <typename T>
Vector2<T> operator*(T s, const Vector2<T>& v);

template <typename T>
Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> operator/(const Vector2<T>& v, T s);

template <typename T>
Vector2<T> operator/(T s, const Vector2<T>& v);

template <typename T>
Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> operator%(const Vector2<T>& v, T s);

template <typename T>
Vector2<T> operator%(T s, const Vector2<T>& v);

template <typename T>
Vector2<T> operator%(const Vector2<T>& a, const Vector2<T>& b);

template <>
Vector2<float> operator%(const Vector2<float>& v, float s);

template <>
Vector2<float> operator%(float s, const Vector2<float>& v);

template <>
Vector2<float> operator%(const Vector2<float>& a, const Vector2<float>& b);

template <>
Vector2<double> operator%(const Vector2<double>& v, double s);

template <>
Vector2<double> operator%(double s, const Vector2<double>& v);

template <>
Vector2<double> operator%(const Vector2<double>& a, const Vector2<double>& b);

template <typename T>
bool operator==(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
bool operator!=(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> operator-(const Vector2<T>& v);

///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

template <typename T>
Vector2<T> floor(const Vector2<T>& v);

template <typename T>
Vector2<T> ceil(const Vector2<T>& v);

template <typename T>
Vector2<T> round(const Vector2<T>& v);

template <typename T>
Vector2<int> lround(const Vector2<T>& v);

template <typename T>
Vector2<T> abs(const Vector2<T>& v);

template <typename T>
Vector2<T> pow(const Vector2<T>& b, T p);

template <typename T>
Vector2<T> pow(const Vector2<T>& b, const Vector2<T>& p);

template <typename T>
Vector2<T> sqrt(const Vector2<T>& v);

///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

template <typename T>
T length(const Vector2<T>& v);

template <typename T>
T dist(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
T distSquared(const Vector2<T>& a, const Vector2<T>& b);

template <typename T>
Vector2<T> normalize(const Vector2<T>& v);

template <typename T>
T sum(const Vector2<T>& v);

template <typename T>
T dot(const Vector2<T>& a, const Vector2<T>& b);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Vector2<float>		Vector2f;
typedef Vector2<double>		Vector2d;
typedef Vector2<Int32>		Vector2i;
typedef Vector2<Uint32>		Vector2u;
typedef Vector2<bool>		Vector2b;

}

#include <poly/Math/Vector2.inl>

#endif