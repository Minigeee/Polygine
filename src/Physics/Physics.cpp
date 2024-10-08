#include <poly/Core/Allocate.h>
#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Model.h>

#include <poly/Math/Functions.h>

#include <poly/Physics/Components.h>
#include <poly/Physics/Events.h>
#include <poly/Physics/Physics.h>

#include <poly/Graphics/Shaders/debug_physics.vert.h>
#include <poly/Graphics/Shaders/debug_physics.frag.h>

#include <reactphysics3d/reactphysics3d.h>

#define WORLD_CAST(x) reinterpret_cast<reactphysics3d::PhysicsWorld*>(x)
#define RBODY_CAST(x) reinterpret_cast<reactphysics3d::RigidBody*>(x)
#define CBODY_CAST(x) reinterpret_cast<reactphysics3d::CollisionBody*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
class PhysicsEventHandler :
	public reactphysics3d::EventListener,
	public reactphysics3d::RaycastCallback
{
public:
	///////////////////////////////////////////////////////////
	PhysicsEventHandler(Physics* physics, Scene* scene) :
		m_physics(physics),
		m_scene(scene),
		m_contactPoints(10)
	{ }


	///////////////////////////////////////////////////////////
	void onContact(const reactphysics3d::CollisionCallback::CallbackData& data) override
	{
		for (Uint32 i = 0; i < data.getNbContactPairs(); ++i)
		{
			auto contactPair = data.getContactPair(i);

			// Only handle start and end events
			if (contactPair.getEventType() == reactphysics3d::CollisionCallback::ContactPair::EventType::ContactStay)
				continue;

			// Copy all contact points into list
			for (Uint32 j = 0; j < contactPair.getNbContactPoints(); ++j)
			{
				auto contactPoint = contactPair.getContactPoint(j);
				const reactphysics3d::Vector3& n = contactPoint.getWorldNormal();
				const reactphysics3d::Vector3& p1 = contactPoint.getLocalPointOnCollider1();
				const reactphysics3d::Vector3& p2 = contactPoint.getLocalPointOnCollider2();

				poly::ContactPoint point = poly::ContactPoint{
					contactPoint.getPenetrationDepth(),
					POLY_VEC3(n),
					POLY_VEC3(p1),
					POLY_VEC3(p2)
				};

				if (j >= m_contactPoints.size())
					m_contactPoints.push_back(point);
				else
					m_contactPoints[j] = point;
			}

			// Create the event object
			E_PhysicsCollision e;

			// Get the associated entity ids
			e.m_entities[0] = m_physics->m_mapBodyToEntity[contactPair.getBody1()];
			e.m_entities[1] = m_physics->m_mapBodyToEntity[contactPair.getBody2()];

			// Create the collider objects
			reactphysics3d::Collider* c1 = contactPair.getCollider1();
			reactphysics3d::Collider* c2 = contactPair.getCollider2();

			e.m_colliders[0].m_collider = c1;
			e.m_colliders[0].m_material = &c1->getMaterial();
			e.m_colliders[0].m_shape = c1->getCollisionShape();

			e.m_colliders[1].m_collider = c2;
			e.m_colliders[1].m_material = &c2->getMaterial();
			e.m_colliders[1].m_shape = c2->getCollisionShape();

			// Set contact point info
			e.m_numContacts = contactPair.getNbContactPoints();
			e.m_contacts = &m_contactPoints[0];

			// Set event type
			if (contactPair.getEventType() == reactphysics3d::CollisionCallback::ContactPair::EventType::ContactStart)
				e.m_type = CollisionEventType::Start;
			else
				e.m_type = CollisionEventType::End;

			// Send the event
			m_scene->sendEvent<E_PhysicsCollision>(e);
		}
	}


	///////////////////////////////////////////////////////////
	void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& data) override
	{
		for (Uint32 i = 0; i < data.getNbOverlappingPairs(); ++i)
		{
			auto overlapPair = data.getOverlappingPair(i);

			// Only handle start and end events
			if (overlapPair.getEventType() == reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStay)
				continue;

			// Create the event object
			E_PhysicsTrigger e;

			// Get the associated entity ids
			e.m_entities[0] = m_physics->m_mapBodyToEntity[overlapPair.getBody1()];
			e.m_entities[1] = m_physics->m_mapBodyToEntity[overlapPair.getBody2()];

			// Create the collider objects
			reactphysics3d::Collider* c1 = overlapPair.getCollider1();
			reactphysics3d::Collider* c2 = overlapPair.getCollider2();

			e.m_colliders[0].m_collider = c1;
			e.m_colliders[0].m_material = &c1->getMaterial();
			e.m_colliders[0].m_shape = c1->getCollisionShape();

			e.m_colliders[1].m_collider = c2;
			e.m_colliders[1].m_material = &c2->getMaterial();
			e.m_colliders[1].m_shape = c2->getCollisionShape();

			// Set event type
			if (overlapPair.getEventType() == reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
				e.m_type = CollisionEventType::Start;
			else
				e.m_type = CollisionEventType::End;

			// Send the event
			m_scene->sendEvent<E_PhysicsTrigger>(e);
		}
	}


	///////////////////////////////////////////////////////////
	float notifyRaycastHit(const reactphysics3d::RaycastInfo& rp3dInfo) override
	{
		poly::RaycastInfo info;

		// Get the associated entity id
		info.m_entity = m_physics->m_mapBodyToEntity[rp3dInfo.body];

		// Create collider object
		reactphysics3d::Collider* c = rp3dInfo.collider;
		info.m_collider.m_collider = c;
		info.m_collider.m_material = &c->getMaterial();
		info.m_collider.m_shape = c->getCollisionShape();

		// Copy scalar and vector properties
		info.m_point = POLY_VEC3(rp3dInfo.worldPoint);
		info.m_normal = POLY_VEC3(rp3dInfo.worldNormal);
		info.m_fraction = rp3dInfo.hitFraction;

		// Add to physics object list
		m_physics->m_raycastInfo.push_back(info);

		// Determine if raycast should continue
		float ret = 1.0f;
		Uint32 max = m_physics->m_maxRaycastIntersects;
		if (max > 0)
			ret = ++m_physics->m_numRaycastIntersects >= max ? 0.0f : 1.0f;

		return ret;
	}


private:
	Physics* m_physics;
	Scene* m_scene;
	std::vector<poly::ContactPoint> m_contactPoints;
};


}
#endif


///////////////////////////////////////////////////////////
reactphysics3d::PhysicsCommon g_common;

///////////////////////////////////////////////////////////
HashMap<void*, Physics::ConcaveMeshData> Physics::s_concaveMeshShapes;

///////////////////////////////////////////////////////////
HashMap<void*, Physics::ConvexMeshData> Physics::s_convexMeshShapes;

///////////////////////////////////////////////////////////
HashMap<float*, void*> Physics::s_heightMapShapes;

///////////////////////////////////////////////////////////
Shader Physics::s_debugShader;


