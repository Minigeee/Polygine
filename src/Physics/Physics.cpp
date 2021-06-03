#include <poly/Core/Allocate.h>
#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Model.h>

#include <poly/Physics/BoxCollider.h>
#include <poly/Physics/CapsuleCollider.h>
#include <poly/Physics/Components.h>
#include <poly/Physics/ConcaveMeshCollider.h>
#include <poly/Physics/ConvexMeshCollider.h>
#include <poly/Physics/HeightMapCollider.h>
#include <poly/Physics/Physics.h>
#include <poly/Physics/SphereCollider.h>

#include <reactphysics3d/reactphysics3d.h>

#define WORLD_CAST(x) reinterpret_cast<reactphysics3d::PhysicsWorld*>(x)
#define RBODY_CAST(x) reinterpret_cast<reactphysics3d::RigidBody*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{


///////////////////////////////////////////////////////////
reactphysics3d::PhysicsCommon g_common;

///////////////////////////////////////////////////////////
HashMap<float, void*> Physics::s_boxShapes;

///////////////////////////////////////////////////////////
HashMap<float, void*> Physics::s_capsuleShapes;

///////////////////////////////////////////////////////////
HashMap<void*, Physics::ConcaveMeshData> Physics::s_concaveMeshShapes;

///////////////////////////////////////////////////////////
HashMap<void*, Physics::ConvexMeshData> Physics::s_convexMeshShapes;

///////////////////////////////////////////////////////////
HashMap<float*, void*> Physics::s_heightMapShapes;

///////////////////////////////////////////////////////////
HashMap<float, void*> Physics::s_sphereShapes;


///////////////////////////////////////////////////////////
Physics::Physics(Scene* scene) :
	Extension		(scene),
	m_world			(g_common.createPhysicsWorld())
{
	reactphysics3d::PhysicsWorld* world = WORLD_CAST(m_world);

	// Add event listeners
	scene->addListener<E_EntitiesCreated>(
		[&, world](const E_EntitiesCreated& e)
		{
			if (e.m_entities->has<RigidBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
				{
					// Create a new rigid body
					reactphysics3d::RigidBody* body = world->createRigidBody(reactphysics3d::Transform::identity());

					// Add data to map
					Entity::Id id = e.m_entities[i].getId();
					std::vector<GroupedBodyData>& group = m_groupedRigidBodies[id.m_group];
					m_rigidBodies[id] = BodyData{ body, id.m_group, group.size() };
					group.push_back(GroupedBodyData{ id, body, Vector3f(0.0f), Quaternion(), RigidBodyType::Dynamic, false });
				}
			}
		}
	);

	scene->addListener<E_EntitiesRemoved>(
		[&, world](const E_EntitiesRemoved& e)
		{
			if (e.m_entities->has<RigidBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
				{
					Entity::Id id = e.m_entities[i].getId();
					std::vector<GroupedBodyData>& group = m_groupedRigidBodies[id.m_group];

					// Destroy the rigid body
					auto it = m_rigidBodies.find(id);
					const BodyData& data = it.value();
					world->destroyRigidBody(RBODY_CAST(data.m_body));

					if (group.size() > 1)
					{
						// Remove the entry from the grouped map by moving the last entry into its spot
						group[data.m_index] = group.back();

						// Update the moved entry's index
						m_rigidBodies[group[data.m_index].m_id].m_index = data.m_index;
					}

					// Remove the last entry
					group.pop_back();

					// Remove the entry from the nongrouped map
					m_rigidBodies.erase(it);
				}
			}
		}
	);
}


///////////////////////////////////////////////////////////
Physics::~Physics()
{
	g_common.destroyPhysicsWorld(WORLD_CAST(m_world));
}


