#include <poly/Physics/CapsuleCollider.h>

namespace poly
{


///////////////////////////////////////////////////////////
CapsuleCollider::CapsuleCollider() :
	m_radius		(0.5f),
	m_height		(1.0f)
{

}


///////////////////////////////////////////////////////////
CapsuleCollider::CapsuleCollider(float radius, float height) :
	m_radius		(radius),
	m_height		(height)
{

}


///////////////////////////////////////////////////////////
void CapsuleCollider::setRadius(float radius)
{
	m_radius = radius;
}


///////////////////////////////////////////////////////////
void CapsuleCollider::setHeight(float height)
{
	m_height = height;
}


///////////////////////////////////////////////////////////
float CapsuleCollider::getRadius() const
{
	return m_radius;
}


///////////////////////////////////////////////////////////
float CapsuleCollider::getHeight() const
{
	return m_height;
}


}