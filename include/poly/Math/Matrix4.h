#ifndef POLY_MATRIX4_H
#define POLY_MATRIX4_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector4.h>

namespace poly
{

template <typename T> struct Matrix2;
template <typename T> struct Matrix3;

///////////////////////////////////////////////////////////
/// \brief 4x4 square matrix struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Matrix4
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor initializes with identity matrix
	///
	///////////////////////////////////////////////////////////
	Matrix4();

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix with a scalar
	///
	/// Equivalent to (idendity matrix) * scalar, the diagonal
	/// will have values equal to the scalar.
	///
	/// \param s A scalar to initialize the matrix
	///
	///////////////////////////////////////////////////////////
	explicit Matrix4(T s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from its values
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param xx The XX element
	/// \param xy The XY element
	/// \param xz The XZ element
	/// \param xw The XW element
	/// \param yx The YX element
	/// \param yy The YY element
	/// \param yz The YZ element
	/// \param yw The YW element
	/// \param zx The ZX element
	/// \param zy The ZY element
	/// \param zz The ZZ element
	/// \param zw The ZW element
	/// \param wx The WX element
	/// \param wy The WY element
	/// \param wz The WZ element
	/// \param ww The WW element
	///
	///////////////////////////////////////////////////////////
	Matrix4(
		T xx, T xy, T xz, T xw,
		T yx, T yy, T yz, T yw,
		T zx, T zy, T zz, T zw,
		T wx, T wy, T wz, T ww
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	Matrix4(const Matrix4<T>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 4 Vector4
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param x The X vector
	/// \param y The Y vector
	/// \param z The Z vector
	/// \param w The W vector
	///
	///////////////////////////////////////////////////////////
	explicit Matrix4(
		const Vector4<T>& x,
		const Vector4<T>& y,
		const Vector4<T>& z,
		const Vector4<T>& w
	);


	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix with a scalar
	///
	/// Equivalent to (idendity matrix) * scalar, the diagonal
	/// will have values equal to the scalar.
	///
	/// \param s A scalar to initialize the matrix
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Matrix4(U s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from its values
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param xx The XX element
	/// \param xy The XY element
	/// \param xz The XZ element
	/// \param xw The XW element
	/// \param yx The YX element
	/// \param yy The YY element
	/// \param yz The YZ element
	/// \param yw The YW element
	/// \param zx The ZX element
	/// \param zy The ZY element
	/// \param zz The ZZ element
	/// \param zw The ZW element
	/// \param wx The WX element
	/// \param wy The WY element
	/// \param wz The WZ element
	/// \param ww The WW element
	///
	///////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Matrix4(const Matrix4<U>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 4 Vector4
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param x The X vector
	/// \param y The Y vector
	/// \param z The Z vector
	/// \param w The W vector
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C, typename D>
	explicit Matrix4(
		const Vector4<A>& x,
		const Vector4<B>& y,
		const Vector4<C>& z,
		const Vector4<D>& w
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 2x2 matrix and 2 scalars
	///
	/// Constructed by overlaying a 2x2 matrix on the top-left
	/// corner of the matrix, and scalars in the bottom-right.
	/// The rest is filled with 0.
	///
	/// \param xy A 2x2 matrix that will occupy the X and Y rows and columns
	/// \param z A scalar that will occupy the ZZ element
	/// \param w A scalar that will occupy the WW element
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Matrix4(const Matrix2<A>& xy, B z, C w);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 2x2 matrix and 2 scalars
	///
	/// Constructed by overlaying a 2x2 matrix on the bottom-right
	/// corner of the matrix, and scalars in the top-left.
	/// The rest is filled with 0.
	///
	/// \param x A scalar that will occupy the XX element
	/// \param y A scalar that will occupy the YY element
	/// \param zw A 2x2 matrix that will occupy the Z and W rows and columns
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Matrix4(A x, B y, const Matrix2<C>& zw);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 3x3 matrix and a scalar
	///
	/// Constructed by overlaying a 3x3 matrix on the top-left
	/// corner of the matrix, and a scalar in the bottom-right.
	/// The rest is filled with 0.
	///
	/// \param xyz A 3x3 matrix that will occupy the X, Y, and Z rows and columns
	/// \param w A scalar that will occupy the WW element
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Matrix4(const Matrix3<A>& xyz, B w);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 3x3 matrix and a scalar
	///
	/// Constructed by overlaying a 3x3 matrix on the bottom-right
	/// corner of the matrix, and a scalar in the top-left.
	/// The rest is filled with 0.
	///
	/// \param x A scalar that will occupy the XX element
	/// \param yzw A 3x3 matrix that will occupy the Y, Z, and W rows and columns
	///
	///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator +=
///
/// Elementwise addition of \a b (scalar) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator +=
///
/// Elementwise addition of \a b (matrix) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, const Matrix4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator -=
///
/// Elementwise subtraction of \a b (scalar) from \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator -=
///
/// Elementwise subtraction of \a b (matrix) from \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, const Matrix4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *=
///
/// Elementwise multiplication of \a b (scalar) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *=
///
/// Matrix multiplication of \a b (matrix) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, const Matrix4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator /=
///
/// Elementwise division of \a a (matrix) by \a b (scalar).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator /=
///
/// Elementwise division of \a a (matrix) by \a b (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, const Matrix4<U>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of unary operator -
///
/// Returns the elementwise negative of the matrix
///
/// \param m A matrix
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator-(const Matrix4<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (scalar) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (matrix) and \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator+(T a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (matrix) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (scalar) from \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (matrix) from \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator-(T a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (matrix) from \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *
///
/// Elementwise multiplcation of \a b (scalar) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *
///
/// Elementwise multiplcation of \a b (matrix) and \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator*(T a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *
///
/// Matrix multiplcation of \a b (matrix) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *
///
/// Matrix multiplcation of \a b (vector) and \a a (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector4<T> operator*(const Matrix4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator *
///
/// Matrix multiplcation of \a b (matrix) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (matrix)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector4<T> operator*(const Vector4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator /
///
/// Elementwise division of \a a (matrix) by \a b (scalar).
///
/// \param a The left operand (matrix)
/// \param b The right operand (scalar)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator /
///
/// Elementwise division of \a a (scalar) by \a b (matrix).
///
/// \param a The left operand (scalar)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator/(T a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator /
///
/// Elementwise division of \a a (matrix) by \a b (matrix).
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, const Matrix4<T>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator ==
///
/// Does a cumulative equality comparison between the two matrices
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return True if all corresponding elements in both matrices are equal to each other
///
///////////////////////////////////////////////////////////
template <typename T>
bool operator==(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Overload of operator ==
///
/// Does a cumulative inequality comparison between the two matrices
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return True if at least one set of corresponding elements are not equal to each other
///
///////////////////////////////////////////////////////////
template <typename T>
bool operator!=(const Matrix4<T>& a, const Matrix4<T>& b);


///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Does an elementwise multiplication
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> elem(const Matrix4<T>& a, const Matrix4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Get the determninant of a matrix
///
/// \param m A matrix
///
/// \return The determinant
///
///////////////////////////////////////////////////////////
template <typename T>
T determinant(const Matrix4<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Get the inverse of a matrix
///
/// This inverse function will not check the validity of the
/// matrix, so make sure the matrix has a valid inverse
/// before using this function. Taking the inverse of an
/// invalid matrix can lead to undefined behavior.
///
/// \param m A matrix
///
/// \return The resulting inverse matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix4<T> inverse(const Matrix4<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix4
/// \brief Get the transpose of a matrix
///
/// \param m A matrix
///
/// \return The resulting inverse matrix
///
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
/// \struct poly::Matrix4
/// \ingroup Math
///
/// Matrix4 is a templated struct that defines a 4x4 matrix.
/// Matrices can be used for several purposes, but their main
/// is to transform vectors and other matrices.
///
/// Either row major or column major can be used with Matrix4.
/// Matrices will always store their data in the same way
/// (xx, xy, xz, xw, yx, yy, yz, ...), the only difference is
/// what the first of the two elements refers to, either the
/// row or the column. For example, while using column major order,
/// yz will refer to column y and row z. If using row major order,
/// it will refer to row y and column z.
///
/// Overloads for the common mathematic operations have been
/// provided, such as +, -, *, and /. All these operations are
/// elementwise functions, except when multiplying a matrix
/// by another matrix, or by a vector. In those cases, normal
/// matrix multiplication is used. If elementwise multiplcation
/// is needed, use the elem() function. A few common matrix functions
/// have also been provided, such as determinant(), inverse(), and
/// transpose().
///
/// Most of the time, using the templated version is not needed.
/// Using one of the typedefs is easier:
/// \li ::Matrix4i
/// \li ::Matrix4u
/// \li ::Matrix4f
/// \li ::Matrix4d
/// \li ::Matrix4b
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Matrix4f a(2.0f); // Same as Matrix4f() * 2.0f
/// Matrix4f b(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
///
/// Matrix4f c = a + b;
/// float d = determinant(b);
///
/// c = a * inverse(b); // Returns identity matrix
///
/// \endcode
///
///////////////////////////////////////////////////////////