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
	typedef Handle Id;

public:
	Entity() = default;
	Entity(Scene* scene);

	template <typename T>
	T* get() const;

	Id getId() const;

private:
	Scene* m_scene;
	Id m_id;
};

}

#endif