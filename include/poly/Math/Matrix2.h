#ifndef POLY_MATRIX2_H
#define POLY_MATRIX2_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector2.h>

namespace poly
{

template <typename T> struct Matrix3;
template <typename T> struct Matrix4;

template <typename T>
struct Matrix2
{
	Matrix2();

	explicit Matrix2(T s);

	Matrix2(
		T xx, T xy,
		T yx, T yy
	);

	Matrix2(const Matrix2<T>& m);

	explicit Matrix2(
		const Vector2<T>& x,
		const Vector2<T>& y
	);


	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Matrix2(U s);

	template <
		typename XX, typename XY,
		typename YX, typename YY>
	Matrix2(
		XX xx, XY xy,
		YX yx, YY yy
	);

	template <typename U>
	Matrix2(const Matrix2<U>& m);

	template <typename A, typename B>
	explicit Matrix2(
		const Vector2<A>& x,
		const Vector2<B>& y
	);

	template <typename U>
	explicit Matrix2(const Matrix3<U>& m);

	template <typename U>
	explicit Matrix2(const Matrix4<U>& m);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	union
	{
		Vector2<T> x;
		Vector2<T> r;
	};

	union
	{
		Vector2<T> y;
		Vector2<T> g;
	};
};

///////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix2<T>& operator+=(Matrix2<T>& a, U b);

template <typename T, typename U>
Matrix2<T>& operator+=(Matrix2<T>& a, const Matrix2<U>& b);

template <typename T, typename U>
Matrix2<T>& operator-=(Matrix2<T>& a, U b);

template <typename T, typename U>
Matrix2<T>& operator-=(Matrix2<T>& a, const Matrix2<U>& b);

template <typename T, typename U>
Matrix2<T>& operator*=(Matrix2<T>& a, U b);

template <typename T, typename U>
Matrix2<T>& operator*=(Matrix2<T>& a, const Matrix2<U>& b);

template <typename T, typename U>
Matrix2<T>& operator/=(Matrix2<T>& a, U b);

template <typename T, typename U>
Matrix2<T>& operator/=(Matrix2<T>& a, const Matrix2<U>& b);


template <typename T>
Matrix2<T> operator-(const Matrix2<T>& m);

template <typename T>
Matrix2<T> operator+(const Matrix2<T>& a, T b);

template <typename T>
Matrix2<T> operator+(T a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator+(const Matrix2<T>& a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator-(const Matrix2<T>& a, T b);

template <typename T>
Matrix2<T> operator-(T a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator-(const Matrix2<T>& a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator*(const Matrix2<T>& a, T b);

template <typename T>
Matrix2<T> operator*(T a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator*(const Matrix2<T>& a, const Matrix2<T>& b);

template <typename T>
Vector2<T> operator*(const Matrix2<T>& m, const Vector2<T>& v);

template <typename T>
Vector2<T> operator*(const Vector2<T>& v, const Matrix2<T>& m);

template <typename T>
Matrix2<T> operator/(const Matrix2<T>& a, T b);

template <typename T>
Matrix2<T> operator/(T a, const Matrix2<T>& b);

template <typename T>
Matrix2<T> operator/(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

template <typename T>
Matrix2<T> elem(const Matrix2<T>& a, const Matrix2<T>& b);

template <typename T>
T determinant(const Matrix2<T>& m);

template <typename T>
Matrix2<T> inverse(const Matrix2<T>& m);

template <typename T>
Matrix2<T> transpose(const Matrix2<T>& m);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Matrix2<float>		Matrix2f;
typedef Matrix2<double>		Matrix2d;
typedef Matrix2<Int32>		Matrix2i;
typedef Matrix2<Uint32>		Matrix2u;
typedef Matrix2<bool>		Matrix2b;

}

#include <poly/Math/Matrix2.inl>

#endif