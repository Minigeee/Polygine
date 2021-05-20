#include <poly/Graphics/Renderable.h>

namespace poly
{


///////////////////////////////////////////////////////////
Renderable::~Renderable()
{

}


///////////////////////////////////////////////////////////
const BoundingBox& Renderable::getBoundingBox() const
{
	return m_boundingBox;
}


///////////////////////////////////////////////////////////
const Sphere& Renderable::getBoundingSphere() const
{
	return m_boundingSphere;
}


}