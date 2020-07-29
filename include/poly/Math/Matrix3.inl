// #include <poly/Math/Matrix2.h>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Matrix3<T>::Matrix3() :
	x(1, 0, 0),
	y(0, 1, 0),
	z(0, 0, 1)
{ }

template <typename T>
inline Matrix3<T>::Matrix3(T s) :
	x(s, 0, 0),
	y(0, s, 0),
	z(0, 0, s)
{ }

template <typename T>
inline Matrix3<T>::Matrix3(
	T xx, T xy, T xz,
	T yx, T yy, T yz,
	T zx, T zy, T zz) :
	x(xx, xy, xz),
	y(yx, yy, yz),
	z(zx, zy, zz)
{ }

template <typename T>
inline Matrix3<T>::Matrix3(const Matrix3<T>& m) :
	x(m.x), y(m.y), z(m.z)
{ }

template <typename T>
inline Matrix3<T>::Matrix3(
	const Vector3<T>& x,
	const Vector3<T>& y,
	const Vector3<T>& z) :
	x(x), y(y), z(z)
{ }

template <typename T>
template <typename U>
inline Matrix3<T>::Matrix3(U s) :
	x(static_cast<T>(s), 0, 0),
	y(0, static_cast<T>(s), 0),
	z(0, 0, static_cast<T>(s))
{ }

template <typename T>
template <
	typename XX, typename XY, typename XZ,
	typename YX, typename YY, typename YZ,
	typename ZX, typename ZY, typename ZZ>
inline Matrix3<T>::Matrix3(
	XX xx, XY xy, XZ xz,
	YX yx, YY yy, YZ yz,
	ZX zx, ZY zy, ZZ zz) :
	x(static_cast<T>(xx), static_cast<T>(xy), static_cast<T>(xz)),
	y(static_cast<T>(yx), static_cast<T>(yy), static_cast<T>(yz)),
	z(static_cast<T>(zx), static_cast<T>(zy), static_cast<T>(zz))
{ }

template <typename T>
template <typename U>
inline Matrix3<T>::Matrix3(const Matrix3<U>& m) :
	x(m.x), y(m.y), z(m.z)
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Matrix3<T>::Matrix3(
	const Vector3<A>& x,
	const Vector3<B>& y,
	const Vector3<C>& z) :
	x(x), y(y), z(z)
{ }

template <typename T>
template <typename A, typename B>
inline Matrix3<T>::Matrix3(const Matrix2<A>& xy, B z) :
	x(xy.x, 0),
	y(xy.y, 0),
	z(0, 0, z)
{ }

