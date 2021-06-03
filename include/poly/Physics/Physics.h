#ifndef POLY_PHYSICS_H
#define POLY_PHYSICS_H

#include <poly/Core/DataTypes.h>

#include <poly/Engine/Extension.h>

namespace poly
{


class BoxCollider;
class CapsuleCollider;
class ConcaveMeshCollider;
class ConvexMeshCollider;
class HeightMapCollider;
class SphereCollider;


class Physics : public Extension
{
public:
	Physics(Scene* scene);

	~Physics();

	void update(float dt);

	void addCollider(const Entity& entity, const BoxCollider& collider);

	void addCollider(const Entity& entity, const CapsuleCollider& collider);

	void addCollider(const Entity& entity, const ConcaveMeshCollider& collider);

	void addCollider(const Entity& entity, const ConvexMeshCollider& collider);

	void addCollider(const Entity& entity, const HeightMapCollider& collider);

	void addCollider(const Entity& entity, const SphereCollider& collider);

private:
	struct BodyData
	{
		void* m_body;
		Uint32 m_group;
		Uint32 m_index;
	};

	struct GroupedBodyData
	{
		Entity::Id m_id;
		void* m_body;
		Vector3f m_position;
		Quaternion m_rotation;
		RigidBodyType m_type;
		bool m_isSleeping;
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
		Uint32* m_indices;
		void* m_faces;
		void* m_vertexArray;
	};

	void* getBoxShape(const Vector3f& dims);

	void* getCapsuleShape(const Vector2f& dims);

	void* getConcaveMeshShape(const ConcaveMeshCollider& collider);

	void* getConvexMeshShape(const ConvexMeshCollider& collider);

	void* getHeightMapShape(const HeightMapCollider& collider);

	void* getSphereShape(float radius);

private:

	void* m_world;

	HashMap<Entity::Id, BodyData> m_rigidBodies;
	HashMap<Uint32, std::vector<GroupedBodyData>> m_groupedRigidBodies;		//!< Map entity group to list of rigid bodies

	static HashMap<float, void*> s_boxShapes;
	static HashMap<float, void*> s_capsuleShapes;
	static HashMap<void*, ConcaveMeshData> s_concaveMeshShapes;
	static HashMap<void*, ConvexMeshData> s_convexMeshShapes;
	static HashMap<float*, void*> s_heightMapShapes;
	static HashMap<float, void*> s_sphereShapes;
};


}

#endif