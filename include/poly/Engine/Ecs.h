#ifndef POLY_ECS_H
#define POLY_ECS_H

#include <poly/Core/HandleArray.h>

#include <tuple>
#include <vector>

namespace poly
{

class ComponentTypeSet
{

};

template <typename... Args>
class Ecs
{
private:
	std::tuple<std::vector<Args>...> m_componentArrays;
	HandleArray<Entity> m_entityArray;
};

}

#endif