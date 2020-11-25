#ifndef POLY_FRUSTUM_H
#define POLY_FRUSTUM_H

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Plane.h>
#include <poly/Math/Sphere.h>

namespace poly
{

class Frustum
{
public:
	enum Side
	{
		Left = 0,
		Right,
		Bottom,
		Top,
		Near,
		Far
	};

public:
	Frustum();

	void setPlane(const Plane& plane, Side side);

	bool contains(const BoundingBox& box) const;

	bool contains(const Sphere& sphere) const;

private:
	Plane mPlanes[6];
};

}

#endif