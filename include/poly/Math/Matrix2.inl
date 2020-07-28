

namespace poly
{

///////////////////////////////////////////////////////////

template <typename T>
inline Matrix2<T>::Matrix2() :
	x(1, 0),
	y(0, 1)
{ }

template <typename T>
inline Matrix2<T>::Matrix2(T s) :
	x(s, 0),
	y(0, s)
{ }

template <typename T>
inline Matrix2<T>::Matrix2(
	T xx, T xy,
	T yx, T yy) :
	x(xx, xy),
	y(yx, yy)
{ }

template <typename T>
inline Matrix2<T>::Matrix2(const Matrix2<T>& m) :
	x(m.x),
	y(m.y)
{ }

template <typename T>
inline Matrix2<T>::Matrix2(const Vector2<T> & x, const Vector2<T>& y) :
	x(x),
	y(y)
{ }

template <typename T>
template <typename U>
inline Matrix2<T>::Matrix2(U s) :
	x(static_cast<T>(s), 0),
	y(0, static_cast<T>(s))
{ }

template <typename T>
template <
	typename XX, typename XY,
	typename YX, typename YY>
inline Matrix2<T>::Matrix2(
	XX xx, XY xy,
	YX yx, YY yy) :
	x(static_cast<T>(xx), static_cast<T>(xy)),
	y(static_cast<T>(yx), static_cast<T>(yy))
{ }

template <typename T>
template <typename U>
inline Matrix2<T>::Matrix2(const Matrix2<U>& m) :
	x(m.x),
	y(m.y)
{ }

template <typename T>
template <typename A, typename B>
inline Matrix2<T>::Matrix2(const Vector2<A> & x, const Vector2<B> & y) :
	x(x),
	y(y)
{ }

///////////////////////////////////////////////////////////

template <typename T, typename U>
Matrix2<T>& operator+=(Matrix2<T>& a, U b)
{
	a.x += b;
	a.y += b;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator+=(Matrix2<T>& a, const Matrix2<U>& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator-=(Matrix2<T>& a, U b)
{
	a.x -= b;
	a.y -= b;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator-=(Matrix2<T>& a, const Matrix2<U>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator*=(Matrix2<T>& a, U b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator*=(Matrix2<T>& a, const Matrix2<U>& b)
{
	a = a * b;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator/=(Matrix2<T>& a, U b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

template <typename T, typename U>
Matrix2<T>& operator/=(Matrix2<T>& a, const Matrix2<U>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix2<T> operator-(const Matrix2<T>& m)
{
	return Matrix2<T>(-m.x, -m.y);
}

template <typename T>
Matrix2<T> operator+(const Matrix2<T>& a, T b)
{
	return Matrix2<T>(a.x + b, a.y + b);
}

template <typename T>
Matrix2<T> operator+(T a, const Matrix2<T>& b)
{
	return Matrix2<T>(a + b.x, a + b.y);
}

template <typename T>
Matrix2<T> operator+(const Matrix2<T>& a, const Matrix2<T>& b)
{
	return Matrix2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
Matrix2<T> operator-(const Matrix2<T>& a, T b)
{
	return Matrix2<T>(a.x - b, a.y - b);
}

template <typename T>
Matrix2<T> operator-(T a, const Matrix2<T>& b)
{
	return Matrix2<T>(a - b.x, a - b.y);
}

template <typename T>
Matrix2<T> operator-(const Matrix2<T>& a, const Matrix2<T>& b)
{
	return Matrix2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
Matrix2<T> operator*(const Matrix2<T>& a, T b)
{
	return Matrix2<T>(a.x * b, a.y * b);
}

template <typename T>
Matrix2<T> operator*(T a, const Matrix2<T>& b)
{
	return Matrix2<T>(a * b.x, a * b.y);
}

template <typename T>
Matrix2<T> operator*(const Matrix2<T>& a, const Matrix2<T>& b)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix2<T>(
		a.x.x * b.x.x + a.y.x * b.x.y,
		a.x.y * b.x.x + a.y.y * b.x.y,
		a.x.x * b.y.x + a.y.x * b.y.y,
		a.x.y * b.y.x + a.y.y * b.y.y
		);
#else
	return Matrix2<T>(
		a.x.x * b.x.x + a.x.y * b.y.x,
		a.x.x * b.x.y + a.x.y * b.x.y,
		a.y.x * b.x.x + a.y.y * b.y.x,
		a.y.x * b.y.y + a.y.y * b.y.y
		);
#endif
}

template <typename T>
Vector2<T> operator*(const Matrix2<T>& m, const Vector2<T>& v)
{
#ifdef USE_COLUMN_MAJOR
	return Vector2<T>(
		m.x.x * v.x + m.y.x * v.y,
		m.x.y * v.x + m.y.y * v.y
		);
#else
	return Vector2<T>(
		m.x.x * v.x + m.x.y * v.y,
		m.y.x * v.x + m.y.y * v.y
		);
#endif
}

template <typename T>
Vector2<T> operator*(const Vector2<T>& v, const Matrix2<T>& m)
{
#ifdef USE_COLUMN_MAJOR
	return Vector2<T>(
		m.x.x * v.x + m.x.y * v.y,
		m.y.x * v.x + m.y.y * v.y
		);
#else
	return Vector2<T>(
		m.x.x * v.x + m.y.x * v.y,
		m.x.y * v.x + m.y.y * v.y
		);
#endif
}

template <typename T>
Matrix2<T> operator/(const Matrix2<T>& a, T b)
{
	return Matrix2<T>(a.x / b, a.y / b);
}

template <typename T>
Matrix2<T> operator/(T a, const Matrix2<T>& b)
{
	return Matrix2<T>(a / b.x, a / b.y);
}

template <typename T>
Matrix2<T> operator/(const Matrix2<T>& a, const Matrix2<T>& b)
{
	return Matrix2<T>(a.x / b.x, a.y / b.y);
}

///////////////////////////////////////////////////////////

template <typename T>
Matrix2<T> elem(const Matrix2<T>& a, const Matrix2<T>& b)
{
	return Matrix2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
T determinant(const Matrix2<T>& m)
{
	return m.x.x * m.y.y - m.x.y * m.y.x;
}

template <typename T>
Matrix2<T> inverse(const Matrix2<T>& m)
{
	T d = static_cast<T>(1) / determinant(m);
	return Matrix2<T>(
		m.y.y, -m.y.x,
		-m.x.y, m.x.x
		) * d;
}

template <typename T>
Matrix2<T> transpose(const Matrix2<T>& m)
{
	return Matrix2<T>(
		m.x.x, m.y.x,
		m.x.y, m.y.y
		)
}

///////////////////////////////////////////////////////////

}