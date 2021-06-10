#ifndef POLY_PHYSICS_H
#define POLY_PHYSICS_H

#include <poly/Core/DataTypes.h>

#include <poly/Engine/Extension.h>

#include <poly/Math/Ray.h>

#include <poly/Physics/Collider.h>
#include <poly/Physics/Joints.h>
#include <poly/Physics/Shapes.h>

namespace poly
{


class PhysicsEventHandler;


///////////////////////////////////////////////////////////
/// \brief A struct returned from raycast queries containing info about the raycast results
///
///////////////////////////////////////////////////////////
struct RaycastInfo
{
	Entity::Id m_entity;	//!< The entity the raycast collided with
	Collider m_collider;	//!< The collider object the raycast collided with
	Vector3f m_point;		//!< The point the ray intersects the collider in world space coordinates
	Vector3f m_normal;		//!< The normal of the intersection surface in world space coordinate system
	float m_fraction;		//!< The fraction of the ray that comes before the intersection point
};


///////////////////////////////////////////////////////////
/// \brief The physics simulation system
///
///////////////////////////////////////////////////////////
class Physics : public Extension
{
	friend PhysicsEventHandler;

public:
	///////////////////////////////////////////////////////////
	/// \brief Create the physics extension with a pointer to a scene
	///
	/// The physics system can be initialized as a scene extension or
	/// as a standalone object, but it's more convenient as a scene
	/// extension. Use getExtension() to use the physics system as an
	/// extension.
	///
	///////////////////////////////////////////////////////////
	Physics(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~Physics();

	///////////////////////////////////////////////////////////
	/// \brief Do a physics simulation step
	///
	/// This function will simulate a physics step, using the elapsed
	/// time given in the parameters. This will apply all the rules of
	/// physics to all the rigid body components existing in the scene.
	/// The update first copies all component data into an internal storage,
	/// then performs the update using its internal data. This way,
	/// the physics engine doesn't bottleneck other threads if using
	/// a multithreaded game loop.
	///
	/// Every time an entity with a RigidBodyComponent or a 
	/// CollisionBodyComponent is added to the scene, the physics
	/// system will automatically track it and apply the laws of
	/// physics to it during this update.
	///
	/// \param dt The elapsed time, or the physics time step (in seconds)
	///
	///////////////////////////////////////////////////////////
	void update(float dt);

	///////////////////////////////////////////////////////////
	/// \brief Perform a raycast test using the existing collision bodies in the physics system
	///
	/// This functions sends a ray in world space coordinate system
	/// and returns the bodies the ray intersects with. The ray
	/// must have an origin, a direction and a maximum distance where
	/// it can detect collisions. The direction vector of the ray
	/// does not have to be normalized.
	///
	/// The types of collision bodies the ray can collide with
	/// can be controlled by setting the collision \a mask. See
	/// Collider::setCollisionCategory() for more details about how
	/// the collision masks work. The maximum
	/// number of collision body intersections the raycast is allowed
	/// to detect before quitting early can be set with \a maxIntersects.
	/// This parameter can be used to improve the performance in cases
	/// where only a single body intersection is needed, by not checking
	/// any further than the body. If the \a maxIntersects is set to
	/// 0, then there will be no maximum number of allowed raycast
	/// collisions, and the raycast will end when the maximum distance
	/// is reached.
	///
	/// This function returns a list of RaycastInfo, containing information
	/// for every collision the raycast detects.
	//
	/// \param ray The ray used to perform the raycast
	/// \param dist The maximum distance allowed in the raycast
	/// \param mask The collision mask used to determine which bodies the ray is allowed to collide with
	/// \param maxIntersects The maximum number of ray intersects the raycast is allowed to detect (0 for unlimited)
	///
	/// \return The list of RaycastInfo containing information on all the raycast collisions
	///
	///////////////////////////////////////////////////////////
	std::vector<RaycastInfo>& raycast(
		const Ray& ray,
		float dist,
		Uint16 mask = 0xFFFF,
		Uint32 maxIntersects = 0
	);

	///////////////////////////////////////////////////////////
	/// \brief Set the global gravity acceleration vector
	///
	/// This sets the gravity acceleration vector. The default
	/// is (0.0, -9.81, 0.0).
	///
	/// \param gravity The gravity acceleration vector
	///
	///////////////////////////////////////////////////////////
	void setGravity(const Vector3f& gravity);

	///////////////////////////////////////////////////////////
	/// \brief Set the global gravity acceleration vector
	///
	/// This sets the gravity acceleration vector. The default
	/// is (0.0, -9.81, 0.0).
	///
	/// \param x The x-component of the gravity acceleration vector
	/// \param y The y-component of the gravity acceleration vector
	/// \param z The z-component of the gravity acceleration vector
	///
	///////////////////////////////////////////////////////////
	void setGravity(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Set if the rigid or collision bodies of the specified entity are allowed to sleep
	///
	/// Sleeping bodies may sometimes fail to send an event
	/// when their collision with another object ends, so it will
	/// be safest to not allow sleep for entities where collision
	/// detection is important.
	///
	/// \param entity The entity containing the body to set sleep allowed for
	/// \param allowed Whether the physics body will be allowed to sleep
	///
	///////////////////////////////////////////////////////////
	void setSleepAllowed(const Entity& entity, bool allowed);

	///////////////////////////////////////////////////////////
	/// \brief Get the gravity acceleration vector
	///
	/// \return The gravity acceleration vector
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getGravity() const;

	///////////////////////////////////////////////////////////
	/// \brief Add a collider to an entity containing a physics body
	///
	/// The entity a collider is being added to must have either a
	/// RigidBodyComponent or a CollisionBodyComponent, or both (both
	/// doesn't make sense though). If both components exist in the
	/// entity, the the collider will only be added to the RigidBodyComponent.
	///
	/// Any of the following physics collider shapes can be used:
	/// \li BoxShape
	/// \li CapsuleShape
	/// \li ConcaveMeshShape
	/// \li ConvexMeshShape
	/// \li HeightMapShape
	/// \li SphereShape
	///
	/// Each physics shape has a position and a rotation quaternion.
	/// Use these properties to set the transform orientation of each
	/// collider in the local space of the physics body. This transform
	/// orientation exists because multiple colliders can be added to
	/// entities with physics bodies, and using multiple colliders,
	/// each with their own shape and orientation, can be used to make
	/// more complex collision shapes.
	///
	/// The initial bounciness and friction coefficient of the collider
	/// can be set using this function, but they can also be changed
	/// later using the returned Collider object.
	///
	/// \param entity The entity to add a collider to
	/// \param shape Any shape that inherits from PhysicsShape
	/// \param bounciness The intial bounciness of the collider
	/// \param friction The initial friction coefficient of the collider
	///
	/// \return A Collider object that can be used to control the properties of the collider
	///
	///////////////////////////////////////////////////////////
	Collider addCollider(
		const Entity& entity,
		const PhysicsShape& shape,
		float bounciness = 0.1f,
		float friction = 0.2f
	);

	///////////////////////////////////////////////////////////
	/// \brief Remove a collider from an entity with a physics body
	///
	/// Remove a collider from an entity with a physics body, using
	/// the index of the collider.
	///
	/// \param entity The entity to remove the collider from
	/// \param index The index of the collider to remove
	///
	///////////////////////////////////////////////////////////
	void removeCollider(const Entity& entity, Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Remove a collider from an entity with a physics body
	///
	/// Remove a collider from an entity with a physics body, using
	/// the index of the collider.
	///
	/// \param entity The entity to remove the collider from
	/// \param index The index of the collider to remove
	///
	///////////////////////////////////////////////////////////
	void removeCollider(const Entity& entity, const Collider& collider);

	///////////////////////////////////////////////////////////
	/// \brief Add a joint between two entities containing rigid bodies
	///
	/// Joints are used to constrain certain axes of motion of one
	/// rigid body relative to another rigid body. The available types
	/// of joints that can be used are:
	///
	/// \li Joint::Type::BallAndSocket
	/// \li Joint::Type::Hinge
	/// \li Joint::Type::Slider
	/// \li Joint::Type::Fixed
	///
	/// Joints can only be added to rigid bodies, and for each joint,
	/// a point in world space coordinate system must be given.
	/// If the joint type allows for an axis, such as Joint::Type::Hinge
	/// and Joint::Type::Slider.
	///
	/// \param e1 The first entity containing a RigidBodyComponent to attach the joint to
	/// \param e2 The second entity containing a RigidBodyComponent to attach the joint to
	/// \param type The type of joint to create
	/// \param point The point to place the joint in world space coordinate system
	/// \param axis The axis of rotation for joints that allow it
	///
	/// \return A Joint object
	///
	///////////////////////////////////////////////////////////
	Joint addJoint(
		const Entity& e1,
		const Entity& e2,
		Joint::Type type,
		const Vector3f& point,
		const Vector3f& axis = Vector3f(0.0f, 1.0f, 0.0f)
	);

	///////////////////////////////////////////////////////////
	/// \brief Remove a joint
	///
	/// This removes the joint between two entities
	///
	/// \param The joint to remove
	///
	///////////////////////////////////////////////////////////
	void removeJoint(const Joint& joint);

private:
	struct BodyData
	{
		void* m_body;
		Uint32 m_group;
		Uint32 m_index;
	};

	struct RigidBodyData
	{
		RigidBodyData(const Entity::Id& id, void* body);

		Entity::Id m_id;
		void* m_body;
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_linearVelocity;
		Vector3f m_angularVelocity;
		RigidBodyType m_type;
		bool m_allowedSleep;
		bool m_isSleeping;
		bool m_massPropertiesUpdated;
	};

	struct CollisionBodyData
	{
		Entity::Id m_id;
		void* m_body;
	};

	struct ConcaveMeshData
	{
		ConcaveMeshData();

		~ConcaveMeshData();

		void* m_shape;
		Uint32* m_indices;
		void* m_vertexArray;
	};

	struct ConvexMeshData
	{
		ConvexMeshData();

		~ConvexMeshData();

		void* m_shape;
		Vector3f* m_vertices;
		Uint32* m_indices;
		void* m_faces;
		void* m_vertexArray;
	};

	void addRigidBody(Entity::Id id);

	void addCollisionBody(Entity::Id id);

	void removeRigidBody(Entity::Id id);

	void removeCollisionBody(Entity::Id id);

	Collider createCollider(const Entity& entity, const PhysicsShape& shape, void* rp3dShape);

	void* getBoxShape(const Vector3f& dims);

	void* getCapsuleShape(const Vector2f& dims);

	void* getConcaveMeshShape(const ConcaveMeshShape& shape);

	void* getConvexMeshShape(const ConvexMeshShape& shape);

	void* getHeightMapShape(const HeightMapShape& shape);

	void* getSphereShape(float radius);

private:
	void* m_world;																//!< The RP3D physics world pointer
	PhysicsEventHandler* m_eventHandler;										//!< The RP3D event handler
	Vector3f m_gravity;															//!< The gravity acceleration vector

	HashMap<Entity::Id, BodyData> m_rigidBodies;								//!< Map entity id to physics body data (rigid bodies)
	HashMap<Entity::Id, BodyData> m_collisionBodies;							//!< Map entity id to physics body data (collision bodies)
	HashMap<Uint32, std::vector<RigidBodyData>> m_groupedRigidBodies;			//!< Map entity group to list of rigid bodies
	HashMap<Uint32, std::vector<CollisionBodyData>> m_groupedCollisionBodies;	//!< Map entity group to list of collision bodies
	HashMap<void*, Entity::Id> m_mapBodyToEntity;								//!< Map collision bodies to entity ids

	Uint32 m_numRaycastIntersects;												//!< The internal counter for the number of raycast intersects
	Uint32 m_maxRaycastIntersects;												//!< The max number of raycast intersects allowed for the current raycast query
	std::vector<poly::RaycastInfo> m_raycastInfo;								//!< The temp storage for storing raycast results

	static HashMap<float, void*> s_boxShapes;
	static HashMap<float, void*> s_capsuleShapes;
	static HashMap<void*, ConcaveMeshData> s_concaveMeshShapes;
	static HashMap<void*, ConvexMeshData> s_convexMeshShapes;
	static HashMap<float*, void*> s_heightMapShapes;
	static HashMap<float, void*> s_sphereShapes;
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::Physics
/// \ingroup Physics
///
/// The physics scene extension is used to simulate physics in
/// a game world. The extension can be used as a standalone object
/// but it is more convenient as an extension because it can be
/// accessed through the scene pointer.
///
/// To use the physics extension, simply call update() in the game
/// loop while passing in the desired time step. For physics, it is
/// normally best to use a relatievely constant time step so
/// that weird results don't occur, such as objects going through
/// floors or walls. The update function will handle applying all
/// the laws of physics to any entity that contain the RigidBodyComponent.
///
/// To add physics bodies to the simulation, create entities in the
/// scene that have either a RigidBodyComponent or a CollisionBodyComponent
/// attached to them, and the physics extension will automatically track
/// them as soon as the entity is created. To change the properties
/// of a physics body, just change its component properties and
/// the changes will be taken into account during the next physics
/// update. All rigid and collision body properties are modifiable
/// except for the center of mass of rigid bodies.
///
/// The difference between rigid bodies and collision bodies
/// is that rigid bodies follow the rules of the physics engine,
/// while collision bodies exist primarily to detect collisions,
/// without influencing the state of the physics world. This means
/// that rigid bodies are affected by gravity, external forces,
/// friction, and forces exerted by collisions, while collision bodies
/// must have their transform set externally. A rigid body by itself
/// can not have any collisions, it needs a Collider to collider with
/// other rigid bodies. To add a collider, addCollider must be called,
/// providing the entity to add the collider to, the shape of the
/// collider, and the position/orientation of the collider in rigid
/// body local space. This function returns a Collider object that
/// can be used to change its properties.
///
/// Other noteable features of the physics extension are raycasting
/// and joints. To do raycasting, use the raycast() function, and
/// to add joints, use the addJoint() function. Please see those
/// functions for more details.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create the scene and create a few physics objects
/// Scene scene;
///
/// // Add an octree for rendering objects
/// Octree octree;
/// scene.addRenderSystem(&octree);
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
///		physics->addCollider(e, BoxShape(1.0f, 1.0f, 1.0f));
/// }
///
/// // Handle physics collision events
/// scene.addListener<E_PhysicsCollision>(
///		[&](const E_PhysicsCollision& e)
///		{
///			std::cout << "Collision " << (e.m_type == CollisionEventType::Start ? "start" : "end") << '\n';
///		}
/// );
///
/// // Game loop
/// while (true)
/// {
///		...
///
///		// Do the physics update
///		physics->update(1.0f / 60.0f);
///
///		// Copy rigid body transform into transform component for the render engine to use
///		scene.system<RigidBodyComponent, TransformComponent>(
///			[&](const Entity::Id& id, const RigidBodyComponent& rbody, TransformComponent& t)
///			{
///				t.m_position = rbody.m_position;
///				t.m_rotation = rbody.m_rotation;
///			}
///		);
///
///		// Update dynamic objects in the octree
///		octree.update();
///
///		...
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////