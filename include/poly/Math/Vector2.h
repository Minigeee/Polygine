#ifndef POLY_VECTOR2_H
#define POLY_VECTOR2_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector3;
template <typename T> struct Vector4;

///////////////////////////////////////////////////////////
/// \brief A templated 2D vector struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Vector2
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes all components with 0.
	///
	///////////////////////////////////////////////////////////
	Vector2();

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with a scalar
	///
	/// Initializes all components with the scalar
	///
	/// \param s A scalar
	///
	///////////////////////////////////////////////////////////
	explicit Vector2(T s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component
	/// \param y The y component
	///
	///////////////////////////////////////////////////////////
	Vector2(T x, T y);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of the same type
	///
	/// \param v A vector of the same type
	///
	///////////////////////////////////////////////////////////
	Vector2(const Vector2<T>& v);


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
	explicit Vector2(U s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component of a different type
	/// \param y The y component of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename X, typename Y>
	Vector2(X x, Y y);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of a different type
	///
	/// \param v A vector of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Vector2(const Vector2<U>& v);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector3
	///
	/// Only the x and y components will be used
	///
	/// \param v A Vector3
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Vector2(const Vector3<U>& v);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector4
	///
	/// Only the x and y components will be used
	///
	/// \param v A Vector4
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	explicit Vector2(const Vector4<U>& v);


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
};


///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator+=(Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator+=(Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator-=(Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator-=(Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator*=(Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator*=(Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator/=(Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator/=(Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator%=(Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<float>& operator%=(Vector2<float>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<double>& operator%=(Vector2<double>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T>& operator%=(Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<float>& operator%=(Vector2<float>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<double>& operator%=(Vector2<double>& a, const Vector2<U>& b);


///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> operator-(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator+(const Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator+(T a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator-(const Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator-(T a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator*(const Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator*(T a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator*(const Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator/(const Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator/(T a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator/(const Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator%(const Vector2<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator%(T a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
template <typename T, typename U>
Vector2<T> operator%(const Vector2<T>& a, const Vector2<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<float> operator%(const Vector2<float>& a, float b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<float> operator%(float a, const Vector2<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<float> operator%(const Vector2<float>& a, const Vector2<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<double> operator%(const Vector2<double>& a, double b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<double> operator%(double a, const Vector2<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<double> operator%(const Vector2<double>& a, const Vector2<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
bool operator==(const Vector2<T>& a, const Vector2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
bool operator!=(const Vector2<T>& a, const Vector2<T>& b);


///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> floor(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> ceil(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> round(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<int> lround(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> abs(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> pow(const Vector2<T>& b, T p);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> pow(const Vector2<T>& b, const Vector2<T>& p);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> sqrt(const Vector2<T>& v);


///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Get the length (or magnitude) of a directional vector
///
/// Equivalent to calculating the distance between (0, 0)
/// and the point the vector represents.
///
/// \param v The vector
///
/// \return The length (or magnitude) of the vector
///
///////////////////////////////////////////////////////////
template <typename T>
T length(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Get the distance between two point vectors
///
/// \param a The first vector, representing point 1
/// \param b The second vector, representing point 2
///
/// \return The distance between the two vectors
///
///////////////////////////////////////////////////////////
template <typename T>
T dist(const Vector2<T>& a, const Vector2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
T distSquared(const Vector2<T>& a, const Vector2<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector2
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
Vector2<T> normalize(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
/// \brief Get the sum of all components in a vector
///
/// \param v The vector
///
/// \return The sum of the vector
///
///////////////////////////////////////////////////////////
template <typename T>
T sum(const Vector2<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector2
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


///////////////////////////////////////////////////////////
// Hash
///////////////////////////////////////////////////////////
#ifndef DOXYGEN_SKIP

namespace std
{

template <typename T>
struct hash<poly::Vector2<T>>
{
	std::size_t operator()(const poly::Vector2<T>& k) const
	{
		return hash<T>()(k.x) * hash<T>()(k.y);
	}
};

}

#endif


#include <poly/Math/Vector2.inl>

#endif

///////////////////////////////////////////////////////////
/// \struct poly::Vector2
///
/// Vector2 is a templated struct that defines a 2D vector. It can be used
/// to represent anything in 2 dimensions, such as points, directions,
/// velocities, etc.
///
/// There are two coordinate sets that can be used to access
/// the vector's components: (x, y) or (r, g). X and R, as well as the
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
/// such as dot(), length(), dist(), normalize(), etc.
///
/// Most of the time, using the templated version is not needed.
/// Using one of the typedefs is easier:
/// \li ::Vector2i
/// \li ::Vector2u
/// \li ::Vector2f
/// \li ::Vector2d
/// \li ::Vector2b
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // It is possible to construct vectors using different types
/// Vector2f a(3.0f, 4);
/// float l = length(a);		// 5.0f
/// a.x = 0.0f;
///
/// // Construct using a scalar
/// Vector2f b(0);
/// float d = dist(a, b);		// 4.0f
///
/// a = normalize(a);			// (0.6f, 0.8f)
/// a *= 2.0f;					// (1.2f, 1.6f)
/// a += Vector2f(0.0f, 0.3f)	// (1.2f, 1.9f)
///
/// \endcode
///
///////////////////////////////////////////////////////////