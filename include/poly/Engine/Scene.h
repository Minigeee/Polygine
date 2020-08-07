#ifndef POLY_SCENE_H
#define POLY_SCENE_H

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Entity.h>

namespace poly
{

class Scene
{
public:
	template <typename... Args>
	Entity createEntity();

	template <typename... Args>
	Entity createEntity(const Args&... components);

	Entity createEntity(const ComponentTypeSet& types);

	template <typename T>
	T* getComponent(Entity::Id id) const;

private:
	Uint32 m_id;
};

}

#endif