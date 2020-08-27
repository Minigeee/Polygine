#ifndef POLY_MATRIX2_H
#define POLY_MATRIX2_H

#include <poly/Core/DataTypes.h>
#include <poly/Math/Vector2.h>

namespace poly
{

template <typename T> struct Matrix3;
template <typename T> struct Matrix4;

///////////////////////////////////////////////////////////
/// \brief 2x2 square matrix struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Matrix2
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor initializes with identity matrix
	///
	///////////////////////////////////////////////////////////
	Matrix2();

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix with a scalar
	///
	/// Equivalent to (idendity matrix) * scalar, the diagonal
	/// will have values equal to the scalar.
	///
	/// \param s A scalar to initialize the matrix
	///
	///////////////////////////////////////////////////////////
	explicit Matrix2(T s);

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
	/// \param yx The YX element
	/// \param yy The YY element
	///
	///////////////////////////////////////////////////////////
	Matrix2(
		T xx, T xy,
		T yx, T yy
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	Matrix2(const Matrix2<T>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 2 Vector2
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param x The X vector
	/// \param y The Y vector
	///
	///////////////////////////////////////////////////////////
	explicit Matrix2(
		const Vector2<T>& x,
		const Vector2<T>& y
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
	explicit Matrix2(U s);

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
	/// \param yx The YX element
	/// \param yy The YY element
	///
	///////////////////////////////////////////////////////////
	template <
		typename XX, typename XY,
		typename YX, typename YY>
	Matrix2(
		XX xx, XY xy,
		YX yx, YY yy
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from another matrix
	///
	/// \param m Another matrix
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Matrix2(const Matrix2<U>& m);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from 2 Vector2
	///
	/// The major order of the matrix is determined by if
	/// USE_COLUMN_MAJOR is defined or not. When using row major,
	/// the first element will refer to the row in the matrix.
	/// When using column major, the first element will refer to
	/// the column.
	///
	/// \param x The X vector
	/// \param y The Y vector
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Matrix2(
		const Vector2<A>& x,
		const Vector2<B>& y
	);

	///////////////////////////////////////////////////////////
	/// \brief Construct a matrix from a 3x3 matrix
	///
	/// Only the X and Y rows and columns are copied from the
	/// 3x3 matrix.
	///
	/// \param m A 3x3 matrix
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Matrix2(const Matrix3<U>& m);

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
	explicit Matrix2(const Matrix4<U>& m);


	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	Vector2<T> x;
	Vector2<T> y;
};

///////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator+=(Matrix2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator+=(Matrix2<T>& a, const Matrix2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator-=(Matrix2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator-=(Matrix2<T>& a, const Matrix2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator*=(Matrix2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator*=(Matrix2<T>& a, const Matrix2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator/=(Matrix2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T>& operator/=(Matrix2<T>& a, const Matrix2<U>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator-(const Matrix2<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator+(const Matrix2<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator+(T a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator+(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator-(const Matrix2<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator-(T a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator-(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator*(const Matrix2<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator*(T a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator*(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Vector2<T> operator*(const Matrix2<T>& a, const Vector2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Vector2<T> operator*(const Vector2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator/(const Matrix2<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator/(T a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> operator/(const Matrix2<T>& a, const Matrix2<T>& b);


///////////////////////////////////////////////////////////
/// \relates Matrix2
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
bool operator==(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
bool operator!=(const Matrix2<T>& a, const Matrix2<T>& b);


///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Matrix2
/// \brief Does an elementwise multiplication
///
/// \param a The left operand (matrix)
/// \param b The right operand (matrix)
///
/// \return The resulting matrix
///
///////////////////////////////////////////////////////////
template <typename T>
Matrix2<T> elem(const Matrix2<T>& a, const Matrix2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Matrix2
/// \brief Get the determninant of a matrix
///
/// \param m A matrix
///
/// \return The determinant
///
///////////////////////////////////////////////////////////
template <typename T>
T determinant(const Matrix2<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix2
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
Matrix2<T> inverse(const Matrix2<T>& m);

///////////////////////////////////////////////////////////
/// \relates Matrix2
/// \brief Get the transpose of a matrix
///
/// \param m A matrix
///
/// \return The resulting inverse matrix
///
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
/// \struct poly::Matrix2
/// \ingroup Math
///
/// Matrix2 is a templated struct that defines a 2x2 matrix.
/// Matrices can be used for several purposes, but their main
/// is to transform vectors and other matrices.
///
/// Either row major or column major can be used with Matrix2.
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
/// \li ::Matrix2i
/// \li ::Matrix2u
/// \li ::Matrix2f
/// \li ::Matrix2d
/// \li ::Matrix2b
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Matrix2f a(2.0f); // Same as Matrix2f() * 2.0f
/// Matrix2f b(1, 2, 3, 4);
///
/// Matrix2f c = a + b;
/// float d = determinant(b);
///
/// c = a * inverse(a); // Returns identity matrix
///
/// \endcode
///
///////////////////////////////////////////////////////////