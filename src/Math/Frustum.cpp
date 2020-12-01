#include <poly/Math/Frustum.h>

namespace poly
{


///////////////////////////////////////////////////////////
void Frustum::setPlane(const Plane& plane, Side side)
{
	mPlanes[side] = plane;
}


///////////////////////////////////////////////////////////
bool Frustum::contains(const BoundingBox& box) const
{
	const Vector3f& min = box.m_min;
	const Vector3f& max = box.m_max;

	for (Uint32 i = 0; i < 6; ++i)
	{
		const Plane& plane = mPlanes[i];

		Vector3f vmin;
		vmin.x = plane.n.x > 0.0f ? max.x : min.x;
		vmin.y = plane.n.y > 0.0f ? max.y : min.y;
		vmin.z = plane.n.z > 0.0f ? max.z : min.z;

		if (dist(plane, vmin) < 0.0f)
			return false;
	}

	return true;
}


///////////////////////////////////////////////////////////
bool Frustum::contains(const Sphere& sphere) const
{
	bool contains = true;

	for (Uint32 i = 0; i < 6; ++i)
	{
		const Plane& plane = mPlanes[i];

		if (dist(plane, sphere.m_position) + sphere.m_radius < 0.0f)
			contains = false;
	}

	return contains;
}


}