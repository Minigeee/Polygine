#include <poly/Physics/Collider.h>

namespace poly
{


///////////////////////////////////////////////////////////
Collider::Collider() :
	m_position		(0.0f),
	m_rotation		()
{

}


///////////////////////////////////////////////////////////
void Collider::setPosition(const Vector3f& pos)
{
	m_position = pos;
}


///////////////////////////////////////////////////////////
void Collider::setPosition(float x, float y, float z)
{
	m_position = Vector3f(x, y, z);
}


///////////////////////////////////////////////////////////
void Collider::setRotation(const Quaternion& rot)
{
	m_rotation = rot;
}


///////////////////////////////////////////////////////////
const Vector3f& Collider::getPosition() const
{
	return m_position;
}


///////////////////////////////////////////////////////////
const Quaternion& Collider::getRotation() const
{
	return m_rotation;
}


}