#include <poly/Math/Functions.h>
#include <poly/Math/Transform.h>

namespace poly
{


///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Vector3f& r, float k)
{
	Vector3f rot(rad(r.x), rad(r.y), rad(r.z));
	Vector3f c(cos(rot.x), cos(rot.y), cos(rot.z));
	Vector3f s(sin(rot.x), sin(rot.y), sin(rot.z));
	
#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		k * (c.z * c.y),
		k * (s.z * c.y),
		k * (-s.y),
		0.0f,

		k * (-s.z * c.x + c.z * s.y * s.x),
		k * (c.z * c.x + s.z * s.y * s.x),
		k * (c.y * s.x),
		0.0f,

		k * (s.z * s.x + c.z * s.y * c.x),
		k * (-c.z * s.x + s.z * s.y * c.x),
		k * (c.y * c.x),
		0.0f,

		t.x, t.y, t.z, 1.0f
	);
#else
	return Matrix4f(
		k * (c.z * c.y),
		k * (-s.z * c.x + c.z * s.y * s.x),
		k * (s.z * s.x + c.z * s.y * c.x),
		t.x,

		k * (s.z * c.y),
		k * (c.z * c.x + s.z * s.y * s.x),
		k * (-c.z * s.x + s.z * s.y * c.x),
		t.y,

		k * (-s.y),
		k * (c.y * s.x),
		k * (c.y * c.x),
		t.z,

		0.0f, 0.0f, 0.0f, 1.0f
	);
#endif
}


///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Quaternion& q, float k)
{
#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		k * (1.0f - 2.0f * (q.y * q.y + q.z * q.z)),
		k * (2.0f * (q.x * q.y + q.w * q.z)),
		k * (2.0f * (q.x * q.z - q.w * q.y)),
		0.0f,

		k * (2.0f * (q.x * q.y - q.w * q.z)),
		k * (1.0f - 2.0f * (q.x * q.x + q.z * q.z)),
		k * (2.0f * (q.y * q.z + q.w * q.x)),
		0.0f,

		k * (2.0f * (q.x * q.z + q.w * q.y)),
		k * (2.0f * (q.y * q.z - q.w * q.x)),
		k * (1.0f - 2.0f * (q.x * q.x + q.y * q.y)),
		0.0f,

		t.x, t.y, t.z, 1.0f
	);
#else
	return Matrix4f(
		k * (1.0f - 2.0f * (q.y * q.y + q.z * q.z)),
		k * (2.0f * (q.x * q.y - q.w * q.z)),
		k * (2.0f * (q.x * q.z + q.w * q.y)),
		t.x,

		k * (2.0f * (q.x * q.y + q.w * q.z)),
		k* (1.0f - 2.0f * (q.x * q.x + q.z * q.z)),
		k* (2.0f * (q.y * q.z - q.w * q.x)),
		t.y,

		k* (2.0f * (q.x * q.z - q.w * q.y)),
		k* (2.0f * (q.y * q.z + q.w * q.x)),
		k* (1.0f - 2.0f * (q.x * q.x + q.y * q.y)),
		t.z,

		0.0f, 0.0f, 0.0f, 1.0f
	);
#endif
}


///////////////////////////////////////////////////////////
Matrix4f toViewMatrix(const Vector3f& p, const Vector3f& f, const Vector3f& r)
{
	Vector3f u = normalize(cross(r, f));

#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		r.x, u.x, -f.x, 0.0f,
		r.y, u.y, -f.y, 0.0f,
		r.z, u.z, -f.z, 0.0f,
		-dot(r, p), -dot(u, p), dot(f, p), 1.0f
	);
#else
	return Matrix4f(
		r.x, r.y, r.z, -dot(r, p),
		u.x, u.y, u.z, -dot(u, p),
		-f.x, -f.y, -f.z, dot(f, p),
		0.0f, 0.0f, 0.0f, 1.0f
	);
#endif
}

Matrix4f toPerspectiveMatrix(float fov, float ar, float near, float far)
{
	float fovy = fov / ar;
	fovy = tan(rad(fovy * 0.5f));

#ifdef USE_COLUMN_MAJOR
	return Matrix4f(
		1.0f / (ar * fovy), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / fovy, 0.0f, 0.0f,
		0.0f, 0.0f, -(far + near) / (far - near), -1.0f,
		0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f
	);
#else
	return Matrix4f(
		1.0f / (ar * fovy), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / fovy, 0.0f, 0.0f,
		0.0f, 0.0f, -(far + near) / (far - near), -2.0f * far * near / (far - near),
		0.0f, 0.0f, -1.0f, 0.0f
	);
#endif
}


}