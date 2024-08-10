#ifndef POLY_CONVEX_MESH_COLLIDER_H
#define POLY_CONVEX_MESH_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A collider object that uses a convex mesh shape
/// \ingroup Physics
///
/// See Collider to see a short usage example for colliders.
///
///////////////////////////////////////////////////////////
class ConvexMeshCollider : public Collider
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Set mesh scale
	///
	/// \param dims The new scale of the mesh
	///
	///////////////////////////////////////////////////////////
	void setScale(const Vector3f& scale);

	///////////////////////////////////////////////////////////
	/// \brief Set mesh scale
	///
	/// \param x The x-component of the scale
	/// \param y The y-component of the scale
	/// \param z The z-component of the scale
	///
	///////////////////////////////////////////////////////////
	void setScale(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Get the scale of the mesh
	///
	/// \return The scale of the mesh
	///
	///////////////////////////////////////////////////////////
	Vector3f getScale() const;
};


}

#endif