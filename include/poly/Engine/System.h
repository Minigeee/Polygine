#ifndef POLY_ECS_SYSTEM_H
#define POLY_ECS_SYSTEM_H

#include <poly/Core/Scheduler.h>

#include <poly/Engine/Entity.h>
#include <poly/Engine/Scene.h>

namespace poly
{

template <typename... Cs>
class System
{
public:
	System();
	System(Scene* scene);

	void update(float dt);

	virtual void process(float dt, const Entity::Id& id, Cs&... components) = 0;

protected:
	Scene* m_scene;
};

}

#include <poly/Engine/System.inl>

#endif