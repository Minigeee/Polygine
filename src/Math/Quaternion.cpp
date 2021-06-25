#include <poly/Math/Functions.h>
#include <poly/Math/Quaternion.h>

#include <math.h>

namespace poly
{


///////////////////////////////////////////////////////////
Quaternion::Quaternion() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{

}


///////////////////////////////////////////////////////////
Quaternion::Quaternion(const Vector3f& a, float r) :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
	float angle = rad(r);
	float s = sin(angle * 0.5f);

	x = a.x * s;
	y = a.y * s;
	z = a.z * s;
	w = cos(angle * 0.5f);
}


///////////////////////////////////////////////////////////
Quaternion::Quaternion(const Vector3f& rot) :
	Quaternion(rot.x, rot.y, rot.z)
{

}


///////////////////////////////////////////////////////////
Quaternion::Quaternion(float _x, float _y, float _z) :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
	Vector3f rot(rad(_x) * 0.5f, rad(_y) * 0.5f, rad(_z) * 0.5f);
	Vector3f c(cos(rot.x), cos(rot.y), cos(rot.z));
	Vector3f s(sin(rot.x), sin(rot.y), sin(rot.z));

	x = s.x * c.y * c.z - c.x * s.y * s.z;
	y = c.x * s.y * c.z + s.x * c.y * s.z;
	z = c.x * c.y * s.z - s.x * s.y * c.z;
	w = c.x * c.y * c.z + s.x * s.y * s.z;
}


///////////////////////////////////////////////////////////
Quaternion::Quaternion(float x, float y, float z, float w) :
	x(x),
	y(y),
	z(z),
	w(w)
{

}


///////////////////////////////////////////////////////////
Quaternion::Quaternion(const Matrix4f& matrix) :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{
#ifdef USE_COLUMN_MAJOR
	Matrix4f m = transpose(matrix);
#else
	Matrix4f m = matrix;
#endif

	float tr, u;
	tr = m.x.x + m.y.y + m.z.z;

	if (tr >= 0)
	{
		u = (float)sqrtf(tr + 1);
		w = (float)0.5 * u;
		u = (float)0.5 / u;
		x = (m.z.y - m.y.z) * u;
		y = (m.x.z - m.z.x) * u;
		z = (m.y.x - m.x.y) * u;
	}
	else
	{
		int i = 0;
		if (m.y.y > m.x.x)
		{
			i = 1;

			if (m.z.z > m.y.y)
				i = 2;
		}

		if (m.z.z > m.x.x)
			i = 2;

		switch (i)
		{
		case 0:
			u = (float)sqrtf((m.x.x - (m.y.y + m.z.z)) + 1);
			x = 0.5f * u;
			u = 0.5f / u;
			y = (m.y.x + m.x.y) * u;
			z = (m.x.z + m.z.x) * u;
			w = (m.z.y - m.y.z) * u;
			break;

		case 1:
			u = (float)sqrtf((m.y.y - (m.z.z + m.x.x)) + 1);
			y = 0.5f * u;
			u = 0.5f / u;
			z = (m.z.y + m.y.z) * u;
			x = (m.y.x + m.x.y) * u;
			w = (m.x.z - m.z.x) * u;
			break;

		case 2:
			u = (float)sqrtf((m.z.z - (m.x.x + m.y.y)) + 1);
			z = 0.5f * u;

			u = 0.5f / u;
			x = (m.x.z + m.z.x) * u;
			y = (m.z.y + m.y.z) * u;
			w = (m.y.x - m.x.y) * u;
			break;
		}
	}
}


///////////////////////////////////////////////////////////
Quaternion::operator Vector3f() const
{
	Vector3f v;

	// Pitch
	float _y = 2.0f * (y * z + w * x);
	float _x = w * w - x * x - y * y + z * z;

	if (fabsf(_x) <= FLT_EPSILON && fabsf(_y) <= FLT_EPSILON)
		v.x = 2.0f * atan2(x, w);
	else
		v.x = atan2(_y, _x);

	// Roll
	_y = 2.0f * (x * y + w * z);
	_x = w * w + x * x - y * y - z * z;

	if (fabsf(_x) <= FLT_EPSILON && fabsf(_y) <= FLT_EPSILON)
		v.z = 0.0f;
	else
		v.z = atan2(_y, _x);

	// Yaw
	v.y = asin(-2.0f * (x * z - w * y));

	v.x = deg(v.x);
	v.y = deg(v.y);
	v.z = deg(v.z);

	return v;
}


