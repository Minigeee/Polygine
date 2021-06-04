#ifndef POLY_PHYSICS_SHAPES_H
#define POLY_PHYSICS_SHAPES_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


class Image;
class Model;


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct PhysicsShape
{
	PhysicsShape();

	Vector3f m_position;
	Quaternion m_rotation;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct BoxShape : public PhysicsShape
{
	BoxShape();

	BoxShape(const Vector3f& dims);

	BoxShape(float x, float y, float z);

	Vector3f m_dimensions;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct CapsuleShape : public PhysicsShape
{
	CapsuleShape();

	CapsuleShape(float radius, float height);

	float m_radius;
	float m_height;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct ConcaveMeshShape : public PhysicsShape
{
	ConcaveMeshShape();

	ConcaveMeshShape(Model* model, Uint32 mesh = 0);

	Model* m_model;
	Uint32 m_meshNum;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct ConvexMeshShape : public PhysicsShape
{
	ConvexMeshShape();

	ConvexMeshShape(Model* model, Uint32 mesh = 0);

	Model* m_model;
	Uint32 m_meshNum;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct HeightMapShape : public PhysicsShape
{
	HeightMapShape();

	HeightMapShape(const Image& hmap, const Vector3f& dims);

	Vector3f m_dimensions;
	Vector2u m_imageSize;
	float* m_data;
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
struct SphereShape : public PhysicsShape
{
	SphereShape();

	SphereShape(float radius);

	float m_radius;
};


}

#endif