///////////////////////////////////////////////////////////
void Physics::update(float dt)
{
	START_PROFILING_FUNC;

	reactphysics3d::PhysicsWorld* world = WORLD_CAST(m_world);

	// Before doing the physics update, copy all component data into the physics engine
	reactphysics3d::CollisionBodyComponents& cbodyComponents = world->mCollisionBodyComponents;
	reactphysics3d::RigidBodyComponents& rbodyComponents = world->mRigidBodyComponents;
	reactphysics3d::TransformComponents& tComponents = world->mTransformComponents;

	std::vector<GroupedBodyData>* group = 0;
	Uint32 groupId = 0;
	Uint32 localIndex = 0;

	START_PROFILING(copyToEngine);

	// Rigid bodies
	m_scene->system<RigidBodyComponent>(
		[&](const Entity::Id& id, RigidBodyComponent& body)
		{
			// Change group if changed
			if (id.m_group != groupId)
			{
				groupId = id.m_group;
				group = &m_groupedRigidBodies[groupId];
				localIndex = 0;
			}

			GroupedBodyData& data = group->at(localIndex);

			// Get indices
			reactphysics3d::Entity entity = RBODY_CAST(data.m_body)->getEntity();
			Uint32 bodyIndex = rbodyComponents.mMapEntityToComponentIndex[entity];
			Uint32 transformIndex = tComponents.mMapEntityToComponentIndex[entity];

			// Inverse mass
			float invMass = body.m_mass == 0.0f ? INFINITY : 1.0f / body.m_mass;
			if (data.m_type != RigidBodyType::Dynamic)
				invMass = 0.0f;

			// Change properties based on body type
			if (body.m_type != data.m_type)
			{
				reactphysics3d::BodyType type = reactphysics3d::BodyType::DYNAMIC;
				if (body.m_type == RigidBodyType::Static)
				{
					body.m_linearVelocity = Vector3f(0.0f);
					body.m_angularVelocity = Vector3f(0.0f);
					body.m_mass = INFINITY;
					invMass = 0.0f;

					type = reactphysics3d::BodyType::STATIC;
				}
				else if (body.m_type == RigidBodyType::Kinematic)
				{
					body.m_mass = INFINITY;
					invMass = 0.0f;

					type = reactphysics3d::BodyType::KINEMATIC;
				}

				// Call the function
				RBODY_CAST(data.m_body)->setType(type);

				// Update new body type
				data.m_type = body.m_type;
			}

			// Check if transform changed
			if (body.m_position != data.m_position || body.m_rotation != data.m_rotation)
			{
				reactphysics3d::Transform transform(RP3D_VEC3(body.m_position), RP3D_QUAT(body.m_rotation));
				reactphysics3d::Vector3 oldCenterOfMass = rbodyComponents.mCentersOfMassWorld[bodyIndex];
				reactphysics3d::Vector3 newCenterOfMass = transform * rbodyComponents.mCentersOfMassLocal[bodyIndex];
				reactphysics3d::Vector3 diff = newCenterOfMass - oldCenterOfMass;

				// Set transform
				tComponents.mTransforms[transformIndex] = transform;

				// Set center of mass
				rbodyComponents.mCentersOfMassWorld[bodyIndex] = newCenterOfMass;

				// Set new linear velocity
				body.m_linearVelocity += cross(body.m_angularVelocity, POLY_VEC3(diff));

				// Update broadphase state if transform was changed outside the engine
				RBODY_CAST(data.m_body)->updateBroadPhaseState(0);

				// Awaken if the body was asleep
				if (data.m_isSleeping)
					RBODY_CAST(data.m_body)->setIsSleeping(false);
			}

			// Check if the body needs to be awakened
			if (data.m_isSleeping)
			{
				float linVelocitySquared = dot(body.m_linearVelocity, body.m_linearVelocity);
				float angVelocitySquared = dot(body.m_angularVelocity, body.m_angularVelocity);
				float forceSquared = dot(body.m_force, body.m_force);
				float torqueSquared = dot(body.m_torque, body.m_torque);

				if (linVelocitySquared > 0.0f || angVelocitySquared > 0.0f || forceSquared > 0.0f || torqueSquared > 0.0f)
					RBODY_CAST(data.m_body)->setIsSleeping(false);
			}

			// Copy values
			if (data.m_type == RigidBodyType::Dynamic)
				rbodyComponents.mMasses[bodyIndex] = body.m_mass;
			rbodyComponents.mInverseMasses[bodyIndex] = invMass;
			rbodyComponents.mLinearVelocities[bodyIndex] = RP3D_VEC3(body.m_linearVelocity);
			rbodyComponents.mAngularVelocities[bodyIndex] = RP3D_VEC3(body.m_angularVelocity);
			rbodyComponents.mLinearDampings[bodyIndex] = body.m_linearDamping;
			rbodyComponents.mAngularDampings[bodyIndex] = body.m_angularDamping;
			rbodyComponents.mExternalForces[bodyIndex] = RP3D_VEC3(body.m_force);
			rbodyComponents.mExternalTorques[bodyIndex] = RP3D_VEC3(body.m_torque);

			++localIndex;
		}
	);

	STOP_PROFILING(copyToEngine);


	// Do the physics update
	world->update(dt);


	// Copy the data back from the physics engine
	START_PROFILING(copyFromEngine);

	// Rigid bodies
	group = 0;
	groupId = 0;
	localIndex = 0;
	m_scene->system<RigidBodyComponent>(
		[&](const Entity::Id& id, RigidBodyComponent& body)
		{
			// Change group if changed
			if (id.m_group != groupId)
			{
				groupId = id.m_group;
				group = &m_groupedRigidBodies[groupId];
				localIndex = 0;
			}

			GroupedBodyData& data = group->at(localIndex);

			// Get indices
			reactphysics3d::Entity entity = RBODY_CAST(data.m_body)->getEntity();
			Uint32 bodyIndex = rbodyComponents.mMapEntityToComponentIndex[entity];
			Uint32 transformIndex = tComponents.mMapEntityToComponentIndex[entity];

			// Transform
			reactphysics3d::Transform& transform = tComponents.mTransforms[transformIndex];

			// Copy data
			body.m_position = POLY_VEC3(transform.getPosition());
			body.m_rotation = POLY_QUAT(transform.getOrientation());
			body.m_mass = rbodyComponents.mMasses[bodyIndex];
			body.m_linearVelocity = POLY_VEC3(rbodyComponents.mLinearVelocities[bodyIndex]);
			body.m_angularVelocity = POLY_VEC3(rbodyComponents.mAngularVelocities[bodyIndex]);
			body.m_centerOfMass = POLY_VEC3(rbodyComponents.mCentersOfMassLocal[bodyIndex]);
			body.m_linearDamping = rbodyComponents.mLinearDampings[bodyIndex];
			body.m_angularDamping = rbodyComponents.mAngularDampings[bodyIndex];

			// Forces are reset to zero
			body.m_force = Vector3f(0.0f);
			body.m_torque = Vector3f(0.0f);

			data.m_position = body.m_position;
			data.m_rotation = body.m_rotation;
			data.m_isSleeping = rbodyComponents.mIsSleeping[bodyIndex];

			++localIndex;
		}
	);

	STOP_PROFILING(copyFromEngine);
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const BoxCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::BoxShape* shape = (reactphysics3d::BoxShape*)getBoxShape(collider.getDimensions());

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	const Vector3f& pos = collider.getPosition();
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const CapsuleCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::CapsuleShape* shape =
		(reactphysics3d::CapsuleShape*)getCapsuleShape(Vector2f(collider.getRadius(), collider.getHeight()));

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	const Vector3f& pos = collider.getPosition();
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const ConcaveMeshCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::ConcaveMeshShape* shape = (reactphysics3d::ConcaveMeshShape*)getConcaveMeshShape(collider);

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	const Vector3f& pos = collider.getPosition();
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const ConvexMeshCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::ConvexMeshShape* shape = (reactphysics3d::ConvexMeshShape*)getConvexMeshShape(collider);

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	const Vector3f& pos = collider.getPosition();
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const HeightMapCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Calculate scale
	const Vector3f& dims = collider.getDimensions();
	Vector3f scale(dims.x / collider.getMapSize().x, dims.y, dims.z / collider.getMapSize().y);

	// Create a height field collider
	reactphysics3d::HeightFieldShape* shape = (reactphysics3d::HeightFieldShape*)getHeightMapShape(collider);

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	Vector3f pos = collider.getPosition();
	pos.y += 0.5f * dims.y;
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	reactphysics3d::Collider* c = body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();

	c->getMaterial().setFrictionCoefficient(1.0f);
}


