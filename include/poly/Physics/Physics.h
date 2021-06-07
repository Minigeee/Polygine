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
	Vector3f m_point;		//!< The point the ray intersects the collider at in world space coordinates
	Vector3f m_normal;		//!< The normal of the intersection surface in world space coordinate system
	float m_fraction;		//!< The fraction of the ray that comes before the intersection point
};


///////////////////////////////////////////////////////////
class Physics : public Extension
{
	friend PhysicsEventHandler;

public:
	Physics(Scene* scene);

	~Physics();

	void update(float dt);

	std::vector<RaycastInfo>& raycast(
		const Ray& ray,
		float dist,
		Uint16 mask = 0xFFFF,
		Uint32 maxIntersects = 0
	);

	void setGravity(const Vector3f& gravity);

	void setGravity(float x, float y, float z);

	void setSleepAllowed(const Entity& entity, bool allowed);

	const Vector3f& getGravity() const;

	Collider addCollider(const Entity& entity, const BoxShape& shape);

	Collider addCollider(const Entity& entity, const CapsuleShape& shape);

	Collider addCollider(const Entity& entity, const ConcaveMeshShape& shape);

	Collider addCollider(const Entity& entity, const ConvexMeshShape& shape);

	Collider addCollider(const Entity& entity, const HeightMapShape& shape);

	Collider addCollider(const Entity& entity, const SphereShape& shape);

	void removeCollider(const Entity& entity, Uint32 index);

	void removeCollider(const Entity& entity, const Collider& collider);

	Joint addJoint(
		const Entity& e1,
		const Entity& e2,
		Joint::Type type,
		const Vector3f& point,
		const Vector3f& axis = Vector3f(0.0f, 1.0f, 0.0f)
	);

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
	void* m_world;
	PhysicsEventHandler* m_eventHandler;
	Vector3f m_gravity;

	HashMap<Entity::Id, BodyData> m_rigidBodies;								//!< Map entity id to physics body data (rigid bodies)
	HashMap<Entity::Id, BodyData> m_collisionBodies;							//!< Map entity id to physics body data (collision bodies)
	HashMap<Uint32, std::vector<RigidBodyData>> m_groupedRigidBodies;			//!< Map entity group to list of rigid bodies
	HashMap<Uint32, std::vector<CollisionBodyData>> m_groupedCollisionBodies;	//!< Map entity group to list of collision bodies
	HashMap<void*, Entity::Id> m_mapBodyToEntity;								//!< Map collision bodies to entity ids

	Uint32 m_numRaycastIntersects;
	Uint32 m_maxRaycastIntersects;
	std::vector<poly::RaycastInfo> m_raycastInfo;

	static HashMap<float, void*> s_boxShapes;
	static HashMap<float, void*> s_capsuleShapes;
	static HashMap<void*, ConcaveMeshData> s_concaveMeshShapes;
	static HashMap<void*, ConvexMeshData> s_convexMeshShapes;
	static HashMap<float*, void*> s_heightMapShapes;
	static HashMap<float, void*> s_sphereShapes;
};


}

#endif