#ifndef POLY_ECS_H
#define POLY_ECS_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/HandleArray.h>

#include <poly/Engine/Entity.h>

#include <functional>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace poly
{

class Scene;

namespace priv
{

template <typename C>
class ComponentData
{
public:
	static void createComponents(Uint16 sceneId, Uint32 groupId, Uint16 num, const C& component);

	static void removeComponents(Uint16 sceneId, Uint32 groupId, const std::vector<Uint16>& indices);

	static C* getComponent(Uint16 sceneId, Uint32 groupId, Uint16 index);

	static std::vector<C>& getGroup(Uint16 sceneId, Uint32 groupId);

	static bool hasGroup(Uint16 sceneId, Uint32 groupId);

	static void removeScene(Uint16 sceneId);

private:
	typedef HashMap<Uint32, std::vector<C>> Data;

	static std::vector<Data> m_data;
};

class EntityGroup
{
public:
	EntityGroup();
	EntityGroup(Scene* scene, Uint16 sceneId);

	template <typename... Cs>
	std::vector<Entity> createEntities(Uint16 num, Cs&&... components);

	void removeEntity(Entity::Id id);

	void removeQueuedEntities();

	template <typename C>
	C* getComponent(Entity::Id id) const;

	template <typename C>
	std::vector<C>& getComponentData() const;

	template <typename... Cs>
	void setComponentTypes(Uint32 groupId);

	template <typename C>
	bool hasComponentType() const;

	bool hasComponentType(Uint32 type) const;

	std::vector<Entity::Id>& getEntityIds();

private:
	template <typename... Cs>
	void removeEntitiesImpl(const std::vector<Entity::Id>& ids);

private:
	Scene* m_scene;
	Uint16 m_sceneId;
	Uint32 m_groupId;

	HandleArray<Entity::Id> m_entityIds;
	HashSet<Uint32> m_componentTypes;
	std::vector<Entity::Id> m_removeQueue;

	std::function<void(const std::vector<Entity::Id>&)> m_removeFunc;
};

///////////////////////////////////////////////////////////
// Static definitions
///////////////////////////////////////////////////////////

template <typename C>
std::vector<typename ComponentData<C>::Data> ComponentData<C>::m_data;

}

template <typename C>
class ComponentArray
{
public:
	struct Group
	{
		Group();
		Group(std::vector<C>& data);

		C* m_data;
		Uint16 m_size;
	};

	struct Iterator
	{
	public:
		Iterator();
		Iterator(ComponentArray<C>* arr);

		C& get();

		Iterator& operator++();
		Iterator operator++(int);

		bool atEnd() const;

	private:
		ComponentArray<C>* m_array;

		C* m_ptr;
		Uint32 m_group;
		Uint16 m_size;
		Uint16 m_index;
	};

public:
	void addGroup(std::vector<C>& group);

	Group& getGroup(Uint32 index);

	Uint32 getNumGroups() const;

	Iterator getIterator();

private:
	std::vector<Group> m_groups;
};


class ComponentTypeSet
{
public:
	template <typename... Cs>
	static ComponentTypeSet create();

	template <typename... Cs>
	void set();

	template <typename C>
	void add();

	template <typename C>
	void remove();

	template <typename C>
	bool has() const;

	const HashSet<Uint32>& getSet() const;

private:
	HashSet<Uint32> m_set;
};

}

#include <poly/Engine/Ecs.inl>

#endif