///////////////////////////////////////////////////////////
void Physics::addCollider(const Entity& entity, const SphereCollider& collider)
{
	ASSERT(entity.has<RigidBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::SphereShape* shape = (reactphysics3d::SphereShape*)getSphereShape(collider.getRadius());

	// Get rigid body
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)m_rigidBodies[entity.getId()].m_body;
	const Vector3f& pos = collider.getPosition();
	const Quaternion& rot = collider.getRotation();
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Add the collider
	body->addCollider(shape, transform);
	body->updateMassPropertiesFromColliders();
}


///////////////////////////////////////////////////////////
void* Physics::getBoxShape(const Vector3f& dims)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	// Use half dimensions
	Vector3f halfDims = 0.5f * dims;
	float hash = dot(Vector3f(0.5f, 1.0f, 2.0f), dims);

	auto it = s_boxShapes.find(hash);
	if (it == s_boxShapes.end())
		shape = s_boxShapes[hash] = g_common.createBoxShape(RP3D_VEC3(halfDims));
	else
		shape = it.value();

	return shape;
}


///////////////////////////////////////////////////////////
void* Physics::getCapsuleShape(const Vector2f& dims)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	float hash = dot(Vector2f(0.5f, 1.0f), dims);

	auto it = s_capsuleShapes.find(hash);
	if (it == s_capsuleShapes.end())
		shape = s_capsuleShapes[hash] = g_common.createCapsuleShape(dims.x, dims.y);
	else
		shape = it.value();

	return shape;
}


