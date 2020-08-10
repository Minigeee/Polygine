#ifndef POLY_ENTITY_H
#define POLY_ENTITY_H

#include <poly/Core/HandleArray.h>

namespace poly
{

class Scene;

class Entity
{
	friend Scene;

public:
	struct Id
	{
		Handle m_handle;
		Uint32 m_group;
	};

public:
	Entity() = default;
	Entity(Id id, Scene* scene = 0) : m_id(id), m_scene(scene) { }

	template <typename T>
	T* get() const;

	Id getId() const { return m_id; }

private:
	Scene* m_scene;
	Id m_id;
};

}

#endif