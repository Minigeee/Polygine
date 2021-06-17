#include <poly/Math/Ray.h>

namespace poly
{


///////////////////////////////////////////////////////////
Ray::Ray() :
	m_origin		(0.0f),
	m_direction		(0.0f, 1.0f, 0.0f)
{

}


///////////////////////////////////////////////////////////
Ray::Ray(const Vector3f& origin, const Vector3f& direction) :
	m_origin		(origin),
	m_direction		(direction)
{

}


}