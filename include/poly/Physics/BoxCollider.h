#ifndef POLY_BOX_COLLIDER_H
#define POLY_BOX_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A collider object that uses a box shape
/// \ingroup Physics
///
/// See Collider to see a short usage example for colliders.
///
///////////////////////////////////////////////////////////
class BoxCollider : public Collider
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Set box dimensions
	///
	/// \param dims The new dimensions of the box
	///
	///////////////////////////////////////////////////////////
	void setDimensions(const Vector3f& dims);

	///////////////////////////////////////////////////////////
	/// \brief Set box dimensions
	///
	/// \param x The x-component of the dimension
	/// \param y The y-component of the dimension
	/// \param z The z-component of the dimension
	///
	///////////////////////////////////////////////////////////
	void setDimensions(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Get the dimensions of the box
	///
	/// \return The dimensions of the box
	///
	///////////////////////////////////////////////////////////
	Vector3f getDimensions() const;
};


}

#endif