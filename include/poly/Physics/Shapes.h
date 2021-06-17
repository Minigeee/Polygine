#ifndef POLY_PHYSICS_SHAPES_H
#define POLY_PHYSICS_SHAPES_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


class Image;
class Model;


///////////////////////////////////////////////////////////
/// \brief The base physics shape class
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	PhysicsShape();

	///////////////////////////////////////////////////////////
	/// \brief The destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~PhysicsShape();

	Vector3f m_position;		//!< The position of the physics shape in the local space of the physics body
	Quaternion m_rotation;		//!< The orientation of the physics shape in the local space of the physics body
};


///////////////////////////////////////////////////////////
/// \brief A box collider physics shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct BoxShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	BoxShape();

	///////////////////////////////////////////////////////////
	/// \brief Create the box shape from its dimensions in the (x, y, z) axes
	///
	/// \param dims The dimensions of the box shape
	///
	///////////////////////////////////////////////////////////
	BoxShape(const Vector3f& dims);

	///////////////////////////////////////////////////////////
	/// \brief Create the box shape from its dimensions in the (x, y, z) axes
	///
	/// \param x The dimensions of the box shape along the x-axis
	/// \param y The dimensions of the box shape along the y-axis
	/// \param z The dimensions of the box shape along the z-axis
	///
	///////////////////////////////////////////////////////////
	BoxShape(float x, float y, float z);

	Vector3f m_dimensions;		//!< The dimensions of the box shape
};


///////////////////////////////////////////////////////////
/// \brief A capsule collider physics shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct CapsuleShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	CapsuleShape();

	///////////////////////////////////////////////////////////
	/// \brief Create a capsule physics shape using its curvature radius and height
	///
	/// \param radius The curvature radius of the hemispheres on the ends of the capsule
	/// \param height The height of the cylinder between the end hemispheres
	///
	///////////////////////////////////////////////////////////
	CapsuleShape(float radius, float height);

	float m_radius;				//!< The curvature radius of the hemispheres on the ends of the capsule
	float m_height;				//!< The height of the cylinder between the end hemispheres
};


///////////////////////////////////////////////////////////
/// \brief A concave mesh shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct ConcaveMeshShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	ConcaveMeshShape();

	///////////////////////////////////////////////////////////
	/// \brief Create a concave mesh shape from a model and a mesh index
	///
	/// \param model A pointer to a model containing vertex data to use for the mesh shape
	/// \param mesh The mesh index to use from the model
	///
	///////////////////////////////////////////////////////////
	ConcaveMeshShape(Model* model, Uint32 mesh = 0);

	Model* m_model;				//!< A pointer to a model containing vertex data to use for the mesh shape
	Uint32 m_meshNum;			//!< The mesh index to use from the model
};


///////////////////////////////////////////////////////////
/// \brief A convex mesh physics shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct ConvexMeshShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	ConvexMeshShape();

	///////////////////////////////////////////////////////////
	/// \brief Create a concave mesh shape from a model and a mesh index
	///
	/// \param model A pointer to a model containing vertex data to use for the mesh shape
	/// \param mesh The mesh index to use from the model
	///
	///////////////////////////////////////////////////////////
	ConvexMeshShape(Model* model, Uint32 mesh = 0);

	Model* m_model;				//!< A pointer to a model containing vertex data to use for the mesh shape
	Uint32 m_meshNum;			//!< The mesh index to use from the model
};


///////////////////////////////////////////////////////////
/// \brief A height map physics shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct HeightMapShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	HeightMapShape();

	///////////////////////////////////////////////////////////
	/// \brief Create a height map physics shape from a height map and the terrain dimensions
	///
	/// \param hmap An image containing the height data
	/// \param dims The dimensions of the terrain along the (x, y, z) axes
	///
	///////////////////////////////////////////////////////////
	HeightMapShape(const Image& hmap, const Vector3f& dims);

	Vector3f m_dimensions;		//!< The dimensions of the terrain along the (x, y, z) axes
	Vector2u m_imageSize;		//!< The size of the height map image
	float* m_data;				//!< A pointer to the height data
};


///////////////////////////////////////////////////////////
/// \brief A sphere physics shape
/// \ingroup Physics
///
///////////////////////////////////////////////////////////
struct SphereShape : public PhysicsShape
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	SphereShape();

	///////////////////////////////////////////////////////////
	/// \brief Create a sphere physics shape from its radius
	///
	/// \param radius The radius of the sphere shape
	///
	///////////////////////////////////////////////////////////
	SphereShape(float radius);

	float m_radius;				//!< The radius of the sphere shape
};


}

#endif