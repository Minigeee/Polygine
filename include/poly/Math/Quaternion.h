#ifndef POLY_QUATERNION_H
#define POLY_QUATERNION_H

#include <poly/Math/Matrix4.h>
#include <poly/Math/Vector3.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A quartenion is a way to represent rotations
///
///////////////////////////////////////////////////////////
class Quaternion
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Quaternion();

	///////////////////////////////////////////////////////////
	/// \brief Create a quartenion from an axis of rotation and the magnitude of rotation in degrees
	///
	/// \param axis The axis of rotation (should be normalized)
	/// \param rot The magnitude of rotation (in degrees)
	///
	///////////////////////////////////////////////////////////
	Quaternion(const Vector3f& axis, float rot);

	///////////////////////////////////////////////////////////
	/// \brief Create a quartnion from euler angles
	///
	/// \param x The euler angle rotation
	///
	///////////////////////////////////////////////////////////
	Quaternion(const Vector3f& rot);

	///////////////////////////////////////////////////////////
	/// \brief Create a quartnion from euler angles
	///
	/// \param x The rotation around the x-axis in degrees
	/// \param y The rotation around the y-axis in degrees
	/// \param z The rotation around the z-axis in degrees
	///
	///////////////////////////////////////////////////////////
	Quaternion(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Create a quaternion from its 4 components
	///
	/// \param x The x-component of the quaternion
	/// \param y The y-component of the quaternion
	/// \param z The z-component of the quaternion
	/// \param w The w-component of the quaternion
	///
	///////////////////////////////////////////////////////////
	Quaternion(float x, float y, float z, float w);

	///////////////////////////////////////////////////////////
	/// \brief Create a quaternion from a 4x4 rotation matrix
	///
	/// \param m The rotation matrix
	///
	///////////////////////////////////////////////////////////
	Quaternion(const Matrix4f& m);

	///////////////////////////////////////////////////////////
	/// \brief Convert quaternion into Euler angles
	///
	///////////////////////////////////////////////////////////
	operator Vector3f() const;

	Quaternion& operator+=(const Quaternion& x);

	Quaternion operator+(const Quaternion& x) const;

	Quaternion& operator*=(const Quaternion& x);

	Quaternion operator*(const Quaternion& x) const;

	Quaternion operator-() const;

	bool operator==(const Quaternion& q) const;

	bool operator!=(const Quaternion& q) const;

	float x, y, z, w;
};


///////////////////////////////////////////////////////////
/// \brief Convert a normalized quaternion into an axis of rotation and rotation
///
/// \param q The normalized quaternion to convert
/// \param axis The output variable where the normalized axis will be written to
/// \param rotation The output variable where the rotation will be written to (degrees)
///
///////////////////////////////////////////////////////////
void toAxisAngle(const Quaternion& q, Vector3f& axis, float& rotation);


///////////////////////////////////////////////////////////
/// \brief Get the magnitude of the quaternion
///
/// \param q The input quaternion
///
/// \return The magnitude of the input quaternion
///
///////////////////////////////////////////////////////////
float magnitude(const Quaternion& q);

///////////////////////////////////////////////////////////
/// \brief Get the magnitude of the quaternion
///
/// \param q The input quaternion
///
/// \return The normalized version of the input quaternion
///
///////////////////////////////////////////////////////////
Quaternion normalize(const Quaternion& q);

///////////////////////////////////////////////////////////
/// \brief Get the conjugate of the quaternion
///
/// \param q The input quaternion
///
/// \return The conjugate of the input quaternion
///
///////////////////////////////////////////////////////////
Quaternion conjugate(const Quaternion& q);

///////////////////////////////////////////////////////////
/// \brief Get the inverse of the quaternion
///
/// \param q The input quaternion
///
/// \return The inverse of the input quaternion
///
///////////////////////////////////////////////////////////
Quaternion inverse(const Quaternion& q);

///////////////////////////////////////////////////////////
/// \brief Interpolate between two quaternions
///
/// \param a The first quaternion
/// \param b The second quaternion
/// \param x The interpolation
///
/// \return The interpolated quaternion
///
///////////////////////////////////////////////////////////
Quaternion slerp(const Quaternion& a, const Quaternion& b, float x);

///////////////////////////////////////////////////////////
/// \brief Convert a quaternion to a 4x4 rotation matrix
///
/// \param q The input quaternion
///
/// \return A 4x4 rotation matrix
///
///////////////////////////////////////////////////////////
Matrix4f toMatrix(const Quaternion& q);

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Quaternion
/// \ingroup Graphics
///
/// idk how a quaternion works
///
///////////////////////////////////////////////////////////