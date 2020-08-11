#ifndef POLY_ENTITY_H
#define POLY_ENTITY_H

#include <poly/Core/HandleArray.h>

namespace poly
{

class Scene;

class Entity
{
public:
	struct Id
	{
		Id();

		Handle m_handle;
		Uint32 m_group;
	};

public:
	Entity();
	Entity(Scene* scene, Id id = Id());

	Id getId() const;

	template <typename C>
	C* get() const;

	template <typename C>
	bool has() const;

	void remove() const;

private:
	Scene* m_scene;
	Id m_id;
};

}

#include <poly/Engine/Entity.inl>

#endif