///////////////////////////////////////////////////////////
Physics::RigidBodyData::RigidBodyData(const Entity::Id& id, void* body) :
	m_id				(id),
	m_body				(body),
	m_position			(0.0f),
	m_linearVelocity	(0.0f),
	m_angularVelocity	(0.0f),
	m_type				(RigidBodyType::Dynamic),
	m_allowedSleep		(true),
	m_isSleeping		(false)
{

}


///////////////////////////////////////////////////////////
Physics::Physics(Scene* scene) :
	Extension				(scene),
	m_world					(g_common.createPhysicsWorld()),
	m_eventHandler			(new priv::PhysicsEventHandler(this, scene)),
	m_debugBufferOffset		(0)
{
	setDebugRenderEnabled(false);

	// Add all current rigid and collision bodies
	scene->system<RigidBodyComponent>(
		[&](const Entity::Id& id, RigidBodyComponent& body)
		{
			// Lock mutex
			std::unique_lock<std::mutex> lock1(m_mutex);
			std::unique_lock<std::mutex> lock2(m_dataMutex);

			addRigidBody(id);
		}
	);
	scene->system<CollisionBodyComponent>(
		[&](const Entity::Id& id, CollisionBodyComponent& body)
		{
			// Lock mutex
			std::unique_lock<std::mutex> lock1(m_mutex);
			std::unique_lock<std::mutex> lock2(m_dataMutex);

			addCollisionBody(id);
		}
	);

	// Add event listeners
	scene->addListener<E_EntitiesCreated>(
		[&](const E_EntitiesCreated& e)
		{
			// Lock mutex
			std::unique_lock<std::mutex> lock1(m_mutex);
			std::unique_lock<std::mutex> lock2(m_dataMutex);

			if (e.m_entities->has<RigidBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
					addRigidBody(e.m_entities[i].getId());
			}
			else if (e.m_entities->has<CollisionBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
					addCollisionBody(e.m_entities[i].getId());
			}
		}
	);

	scene->addListener<E_EntitiesRemoved>(
		[&](const E_EntitiesRemoved& e)
		{
			// Lock mutex
			std::unique_lock<std::mutex> lock1(m_mutex);
			std::unique_lock<std::mutex> lock2(m_dataMutex);

			if (e.m_entities->has<RigidBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
					removeRigidBody(e.m_entities[i].getId());
			}
			else if (e.m_entities->has<CollisionBodyComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
					removeCollisionBody(e.m_entities[i].getId());
			}
		}
	);

	// Set event listener
	WORLD_CAST(m_world)->setEventListener(m_eventHandler);
}


///////////////////////////////////////////////////////////
Physics::~Physics()
{
	g_common.destroyPhysicsWorld(WORLD_CAST(m_world));
	delete m_eventHandler;
}


///////////////////////////////////////////////////////////
void Physics::addRigidBody(Entity::Id id)
{
	// Create a new rigid body
	reactphysics3d::RigidBody* body = WORLD_CAST(m_world)->createRigidBody(reactphysics3d::Transform::identity());

	// Add data to map
	std::vector<RigidBodyData>& group = m_groupedRigidBodies[id.m_group];
	m_rigidBodies[id] = BodyData{ body, id.m_group, group.size() };
	group.push_back(RigidBodyData(id, body));

	// Map id to body
	m_mapBodyToEntity[(reactphysics3d::CollisionBody*)body] = id;
}


///////////////////////////////////////////////////////////
void Physics::addCollisionBody(Entity::Id id)
{
	// Create a new collision body
	reactphysics3d::CollisionBody* body = WORLD_CAST(m_world)->createCollisionBody(reactphysics3d::Transform::identity());

	// Add data to map
	std::vector<CollisionBodyData>& group = m_groupedCollisionBodies[id.m_group];
	m_collisionBodies[id] = BodyData{ body, id.m_group, group.size() };
	group.push_back(CollisionBodyData{ id, body });

	// Map id to body
	m_mapBodyToEntity[(reactphysics3d::CollisionBody*)body] = id;
}