template <typename T>
template <typename A, typename B>
inline Matrix3<T>::Matrix3(A x, const Matrix2<B>& yz) :
	x(x, 0, 0),
	y(0, yz.x),
	z(0, yz.y)
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix3<T>& operator+=(Matrix3<T>& a, U b)
{
	a.x += b;
	a.y += b;
	a.z += b;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator+=(Matrix3<T>& a, const Matrix3<U>& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator-=(Matrix3<T>& a, U b)
{
	a.x -= b;
	a.y -= b;
	a.z -= b;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator-=(Matrix3<T>& a, const Matrix3<U>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator*=(Matrix3<T>& a, U b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator*=(Matrix3<T>& a, const Matrix3<U>& b)
{
	a = a * b;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator/=(Matrix3<T>& a, U b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return a;
}

template <typename T, typename U>
Matrix3<T>& operator/=(Matrix3<T>& a, const Matrix3<U>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix3<T> operator-(const Matrix3<T>& m)
{
	return Matrix3<T>(-m.x, -m.y, -m.z);
}

template <typename T>
Matrix3<T> operator+(const Matrix3<T>& a, T b)
{
	return Matrix3<T>(a.x + b, a.y + b, a.z + b);
}

template <typename T>
Matrix3<T> operator+(T a, const Matrix3<T>& b)
{
	return Matrix3<T>(a + b.x, a + b.y, a + b.z);
}

template <typename T>
Matrix3<T> operator+(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return Matrix3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
Matrix3<T> operator-(const Matrix3<T>& a, T b)
{
	return Matrix3<T>(a.x - b, a.y - b, a.z - b);
}

template <typename T>
Matrix3<T> operator-(T a, const Matrix3<T>& b)
{
	return Matrix3<T>(a - b.x, a - b.y, a - b.z);
}

template <typename T>
Matrix3<T> operator-(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return Matrix3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T>
Matrix3<T> operator*(const Matrix3<T>& a, T b)
{
	return Matrix3<T>(a.x * b, a.y * b, a.z * b);
}

template <typename T>
Matrix3<T> operator*(T a, const Matrix3<T>& b)
{
	return Matrix3<T>(a * b.x, a * b.y, a * b.z);
}

template <typename T>
Matrix3<T> operator*(const Matrix3<T>& a, const Matrix3<T>& b)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix3<T>(
		a.x.x * b.x.x + a.y.x * b.x.y + a.z.x * b.x.z,
		a.x.y * b.x.x + a.y.y * b.x.y + a.z.y * b.x.z,
		a.x.z * b.x.x + a.y.z * b.x.y + a.z.z * b.x.z,

		a.x.x * b.y.x + a.y.x * b.y.y + a.z.x * b.y.z,
		a.x.y * b.y.x + a.y.y * b.y.y + a.z.y * b.y.z,
		a.x.z * b.y.x + a.y.z * b.y.y + a.z.z * b.y.z,

		a.x.x * b.z.x + a.y.x * b.z.y + a.z.x * b.z.z,
		a.x.y * b.z.x + a.y.y * b.z.y + a.z.y * b.z.z,
		a.x.z * b.z.x + a.y.z * b.z.y + a.z.z * b.z.z
		);
#else
	return Matrix3<T>(
		a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x,
		a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y,
		a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z,

		a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x,
		a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y,
		a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z,

		a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x,
		a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y,
		a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z
		);
#endif
}

template <typename T>
Vector3<T> operator*(const Matrix3<T>& m, const Vector3<T>& v)
{
#ifdef USE_COLUMN_MAJOR
	return Vector3<T>(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z
		);
#else
	return Vector3<T>(
		m.x.x * v.x + m.x.y * v.y + m.x.z * v.z,
		m.y.x * v.x + m.y.y * v.y + m.y.z * v.z,
		m.z.x * v.x + m.z.y * v.y + m.z.z * v.z
		);
#endif
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& v, const Matrix3<T>& m)
{
#ifdef USE_COLUMN_MAJOR
	return Vector3<T>(
		m.x.x * v.x + m.x.y * v.y + m.x.z * v.z,
		m.y.x * v.x + m.y.y * v.y + m.y.z * v.z,
		m.z.x * v.x + m.z.y * v.y + m.z.z * v.z
		);
#else
	return Vector3<T>(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z
		);
#endif
}

template <typename T>
Matrix3<T> operator/(const Matrix3<T>& a, T b)
{
	return Matrix3<T>(a.x / b, a.y / b, a.z / b);
}

template <typename T>
Matrix3<T> operator/(T a, const Matrix3<T>& b)
{
	return Matrix3<T>(a / b.x, a / b.y, a / b.z);
}

template <typename T>
Matrix3<T> operator/(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return Matrix3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template <typename T>
bool operator==(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
bool operator!=(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix3<T> elem(const Matrix3<T>& a, const Matrix3<T>& b)
{
	return Matrix3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
T determinant(const Matrix3<T>& m)
{
	return
		m.x.x * (m.y.y * m.z.z - m.z.y * m.y.z) -
		m.y.x * (m.x.y * m.z.z - m.z.y * m.x.z) +
		m.z.x * (m.x.y * m.y.z - m.y.y * m.x.z);
}

template <typename T>
Matrix3<T> inverse(const Matrix3<T>& m)
{
	T d = static_cast<T>(1) / determinant(m);

#ifdef USE_COLUMN_MAJOR
	return Matrix3<T>(
		+(m.y.y * m.z.z - m.z.y * m.y.z),
		-(m.x.y * m.z.z - m.z.y * m.x.z),
		+(m.x.y * m.y.z - m.y.y * m.x.z),

		-(m.y.x * m.z.z - m.z.x * m.y.z),
		+(m.x.x * m.z.z - m.z.x * m.x.z),
		-(m.x.x * m.y.z - m.y.x * m.x.z),

		+(m.y.x * m.z.y - m.z.x * m.y.y),
		-(m.x.x * m.z.y - m.z.x * m.x.y),
		+(m.x.x * m.y.y - m.y.x * m.x.y)
		) * d;
#else
	return Matrix3<T>(
		+(m.y.y * m.z.z - m.y.z * m.z.y),
		-(m.x.y * m.z.z - m.x.z * m.z.y),
		+(m.x.y * m.y.z - m.x.z * m.y.y),

		-(m.y.x * m.z.z - m.y.z * m.z.x),
		+(m.x.x * m.z.z - m.x.z * m.z.x),
		-(m.x.x * m.y.z - m.x.z * m.y.x),

		+(m.y.x * m.z.y - m.y.y * m.z.x),
		-(m.x.x * m.z.y - m.x.y * m.z.x),
		+(m.x.x * m.y.y - m.x.y * m.y.x)
		) * d;
#endif
}

template <typename T>
Matrix3<T> transpose(const Matrix3<T>& m)
{
	return Matrix3<T>(
		m.x.x, m.y.x, m.z.x,
		m.x.y, m.y.y, m.z.y,
		m.x.z, m.y.z, m.z.z
		);
}

///////////////////////////////////////////////////////////

}