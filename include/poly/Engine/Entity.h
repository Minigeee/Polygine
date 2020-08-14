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

		bool operator==(const Entity::Id& other) const;
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

namespace std
{

template <>
struct hash<poly::Entity::Id>
{
	std::size_t operator()(const poly::Entity::Id& k) const
	{
		return hash<poly::Uint64>()(*reinterpret_cast<const poly::Uint64*>(&k));
	}
};

}

#include <poly/Engine/Entity.inl>

#endif