///////////////////////////////////////////////////////////
void Physics::removeRigidBody(Entity::Id id)
{
	std::vector<RigidBodyData>& group = m_groupedRigidBodies[id.m_group];

	// Destroy the rigid body
	auto it = m_rigidBodies.find(id);
	const BodyData& data = it.value();
	WORLD_CAST(m_world)->destroyRigidBody(RBODY_CAST(data.m_body));

	// Remove body to entity mapping
	m_mapBodyToEntity.erase((reactphysics3d::CollisionBody*)RBODY_CAST(data.m_body));

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


///////////////////////////////////////////////////////////
void Physics::removeCollisionBody(Entity::Id id)
{
	std::vector<CollisionBodyData>& group = m_groupedCollisionBodies[id.m_group];

	// Destroy the collision body
	auto it = m_collisionBodies.find(id);
	const BodyData& data = it.value();
	WORLD_CAST(m_world)->destroyCollisionBody(CBODY_CAST(data.m_body));

	// Remove body to entity mapping
	m_mapBodyToEntity.erase(CBODY_CAST(data.m_body));

	if (group.size() > 1)
	{
		// Remove the entry from the grouped map by moving the last entry into its spot
		group[data.m_index] = group.back();

		// Update the moved entry's index
		m_collisionBodies[group[data.m_index].m_id].m_index = data.m_index;
	}

	// Remove the last entry
	group.pop_back();

	// Remove the entry from the nongrouped map
	m_collisionBodies.erase(it);
}


///////////////////////////////////////////////////////////
std::unique_lock<std::mutex> Physics::lock()
{
	return std::unique_lock<std::mutex>(m_mutex);
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

	std::vector<RigidBodyData>* group = 0;
	Uint32 groupId = 0;
	Uint32 localIndex = 0;

	// Rigid bodies
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

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

			RigidBodyData& data = group->at(localIndex);

			// Get indices
			reactphysics3d::Entity entity = RBODY_CAST(data.m_body)->getEntity();
			Uint32 bodyIndex = rbodyComponents.mMapEntityToComponentIndex[entity];
			Uint32 transformIndex = tComponents.mMapEntityToComponentIndex[entity];

			// Inverse mass
			float invMass = body.m_mass == 0.0f ? INFINITY : 1.0f / body.m_mass;
			if (data.m_type != RigidBodyType::Dynamic)
				invMass = 0.0f;

			// Inverse inertia tensor
			Vector3f invInertiaTensor(
				body.m_inertiaTensor.x == INFINITY ? 0.0f : 1.0f / body.m_inertiaTensor.x,
				body.m_inertiaTensor.y == INFINITY ? 0.0f : 1.0f / body.m_inertiaTensor.y,
				body.m_inertiaTensor.z == INFINITY ? 0.0f : 1.0f / body.m_inertiaTensor.z
			);

			// Change properties based on body type
			reactphysics3d::BodyType type = reactphysics3d::BodyType::DYNAMIC;
			if (body.m_type == RigidBodyType::Static)
			{
				body.m_linearVelocity = Vector3f(0.0f);
				body.m_angularVelocity = Vector3f(0.0f);
				invInertiaTensor = Vector3f(0.0f);
				invMass = 0.0f;

				type = reactphysics3d::BodyType::STATIC;
			}
			else if (body.m_type == RigidBodyType::Kinematic)
			{
				invInertiaTensor = Vector3f(0.0f);
				invMass = 0.0f;

				type = reactphysics3d::BodyType::KINEMATIC;
			}

			if (body.m_type != data.m_type)
			{
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
			data.m_isSleeping = rbodyComponents.mIsSleeping[bodyIndex];
			if (data.m_allowedSleep && data.m_isSleeping)
			{
				float linVelocitySquared = dot(body.m_linearVelocity, body.m_linearVelocity);
				float angVelocitySquared = dot(body.m_angularVelocity, body.m_angularVelocity);
				float forceSquared = dot(body.m_force, body.m_force);
				float torqueSquared = dot(body.m_torque, body.m_torque);

				if (linVelocitySquared > 0.0f || angVelocitySquared > 0.0f || forceSquared > 0.0f || torqueSquared > 0.0f)
					RBODY_CAST(data.m_body)->setIsSleeping(false);
			}

			// Update mass properties
			if (data.m_type == RigidBodyType::Dynamic)
			{
				if (data.m_massPropertiesUpdated)
					// Update center of mass if the properties were updated
					body.m_centerOfMass = POLY_VEC3(rbodyComponents.mCentersOfMassLocal[bodyIndex]);

				// Set mass
				rbodyComponents.mMasses[bodyIndex] = body.m_mass;

				// Update inertia tensor
				if (data.m_massPropertiesUpdated && body.m_inertiaTensor == Vector3f(1.0f))
					// Update the component inertia tensor if it wasn't modified externally
					body.m_inertiaTensor = POLY_VEC3(rbodyComponents.mLocalInertiaTensors[bodyIndex]);
				else
					// If it was modified, 
					rbodyComponents.mLocalInertiaTensors[bodyIndex] = RP3D_VEC3(body.m_inertiaTensor);

				data.m_massPropertiesUpdated = false;
			}

			// Copy values
			rbodyComponents.mInverseMasses[bodyIndex] = invMass;
			rbodyComponents.mInverseInertiaTensorsLocal[bodyIndex] = RP3D_VEC3(invInertiaTensor);
			rbodyComponents.mLinearVelocities[bodyIndex] = RP3D_VEC3(body.m_linearVelocity);
			rbodyComponents.mAngularVelocities[bodyIndex] = RP3D_VEC3(body.m_angularVelocity);
			rbodyComponents.mLinearDampings[bodyIndex] = body.m_linearDamping;
			rbodyComponents.mAngularDampings[bodyIndex] = body.m_angularDamping;
			rbodyComponents.mExternalForces[bodyIndex] = RP3D_VEC3(body.m_force);
			rbodyComponents.mExternalTorques[bodyIndex] = RP3D_VEC3(body.m_torque);

			// Cache certain properties
			data.m_position = body.m_position;
			data.m_rotation = body.m_rotation;
			data.m_linearVelocity = body.m_linearVelocity;
			data.m_angularVelocity = body.m_angularVelocity;

			// Forces are reset to zero
			body.m_force = Vector3f(0.0f);
			body.m_torque = Vector3f(0.0f);

			++localIndex;
		}
	);

	// Collision bodies
	std::vector<CollisionBodyData>* cbodyGroup = 0;
	groupId = 0;
	localIndex = 0;
	m_scene->system<CollisionBodyComponent>(
		[&](const Entity::Id& id, const CollisionBodyComponent& body)
		{
			// Change group if changed
			if (id.m_group != groupId)
			{
				groupId = id.m_group;
				cbodyGroup = &m_groupedCollisionBodies[groupId];
				localIndex = 0;
			}

			CollisionBodyData& data = cbodyGroup->at(localIndex);

			// Get indices
			reactphysics3d::Entity entity = CBODY_CAST(data.m_body)->getEntity();
			Uint32 transformIndex = tComponents.mMapEntityToComponentIndex[entity];

			// Collision body transforms will only be modified externally
			reactphysics3d::Transform transform(RP3D_VEC3(body.m_position), RP3D_QUAT(body.m_rotation));
			tComponents.mTransforms[transformIndex] = transform;
			RBODY_CAST(data.m_body)->updateBroadPhaseState(0);
		}
	);

	lock1.unlock();
	lock2.unlock();


	// Do the physics update
	lock2.lock();
	world->update(dt);
	lock2.unlock();


	// Copy the data back from the physics engine
	lock1.lock();
	lock2.lock();

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

			RigidBodyData& data = group->at(localIndex);

			// Get indices
			reactphysics3d::Entity entity = RBODY_CAST(data.m_body)->getEntity();
			Uint32 bodyIndex = rbodyComponents.mMapEntityToComponentIndex[entity];
			Uint32 transformIndex = tComponents.mMapEntityToComponentIndex[entity];

			// Transform
			reactphysics3d::Transform& transform = tComponents.mTransforms[transformIndex];

			// Copy data
			if (body.m_position == data.m_position)
				body.m_position = POLY_VEC3(transform.getPosition());
			if (body.m_rotation == data.m_rotation)
				body.m_rotation = POLY_QUAT(transform.getOrientation());
			if (body.m_linearVelocity == data.m_linearVelocity)
				body.m_linearVelocity = POLY_VEC3(rbodyComponents.mLinearVelocities[bodyIndex]);
			if (body.m_angularVelocity == data.m_angularVelocity)
				body.m_angularVelocity = POLY_VEC3(rbodyComponents.mAngularVelocities[bodyIndex]);

			++localIndex;
		}
	);
}


///////////////////////////////////////////////////////////
std::vector<RaycastInfo>& Physics::raycast(const Ray& ray, float dist, Uint16 mask, Uint32 maxIntersects)
{
	Vector3f end = ray.m_origin + dist * normalize(ray.m_direction);
	reactphysics3d::Ray rp3dRay(RP3D_VEC3(ray.m_origin), RP3D_VEC3(end));

	// Clear the raycast info list
	m_numRaycastIntersects = 0;
	m_maxRaycastIntersects = maxIntersects;
	m_raycastInfo.clear();

	{
		// Lock mutex
		std::unique_lock<std::mutex> lock(m_mutex);

		// Raycast test
		WORLD_CAST(m_world)->raycast(rp3dRay, m_eventHandler, mask);
	}

	return m_raycastInfo;
}


///////////////////////////////////////////////////////////
void Physics::setGravity(const Vector3f& gravity)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);

	m_gravity = gravity;
	WORLD_CAST(m_world)->setGravity(RP3D_VEC3(m_gravity));
}


///////////////////////////////////////////////////////////
void Physics::setGravity(float x, float y, float z)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);

	m_gravity = Vector3f(x, y, z);
	WORLD_CAST(m_world)->setGravity(RP3D_VEC3(m_gravity));
}


///////////////////////////////////////////////////////////
void Physics::setSleepAllowed(const Entity& entity, bool allowed)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock1(m_dataMutex);

	BodyData* data = 0;
	{
		auto it = m_rigidBodies.find(entity.getId());
		if (it != m_rigidBodies.end())
			data = &it.value();
		else
			return;
	}

	{
		// Lock mutex
		std::unique_lock<std::mutex> lock2(m_mutex);

		// Change option in engine
		reactphysics3d::RigidBody* body = RBODY_CAST(data->m_body);
		body->setIsAllowedToSleep(allowed);
	}

	// Update cache
	RigidBodyData& group = m_groupedRigidBodies[data->m_group][data->m_index];
	group.m_allowedSleep = allowed;
}