///////////////////////////////////////////////////////////
void* Physics::getConcaveMeshShape(const ConcaveMeshCollider& collider)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	void* ptr = collider.getIndices() ? (void*)collider.getIndices() : (void*)collider.getVertices();

	auto it = s_concaveMeshShapes.find(ptr);
	if (it == s_concaveMeshShapes.end())
	{
		ConcaveMeshData& data = s_concaveMeshShapes[ptr];

		// If there are no indices, allocate a new array
		data.m_indices = collider.getIndices();
		if (!data.m_indices)
		{
			data.m_indices = (Uint32*)MALLOC_DBG(collider.getNumVertices() * sizeof(Uint32));
			for (Uint32 i = 0; i < collider.getNumVertices(); ++i)
				data.m_indices[i] = i;
		}

		// Create vertex array
		reactphysics3d::TriangleVertexArray* vertexArray = new reactphysics3d::TriangleVertexArray(
			collider.getNumUniqueVertices(),
			collider.getVertices(),
			sizeof(Vertex),
			collider.getNumVertices() / 3,
			data.m_indices,
			sizeof(Uint32),
			reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
		);

		// Create mesh
		reactphysics3d::TriangleMesh* mesh = g_common.createTriangleMesh();
		mesh->addSubpart(vertexArray);

		// Create shape
		data.m_shape = shape = g_common.createConcaveMeshShape(mesh);
	}
	else
		shape = it.value().m_shape;

	return shape;
}


