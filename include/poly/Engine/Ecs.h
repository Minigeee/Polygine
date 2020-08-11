#ifndef POLY_ECS_H
#define POLY_ECS_H

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

///////////////////////////////////////////////////////////
// Unique variadic template parameters
///////////////////////////////////////////////////////////

template <typename... Args>
struct ParameterPack {};

///////////////////////////////////////////////////////////

template <typename... Args>
struct ParamsContain;

template <typename A, typename... Args, typename B>
struct ParamsContain<ParameterPack<A, Args...>, B>
{
	static constexpr bool value = ParamsContain<ParameterPack<Args...>, B>::value;
};

template <typename A, typename... Args>
struct ParamsContain<ParameterPack<A, Args...>, A>
{
	static constexpr bool value = true;
};

template <typename A>
struct ParamsContain<ParameterPack<>, A>
{
	static constexpr bool value = false;
};

///////////////////////////////////////////////////////////

template <typename... Args>
struct IsUnique;

template <typename A, typename... Args>
struct IsUnique<A, Args...>
{
	static constexpr bool value = !ParamsContain<ParameterPack<Args...>, A>::value && IsUnique<Args...>::value;
};

template <typename A>
struct IsUnique<A>
{
	static constexpr bool value = true;
};

template <>
struct IsUnique<>
{
	static constexpr bool value = true;
};

///////////////////////////////////////////////////////////

template <typename C>
class ComponentData
{
public:
	static void createComponents(Uint16 sceneId, Uint32 groupId, Uint16 num, const C& component);

	static void removeComponents(Uint16 sceneId, Uint32 groupId, const std::vector<Uint16>& indices);

	static C* getComponent(Uint16 sceneId, Uint32 groupId, Uint16 index);

	static std::vector<C>& getGroup(Uint16 sceneId, Uint32 groupId);

	static bool hasGroup(Uint16 sceneId, Uint32 groupId);

private:
	typedef std::unordered_map<Uint32, std::vector<C>> Data;

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

private:
	template <typename... Cs>
	void removeEntitiesImpl(const std::vector<Entity::Id>& ids);

private:
	Scene* m_scene;
	Uint16 m_sceneId;
	Uint32 m_groupId;

	HandleArray<Entity::Id> m_entityIds;
	std::unordered_set<Uint32> m_componentTypes;
	std::vector<Entity::Id> m_removeQueue;

	std::function<void(const std::vector<Entity::Id>&)> m_removeFunc;
};

template <typename C>
struct ComponentSetData
{
	C m_data;
};

///////////////////////////////////////////////////////////
// Static definitions
///////////////////////////////////////////////////////////

template <typename C>
std::vector<typename ComponentData<C>::Data> ComponentData<C>::m_data;

}

template <typename... Cs>
class ComponentSet :
	public priv::ComponentSetData<Cs>...
{
	static_assert(priv::IsUnique<Cs...>::value, "Component sets are not allowed to have duplicate component types");

public:
	template <typename C>
	void set(const C& component);

	template <typename C>
	C& get();
};

}

#include <poly/Engine/Ecs.inl>

#endif