///////////////////////////////////////////////////////////
const Vector3f& Physics::getGravity() const
{
	return m_gravity;
}


///////////////////////////////////////////////////////////
void Physics::createCollider(Collider& collider, const Entity& entity, void* rp3dShape, const Vector3f& pos, const Quaternion& rot, Collider::Type type)
{
	// Create transform
	reactphysics3d::Transform transform(RP3D_VEC3(pos), RP3D_QUAT(rot));

	// Set type
	collider.m_type = type;

	if (entity.has<RigidBodyComponent>())
	{
		// Get rigid body
		BodyData& data = m_rigidBodies[entity.getId()];
		reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)data.m_body;

		// Add the collider
		reactphysics3d::Collider* rp3dCollider = body->addCollider((reactphysics3d::CollisionShape*)rp3dShape, transform);
		body->updateMassPropertiesFromColliders();

		// Setup poly collider
		collider.init(rp3dCollider);

		m_groupedRigidBodies[data.m_group][data.m_index].m_massPropertiesUpdated = true;

		// Add the collider to the list
		auto locks = lockComponents<RigidBodyComponent>();

		data.m_colliders.push_back(collider);
		RigidBodyComponent* component = entity.get<RigidBodyComponent>();
		component->m_colliders = &data.m_colliders[0];
		component->m_numColliders = data.m_colliders.size();
	}
	else
	{
		// Get collision body
		BodyData& data = m_collisionBodies[entity.getId()];
		reactphysics3d::CollisionBody* body = (reactphysics3d::CollisionBody*)data.m_body;

		// Add the collider
		reactphysics3d::Collider* rp3dCollider = body->addCollider((reactphysics3d::CollisionShape*)rp3dShape, transform);

		// Setup poly collider
		collider.init(rp3dCollider);

		// Add the collider to the list
		auto locks = lockComponents<CollisionBodyComponent>();

		data.m_colliders.push_back(collider);
		CollisionBodyComponent* component = entity.get<CollisionBodyComponent>();
		component->m_colliders = &data.m_colliders[0];
		component->m_numColliders = data.m_colliders.size();
	}
}


///////////////////////////////////////////////////////////
BoxCollider Physics::addCollider(const Entity& entity, const BoxShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::BoxShape* rp3dShape = (reactphysics3d::BoxShape*)createBoxShape(shape.m_dimensions);

	// Create collider
	BoxCollider collider;
	createCollider(collider, entity, rp3dShape, position, rotation, Collider::Box);

	return collider;
}


///////////////////////////////////////////////////////////
CapsuleCollider Physics::addCollider(const Entity& entity, const CapsuleShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::CapsuleShape* rp3dShape = (reactphysics3d::CapsuleShape*)createCapsuleShape(Vector2f(shape.m_radius, shape.m_height));

	// Create collider
	CapsuleCollider collider;
	createCollider(collider, entity, rp3dShape, position, rotation, Collider::Capsule);

	return collider;
}


///////////////////////////////////////////////////////////
ConcaveMeshCollider Physics::addCollider(const Entity& entity, const ConcaveMeshShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::ConcaveMeshShape* rp3dShape = (reactphysics3d::ConcaveMeshShape*)createConcaveMeshShape(shape);

	// Create collider
	ConcaveMeshCollider collider;
	createCollider(collider, entity, rp3dShape, position, rotation, Collider::ConcaveMesh);

	return collider;
}


///////////////////////////////////////////////////////////
ConvexMeshCollider Physics::addCollider(const Entity& entity, const ConvexMeshShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::ConvexMeshShape* rp3dShape = (reactphysics3d::ConvexMeshShape*)createConvexMeshShape(shape);

	// Create collider
	ConvexMeshCollider collider;
	createCollider(collider, entity, rp3dShape, position, rotation, Collider::ConvexMesh);

	return collider;
}


///////////////////////////////////////////////////////////
HeightMapCollider Physics::addCollider(const Entity& entity, const HeightMapShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::HeightFieldShape* rp3dShape = (reactphysics3d::HeightFieldShape*)createHeightMapShape(shape);
	Vector3f newPos(position);
	newPos.y += 0.5f * shape.m_dimensions.y;

	// Create collider
	HeightMapCollider collider;
	createCollider(collider, entity, rp3dShape, newPos, rotation, Collider::HeightMap);

	return collider;
}


///////////////////////////////////////////////////////////
SphereCollider Physics::addCollider(const Entity& entity, const SphereShape& shape, const Vector3f& position, const Quaternion& rotation)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	// Create shape
	reactphysics3d::HeightFieldShape* rp3dShape = (reactphysics3d::HeightFieldShape*)createSphereShape(shape.m_radius);

	// Create collider
	SphereCollider collider;
	createCollider(collider, entity, rp3dShape, position, rotation, Collider::Sphere);

	return collider;
}


///////////////////////////////////////////////////////////
void Physics::removeCollider(const Entity& entity, const Collider& collider)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock1(m_dataMutex);

	BodyData* data = 0;
	reactphysics3d::CollisionBody* body = 0;
	{
		auto it = m_rigidBodies.find(entity.getId());
		if (it != m_rigidBodies.end())
		{
			data = &it.value();
			body = dynamic_cast<reactphysics3d::CollisionBody*>(RBODY_CAST(data->m_body));

			// Remove it from list
			std::vector<Collider>& colliders = data->m_colliders;
			for (Uint32 i = 0; i < colliders.size(); ++i)
			{
				if (colliders[i].m_collider == collider.m_collider)
				{
					colliders.erase(colliders.begin() + i);

					// Update component data
					auto locks = lockComponents<RigidBodyComponent>();

					RigidBodyComponent* component = entity.get<RigidBodyComponent>();
					component->m_colliders = &colliders[0];
					component->m_numColliders = colliders.size();
				}
			}
		}
		else
		{
			it = m_collisionBodies.find(entity.getId());
			if (it != m_collisionBodies.end())
			{
				data = &it.value();
				body = CBODY_CAST(data->m_body);

				// Remove it from list
				std::vector<Collider>& colliders = data->m_colliders;
				for (Uint32 i = 0; i < colliders.size(); ++i)
				{
					if (colliders[i].m_collider == collider.m_collider)
					{
						colliders.erase(colliders.begin() + i);

						// Update component data
						auto locks = lockComponents<RigidBodyComponent>();

						CollisionBodyComponent* component = entity.get<CollisionBodyComponent>();
						component->m_colliders = &colliders[0];
						component->m_numColliders = colliders.size();
					}
				}
			}
			else
				return;
		}
	}

	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);

	// Remove the collider
	body->removeCollider(reinterpret_cast<reactphysics3d::Collider*>(collider.m_collider));
}


///////////////////////////////////////////////////////////
void* Physics::createBoxShape(const Vector3f& dims)
{
	// Create a new shape that will be shareable
	void* shape = 0;

	// Use half dimensions
	Vector3f halfDims = 0.5f * dims;
	return g_common.createBoxShape(RP3D_VEC3(halfDims));
}


