#include <poly/Graphics/Image.h>

#include <poly/Physics/HeightMapCollider.h>

namespace poly
{


///////////////////////////////////////////////////////////
HeightMapCollider::HeightMapCollider() :
	m_mapSize		(0),
	m_data			(0),
	m_dimensions	(0.0f)
{

}


///////////////////////////////////////////////////////////
HeightMapCollider::HeightMapCollider(const Image& hmap, const Vector3f& dims) :
	m_mapSize		(hmap.getWidth(), hmap.getHeight()),
	m_data			((float*)hmap.getData()),
	m_dimensions	(dims)
{

}


///////////////////////////////////////////////////////////
void HeightMapCollider::setHeightMap(const Image& hmap)
{
	m_mapSize = Vector2u(hmap.getWidth(), hmap.getHeight());
	m_data = (float*)hmap.getData();
}


///////////////////////////////////////////////////////////
void HeightMapCollider::setHeightData(float* data)
{
	m_data = data;
}


///////////////////////////////////////////////////////////
void HeightMapCollider::setMapSize(const Vector2u& size)
{
	m_mapSize = size;
}


///////////////////////////////////////////////////////////
void HeightMapCollider::setMapSize(Uint32 w, Uint32 h)
{
	m_mapSize = Vector2u(w, h);
}


///////////////////////////////////////////////////////////
void HeightMapCollider::setDimensions(const Vector3f& dims)
{
	m_dimensions = dims;
}


///////////////////////////////////////////////////////////
void HeightMapCollider::setDimensions(float x, float y, float z)
{
	m_dimensions = Vector3f(x, y, z);
}


///////////////////////////////////////////////////////////
const Vector2u& HeightMapCollider::getMapSize() const
{
	return m_mapSize;
}


///////////////////////////////////////////////////////////
float* HeightMapCollider::getHeightData() const
{
	return m_data;
}


///////////////////////////////////////////////////////////
const Vector3f& HeightMapCollider::getDimensions() const
{
	return m_dimensions;
}


}