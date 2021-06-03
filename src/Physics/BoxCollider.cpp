#include <poly/Physics/BoxCollider.h>

namespace poly
{


///////////////////////////////////////////////////////////
BoxCollider::BoxCollider() :
	m_dimensions		(1.0f)
{

}


///////////////////////////////////////////////////////////
BoxCollider::BoxCollider(const Vector3f& dims) :
	m_dimensions		(dims)
{

}


///////////////////////////////////////////////////////////
BoxCollider::BoxCollider(float x, float y, float z) :
	m_dimensions		(Vector3f(x, y, z))
{

}


///////////////////////////////////////////////////////////
void BoxCollider::setDimensions(const Vector3f& dims)
{
	m_dimensions = dims;
}


///////////////////////////////////////////////////////////
void BoxCollider::setDimensions(float x, float y, float z)
{
	m_dimensions = Vector3f(x, y, z);
}


///////////////////////////////////////////////////////////
const Vector3f& BoxCollider::getDimensions() const
{
	return m_dimensions;
}


}