///////////////////////////////////////////////////////////
void* Physics::createCapsuleShape(const Vector2f& dims)
{
	return g_common.createCapsuleShape(dims.x, dims.y);
}


///////////////////////////////////////////////////////////
void* Physics::createConcaveMeshShape(const ConcaveMeshShape& shape)
{
	// Create a new shape that will be shareable
	void* rp3d = 0;

	const std::vector<Vertex>& vertices = shape.m_model->getVertices();
	const std::vector<Uint32>& indices = shape.m_model->getIndices();
	bool usesIndices = indices.size() > 0;

	// Get the vertex offset and number of vertices
	Uint32 numMeshes = shape.m_model->getNumMeshes();
	Uint32 vertexOffset = shape.m_model->getMesh(shape.m_meshNum)->m_offset;
	Uint32 numVertices =
		shape.m_meshNum + 1 == numMeshes ?
		(usesIndices ? indices.size() : vertices.size()) :
		shape.m_model->getMesh(shape.m_meshNum + 1)->m_offset;
	numVertices -= vertexOffset;

	// Get the key pointer that will be used to create a map entry
	void* key = usesIndices ? (void*)&indices[vertexOffset] : (void*)&vertices[vertexOffset];

	auto it = s_concaveMeshShapes.find(key);
	if (it == s_concaveMeshShapes.end())
	{
		// Create a map entry
		ConcaveMeshData& data = s_concaveMeshShapes[key];

		// If there are no indices, allocate a new array
		if (!usesIndices)
		{
			data.m_indices = (Uint32*)MALLOC_DBG(numVertices * sizeof(Uint32));
			for (Uint32 i = 0; i < numVertices; ++i)
				data.m_indices[i] = i;
		}
		else
			data.m_indices = (Uint32*)&indices[vertexOffset];

		// Create vertex array
		reactphysics3d::TriangleVertexArray* vertexArray = new reactphysics3d::TriangleVertexArray(
			usesIndices ? vertices.size() : numVertices,
			usesIndices ? &vertices[0] : &vertices[vertexOffset],
			sizeof(Vertex),
			numVertices / 3,
			data.m_indices,
			sizeof(Uint32),
			reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
		);

		// Create mesh
		reactphysics3d::TriangleMesh* mesh = g_common.createTriangleMesh();
		mesh->addSubpart(vertexArray);

		// Create shape
		data.m_shape = rp3d = g_common.createConcaveMeshShape(mesh);
	}
	else
		rp3d = it.value().m_shape;

	return rp3d;
}


///////////////////////////////////////////////////////////
void* Physics::createHeightMapShape(const HeightMapShape& shape)
{
	// Create a new shape that will be shareable
	void* rp3d = 0;

	auto it = s_heightMapShapes.find(shape.m_data);
	if (it == s_heightMapShapes.end())
	{
		// Calculate scale
		const Vector3f& dims = shape.m_dimensions;
		Vector3f scale(dims.x / shape.m_imageSize.x, dims.y, dims.z / shape.m_imageSize.y);

		rp3d = s_heightMapShapes[shape.m_data] = g_common.createHeightFieldShape(
			shape.m_imageSize.x,
			shape.m_imageSize.y,
			0.0f,
			1.0f,
			shape.m_data,
			reactphysics3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE,
			1,
			1.0f,
			RP3D_VEC3(scale)
		);
	}
	else
		rp3d = it.value();

	return rp3d;
}


///////////////////////////////////////////////////////////
void* Physics::createSphereShape(float radius)
{
	return g_common.createSphereShape(radius);
}


///////////////////////////////////////////////////////////
Joint Physics::addJoint(const Entity& e1, const Entity& e2, Joint::Type type, const Vector3f& point, const Vector3f& axis)
{
	ASSERT(e1.has<RigidBodyComponent>() && e2.has<RigidBodyComponent>(), "Entities must have a RigidBodyComponent to be attached to joints");

	Joint joint;

	// Lock mutexes
	std::unique_lock<std::mutex> lock1(m_dataMutex);
	std::unique_lock<std::mutex> lock2(m_mutex);

	std::cout << "Lock mutex\n";

	// Get the two bodies
	reactphysics3d::RigidBody* b1 = RBODY_CAST(m_rigidBodies[e1.getId()].m_body);
	reactphysics3d::RigidBody* b2 = RBODY_CAST(m_rigidBodies[e2.getId()].m_body);
	lock1.unlock();

	if (type == Joint::BallAndSocket)
	{
		reactphysics3d::BallAndSocketJointInfo info(b1, b2, RP3D_VEC3(point));
		auto rp3d = dynamic_cast<reactphysics3d::BallAndSocketJoint*>(WORLD_CAST(m_world)->createJoint(info));
		joint.setJoint(rp3d, type);
	}
	else if (type == Joint::Hinge)
	{
		reactphysics3d::HingeJointInfo info(b1, b2, RP3D_VEC3(point), RP3D_VEC3(axis));
		auto rp3d = dynamic_cast<reactphysics3d::HingeJoint*>(WORLD_CAST(m_world)->createJoint(info));
		joint.setJoint(rp3d, type);
	}
	else if (type == Joint::Slider)
	{
		reactphysics3d::SliderJointInfo info(b1, b2, RP3D_VEC3(point), RP3D_VEC3(axis));
		auto rp3d = dynamic_cast<reactphysics3d::SliderJoint*>(WORLD_CAST(m_world)->createJoint(info));
		joint.setJoint(rp3d, type);
	}
	else
	{
		reactphysics3d::FixedJointInfo info(b1, b2, RP3D_VEC3(point));
		auto rp3d = dynamic_cast<reactphysics3d::FixedJoint*>(WORLD_CAST(m_world)->createJoint(info));
		joint.setJoint(rp3d, type);
	}

	return joint;
}


///////////////////////////////////////////////////////////
void Physics::removeJoint(const Joint& joint)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);

	if (joint.m_type == Joint::BallAndSocket)
	{
		auto rp3d = reinterpret_cast<reactphysics3d::BallAndSocketJoint*>(joint.m_joint);
		WORLD_CAST(m_world)->destroyJoint(rp3d);
	}
	else if (joint.m_type == Joint::Hinge)
	{
		auto rp3d = reinterpret_cast<reactphysics3d::HingeJoint*>(joint.m_joint);
		WORLD_CAST(m_world)->destroyJoint(rp3d);
	}
	else if (joint.m_type == Joint::Slider)
	{
		auto rp3d = reinterpret_cast<reactphysics3d::SliderJoint*>(joint.m_joint);
		WORLD_CAST(m_world)->destroyJoint(rp3d);
	}
	else
	{
		auto rp3d = reinterpret_cast<reactphysics3d::FixedJoint*>(joint.m_joint);
		WORLD_CAST(m_world)->destroyJoint(rp3d);
	}
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
	if (m_vertices)
		FREE_DBG(m_vertices);

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


