#ifndef POLY_VECTOR4_H
#define POLY_VECTOR4_H

#include <poly/Core/DataTypes.h>

namespace poly
{

template <typename T> struct Vector2;
template <typename T> struct Vector3;

///////////////////////////////////////////////////////////
/// \brief A templated 4D vector struct
///
///////////////////////////////////////////////////////////
template <typename T>
struct Vector4
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes all components with 0.
	///
	///////////////////////////////////////////////////////////
	Vector4();

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with a scalar
	///
	/// Initializes all components with the scalar
	///
	/// \param s A scalar
	///
	///////////////////////////////////////////////////////////
	explicit Vector4(T s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component
	/// \param y The y component
	/// \param z The z component
	/// \param w The w component
	///
	///////////////////////////////////////////////////////////
	Vector4(T x, T y, T z, T w);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of the same type
	///
	/// \param v A vector of the same type
	///
	///////////////////////////////////////////////////////////
	Vector4(const Vector4<T>& v);

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
	explicit Vector4(U s);

	///////////////////////////////////////////////////////////
	/// \brief Construct a vector with its components
	///
	/// \param x The x component of a different type
	/// \param y The y component of a different type
	/// \param z The z component of a different type
	/// \param w The w component of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename X, typename Y, typename Z, typename W>
	Vector4(X x, Y y, Z z, W w);

	///////////////////////////////////////////////////////////
	/// \brief Construct using another vector of a different type
	///
	/// \param v A vector of a different type
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	Vector4(const Vector4<U>& v);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector2 and two scalars
	///
	/// \param xy A Vector2 containing the x and y components
	/// \param z The z component
	/// \param w The w component
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Vector4(const Vector2<A>& xy, B z, C w);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector2 and two scalars
	///
	/// \param x The x component
	/// \param yz A Vector2 containing the y and z components
	/// \param w The w component
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Vector4(A x, const Vector2<B>& yz, C w);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector2 and two scalars
	///
	/// \param x The x component
	/// \param y The y component
	/// \param zw A Vector2 containing the z and w components
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B, typename C>
	explicit Vector4(A x, B y, const Vector2<C>& zw);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a two Vector2's
	///
	/// \param xy A Vector2 containing the x and y components
	/// \param zw A Vector2 containing the z and w components
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Vector4(const Vector2<A>& xy, const Vector2<B>& zw);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector3 and a scalar
	///
	/// \param xyz A Vector3 containing the x, y, z and components
	/// \param w The w component
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Vector4(const Vector3<A>& xyz, B w);

	///////////////////////////////////////////////////////////
	/// \brief Construct using a Vector3 and a scalar
	///
	/// \param x The x component
	/// \param yzw A Vector3 containing the y, z, and w components
	///
	///////////////////////////////////////////////////////////
	template <typename A, typename B>
	explicit Vector4(A x, const Vector3<B>& yzw);

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

	///////////////////////////////////////////////////////////
	/// \brief The W and A component that share the same space
	///
	///////////////////////////////////////////////////////////
	union
	{
		T w; //!< The W component
		T a; //!< The A component
	};
};

///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator+=(Vector4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator+=(Vector4<T>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator-=(Vector4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator-=(Vector4<T>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator*=(Vector4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator*=(Vector4<T>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator/=(Vector4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator/=(Vector4<T>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator%=(Vector4<T>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<float>& operator%=(Vector4<float>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<double>& operator%=(Vector4<double>& a, U b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T>& operator%=(Vector4<T>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<float>& operator%=(Vector4<float>& a, const Vector4<U>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<double>& operator%=(Vector4<double>& a, const Vector4<U>& b);


///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator-(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator+(const Vector4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator+(T a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator-(const Vector4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator-(T a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator*(const Vector4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator*(T a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator*(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator/(const Vector4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator/(T a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator/(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator%(const Vector4<T>& a, T b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator%(T a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> operator%(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<float> operator%(const Vector4<float>& a, float b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<float> operator%(float a, const Vector4<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<float> operator%(const Vector4<float>& a, const Vector4<float>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<double> operator%(const Vector4<double>& a, double b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<double> operator%(double a, const Vector4<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<double> operator%(const Vector4<double>& a, const Vector4<double>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
bool operator==(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
bool operator!=(const Vector4<T>& a, const Vector4<T>& b);


///////////////////////////////////////////////////////////
// Math
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> floor(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> ceil(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> round(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<int> lround(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> abs(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
/// \brief Get the result of a power operation on a vector
///
/// Applys pow(b, p) to each of the vector's component
///
/// \param b The base vector
/// \param b The power scalar
///
/// \return The resulting vector
///
///////////////////////////////////////////////////////////
template <typename T>
Vector4<T> pow(const Vector4<T>& b, T p);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> pow(const Vector4<T>& b, const Vector4<T>& p);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> sqrt(const Vector4<T>& v);

///////////////////////////////////////////////////////////
// Utility
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \relates Vector4
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
T length(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
/// \brief Get the distance between two point vectors
///
/// \param a The first vector, representing point 1
/// \param a The second vector, representing point 2
///
/// \return The distance between the two vectors
///
///////////////////////////////////////////////////////////
template <typename T>
T dist(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
/// \brief Get the squared distance between two point vectors
///
/// This operation is slightly faster than dist() because
/// it does not involve a sqrt() operation. This should only be
/// used when finding the actual distance does not matter,
/// and distance is only needed for comparison.
///
/// \param a The first vector, representing point 1
/// \param a The second vector, representing point 2
///
/// \return The distance between the two vectors
///
///////////////////////////////////////////////////////////
template <typename T>
T distSquared(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
Vector4<T> normalize(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
/// \brief Get the sum of all components in a vector
///
/// \param v The vector
///
/// \return The sum of the vector
///
///////////////////////////////////////////////////////////
template <typename T>
T sum(const Vector4<T>& v);

///////////////////////////////////////////////////////////
/// \relates Vector4
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
T dot(const Vector4<T>& a, const Vector4<T>& b);

///////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////

typedef Vector4<float>		Vector4f;
typedef Vector4<double>		Vector4d;
typedef Vector4<Int32>		Vector4i;
typedef Vector4<Uint32>		Vector4u;
typedef Vector4<bool>		Vector4b;

template <typename T>
using Color = Vector4<T>;
typedef Color<float>		Colorf;
typedef Color<double>		Colord;
typedef Color<Uint8>		Color8;
typedef Color<Uint16>		Color16;
typedef Color<Uint32>		Color32;

}

#include <poly/Math/Vector4.inl>

#endif

///////////////////////////////////////////////////////////
/// \struct poly::Vector4
///
/// Vector2 is a templated struct that defines a 4D vector. It can be used
/// to represent anything in 4 dimensions, such as points, directions,
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
/// \li ::Vector4i
/// \li ::Vector4u
/// \li ::Vector4f
/// \li ::Vector4d
/// \li ::Vector4b
///
/// A ::Color is also defined as a Vector4:
/// \code
/// template <typename T> using Color = Vector4<T>;
/// \endcode
///
/// Here are the color typedefs:
/// \li ::Colorf
/// \li ::Colord
/// \li ::Color8
/// \li ::Color16
/// \li ::Color32
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // It is possible to construct vectors using different types
/// Vector4f a(3.0f, 4, 5.0, 1.0f);
/// a.x = 0.0f;
///
/// // Operator % uses fmod() for float
/// Vector3f b = a % 2.5f;		// (0.0f, 1.5f, 2.5f, 1.0f)
///
/// a *= 2.0f;					// (0.0f, 8.0f, 10.0f, 2.0f)
/// a += Vector3f(0.3f)			// (0.3f, 8.3f, 10.3f, 2.3f)
///
/// \endcode
///
///////////////////////////////////////////////////////////