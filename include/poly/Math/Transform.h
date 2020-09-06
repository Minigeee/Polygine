#ifndef  POLY_TRANSFORM_FUNCS_H
#define POLY_TRANSFORM_FUNCS_H

#include <poly/Math/Matrix4.h>
#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{

Matrix4f toTransformMatrix(const Vector3f& t, const Vector3f& r, float s);

Matrix4f toTransformMatrix(const Vector3f& t, const Quaternion& r, float s);

Matrix4f toViewMatrix(const Vector3f& p, const Vector3f& f, const Vector3f& r);

Matrix4f toPerspectiveMatrix(float fov, float ar, float near, float far);

}

#endif