#ifndef DOXYGEN_SKIP
///////////////////////////////////////////////////////////
namespace priv
{


///////////////////////////////////////////////////////////
struct ConvexMeshFace
{
	std::vector<Uint32> m_vertices;
	Vector3f m_normal;
};


///////////////////////////////////////////////////////////
struct ConvexMeshFaceInfo
{
	Uint32 m_index;
	Uint32 m_numVertices;
};


///////////////////////////////////////////////////////////
std::vector<Vector3f> mergeVertices(const std::vector<Vertex>& vertices, std::vector<Uint32>& indices)
{
	// The list vertices to return
	std::vector<Vector3f> merged;

	constexpr float mergeDist = 0.0001f;
	constexpr float mergeDistSquared = mergeDist * mergeDist;

	// This loop is O(n^2), so it is pretty inefficient, but is simple
	for (Uint32 v = 0; v < vertices.size(); ++v)
	{
		// Check if any of the added vertices are close enough
		// This uses a linear search
		int index = -1;
		for (Uint32 i = 0; i < merged.size(); ++i)
		{
			Vector3f offset = merged[i] - vertices[v].m_position;
			float distSquared = dot(offset, offset);

			// Check if the two vertices are close enough
			if (distSquared <= mergeDistSquared)
			{
				index = i;
				break;
			}
		}

		// If a close vertex was found, don't add the vertex to the merged list, and add its index instead
		if (index >= 0)
			indices.push_back((Uint32)index);

		// Otherwise add it as a new vertex
		else
		{
			merged.push_back(vertices[v].m_position);
			indices.push_back(merged.size() - 1);
		}
	}

	// Return the list of merged vertices
	return merged;
}


///////////////////////////////////////////////////////////
bool isSharedVertex(const Vector3i& shared, Uint32 vertex)
{
	return (int)vertex == shared.x || (int)vertex == shared.y || (int)vertex == shared.z;
}


///////////////////////////////////////////////////////////
std::vector<ConvexMeshFaceInfo> mergeFaces(std::vector<Vector3f>& vertices, std::vector<Uint32>& indices)
{
	// Keep track of which vertices belong to which faces
	std::vector<std::vector<Uint32>> mapVertexToFaces(vertices.size());

	// Create a list of triangle faces
	std::vector<ConvexMeshFace> faces;
	std::vector<bool> isFaceValid;
	std::vector<Uint32> mapFaceIndexCorrection;

	for (Uint32 v = 0; v < indices.size(); v += 3)
	{
		ConvexMeshFace face;

		// Normal
		Vector3f v0 = vertices[indices[v + 0]] - vertices[indices[v + 1]];
		Vector3f v1 = vertices[indices[v + 0]] - vertices[indices[v + 2]];
		face.m_normal = normalize(cross(v0, v1));

		// Vertices
		face.m_vertices.push_back(indices[v + 0]);
		face.m_vertices.push_back(indices[v + 1]);
		face.m_vertices.push_back(indices[v + 2]);

		// Map shared faces to shared vertices
		HashMap<Uint32, Vector3i> mapFaceToSharedVertices;
		for (Uint32 i = 0; i < 3; ++i)
		{
			std::vector<Uint32>& sharedFaces = mapVertexToFaces[indices[v + i]];

			// For every shared face, add the vertices the shared face shares with the current face
			for (Uint32 j = 0; j < sharedFaces.size(); ++j)
			{
				auto it = mapFaceToSharedVertices.find(sharedFaces[j]);

				// Each face can only share a maximum of 3 vertices with the new faces
				// because the new face is guaranteed to be a triangle with 3 vertices
				// So using a vector3 is ok in this situation where the max number of shared vertices is known
				if (it == mapFaceToSharedVertices.end())
					// Create a new mapping with the first one filled, and the rest as a negative number
					mapFaceToSharedVertices[sharedFaces[j]] = Vector3i(indices[v + i], -1, -1);
				else
				{
					if (it.value().y < 0)
						it.value().y = indices[v + i];
					else
						it.value().z = indices[v + i];
				}
			}
		}

		// If shared faces were found, check if the shared faces are coplanar
		HashMap<Uint32, bool> faceIsCoplanar;
		bool facesMerged = false;
		for (auto it = mapFaceToSharedVertices.begin(); it != mapFaceToSharedVertices.end(); ++it)
		{
			Uint32 faceIndex = mapFaceIndexCorrection[it->first];
			const Vector3i& sharedVertices = it->second;

			// If they only share a single vertex, they cannot be merged
			if (sharedVertices.y < 0)
			{
				faceIsCoplanar[faceIndex] = false;
				continue;
			}

			// If they share 3 vertices, they are coplanar
			Uint32 numShared = (sharedVertices.z >= 0 ? 3 : 2);
			bool coplanar = numShared == 3;

			constexpr float dotThreshold = 0.999998487f;
			coplanar |= (dot(faces[faceIndex].m_normal, face.m_normal) >= dotThreshold);
			faceIsCoplanar[faceIndex] = coplanar;
			facesMerged |= coplanar;

			// If they are coplanar, merge them
			if (coplanar)
			{
				std::vector<Uint32> newVertices;
				const ConvexMeshFace& coplanarFace = faces[faceIndex];

				// 2 shared and 3 shared are handled differently
				if (numShared == 2)
				{
					// 2 shared vertices
					Uint32 current = 0;
					Uint32 index = 0;
					const ConvexMeshFace* coplanarFaces[] = { &face, &coplanarFace };

					// Add the first vertex
					newVertices.push_back(coplanarFaces[current]->m_vertices[index]);

					// Add the vertices
					Uint32 nextIndex = (index + 1) % coplanarFaces[current]->m_vertices.size();
					while (newVertices.size() < coplanarFace.m_vertices.size() + 1)
					{
						Uint32 currentVertex = coplanarFaces[current]->m_vertices[index];

						// Check if need to switch face
						if (
							isSharedVertex(sharedVertices, coplanarFaces[current]->m_vertices[index]) &&
							isSharedVertex(sharedVertices, coplanarFaces[current]->m_vertices[nextIndex]))
						{
							current = (current + 1) % 2;

							// Find the index of the vertex
							const std::vector<Uint32>& currentVertices = coplanarFaces[current]->m_vertices;
							for (Uint32 i = 0; i < currentVertices.size(); ++i)
							{
								if (currentVertices[i] == currentVertex)
								{
									index = i;
									break;
								}
							}

							// Update next vertex
							nextIndex = (index + 1) % currentVertices.size();
						}

						// Update current and next index
						index = nextIndex;
						nextIndex = (index + 1) % coplanarFaces[current]->m_vertices.size();

						// Add the current vertex
						newVertices.push_back(coplanarFaces[current]->m_vertices[index]);
					}
				}
				else
				{
					// 3 shared vertices

					// Need to find the middle shared vertex on the coplanar face
					Uint32 middleIndex = 0;
					for (Uint32 i = 0; i < coplanarFace.m_vertices.size(); ++i)
					{
						Uint32 nextIndex = (i + 1) % coplanarFace.m_vertices.size();

						if (
							!isSharedVertex(sharedVertices, coplanarFace.m_vertices[i]) &&
							isSharedVertex(sharedVertices, coplanarFace.m_vertices[nextIndex]))
						{
							middleIndex = (nextIndex + 1) % coplanarFace.m_vertices.size();
							break;
						}
					}

					// Add every vertex in order except for the middle index
					for (Uint32 i = 0; i < coplanarFace.m_vertices.size(); ++i)
					{
						// Skip middle index
						if (i == middleIndex) continue;

						newVertices.push_back(coplanarFace.m_vertices[i]);
					}
				}

				// Replace the vertices of the current face
				face.m_vertices = newVertices;
			}
		}

		// If faces were merged, update every face that was merged
		if (facesMerged)
		{
			int newFaceIndex = -1;

			for (auto it = faceIsCoplanar.begin(); it != faceIsCoplanar.end(); ++it)
			{
				// Skip faces that weren't merged
				if (!it->second) continue;

				// No need to correct this face index because it is already corrected
				Uint32 faceIndex = it->first;

				if (newFaceIndex < 0)
				{
					// If the new face wasn't assigned yet, assign it
					faces[faceIndex] = face;

					// Update mappings
					for (Uint32 i = 0; i < face.m_vertices.size(); ++i)
					{
						// Get the mapped faces
						std::vector<Uint32>& mapping = mapVertexToFaces[face.m_vertices[i]];

						bool found = false;
						for (Uint32 j = 0; j < mapping.size(); ++j)
						{
							if (mapping[j] == faceIndex)
							{
								found = true;
								break;
							}
						}

						// If not found in the mapping, add it to the mapping
						if (!found)
							mapping.push_back(faceIndex);
					}

					// Update new face index
					newFaceIndex = faceIndex;
				}
				else
				{
					// Otherwise, invalidate the face
					isFaceValid[faceIndex] = false;

					// Add to mapping
					mapFaceIndexCorrection[faceIndex] = newFaceIndex;
				}
			}

			// Add to mapping
			isFaceValid.push_back(false);
			mapFaceIndexCorrection.push_back(newFaceIndex);
		}
		else
		{
			// Map vertices to face
			Uint32 faceIndex = v / 3;
			mapVertexToFaces[indices[v + 0]].push_back(faceIndex);
			mapVertexToFaces[indices[v + 1]].push_back(faceIndex);
			mapVertexToFaces[indices[v + 2]].push_back(faceIndex);

			// Add the face
			faces.push_back(std::move(face));

			// Mark this face as valid
			isFaceValid.push_back(true);
			mapFaceIndexCorrection.push_back(faces.size() - 1);
		}
	}

	// After the previous loop, should have a list of faces,
	// and a list of booleans indicating which ones are valid

	// Create the indices array, and the faces info array
	std::vector<ConvexMeshFaceInfo> info;
	indices.clear();

	for (Uint32 i = 0; i < isFaceValid.size(); ++i)
	{
		// Skip invalid faces
		if (!isFaceValid[i]) continue;

		const ConvexMeshFace& face = faces[mapFaceIndexCorrection[i]];

		// Add info
		info.push_back(ConvexMeshFaceInfo{ indices.size(), face.m_vertices.size() });

		// Add all indices
		for (Uint32 j = 0; j < face.m_vertices.size(); ++j)
			indices.push_back(face.m_vertices[j]);
	}

	return info;
}


}
#endif


