#ifndef POLY_BOX_COLLIDER_H
#define POLY_BOX_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


class BoxCollider : public Collider
{
public:
	BoxCollider();

	BoxCollider(const Vector3f& dims);

	BoxCollider(float x, float y, float z);

	void setDimensions(const Vector3f& dims);

	void setDimensions(float x, float y, float z);

	const Vector3f& getDimensions() const;

private:
	Vector3f m_dimensions;
};


}

#endif