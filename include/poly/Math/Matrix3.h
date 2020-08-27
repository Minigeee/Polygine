#ifndef POLY_MATRIX3_H
#define POLY_MATRIX3_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector3.h>

namespace poly
{

template <typename T> struct Matrix2;
template <typename T> struct Matrix4;

///////////////////////////////////////////////////////////
/// \brief 3x3 square matrix struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Matrix3
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor initializes with identity matrix
	///
	///////////////////////////////////////////////////////////
	Matrix3();

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix with a scalar
	///
	/// Equivalent to (idendity matrix) * scalar, the diagonal
	/// will have values equal to the scalar.
	///
	/// \param s A scalar to initialize the matrix
	///
	///////////////////////////////////////////////////////////
	explicit Matrix3(T s);

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
	/// \param yx The YX element
	/// \param yy The YY element
	/// \param yz The YZ element
	/// \param zx The ZX element
	/// \param zy The ZY element
	/// \param zz The ZZ element
	///
	///////////////////////////////////////////////////////////
	Matrix3(
		T xx, T xy, T xz,
		T yx, T yy, T yz,
		T zx, T zy, T zz
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	Matrix3(const Matrix3<T>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 3 Vector3
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
	///
	///////////////////////////////////////////////////////////
	explicit Matrix3(
		const Vector3<T>& x,
		const Vector3<T>& y,
		const Vector3<T>& z
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
	explicit Matrix3(U s);

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
	/// \param yx The YX element
	/// \param yy The YY element
	/// \param yz The YZ element
	/// \param zx The ZX element
	/// \param zy The ZY element
	/// \param zz The ZZ element
	///
	///////////////////////////////////////////////////////////
	template <
		typename XX, typename XY, typename XZ,
		typename YX, typename YY, typename YZ,
		typename ZX, typename ZY, typename ZZ>
	Matrix3(
		XX xx, XY xy, XZ xz,
		YX yx, YY yy, YZ yz,
		ZX zx, ZY zy, ZZ zz
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Matrix3(const Matrix3<U>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 3 Vector3
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
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Matrix3(
		const Vector3<A>& x,
		const Vector3<B>& y,
		const Vector3<C>& z
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 2x2 matrix and a scalar
	///
	/// Constructed by overlaying a 2x2 matrix on the top-left
	/// corner of the matrix, and a scalar in the bottom-right.
	/// The rest is filled with 0.
	///
	/// \param xy A 2x2 matrix that will occupy the X and Y rows and columns
	/// \param z A scalar that will occupy the ZZ element
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Matrix3(const Matrix2<A>& xy, B z);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 2x2 matrix and a scalar
	///
	/// Constructed by overlaying a 2x2 matrix on the bottom-right
	/// corner of the matrix, and a scalar in the top-left.
	/// The rest is filled with 0.
	///
	/// \param x A scalar that will occupy the XX element
	/// \param yz A 2x2 matrix that will occupy the Y and Z rows and columns
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Matrix3(A x, const Matrix2<B>& yz);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 4x4 matrix
	///
	/// Only the X and Y rows and columns are copied from the
	/// 4x4 matrix.
	///
	/// \param m A 4x4 matrix
	///
	///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator+=(Matrix3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator+=(Matrix3<T>& a, const Matrix3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator-=(Matrix3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator-=(Matrix3<T>& a, const Matrix3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator*=(Matrix3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator*=(Matrix3<T>& a, const Matrix3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator/=(Matrix3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T>& operator/=(Matrix3<T>& a, const Matrix3<U>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator-(const Matrix3<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator+(const Matrix3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator+(T a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator+(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator-(const Matrix3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator-(T a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator-(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator*(const Matrix3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator*(T a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator*(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Vector3<T> operator*(const Matrix3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Vector3<T> operator*(const Vector3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator/(const Matrix3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator/(T a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> operator/(const Matrix3<T>& a, const Matrix3<T>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix3
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
bool operator==(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
bool operator!=(const Matrix3<T>& a, const Matrix3<T>& b);


///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Matrix3
/// \brief Does an elementwise multiplication
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix3<T> elem(const Matrix3<T>& a, const Matrix3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix3
/// \brief Get the determninant of a matrix
///
/// \param m A matrix
///
/// \return The determinant
///
///////////////////////////////////////////////////////////
template <typename T>
T determinant(const Matrix3<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix3
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
Matrix3<T> inverse(const Matrix3<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix3
/// \brief Get the transpose of a matrix
///
/// \param m A matrix
///
/// \return The resulting inverse matrix
///
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
/// \struct poly::Matrix3
/// \ingroup Math
///
/// Matrix3 is a templated struct that defines a 3x3 matrix.
/// Matrices can be used for several purposes, but their main
/// is to transform vectors and other matrices.
///
/// Either row major or column major can be used with Matrix3.
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
/// \li ::Matrix3i
/// \li ::Matrix3u
/// \li ::Matrix3f
/// \li ::Matrix3d
/// \li ::Matrix3b
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Matrix3f a(2.0f); // Same as Matrix3f() * 2.0f
/// Matrix3f b(1, 2, 3, 4, 5, 6, 7, 8, 9);
///
/// Matrix3f c = a + b;
/// float d = determinant(b);
///
/// c = a * inverse(b); // Returns identity matrix
///
/// \endcode
///
///////////////////////////////////////////////////////////