///////////////////////////////////////////////////////////
void* Physics::createConvexMeshShape(const ConvexMeshShape& shape)
{
	// Create a new shape that will be shareable
	void* rp3d = 0;

	const std::vector<Vertex>& vertices = shape.m_model->getVertices();
	std::vector<Uint32> indices = shape.m_model->getIndices();
	bool usesIndices = indices.size() > 0;

	// Get the key pointer that will be used to create a map entry
	Uint32 vertexOffset = shape.m_model->getMesh(shape.m_meshNum)->m_offset;
	void* key = usesIndices ? (void*)&indices[vertexOffset] : (void*)&vertices[vertexOffset];

	auto it = s_convexMeshShapes.find(key);
	if (it == s_convexMeshShapes.end())
	{
		// Merge vertices and generate indices
		std::vector<Vector3f> merged;
		if (indices.size() == 0)
			merged = priv::mergeVertices(vertices, indices);
		else
		{
			merged.reserve(vertices.size());
			for (Uint32 i = 0; i < vertices.size(); ++i)
				merged.push_back(vertices[i].m_position);
		}

		// Merge coplanar faces and generate face info
		auto faceInfo = priv::mergeFaces(merged, indices);

		// Create a map entry
		ConvexMeshData& data = s_convexMeshShapes[key];

		// Allocate space for permenant vertex array
		data.m_vertices = (Vector3f*)MALLOC_DBG(merged.size() * sizeof(Vector3f));
		memcpy(data.m_vertices, &merged[0], merged.size() * sizeof(Vector3f));

		// Allocate space for permenant index array
		data.m_indices = (Uint32*)MALLOC_DBG(indices.size() * sizeof(Uint32));
		memcpy(data.m_indices, &indices[0], indices.size() * sizeof(Uint32));

		// Create the faces array
		reactphysics3d::PolygonVertexArray::PolygonFace* faces =
			(reactphysics3d::PolygonVertexArray::PolygonFace*)MALLOC_DBG(
				faceInfo.size() * sizeof(reactphysics3d::PolygonVertexArray::PolygonFace)
			);
		data.m_faces = faces;

		// Set face data
		for (Uint32 f = 0; f < faceInfo.size(); ++f)
		{
			faces[f].indexBase = faceInfo[f].m_index;
			faces[f].nbVertices = faceInfo[f].m_numVertices;
		}

		// Create vertex array
		reactphysics3d::PolygonVertexArray* vertexArray = new reactphysics3d::PolygonVertexArray(
			merged.size(),
			data.m_vertices,
			sizeof(Vector3f),
			data.m_indices,
			sizeof(Uint32),
			faceInfo.size(),
			faces,
			reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
		);
		data.m_vertexArray = vertexArray;

		// Create mesh
		reactphysics3d::PolyhedronMesh* mesh = g_common.createPolyhedronMesh(vertexArray);

		// Create shape
		data.m_shape = rp3d = g_common.createConvexMeshShape(mesh);
	}
	else
		rp3d = it.value().m_shape;

	return rp3d;
}


///////////////////////////////////////////////////////////
void Physics::setDebugRenderEnabled(bool enabled)
{
	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);

	WORLD_CAST(m_world)->setIsDebugRenderingEnabled(enabled);

	if (enabled)
	{
		reactphysics3d::DebugRenderer& debugRenderer = WORLD_CAST(m_world)->getDebugRenderer();
		debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

		if (!m_debugRenderArray.getId())
		{
			// Create vertex buffer
			m_debugRenderBuffer.create((float*)0, 100 * 1024, BufferUsage::Stream);

			// Create vao
			m_debugRenderArray.bind();
		}
	}
}


///////////////////////////////////////////////////////////
bool Physics::isDebugRenderEnabled()
{
	// Lock mutex
	std::unique_lock<std::mutex> lock(m_mutex);
	return WORLD_CAST(m_world)->getIsDebugRenderingEnabled();
}


