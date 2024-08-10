#ifndef POLY_CAPSULE_COLLIDER_H
#define POLY_CAPSULE_COLLIDER_H

#include <poly/Physics/Collider.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A collider object that uses a capsule shape
/// \ingroup Physics
///
/// See Collider to see a short usage example for colliders.
///
///////////////////////////////////////////////////////////
class CapsuleCollider : public Collider
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Set capsule radius
	///
	/// This is the radius of the cylinder portion of the capsule,
	/// as well as the hemisphere ends of the capsule.
	///
	/// \param radius The radius of the capsule
	///
	///////////////////////////////////////////////////////////
	void setRadius(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Set capsule height
	///
	/// This is the height of the cylinder portion of the capsule
	/// between the end hemispheres.
	///
	/// \param height The height of the capsule
	///
	///////////////////////////////////////////////////////////
	void setHeight(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Get the radius of the capsule
	///
	/// \return The radius of the capsule
	///
	///////////////////////////////////////////////////////////
	float getRadius() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the height of the capsule
	///
	/// \return The height of the capsule
	///
	///////////////////////////////////////////////////////////
	float getHeight() const;
};


}

#endif