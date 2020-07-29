#ifndef POLY_MATRIX3_H
#define POLY_MATRIX3_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector3.h>

namespace poly
{

template <typename T> struct Matrix2;
template <typename T> struct Matrix4;

template <typename T>
struct Matrix3
{
	Matrix3();

	explicit Matrix3(T s);

	Matrix3(
		T xx, T xy, T xz,
		T yx, T yy, T yz,
		T zx, T zy, T zz
	);

	Matrix3(const Matrix3<T>& m);

	explicit Matrix3(
		const Vector3<T>& x,
		const Vector3<T>& y,
		const Vector3<T>& z
	);


	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Matrix3(U s);

	template <
		typename XX, typename XY, typename XZ,
		typename YX, typename YY, typename YZ,
		typename ZX, typename ZY, typename ZZ>
	Matrix3(
		XX xx, XY xy, XZ xz,
		YX yx, YY yy, YZ yz,
		ZX zx, ZY zy, ZZ zz
	);

	template <typename U>
	Matrix3(const Matrix3<U>& m);

	template <typename A, typename B, typename C>
	explicit Matrix3(
		const Vector3<A>& x,
		const Vector3<B>& y,
		const Vector3<C>& z
	);

	template <typename A, typename B>
	explicit Matrix3(const Matrix2<A>& xy, B z);

	template <typename A, typename B>
	explicit Matrix3(A x, const Matrix2<B>& yz);

	template <typename U>
	explicit Matrix3(const Matrix4<U>& m);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	Vector3<T> x;
	Vector3<T> y;
	Vector3<T> z;
};

///////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix3<T>& operator+=(Matrix3<T>& a, U b);

template <typename T, typename U>
Matrix3<T>& operator+=(Matrix3<T>& a, const Matrix3<U>& b);

template <typename T, typename U>
Matrix3<T>& operator-=(Matrix3<T>& a, U b);

template <typename T, typename U>
Matrix3<T>& operator-=(Matrix3<T>& a, const Matrix3<U>& b);

template <typename T, typename U>
Matrix3<T>& operator*=(Matrix3<T>& a, U b);

template <typename T, typename U>
Matrix3<T>& operator*=(Matrix3<T>& a, const Matrix3<U>& b);

template <typename T, typename U>
Matrix3<T>& operator/=(Matrix3<T>& a, U b);

template <typename T, typename U>
Matrix3<T>& operator/=(Matrix3<T>& a, const Matrix3<U>& b);


template <typename T>
Matrix3<T> operator-(const Matrix3<T>& m);

template <typename T>
Matrix3<T> operator+(const Matrix3<T>& a, T b);

template <typename T>
Matrix3<T> operator+(T a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator+(const Matrix3<T>& a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator-(const Matrix3<T>& a, T b);

template <typename T>
Matrix3<T> operator-(T a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator-(const Matrix3<T>& a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator*(const Matrix3<T>& a, T b);

template <typename T>
Matrix3<T> operator*(T a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator*(const Matrix3<T>& a, const Matrix3<T>& b);

template <typename T>
Vector3<T> operator*(const Matrix3<T>& m, const Vector3<T>& v);

template <typename T>
Vector3<T> operator*(const Vector3<T>& v, const Matrix3<T>& m);

template <typename T>
Matrix3<T> operator/(const Matrix3<T>& a, T b);

template <typename T>
Matrix3<T> operator/(T a, const Matrix3<T>& b);

template <typename T>
Matrix3<T> operator/(const Matrix3<T>& a, const Matrix3<T>& b);


template <typename T>
bool operator==(const Matrix3<T>& a, const Matrix3<T>& b);

template <typename T>
bool operator!=(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

template <typename T>
Matrix3<T> elem(const Matrix3<T>& a, const Matrix3<T>& b);

template <typename T>
T determinant(const Matrix3<T>& m);

template <typename T>
Matrix3<T> inverse(const Matrix3<T>& m);

template <typename T>
Matrix3<T> transpose(const Matrix3<T>& m);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Matrix3<float>		Matrix3f;
typedef Matrix3<double>		Matrix3d;
typedef Matrix3<Int32>		Matrix3i;
typedef Matrix3<Uint32>		Matrix3u;
typedef Matrix3<bool>		Matrix3b;

}

#include <poly/Math/Matrix3.inl>

#endif