///////////////////////////////////////////////////////////
void Physics::render(Camera& camera, FrameBuffer& target)
{
	START_PROFILING_FUNC;

	// Enable debug rendering
	if (!isDebugRenderEnabled())
		setDebugRenderEnabled(true);

	reactphysics3d::PhysicsWorld* world = WORLD_CAST(m_world);
	reactphysics3d::DebugRenderer& debugRenderer = WORLD_CAST(m_world)->getDebugRenderer();

	// Cull and decide which geometries are in the frustum
	const Frustum& frustum = camera.getFrustum();

	std::vector<DebugRenderPoint> triangles, lines;
	triangles.reserve(debugRenderer.getNbTriangles() * 3);
	lines.reserve(debugRenderer.getNbLines() * 2);

	{
		std::unique_lock<std::mutex> lock(m_mutex);

		// Lines
		for (Uint32 i = 0; i < debugRenderer.getNbLines(); ++i)
		{
			const reactphysics3d::DebugRenderer::DebugLine& line = debugRenderer.getLinesArray()[i];

			DebugRenderPoint p1, p2;
			p1.m_point = POLY_VEC3(line.point1);
			p2.m_point = POLY_VEC3(line.point2);

			BoundingBox box;
			box.m_min.x = std::min(p1.m_point.x, p2.m_point.x);
			box.m_min.y = std::min(p1.m_point.y, p2.m_point.y);
			box.m_min.z = std::min(p1.m_point.z, p2.m_point.z);
			box.m_max.x = std::max(p1.m_point.x, p2.m_point.x);
			box.m_max.y = std::max(p1.m_point.y, p2.m_point.y);
			box.m_max.z = std::max(p1.m_point.z, p2.m_point.z);

			// Only add to list if the line is in frustum
			if (frustum.contains(box))
			{
				p1.m_color = Vector3f((line.color1 >> 16) & 0xFF, (line.color1 >> 8) & 0xFF, (line.color1 >> 0) & 0xFF) / 255.0f;
				p2.m_color = Vector3f((line.color2 >> 16) & 0xFF, (line.color2 >> 8) & 0xFF, (line.color2 >> 0) & 0xFF) / 255.0f;

				lines.push_back(p1);
				lines.push_back(p2);
			}
		}

		// Triangles
		for (Uint32 i = 0; i < debugRenderer.getNbTriangles(); ++i)
		{
			const reactphysics3d::DebugRenderer::DebugTriangle& triangle = debugRenderer.getTrianglesArray()[i];

			DebugRenderPoint p[3];
			p[0].m_point = POLY_VEC3(triangle.point1);
			p[1].m_point = POLY_VEC3(triangle.point2);
			p[2].m_point = POLY_VEC3(triangle.point3);

			BoundingBox box;
			box.m_min = p[0].m_point;
			box.m_max = p[0].m_point;
			for (Uint32 i = 1; i < 3; ++i)
			{
				if (p[i].m_point.x < box.m_min.x)
					box.m_min.x = p[i].m_point.x;
				else if (p[i].m_point.x > box.m_max.x)
					box.m_max.x = p[i].m_point.x;

				if (p[i].m_point.y < box.m_min.y)
					box.m_min.y = p[i].m_point.y;
				else if (p[i].m_point.y > box.m_max.y)
					box.m_max.y = p[i].m_point.y;

				if (p[i].m_point.z < box.m_min.z)
					box.m_min.z = p[i].m_point.z;
				else if (p[i].m_point.z > box.m_max.z)
					box.m_max.z = p[i].m_point.z;
			}

			// Only add to list if the line is in frustum
			if (frustum.contains(box))
			{
				p[0].m_color = Vector3f((triangle.color1 >> 16) & 0xFF, (triangle.color1 >> 8) & 0xFF, (triangle.color1 >> 0) & 0xFF) / 255.0f;
				p[1].m_color = Vector3f((triangle.color2 >> 16) & 0xFF, (triangle.color2 >> 8) & 0xFF, (triangle.color2 >> 0) & 0xFF) / 255.0f;
				p[2].m_color = Vector3f((triangle.color3 >> 16) & 0xFF, (triangle.color3 >> 8) & 0xFF, (triangle.color3 >> 0) & 0xFF) / 255.0f;

				triangles.push_back(p[0]);
				triangles.push_back(p[1]);
				triangles.push_back(p[2]);
			}
		}
	}

	if (lines.size() + triangles.size() == 0)
		return;


	// Stream instance data
	Uint32 size = (lines.size() + triangles.size()) * sizeof(DebugRenderPoint);
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_debugBufferOffset + size > m_debugRenderBuffer.getSize())
	{
		flags |= MapBufferFlags::InvalidateBuffer;
		m_debugBufferOffset = 0;
	}

	// Map the buffer
	DebugRenderPoint* buffer = (DebugRenderPoint*)m_debugRenderBuffer.map(m_debugBufferOffset, size, flags);

	Uint32 lineOffset = m_debugBufferOffset;
	for (Uint32 i = 0; i < lines.size(); ++i)
		buffer[i] = lines[i];
	m_debugBufferOffset += lines.size() * sizeof(DebugRenderPoint);

	Uint32 triangleOffset = m_debugBufferOffset;
	for (Uint32 i = 0; i < triangles.size(); ++i)
		buffer[i + lines.size()] = triangles[i];
	m_debugBufferOffset += triangles.size() * sizeof(DebugRenderPoint);

	// After pushing all instance data, unmap the buffer
	m_debugRenderBuffer.unmap();


	// Bind target
	target.bind();

	// Get shader and bind
	Shader& shader = getDebugShader();
	shader.bind();

	camera.apply(&shader);

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Use wireframe mode
	glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

	m_debugRenderArray.bind();

	// Lines
	m_debugRenderArray.addBuffer(m_debugRenderBuffer, 0, 3, sizeof(DebugRenderPoint), lineOffset);
	m_debugRenderArray.addBuffer(m_debugRenderBuffer, 1, 3, sizeof(DebugRenderPoint), lineOffset + sizeof(Vector3f));
	m_debugRenderArray.setNumVertices(lines.size());
	m_debugRenderArray.setDrawMode(DrawMode::Lines);
	m_debugRenderArray.draw();

	// Triangles
	m_debugRenderArray.addBuffer(m_debugRenderBuffer, 0, 3, sizeof(DebugRenderPoint), triangleOffset);
	m_debugRenderArray.addBuffer(m_debugRenderBuffer, 1, 3, sizeof(DebugRenderPoint), triangleOffset + sizeof(Vector3f));
	m_debugRenderArray.setNumVertices(triangles.size());
	m_debugRenderArray.setDrawMode(DrawMode::Triangles);
	m_debugRenderArray.draw();

	// Change back to fill mode
	glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}


///////////////////////////////////////////////////////////
Shader& Physics::getDebugShader()
{
	if (!s_debugShader.getId())
	{
		s_debugShader.load("poly/debug_physics.vert", SHADER_DEBUG_PHYSICS_VERT, Shader::Vertex);
		s_debugShader.load("poly/debug_physics.frag", SHADER_DEBUG_PHYSICS_FRAG, Shader::Fragment);
		s_debugShader.compile();
	}

	return s_debugShader;
}


}