#include <poly/Math/Matrix2.h>
#include <poly/Math/Matrix3.h>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Matrix4<T>::Matrix4() :
	x(1, 0, 0, 0),
	y(0, 1, 0, 0),
	z(0, 0, 1, 0),
	w(0, 0, 0, 1)
{ }

template <typename T>
inline Matrix4<T>::Matrix4(T s) :
	x(s, 0, 0, 0),
	y(0, s, 0, 0),
	z(0, 0, s, 0),
	w(0, 0, 0, s)
{ }

template <typename T>
inline Matrix4<T>::Matrix4(
	T xx, T xy, T xz, T xw,
	T yx, T yy, T yz, T yw,
	T zx, T zy, T zz, T zw,
	T wx, T wy, T wz, T ww):
	x(xx, xy, xz, xw),
	y(yx, yy, yz, yw),
	z(zx, zy, zz, zw),
	w(wx, wy, wz, ww)
{ }

template <typename T>
inline Matrix4<T>::Matrix4(const Matrix4<T>& m) :
	x(m.x),
	y(m.y),
	z(m.z),
	w(m.w)
{ }

template <typename T>
inline Matrix4<T>::Matrix4(
	const Vector4<T>& x,
	const Vector4<T>& y, 
	const Vector4<T>& z, 
	const Vector4<T>& w) :
	x(x),
	y(y),
	z(z),
	w(w)
{ }

template <typename T>
template <typename U>
inline Matrix4<T>::Matrix4(U s) :
	x(static_cast<T>(s), 0, 0, 0),
	y(0, static_cast<T>(s), 0, 0),
	z(0, 0, static_cast<T>(s), 0),
	w(0, 0, 0, static_cast<T>(s))
{ }

template <typename T>
template <
	typename XX, typename XY, typename XZ, typename XW,
	typename YX, typename YY, typename YZ, typename YW,
	typename ZX, typename ZY, typename ZZ, typename ZW,
	typename WX, typename WY, typename WZ, typename WW>
inline Matrix4<T>::Matrix4(
		XX xx, XY xy, XZ xz, XW xw,
		YX yx, YY yy, YZ yz, YW yw,
		ZX zx, ZY zy, ZZ zz, ZW zw,
		WX wx, WY wy, WZ wz, WW ww) :
	x(static_cast<T>(xx), static_cast<T>(xy), static_cast<T>(xz), static_cast<T>(xw)),
	y(static_cast<T>(yx), static_cast<T>(yy), static_cast<T>(yz), static_cast<T>(yw)),
	z(static_cast<T>(zx), static_cast<T>(zy), static_cast<T>(zz), static_cast<T>(zw)),
	w(static_cast<T>(wx), static_cast<T>(wy), static_cast<T>(wz), static_cast<T>(ww))
{ }

template <typename T>
template <typename U>
inline Matrix4<T>::Matrix4(const Matrix4<U> & m) :
	x(m.x),
	y(m.y),
	z(m.z),
	w(m.w)
{ }

template <typename T>
template <typename A, typename B, typename C, typename D>
inline Matrix4<T>::Matrix4(
	const Vector4<A> & x,
	const Vector4<B> & y,
	const Vector4<C> & z,
	const Vector4<D> & w) :
	x(x),
	y(y),
	z(z),
	w(w)
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Matrix4<T>::Matrix4(const Matrix2<A>& xy, B z, C w) :
	x(xy.x, 0, 0),
	y(xy.y, 0, 0),
	z(0, 0, z, 0),
	w(0, 0, 0, w)
{ }

template <typename T>
template <typename A, typename B, typename C>
inline Matrix4<T>::Matrix4(A x, B y, const Matrix2<C>& zw) :
	x(x, 0, 0, 0),
	y(0, y, 0, 0),
	z(0, 0, zw.z),
	w(0, 0, zw.w)
{ }

template <typename T>
template <typename A, typename B>
inline Matrix4<T>::Matrix4(const Matrix3<A>& xyz, B w) :
	x(xyz.x, 0),
	y(xyz.y, 0),
	z(xyz.z, 0),
	w(0, 0, 0, w)
{ }

