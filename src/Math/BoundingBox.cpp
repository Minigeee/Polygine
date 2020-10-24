#include <poly/Math/BoundingBox.h>

namespace poly
{


///////////////////////////////////////////////////////////
BoundingBox::BoundingBox() :
	m_min	(0.0f),
	m_max	(0.0f)
{

}


///////////////////////////////////////////////////////////
BoundingBox::BoundingBox(const Vector3f& min, const Vector3f& max) :
	m_min	(min),
	m_max	(max)
{

}


///////////////////////////////////////////////////////////
Vector3f BoundingBox::getCenter() const
{
	return (m_min + m_max) * 0.5f;
}


///////////////////////////////////////////////////////////
Vector3f BoundingBox::getDimensions() const
{
	return m_max - m_min;
}


}