///////////////////////////////////////////////////////////
void* Physics::getConvexMeshShape(const ConvexMeshCollider& collider)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	void* ptr = collider.getIndices() ? (void*)collider.getIndices() : (void*)collider.getVertices();

	auto it = s_convexMeshShapes.find(ptr);
	if (it == s_convexMeshShapes.end())
	{
		ConvexMeshData& data = s_convexMeshShapes[ptr];

		// If there are no indices, allocate a new array
		data.m_indices = collider.getIndices();
		if (!data.m_indices)
		{
			data.m_indices = (Uint32*)MALLOC_DBG(collider.getNumVertices() * sizeof(Uint32));
			for (Uint32 i = 0; i < collider.getNumVertices(); ++i)
				data.m_indices[i] = i;
		}

		// Create the faces array
		Uint32 numFaces = collider.getNumVertices() / 3;
		reactphysics3d::PolygonVertexArray::PolygonFace* faces =
			(reactphysics3d::PolygonVertexArray::PolygonFace*)MALLOC_DBG(
				numFaces * sizeof(reactphysics3d::PolygonVertexArray::PolygonFace)
			);
		data.m_faces = faces;

		// Set face data
		for (Uint32 f = 0; f < numFaces; ++f)
		{
			faces[f].indexBase = f * 3;
			faces[f].nbVertices = 3;
		}

		// Create vertex array
		reactphysics3d::PolygonVertexArray* vertexArray = new reactphysics3d::PolygonVertexArray(
			collider.getNumUniqueVertices(),
			collider.getVertices(),
			sizeof(Vertex),
			data.m_indices,
			sizeof(Uint32),
			numFaces,
			faces,
			reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
		);
		data.m_vertexArray = vertexArray;

		// Create mesh
		reactphysics3d::PolyhedronMesh* mesh = g_common.createPolyhedronMesh(vertexArray);

		// Create shape
		data.m_shape = shape = g_common.createConvexMeshShape(mesh);
	}
	else
		shape = it.value().m_shape;

	return shape;
}


///////////////////////////////////////////////////////////
void* Physics::getHeightMapShape(const HeightMapCollider& collider)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	auto it = s_heightMapShapes.find(collider.getHeightData());
	if (it == s_heightMapShapes.end())
	{
		// Calculate scale
		const Vector3f& dims = collider.getDimensions();
		Vector3f scale(dims.x / collider.getMapSize().x, dims.y, dims.z / collider.getMapSize().y);

		shape = s_heightMapShapes[collider.getHeightData()] = g_common.createHeightFieldShape(
			collider.getMapSize().x,
			collider.getMapSize().y,
			0.0f,
			1.0f,
			collider.getHeightData(),
			reactphysics3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE,
			1,
			1.0f,
			RP3D_VEC3(scale)
		);
	}
	else
		shape = it.value();

	return shape;
}


///////////////////////////////////////////////////////////
void* Physics::getSphereShape(float radius)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	auto it = s_sphereShapes.find(radius);
	if (it == s_sphereShapes.end())
		shape = s_sphereShapes[radius] = g_common.createSphereShape(radius);
	else
		shape = it.value();

	return shape;
}


///////////////////////////////////////////////////////////
Physics::ConcaveMeshData::ConcaveMeshData() :
	m_shape			(0),
	m_indices		(0),
	m_vertexArray	(0)
{

}


///////////////////////////////////////////////////////////
Physics::ConcaveMeshData::~ConcaveMeshData()
{
	if (m_indices)
		FREE_DBG(m_indices);

	if (m_vertexArray)
		delete reinterpret_cast<reactphysics3d::TriangleVertexArray*>(m_vertexArray);

	m_indices = 0;
	m_vertexArray = 0;
}


///////////////////////////////////////////////////////////
Physics::ConvexMeshData::ConvexMeshData() :
	m_shape			(0),
	m_indices		(0),
	m_faces			(0),
	m_vertexArray	(0)
{

}


///////////////////////////////////////////////////////////
Physics::ConvexMeshData::~ConvexMeshData()
{
	if (m_indices)
		FREE_DBG(m_indices);

	if (m_faces)
		FREE_DBG(m_faces);

	if (m_vertexArray)
		delete reinterpret_cast<reactphysics3d::PolygonVertexArray*>(m_vertexArray);

	m_indices = 0;
	m_faces = 0;
	m_vertexArray = 0;
}


}