template <typename T>
template <typename A, typename B>
inline Matrix4<T>::Matrix4(A x, const Matrix3<B>& yzw) :
	x(x, 0, 0, 0),
	y(0, yzw.y),
	z(0, yzw.z),
	w(0, yzw.w),
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, U b)
{
	a.x += b;
	a.y += b;
	a.z += b;
	a.w += b;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator+=(Matrix4<T>& a, const Matrix4<U>& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, U b)
{
	a.x -= b;
	a.y -= b;
	a.z -= b;
	a.w -= b;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator-=(Matrix4<T>& a, const Matrix4<U>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, U b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator*=(Matrix4<T>& a, const Matrix4<U>& b)
{
	a = a * b;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, U b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	a.w /= b;
	return a;
}

template <typename T, typename U>
Matrix4<T>& operator/=(Matrix4<T>& a, const Matrix4<U>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix4<T> operator-(const Matrix4<T>& m)
{
	return Matrix4<T>(-m.x, -m.y, -m.z, -m.w);
}

template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, T b)
{
	return Matrix4<T>(a.x + b, a.y + b, a.z + b, a.w + b);
}

template <typename T>
Matrix4<T> operator+(T a, const Matrix4<T>& b)
{
	return Matrix4<T>(a + b.x, a + b.y, a + b.z, a + b.w);
}

template <typename T>
Matrix4<T> operator+(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return Matrix4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, T b)
{
	return Matrix4<T>(a.x - b, a.y - b, a.z - b, a.w - b);
}

template <typename T>
Matrix4<T> operator-(T a, const Matrix4<T>& b)
{
	return Matrix4<T>(a - b.x, a - b.y, a - b.z, a - b.w);
}

template <typename T>
Matrix4<T> operator-(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return Matrix4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, T b)
{
	return Matrix4<T>(a.x * b, a.y * b, a.z * b, a.w * b);
}

template <typename T>
Matrix4<T> operator*(T a, const Matrix4<T>& b)
{
	return Matrix4<T>(a * b.x, a * b.y, a * b.z, a * b.w);
}

template <typename T>
Matrix4<T> operator*(const Matrix4<T>& a, const Matrix4<T>& b)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix4<T>(
		a.x.x * b.x.x + a.y.x * b.x.y + a.z.x * b.x.z + a.w.x * b.x.w,
		a.x.y * b.x.x + a.y.y * b.x.y + a.z.y * b.x.z + a.w.y * b.x.w,
		a.x.z * b.x.x + a.y.z * b.x.y + a.z.z * b.x.z + a.w.z * b.x.w,
		a.x.w * b.x.x + a.y.w * b.x.y + a.z.w * b.x.z + a.w.w * b.x.w,

		a.x.x * b.y.x + a.y.x * b.y.y + a.z.x * b.y.z + a.w.x * b.y.w,
		a.x.y * b.y.x + a.y.y * b.y.y + a.z.y * b.y.z + a.w.y * b.y.w,
		a.x.z * b.y.x + a.y.z * b.y.y + a.z.z * b.y.z + a.w.z * b.y.w,
		a.x.w * b.y.x + a.y.w * b.y.y + a.z.w * b.y.z + a.w.w * b.y.w,
		
		a.x.x * b.z.x + a.y.x * b.z.y + a.z.x * b.z.z + a.w.x * b.z.w,
		a.x.y * b.z.x + a.y.y * b.z.y + a.z.y * b.z.z + a.w.y * b.z.w,
		a.x.z * b.z.x + a.y.z * b.z.y + a.z.z * b.z.z + a.w.z * b.z.w,
		a.x.w * b.z.x + a.y.w * b.z.y + a.z.w * b.z.z + a.w.w * b.z.w,
		
		a.x.x * b.w.x + a.y.x * b.w.y + a.z.x * b.w.z + a.w.x * b.w.w,
		a.x.y * b.w.x + a.y.y * b.w.y + a.z.y * b.w.z + a.w.y * b.w.w,
		a.x.z * b.w.x + a.y.z * b.w.y + a.z.z * b.w.z + a.w.z * b.w.w,
		a.x.w * b.w.x + a.y.w * b.w.y + a.z.w * b.w.z + a.w.w * b.w.w
		);
#else
	return Matrix4<T>(
		a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x + a.x.w * b.w.x,
		a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y + a.x.w * b.w.y,
		a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z + a.x.w * b.w.z,
		a.x.x * b.x.w + a.x.y * b.y.w + a.x.z * b.z.w + a.x.w * b.w.w,

		a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x + a.y.w * b.w.x,
		a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y + a.y.w * b.w.y,
		a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z + a.y.w * b.w.z,
		a.y.x * b.x.w + a.y.y * b.y.w + a.y.z * b.z.w + a.y.w * b.w.w,
		
		a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x + a.z.w * b.w.x,
		a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y + a.z.w * b.w.y,
		a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z + a.z.w * b.w.z,
		a.z.x * b.x.w + a.z.y * b.y.w + a.z.z * b.z.w + a.z.w * b.w.w,
		
		a.w.x * b.x.x + a.w.y * b.y.x + a.w.z * b.z.x + a.w.w * b.w.x,
		a.w.x * b.x.y + a.w.y * b.y.y + a.w.z * b.z.y + a.w.w * b.w.y,
		a.w.x * b.x.z + a.w.y * b.y.z + a.w.z * b.z.z + a.w.w * b.w.z,
		a.w.x * b.x.w + a.w.y * b.y.w + a.w.z * b.z.w + a.w.w * b.w.w
		);
#endif
}

template <typename T>
Vector4<T> operator*(const Matrix4<T>& m, const Vector4<T>& v)
{
#ifdef USE_COLUMN_MAJOR
	return Vector4<T>(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w,
		m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w
		);
#else
	return Vector4<T>(
		m.x.x * v.x + m.x.y * v.y + m.x.z * v.z + m.x.w * v.w,
		m.y.x * v.x + m.y.y * v.y + m.y.z * v.z + m.y.w * v.w,
		m.z.x * v.x + m.z.y * v.y + m.z.z * v.z + m.z.w * v.w,
		m.w.x * v.x + m.w.y * v.y + m.w.z * v.z + m.w.w * v.w
		);
#endif
}

template <typename T>
Vector4<T> operator*(const Vector4<T>& v, const Matrix4<T>& m)
{
#ifdef USE_COLUMN_MAJOR
	return Vector4<T>(
		m.x.x * v.x + m.x.y * v.y + m.x.z * v.z + m.x.w * v.w,
		m.y.x * v.x + m.y.y * v.y + m.y.z * v.z + m.y.w * v.w,
		m.z.x * v.x + m.z.y * v.y + m.z.z * v.z + m.z.w * v.w,
		m.w.x * v.x + m.w.y * v.y + m.w.z * v.z + m.w.w * v.w
		);
#else
	return Vector4<T>(
		m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w,
		m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w,
		m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w,
		m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w
		);
#endif
}

template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, T b)
{
	return Matrix4<T>(a.x / b, a.y / b, a.z / b, a.w / b);
}

template <typename T>
Matrix4<T> operator/(T a, const Matrix4<T>& b)
{
	return Matrix4<T>(a / b.x, a / b.y, a / b.z, a / b.w);
}

template <typename T>
Matrix4<T> operator/(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return Matrix4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T>
bool operator==(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
bool operator!=(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix4<T> elem(const Matrix4<T>& a, const Matrix4<T>& b)
{
	return Matrix4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T>
T determinant(const Matrix4<T>& m)
{
	T xy = m.z.x * m.w.y - m.z.y * m.w.x;
	T xz = m.z.x * m.w.z - m.z.z * m.w.x;
	T xw = m.z.x * m.w.w - m.z.w * m.w.x;
	T yz = m.z.y * m.w.z - m.z.z * m.w.y;
	T yw = m.z.y * m.w.w - m.z.w * m.w.y;
	T zw = m.z.z * m.w.w - m.z.w * m.w.z;

	T dx = m.y.y * zw - m.y.z * yw + m.y.w * yz;
	T dy = m.y.x * zw - m.y.z * xw + m.y.w * xz;
	T dz = m.y.x * yw - m.y.y * xw + m.y.w * xy;
	T dw = m.y.x * yz - m.y.y * xz + m.y.z * xy;

	return m.x.x * dx - m.x.y * dy + m.x.z * dz - m.x.w * dw;
}

template <typename T>
Matrix4<T> inverse(const Matrix4<T>& x)
{
#ifdef USE_COLUMN_MAJOR
	const T* m = &x.x.x;
#else
	Matrix4<T> t = transpose(x);
	const T* m = &t.x.x;
#endif
	T inv[16], det;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

#ifdef USE_COLUMN_MAJOR
	return Matrix4<T>(
		inv[0], inv[1], inv[2], inv[3],
		inv[4], inv[5], inv[6], inv[7],
		inv[8], inv[9], inv[10], inv[11],
		inv[12], inv[13], inv[14], inv[15]
		) / det;
#else
	return Matrix4<T>(
		inv[0], inv[4], inv[8], inv[12],
		inv[1], inv[5], inv[9], inv[13],
		inv[2], inv[6], inv[10], inv[14],
		inv[3], inv[7], inv[11], inv[15]
		) / det;
#endif
}

template <typename T>
Matrix4<T> transpose(const Matrix4<T>& m)
{
	return Matrix4<T>(
		m.x.x, m.y.x, m.z.x, m.w.x,
		m.x.y, m.y.y, m.z.y, m.w.y,
		m.x.z, m.y.z, m.z.z, m.w.z,
		m.x.w, m.y.w, m.z.w, m.w.w
		);
}

///////////////////////////////////////////////////////////

}