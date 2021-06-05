#include <poly/Core/Allocate.h>
#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Model.h>

#include <poly/Physics/Components.h>
#include <poly/Physics/Events.h>
#include <poly/Physics/Physics.h>

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


///////////////////////////////////////////////////////////
class PhysicsEventHandler : public reactphysics3d::EventListener
{
public:
	///////////////////////////////////////////////////////////
	PhysicsEventHandler(Physics* physics, Scene* scene) :
		m_physics			(physics),
		m_scene				(scene),
		m_contactPoints		(10)
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
			reactphysics3d::Collider* c2 = contactPair.getCollider1();

			e.m_colliders[0].m_bounciness = c1->getMaterial().getBounciness();
			e.m_colliders[0].m_frictionCoefficient = c1->getMaterial().getFrictionCoefficient();
			e.m_colliders[0].m_rollingResistance = c1->getMaterial().getRollingResistance();
			e.m_colliders[0].m_collisionCategory = c1->getCollisionCategoryBits();
			e.m_colliders[0].m_collisionMask = c1->getCollideWithMaskBits();

			e.m_colliders[1].m_bounciness = c2->getMaterial().getBounciness();
			e.m_colliders[1].m_frictionCoefficient = c2->getMaterial().getFrictionCoefficient();
			e.m_colliders[1].m_rollingResistance = c2->getMaterial().getRollingResistance();
			e.m_colliders[1].m_collisionCategory = c2->getCollisionCategoryBits();
			e.m_colliders[1].m_collisionMask = c2->getCollideWithMaskBits();

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
			reactphysics3d::Collider* c2 = overlapPair.getCollider1();

			e.m_colliders[0].m_bounciness = c1->getMaterial().getBounciness();
			e.m_colliders[0].m_frictionCoefficient = c1->getMaterial().getFrictionCoefficient();
			e.m_colliders[0].m_rollingResistance = c1->getMaterial().getRollingResistance();
			e.m_colliders[0].m_collisionCategory = c1->getCollisionCategoryBits();
			e.m_colliders[0].m_collisionMask = c1->getCollideWithMaskBits();

			e.m_colliders[1].m_bounciness = c2->getMaterial().getBounciness();
			e.m_colliders[1].m_frictionCoefficient = c2->getMaterial().getFrictionCoefficient();
			e.m_colliders[1].m_rollingResistance = c2->getMaterial().getRollingResistance();
			e.m_colliders[1].m_collisionCategory = c2->getCollisionCategoryBits();
			e.m_colliders[1].m_collisionMask = c2->getCollideWithMaskBits();

			// Set event type
			if (overlapPair.getEventType() == reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
				e.m_type = CollisionEventType::Start;
			else
				e.m_type = CollisionEventType::End;

			// Send the event
			m_scene->sendEvent<E_PhysicsTrigger>(e);
		}
	}


private:
	Physics* m_physics;
	Scene* m_scene;
	std::vector<poly::ContactPoint> m_contactPoints;
};


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
	Extension		(scene),
	m_world			(g_common.createPhysicsWorld()),
	m_eventHandler	(new PhysicsEventHandler(this, scene))
{
	// Add all current rigid and collision bodies
	scene->system<RigidBodyComponent>(
		[&](const Entity::Id& id, RigidBodyComponent& body)
		{
			addRigidBody(id);
		}
	);
	scene->system<CollisionBodyComponent>(
		[&](const Entity::Id& id, CollisionBodyComponent& body)
		{
			addCollisionBody(id);
		}
	);

	// Add event listeners
	scene->addListener<E_EntitiesCreated>(
		[&](const E_EntitiesCreated& e)
		{
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
				body.m_inertiaTensor = Vector3f(INFINITY);
				body.m_mass = INFINITY;
				invInertiaTensor = Vector3f(0.0f);
				invMass = 0.0f;

				type = reactphysics3d::BodyType::STATIC;
			}
			else if (body.m_type == RigidBodyType::Kinematic)
			{
				body.m_inertiaTensor = Vector3f(INFINITY);
				body.m_mass = INFINITY;
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

			// Forces are reset to zero
			body.m_force = Vector3f(0.0f);
			body.m_torque = Vector3f(0.0f);

			++localIndex;
		}
	);

	STOP_PROFILING(copyFromEngine);
}


///////////////////////////////////////////////////////////
void Physics::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
	WORLD_CAST(m_world)->setGravity(RP3D_VEC3(m_gravity));
}


///////////////////////////////////////////////////////////
void Physics::setGravity(float x, float y, float z)
{
	m_gravity = Vector3f(x, y, z);
	WORLD_CAST(m_world)->setGravity(RP3D_VEC3(m_gravity));
}