///////////////////////////////////////////////////////////
Quaternion& Quaternion::operator+=(const Quaternion& x)
{
	return (*this = *this + x);
}


///////////////////////////////////////////////////////////
Quaternion Quaternion::operator+(const Quaternion& v) const
{
	return Quaternion(x + v.x, y + v.y, z + v.z, w + v.w);
}


///////////////////////////////////////////////////////////
Quaternion& Quaternion::operator*=(const Quaternion& x)
{
	return (*this = *this * x);
}


///////////////////////////////////////////////////////////
Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion r;

	r.w = w * q.w - x * q.x - y * q.y - z * q.z;
	r.x = w * q.x + x * q.w + y * q.z - z * q.y;
	r.y = w * q.y + y * q.w + z * q.x - x * q.z;
	r.z = w * q.z + z * q.w + x * q.y - y * q.x;

	return r;
}


///////////////////////////////////////////////////////////
bool Quaternion::operator==(const Quaternion& q) const
{
	return x == q.x && y == q.y && z == q.z && w == q.w;
}


///////////////////////////////////////////////////////////
bool Quaternion::operator!=(const Quaternion& q) const
{
	return x != q.x || y != q.y || z != q.z || w != q.w;
}


///////////////////////////////////////////////////////////
Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}


///////////////////////////////////////////////////////////
void toAxisAngle(const Quaternion& q, Vector3f& axis, float& rotation)
{
	float angle = 2.0f * acos(q.w);
	float denom = sin(angle * 0.5f);
	axis = (fabsf(denom) <= FLT_EPSILON ? Vector3f(0.0f, 1.0f, 0.0f) : Vector3f(q.x, q.y, q.z) / denom);
	rotation = deg(angle);
}


///////////////////////////////////////////////////////////
float magnitude(const Quaternion& q)
{
	return ::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}


///////////////////////////////////////////////////////////
Quaternion normalize(const Quaternion& q)
{
	float n = 1.0f / magnitude(q);
	return Quaternion(q.x * n, q.y * n, q.z * n, q.w);
}


///////////////////////////////////////////////////////////
Quaternion conjugate(const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}


///////////////////////////////////////////////////////////
Quaternion inverse(const Quaternion& q)
{
	float n = 1.0f / magnitude(q);
	return Quaternion(-q.x * n, -q.y * n, -q.z * n, q.w * n);
}


///////////////////////////////////////////////////////////
Quaternion slerp(const Quaternion& a, const Quaternion& b, float x)
{
	float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	if (dot < -1.0f) dot = -1.0f;
	if (dot > 1.0f) dot = 1.0f;

	// Calc theta
	float theta = acos(dot);
	if (theta < 0.0f) theta = -theta;
	if (theta == 0.0f) theta = FLT_EPSILON;

	float st = sin(theta);
	float sut = sin(x * theta);
	float sout = sin((1.0f - x) * theta);
	float coeff1 = sout / st;
	float coeff2 = sut / st;

	Quaternion result;
	result.x = coeff1 * a.x + coeff2 * b.x;
	result.y = coeff1 * a.y + coeff2 * b.y;
	result.z = coeff1 * a.z + coeff2 * b.z;
	result.w = coeff1 * a.w + coeff2 * b.w;

	result = normalize(result);
	return result;
}


///////////////////////////////////////////////////////////
Matrix4f toMatrix(const Quaternion& q)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		1.0f - 2.0f * (q.y * q.y + q.z * q.z),
		2.0f * (q.x * q.y + q.w * q.z),
		2.0f * (q.x * q.z - q.w * q.y),
		0.0f,

		2.0f * (q.x * q.y - q.w * q.z),
		1.0f - 2.0f * (q.x * q.x + q.z * q.z),
		2.0f * (q.y * q.z + q.w * q.x),
		0.0f,

		2.0f * (q.x * q.z + q.w * q.y),
		2.0f * (q.y * q.z - q.w * q.x),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	);
#else
	return Matrix4f(
		1.0f - 2.0f * (q.y * q.y + q.z * q.z),
		2.0f * (q.x * q.y - q.w * q.z),
		2.0f * (q.x * q.z + q.w * q.y),
		0.0f,

		2.0f * (q.x * q.y + q.w * q.z),
		1.0f - 2.0f * (q.x * q.x + q.z * q.z),
		2.0f * (q.y * q.z - q.w * q.x),
		0.0f,

		2.0f * (q.x * q.z - q.w * q.y),
		2.0f * (q.y * q.z + q.w * q.x),
		1.0f - 2.0f * (q.x * q.x + q.y * q.y),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	);
#endif
}


}