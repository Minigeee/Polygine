#ifndef POLY_QUATERNION_H
#define POLY_QUATERNION_H

#include <poly/Math/Matrix4.h>
#include <poly/Math/Vector3.h>

namespace poly
{

class Quaternion
{
public:
	Quaternion();

	Quaternion(const Vector3f& axis, float rot);

	Quaternion(float x, float y, float z);

	Quaternion(float x, float y, float z, float w);

	Quaternion(const Matrix4f& m);

	Quaternion& operator+=(const Quaternion& x);

	Quaternion operator+(const Quaternion& x) const;

	Quaternion& operator*=(const Quaternion& x);

	Quaternion operator*(const Quaternion& x) const;

	Quaternion operator-() const;

	float x, y, z, w;
};



float magnitude(const Quaternion& q);

Quaternion normalize(const Quaternion& q);

Quaternion conjugate(const Quaternion& q);

Quaternion inverse(const Quaternion& q);


Quaternion slerp(const Quaternion& a, const Quaternion& b, float x);

Matrix4f toMatrix(const Quaternion& q);

}

#endif