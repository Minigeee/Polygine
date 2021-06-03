#ifndef POLY_COLLIDER_H
#define POLY_COLLIDER_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


class Collider
{
public:
	Collider();

	void setPosition(const Vector3f& position);

	void setPosition(float x, float y, float z);

	void setRotation(const Quaternion& rot);

	const Vector3f& getPosition() const;

	const Quaternion& getRotation() const;

private:
	Vector3f m_position;
	Quaternion m_rotation;
};


}

#endif