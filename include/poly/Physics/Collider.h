#ifndef POLY_COLLIDER_H
#define POLY_COLLIDER_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{

#ifndef DOXYGEN_SKIP
namespace priv
{
class PhysicsEventHandler;
}
#endif


class Physics;


///////////////////////////////////////////////////////////
/// \brief A physics class used to set the properties of physics body colliders
///
///////////////////////////////////////////////////////////
class Collider
{
	friend Physics;
	friend priv::PhysicsEventHandler;

public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	Collider();

	///////////////////////////////////////////////////////////
	/// \brief Set the bounciness of the collider
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). A bounciness value of 0
	/// is the lowest bounciness value a collider can have, and a
	/// value of 1 is the highest.
	///
	/// The initial value is 0.1.
	///
	/// \param bounciness The bounciness value
	///
	///////////////////////////////////////////////////////////
	void setBounciness(float bounciness);

	///////////////////////////////////////////////////////////
	/// \brief Set the friction coefficient of the collider
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). The force exerted by
	/// friction always moves in the opposite direction of motion,
	/// and the magnitude of the force is equal to the mass of the
	/// body multiplied by the friction coefficient.
	///
	/// The initial value is 0.2.
	///
	/// \param coefficient The friction coefficient
	///
	///////////////////////////////////////////////////////////
	void setFrictionCoefficient(float coefficient);

	///////////////////////////////////////////////////////////
	/// \brief Set the rolling resistance of the collider
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). The rolling resistance
	/// determines how much resistance to apply to rolling objects.
	/// A resistance of 0 means that no resistance is applied, and
	/// a resistance of 1 means that maximum resistance is applied.
	///
	/// The initial value is 0.0.
	///
	/// \param resistance The rolling resistance value
	///
	///////////////////////////////////////////////////////////
	void setRollingResistance(float resistance);

	///////////////////////////////////////////////////////////
	/// \brief Set the collision category bitfield of the collider
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). This category bitfield
	/// will be used when testing for collisions to determine which
	/// other colliders the current collider is allowed to interact
	/// with. A collision occurs and is handled when the result of
	/// a binary AND operation between the category and mask bitfields
	/// evaluates to TRUE.
	///
	/// The initial value is 0x0001.
	///
	/// \see setCollisionMask
	///
	/// \param category The collision category bitfield
	///
	///////////////////////////////////////////////////////////
	void setCollisionCategory(Uint16 category);

	///////////////////////////////////////////////////////////
	/// \brief Set the collision mask bitfield of the collider
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). The collision mask bitfield
	/// determines which collision categories the collider is allowed
	/// to collide with. A collision occurs and is handled when the result of
	/// a binary AND operation between the category and mask bitfields
	/// evaluates to TRUE.
	///
	/// The initial value is 0xFFFF.
	///
	/// \see setCollisionCategory
	///
	/// \param mask The collision mask bitfield
	///
	///////////////////////////////////////////////////////////
	void setCollisionMask(Uint16 mask);

	///////////////////////////////////////////////////////////
	/// \brief Set whether the collider should be treated as a trigger
	///
	/// Setting this property will only take affect if the collider
	/// is attached to a physics body, by using the collider object
	/// returned from Physics::addCollider(). Trigger colliders can
	/// detect collisions, but they do not affect any of the bodies
	/// that collide with it.
	///
	/// The initial value is false.
	///
	/// \param trigger Whether the collider should be a trigger or not
	///
	///////////////////////////////////////////////////////////
	void setIsTrigger(bool trigger);

	///////////////////////////////////////////////////////////
	/// \brief Get the collider bounciness value
	///
	/// \return The bounciness value
	///
	///////////////////////////////////////////////////////////
	float getBounciness() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the friction coefficient
	///
	/// \return The friction coefficient
	///
	///////////////////////////////////////////////////////////
	float getFrictionCoefficient() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the rolling resistance
	///
	/// \return The rolling resistance
	///
	///////////////////////////////////////////////////////////
	float getRollingResistance() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision category bitfield
	///
	/// \return The collision category bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionCategory() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision mask bitfield
	///
	/// \return The collision mask bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionMask() const;

	///////////////////////////////////////////////////////////
	/// \brief Check whether the collider is a trigger or not
	///
	/// \return True if the collider is a trigger
	///
	///////////////////////////////////////////////////////////
	bool isTrigger() const;

private:
	void setCollider(void* collider);

private:
	void* m_collider;				//!< A pointer to the internal collider object
	float m_bounciness;				//!< The bounciness
	float m_frictionCoefficient;	//!< The friction coefficient
	float m_rollingResistance;		//!< The rolling resistance
	Uint16 m_collisionCategory;		//!< The collision category bitfield
	Uint16 m_collisionMask;			//!< The collision mask bitfield
	bool m_isTrigger;				//!< The trigger flag
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Collider
/// \ingroup Physics
///
/// The collider object is used to read and modify the properties
/// of a collider that is attached to an entity with a physics
/// body. Most of the time, the Collider object will be generated
/// by the Physics extension using Physics::addCollider(), and
/// the collider object can be used to change its properties or
/// change its collision mask after the initial collider was created.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create the scene and create a few physics objects
/// Scene scene;
///
/// // Get the physics extension
///	Physics* physics = scene.getExtension<Physics>();
///
/// // Load a box model
/// Model model("models/box.dae");
///
/// for (int i = 0; i < 10; ++i)
/// {
///		RigidBodyComponent rbody;
///		rbody.m_position = Vector3f(0.0f, 5.0f + i, 0.0f);
///		Entity e = scene.createEntity(TransformComponent(), rbody, RenderComponent(&model), DynamicTag());
///
///		// Create a collider
///		Collider collider = physics->addCollider(e, BoxShape(1.0f, 1.0f, 1.0f));
///		// Set properties
///		collider.setRollingResistance(0.1f);
///		collider.setCollisionCategory(2);
///		collider.setCollisionMask(0xFFFF - 1);
/// }
///
/// ...
///
/// \endcode
///
///////////////////////////////////////////////////////////