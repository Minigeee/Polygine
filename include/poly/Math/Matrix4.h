#ifndef POLY_MATRIX4_H
#define POLY_MATRIX4_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector4.h>

namespace poly
{

template <typename T> struct Matrix2;
template <typename T> struct Matrix3;

template <typename T>
struct Matrix4
{
	Matrix4();

	explicit Matrix4(T s);

	Matrix4(
		T xx, T xy, T xz, T xw,
		T yx, T yy, T yz, T yw,
		T zx, T zy, T zz, T zw,
		T wx, T wy, T wz, T ww
	);

	Matrix4(const Matrix4<T>& m);

	explicit Matrix4(
		const Vector4<T>& x,
		const Vector4<T>& y,
		const Vector4<T>& z,
		const Vector4<T>& w
	);


	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	template <typename U>
	explicit Matrix4(U s);

	template <
		typename XX, typename XY, typename XZ, typename XW,
		typename YX, typename YY, typename YZ, typename YW,
		typename ZX, typename ZY, typename ZZ, typename ZW,
		typename WX, typename WY, typename WZ, typename WW>
		Matrix4(
			XX xx, XY xy, XZ xz, XW xw,
			YX yx, YY yy, YZ yz, YW yw,
			ZX zx, ZY zy, ZZ zz, ZW zw,
			WX wx, WY wy, WZ wz, WW ww
		);

	template <typename U>
	Matrix4(const Matrix4<U>& m);

	template <typename A, typename B, typename C, typename D>
	explicit Matrix4(
		const Vector4<A>& x,
		const Vector4<B>& y,
		const Vector4<C>& z,
		const Vector4<D>& w
	);

	template <typename A, typename B, typename C>
	explicit Matrix4(const Matrix2<A>& xy, B z, C w);

	template <typename A, typename B, typename C>
	explicit Matrix4(A x, B y, const Matrix2<C>& zw);

	template <typename A, typename B>
	explicit Matrix4(const Matrix3<A>& xyz, B w);

	template <typename A, typename B>
	explicit Matrix4(A x, const Matrix3<B>& yzw);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	Vector4<T> x;
	Vector4<T> y;
	Vector4<T> z;
	Vector4<T> w;
};

///////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, U b);

template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, const Matrix4<U>& b);

template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, U b);

template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, const Matrix4<U>& b);

template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, U b);

template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, const Matrix4<U>& b);

template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, U b);

template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, const Matrix4<U>& b);


template <typename T>
Matrix4<T> operator-(const Matrix4<T>& m);

template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, T b);

template <typename T>
Matrix4<T> operator+(T a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, T b);

template <typename T>
Matrix4<T> operator-(T a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, T b);

template <typename T>
Matrix4<T> operator*(T a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, const Matrix4<T>& b);

template <typename T>
Vector4<T> operator*(const Matrix4<T>& m, const Vector4<T>& v);

template <typename T>
Vector4<T> operator*(const Vector4<T>& v, const Matrix4<T>& m);

template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, T b);

template <typename T>
Matrix4<T> operator/(T a, const Matrix4<T>& b);

template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, const Matrix4<T>& b);


template <typename T>
bool operator==(const Matrix4<T>& a, const Matrix4<T>& b);

template <typename T>
bool operator!=(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

template <typename T>
Matrix4<T> elem(const Matrix4<T>& a, const Matrix4<T>& b);

template <typename T>
T determinant(const Matrix4<T>& m);

template <typename T>
Matrix4<T> inverse(const Matrix4<T>& m);

template <typename T>
Matrix4<T> transpose(const Matrix4<T>& m);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Matrix4<float>		Matrix4f;
typedef Matrix4<double>		Matrix4d;
typedef Matrix4<Int32>		Matrix4i;
typedef Matrix4<Uint32>		Matrix4u;
typedef Matrix4<bool>		Matrix4b;

}

#include <poly/Math/Matrix4.inl>

#endif