#include <poly/Physics/Shapes.h>

#include <poly/Graphics/Image.h>
#include <poly/Graphics/Model.h>

namespace poly
{


///////////////////////////////////////////////////////////
PhysicsShape::PhysicsShape() :
	m_position		(0.0f)
{

}


///////////////////////////////////////////////////////////
PhysicsShape::~PhysicsShape()
{

}


///////////////////////////////////////////////////////////
BoxShape::BoxShape() :
	m_dimensions		(1.0f)
{

}


///////////////////////////////////////////////////////////
BoxShape::BoxShape(const Vector3f& dims) :
	m_dimensions		(dims)
{

}


///////////////////////////////////////////////////////////
BoxShape::BoxShape(float x, float y, float z) :
	m_dimensions		(x, y, z)
{

}


///////////////////////////////////////////////////////////
CapsuleShape::CapsuleShape() :
	m_radius		(0.5f),
	m_height		(1.0f)
{

}


///////////////////////////////////////////////////////////
CapsuleShape::CapsuleShape(float radius, float height) :
	m_radius		(radius),
	m_height		(height)
{

}


///////////////////////////////////////////////////////////
ConcaveMeshShape::ConcaveMeshShape() :
	m_model			(0),
	m_meshNum		(0)
{

}


///////////////////////////////////////////////////////////
ConcaveMeshShape::ConcaveMeshShape(Model* model, Uint32 meshNum) :
	m_model			(model),
	m_meshNum		(meshNum)
{

}


///////////////////////////////////////////////////////////
ConvexMeshShape::ConvexMeshShape() :
	m_model			(0),
	m_meshNum		(0)
{

}


///////////////////////////////////////////////////////////
ConvexMeshShape::ConvexMeshShape(Model* model, Uint32 meshNum) :
	m_model			(model),
	m_meshNum		(meshNum)
{

}


///////////////////////////////////////////////////////////
HeightMapShape::HeightMapShape() :
	m_dimensions		(0.0f),
	m_imageSize			(0),
	m_data				(0)
{

}


///////////////////////////////////////////////////////////
HeightMapShape::HeightMapShape(const Image& hmap, const Vector3f& dims) :
	m_dimensions		(dims),
	m_imageSize			(hmap.getWidth(), hmap.getHeight()),
	m_data				((float*)hmap.getData())
{

}


///////////////////////////////////////////////////////////
SphereShape::SphereShape() :
	m_radius		(0.5f)
{

}


///////////////////////////////////////////////////////////
SphereShape::SphereShape(float radius) :
	m_radius		(radius)
{

}


}