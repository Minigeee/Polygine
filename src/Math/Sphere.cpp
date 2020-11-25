#include <poly/Math/Sphere.h>

namespace poly
{


///////////////////////////////////////////////////////////
Sphere::Sphere() :
	m_position	(0.0f),
	m_radius	(1.0f)
{

}


///////////////////////////////////////////////////////////
Sphere::Sphere(const Vector3f& p, float r) :
	m_position	(p),
	m_radius	(r)
{

}


}