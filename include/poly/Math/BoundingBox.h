#ifndef POLY_BOUNDING_BOX_H
#define POLY_BOUNDING_BOX_H

#include <poly/Math/Vector3.h>

namespace poly
{

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const Vector3f& min, const Vector3f& max);

	Vector3f getCenter() const;

	Vector3f getDimensions() const;

	Vector3f m_min;
	Vector3f m_max;
};

}

#endif