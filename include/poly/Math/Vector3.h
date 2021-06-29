#ifndef POLY_VECTOR3_H
#define POLY_VECTOR3_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector2;
template <typename T> struct Vector4;

///////////////////////////////////////////////////////////
/// \brief A templated 3D vector struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Vector3
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes all components with 0.
	///
	///////////////////////////////////////////////////////////
	Vector3();

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with a scalar
	///
	/// Initializes all components with the scalar
	///
	/// \param s A scalar
	///
	///////////////////////////////////////////////////////////
	explicit Vector3(T s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component
	/// \param y The y component
	/// \param z The z component
	///
	///////////////////////////////////////////////////////////
	Vector3(T x, T y, T z);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of the same type
	///
	/// \param v A vector of the same type
	///
	///////////////////////////////////////////////////////////
	Vector3(const Vector3<T>& v);


	///////////////////////////////////////////////////////////
	// Constructors
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with a scalar
	///
	/// Initializes all components with the scalar
	///
	/// \param s A scalar of a differnt type
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Vector3(U s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component of a different type
	/// \param y The y component of a different type
	/// \param z The z component of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename X, typename Y, typename Z>
	Vector3(X x, Y y, Z z);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of a different type
	///
	/// \param v A vector of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Vector3(const Vector3<U>& v);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector2 and a scalar
	///
	/// \param xy A Vector2 containing the x and y components
	/// \param z The z component
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Vector3(const Vector2<A>& xy, B z);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector2 and a scalar
	///
	/// \param x The x component
	/// \param yz A Vector2 containing the y and z components
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Vector3(A x, const Vector2<B>& yz);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector4
	///
	/// Only the x and y components will be used
	///
	/// \param v A Vector4
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Vector3(const Vector4<U>& v);

	///////////////////////////////////////////////////////////
	// Members
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	/// \brief The X and R component that share the same space
	///
	///////////////////////////////////////////////////////////
	union
	{
		T x; //!< The X component
		T r; //!< The R component
	};

	///////////////////////////////////////////////////////////
	/// \brief The Y and G component that share the same space
	///
	///////////////////////////////////////////////////////////
	union
	{
		T y; //!< The Y component
		T g; //!< The G component
	};

	///////////////////////////////////////////////////////////
	/// \brief The Z and B component that share the same space
	///
	///////////////////////////////////////////////////////////
	union
	{
		T z; //!< The Z component
		T b; //!< The B component
	};
};

///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator +=
///
/// Elementwise add of \a b (scalar) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator+=(Vector3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overloade of operator +=
///
/// Elementwise add of \a b (vector) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator+=(Vector3<T>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator -=
///
/// Elementwise subtract of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator-=(Vector3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator -=
///
/// Elementwise subtract of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator-=(Vector3<T>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator *=
///
/// Elementwise multiplication of \a b (scalar) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator*=(Vector3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator *=
///
/// Elementwise multiplication of \a b (vector) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator*=(Vector3<T>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator /=
///
/// Elementwise division of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator/=(Vector3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator /=
///
/// Elementwise division of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator/=(Vector3<T>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator%=(Vector3<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
/// Specialization for \a float that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename U>
Vector3<float>& operator%=(Vector3<float>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
/// Specialization for \a double that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename U>
Vector3<double>& operator%=(Vector3<double>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
Vector3<T>& operator%=(Vector3<T>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
/// Specialization for \a float that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename U>
Vector3<float>& operator%=(Vector3<float>& a, const Vector3<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %=
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
/// Specialization for \a double that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return A reference to \a a
///
///////////////////////////////////////////////////////////
template <typename U>
Vector3<double>& operator%=(Vector3<double>& a, const Vector3<U>& b);


///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of unary operator -
///
/// Elementwise inverse operator.
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator-(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (scalar) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator+(const Vector3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (vector) and \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator+(T a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator +
///
/// Elementwise addition of \a b (vector) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator-(const Vector3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (vector) from \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator-(T a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator -
///
/// Elementwise subtraction of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator *
///
/// Elementwise multiplcation of \a b (scalar) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator*(const Vector3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator *
///
/// Elementwise multiplcation of \a b (vector) and \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator*(T a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator *
///
/// Elementwise multiplcation of \a b (vector) and \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator /
///
/// Elementwise division of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator/(const Vector3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator /
///
/// Elementwise division of \a b (vector) from \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator/(T a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator /
///
/// Elementwise division of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator%(const Vector3<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (scalar).
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator%(T a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> operator%(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
/// Specialization for \a float that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<float> operator%(const Vector3<float>& a, float b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (scalar).
/// Specialization for \a float that uses fmod().
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<float> operator%(float a, const Vector3<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
/// Specialization for \a float that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<float> operator%(const Vector3<float>& a, const Vector3<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (scalar) from \a a (vector).
/// Specialization for \a double that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (scalar)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<double> operator%(const Vector3<double>& a, double b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (scalar).
/// Specialization for \a double that uses fmod().
///
/// \param a The left operand (scalar)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<double> operator%(double a, const Vector3<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator %
///
/// Elementwise modulus of \a b (vector) from \a a (vector).
/// Specialization for \a double that uses fmod().
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <>
Vector3<double> operator%(const Vector3<double>& a, const Vector3<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator ==
///
/// Do an elementwise equality comparison between two vectors.
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return True if all corresponding elements in each vector equal each other
///
///////////////////////////////////////////////////////////
template <typename T>
bool operator==(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Overload of operator !=
///
/// Do an elementwise nonequality comparison between two vectors.
///
/// \param a The left operand (vector)
/// \param b The right operand (vector)
///
/// \return True if at least one set of corresponding elements don't equal each other
///
///////////////////////////////////////////////////////////
template <typename T>
bool operator!=(const Vector3<T>& a, const Vector3<T>& b);


///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the floor of a vector
///
/// Applys floor() to each of the vector's components
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> floor(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the ceiling of a vector
///
/// Applys ceil() to each of the vector's components
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> ceil(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the rounded version of a vector
///
/// Applys floor() to each of the vector's components.
/// The resulting vector will retain the data type
/// of the original vector. So if a Vector2f was used,
/// a Vector2f will be returned.
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> round(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the rounded version of a vector in integer form
///
/// Applys floor() to each of the vector's components.
/// The resulting vector will be an integer type.
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<int> lround(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the absolute value of a vector
///
/// Applys abs() to each of the vector's components
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> abs(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the result of a power operation on a vector
///
/// Applys pow(b, p) to each of the vector's component
///
/// \param b The base vector
/// \param p The power scalar
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> pow(const Vector3<T>& b, T p);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the result of a power operation on a vector
///
/// Applys pow(b, p) to each of the vector's component
///
/// \param b The base vector
/// \param p The power vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> pow(const Vector3<T>& b, const Vector3<T>& p);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the square root of a vector
///
/// Applys sqrt() to each of the vector's components
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> sqrt(const Vector3<T>& v);


///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the length (or magnitude) of a directional vector
///
/// Equivalent to calculating the distance between (0, 0, 0)
/// and the point the vector represents.
///
/// \param v The vector
///
/// \return The length (or magnitude) of the vector
///
///////////////////////////////////////////////////////////
template <typename T>
T length(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the distance between two point vectors
///
/// \param a The first vector, representing point 1
/// \param b The second vector, representing point 2
///
/// \return The distance between the two vectors
///
///////////////////////////////////////////////////////////
template <typename T>
T dist(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the squared distance between two point vectors
///
/// This operation is slightly faster than dist() because
/// it does not involve a sqrt() operation. This should only be
/// used when finding the actual distance does not matter,
/// and distance is only needed for comparison.
///
/// \param a The first vector, representing point 1
/// \param b The second vector, representing point 2
///
/// \return The distance between the two vectors
///
///////////////////////////////////////////////////////////
template <typename T>
T distSquared(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get a vector with the same direction, but with a magnitude of 1
///
/// The resulting vector will retain the same direction as the
/// original, but it will be a unit vector, a vector with a
/// magnitude of 1.
///
/// \param v The original vector
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> normalize(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the sum of all components in a vector
///
/// \param v The vector
///
/// \return The sum of the vector
///
///////////////////////////////////////////////////////////
template <typename T>
T sum(const Vector3<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the dot product of two vectors
///
/// The dot product of two vectors, a and b, is defined as:
/// length(a) * length(b) * cos(theta). Where theta is the
/// angle between the two vectors.
///
/// \param a The first vector
/// \param b The second vector
///
/// \return The dot product result (scalar)
///
///////////////////////////////////////////////////////////
template <typename T>
T dot(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Get the cross product of two vectors
///
/// The dot product of two vectors, a and b, is defined as:
/// length(a) * length(b) * sin(theta) * n. Where theta is the
/// angle between the two vectors, and n is the unit vector
/// pointing at a right angle from both a and b.
///
/// \param a The first vector
/// \param b The second vector
///
/// \return The cross product result (vector)
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector3
/// \brief Linearly interpolate between two vectors
///
/// When interpolating, using a \a factor of 0.0 will return
/// the first parameter, using a \a factor of 1.0 will return
/// the second parameter, and a value in between will mix
/// the values.
///
/// \param a The first vector to interpolate
/// \param b The second vector to interpolate
/// \param factor The interpolation factor
///
/// \return The interpolated vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector3<T> mix(const Vector3<T>& a, const Vector3<T>& b, float factor);

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

///////////////////////////////////////////////////////////
/// \struct poly::Vector3
///
/// Vector2 is a templated struct that defines a 3D vector. It can be used
/// to represent anything in 3 dimensions, such as points, directions,
/// velocities, colors, etc.
///
/// There are two coordinate sets that can be used to access
/// the vector's components: (x, y, z) or (r, g, b). X and R, as well as the
/// other sets, both refer to the same location in memory, so either one can
/// be used to access and modify the first component of the
/// vector.
///
/// Overloads for most of the common mathematics functions
/// have been provided, such as +, -, *, /, and %. For the modulus
/// operator, specialized functions have been provided for
/// floating point types that use fmod() to allow the % operator
/// to be useable with floating point type vectors. Several other
/// common math functions have been provided as well, such
/// as floor(), ceil(), abs(), sqrt(), pow(), etc. Most of these
/// functions are elementwise functions, so they will apply their
/// operation to each element individually.
///
/// Also, a few vector specific functions have been provided,
/// such as cross(), dot(), length(), dist(), normalize(), etc.
///
/// Most of the time, using the templated version is not needed.
/// Using one of the typedefs is easier:
/// \li ::Vector3i
/// \li ::Vector3u
/// \li ::Vector3f
/// \li ::Vector3d
/// \li ::Vector3b
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // It is possible to construct vectors using different types
/// Vector3f a(3.0f, 4, 5.0);
/// float l = length(a);		// 7.071f
/// a.x = 0.0f;
///
/// // Operator % uses fmod() for float
/// Vector3f b = a % 2.5f;		// (0.0f, 1.5f, 2.5f)
///
/// a *= 2.0f;					// (0.0f, 8.0f, 10.0f)
/// a += Vector3f(0.3f)			// (0.3f, 8.3f, 10.3f)
///
/// \endcode
///
///////////////////////////////////////////////////////////