///////////////////////////////////////////////////////////
void Physics::setSleepAllowed(const Entity& entity, bool allowed)
{
	BodyData& data = m_rigidBodies[entity.getId()];

	// Change option in engine
	reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)data.m_body;
	body->setIsAllowedToSleep(allowed);

	// Update cache
	RigidBodyData& group = m_groupedRigidBodies[data.m_group][data.m_index];
	group.m_allowedSleep = allowed;
}


///////////////////////////////////////////////////////////
const Vector3f& Physics::getGravity() const
{
	return m_gravity;
}


///////////////////////////////////////////////////////////
Collider Physics::createCollider(const Entity& entity, const PhysicsShape& shape, void* rp3dShape)
{
	// Create transform
	reactphysics3d::Transform transform(RP3D_VEC3(shape.m_position), RP3D_QUAT(shape.m_rotation));
	Collider collider;

	if (entity.has<RigidBodyComponent>())
	{
		// Get rigid body
		BodyData& data = m_rigidBodies[entity.getId()];
		reactphysics3d::RigidBody* body = (reactphysics3d::RigidBody*)data.m_body;

		// Add the collider
		collider.setCollider(body->addCollider((reactphysics3d::CollisionShape*)rp3dShape, transform));
		body->updateMassPropertiesFromColliders();
		m_groupedRigidBodies[data.m_group][data.m_index].m_massPropertiesUpdated = true;
	}
	else
	{
		// Get collision body
		BodyData& data = m_collisionBodies[entity.getId()];
		reactphysics3d::CollisionBody* body = (reactphysics3d::CollisionBody*)data.m_body;

		// Add the collider
		collider.setCollider(body->addCollider((reactphysics3d::CollisionShape*)rp3dShape, transform));
	}

	return collider;
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const BoxShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a box shape
	reactphysics3d::CollisionShape* rp3dShape = (reactphysics3d::BoxShape*)getBoxShape(shape.m_dimensions);

	return createCollider(entity, shape, rp3dShape);
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const CapsuleShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a capsule shape
	reactphysics3d::CollisionShape* rp3dShape =
		(reactphysics3d::CapsuleShape*)getCapsuleShape(Vector2f(shape.m_radius, shape.m_height));

	return createCollider(entity, shape, rp3dShape);
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const ConcaveMeshShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a concave mesh shape
	reactphysics3d::CollisionShape* rp3dShape = (reactphysics3d::ConcaveMeshShape*)getConcaveMeshShape(shape);

	return createCollider(entity, shape, rp3dShape);
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const ConvexMeshShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a convex mesh shape
	reactphysics3d::CollisionShape* rp3dShape = (reactphysics3d::ConvexMeshShape*)getConvexMeshShape(shape);

	return createCollider(entity, shape, rp3dShape);
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const HeightMapShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Create a height field shape
	reactphysics3d::CollisionShape* rp3dShape = (reactphysics3d::HeightFieldShape*)getHeightMapShape(shape);

	// Adjust y-position of height shape
	HeightMapShape newShape(shape);
	newShape.m_position.y += 0.5f * shape.m_dimensions.y;

	return createCollider(entity, newShape, rp3dShape);
}


///////////////////////////////////////////////////////////
Collider Physics::addCollider(const Entity& entity, const SphereShape& shape)
{
	ASSERT(entity.has<RigidBodyComponent>() || entity.has<CollisionBodyComponent>(), "A collider can only be added to an entity with a rigid body or a collision body");

	// Get a sphere shape
	reactphysics3d::CollisionShape* rp3dShape = (reactphysics3d::SphereShape*)getSphereShape(shape.m_radius);

	return createCollider(entity, shape, rp3dShape);
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
void* Physics::getConcaveMeshShape(const ConcaveMeshShape& shape)
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
void* Physics::getConvexMeshShape(const ConvexMeshShape& shape)
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

	auto it = s_convexMeshShapes.find(key);
	if (it == s_convexMeshShapes.end())
	{
		// Create a map entry
		ConvexMeshData& data = s_convexMeshShapes[key];

		// If there are no indices, allocate a new array
		if (!usesIndices)
		{
			data.m_indices = (Uint32*)MALLOC_DBG(numVertices * sizeof(Uint32));
			for (Uint32 i = 0; i < numVertices; ++i)
				data.m_indices[i] = i;
		}
		else
			data.m_indices = (Uint32*)&indices[vertexOffset];

		// Create the faces array
		Uint32 numFaces = numVertices / 3;
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
			usesIndices ? vertices.size() : numVertices,
			usesIndices ? &vertices[0] : &vertices[vertexOffset],
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
		data.m_shape = rp3d = g_common.createConvexMeshShape(mesh);
	}
	else
		rp3d = it.value().m_shape;

	return rp3d;
}


///////////////////////////////////////////////////////////
void* Physics::getHeightMapShape(const HeightMapShape& shape)
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