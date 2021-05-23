#include <poly/Core/Logger.h>
#include <poly/Core/Macros.h>
#include <poly/Core/ObjectPool.h>

namespace poly
{

namespace priv
{


// Lazy variadic template binding
template <typename... T>
std::function<void(T...)> bind(UIElement*, std::function<void(UIElement*, T...)>&);

template <>
inline std::function<void()> bind(UIElement* elem, std::function<void(UIElement*)>& func)
{
	return std::bind(func, elem);
}

template <typename A>
inline std::function<void(A)> bind(UIElement* elem, std::function<void(UIElement*, A)>& func)
{
	return std::bind(func, elem, std::placeholders::_1);
}

template <typename A, typename B>
inline std::function<void(A, B)> bind(UIElement* elem, std::function<void(UIElement*, A, B)>& func)
{
	return std::bind(func, elem, std::placeholders::_1, std::placeholders::_2);
}

template <typename A, typename B, typename C>
inline std::function<void(A, B, C)> bind(UIElement* elem, std::function<void(UIElement*, A, B, C)>& func)
{
	return std::bind(func, elem, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}


}


///////////////////////////////////////////////////////////
template <typename... T>
inline bool UIParser::parse(XmlAttribute attr, UIElement* element, std::function<void(T...)>& out)
{
	auto it = s_funcs.find(attr.getValue());
	if (it == s_funcs.end())
	{
		LOG_WARNING("There is no callback function bound to %s", attr.getValue());
		return false;
	}

	// Cast the void pointer into a pointer of the correct type
	CallbackData& data = it.value();

	// Check if the correct arguments are being used
	std::vector<Uint32> argTypes;
	PARAM_EXPAND(argTypes.push_back(TypeInfo::getId<T>()));

	if (argTypes.size() != data.m_argTypes.size())
	{
		LOG_WARNING("The callback function bound to %s() has the incorrect number of arguments", attr.getValue());
		return false;
	}

	for (Uint32 i = 0; i < argTypes.size(); ++i)
	{
		if (argTypes[i] != data.m_argTypes[i])
		{
			LOG_WARNING("The callback function bound to %s() has the incorrect argument types", attr.getValue());
			return false;
		}
	}

	// Take different actions based on if the element gets passed or not
	if (data.m_passesElement)
	{
		auto funcPtr = reinterpret_cast<std::function<void(UIElement*, T...)>*>(data.m_funcPtr);
		out = priv::bind<T...>(element, *funcPtr);
	}
	else
		out = *reinterpret_cast<std::function<void(T...)>*>(data.m_funcPtr);

	return true;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void UIParser::addElementType(const std::string& nodeName)
{
	s_elements[nodeName] = [&]() -> UIElement* { return (UIElement*)Pool<T>::alloc(); };
}


///////////////////////////////////////////////////////////
template <typename... T>
inline bool UIParser::addCallback(const std::string& funcName, const std::function<void(T...)>& func)
{
	if (s_funcs.find(funcName) != s_funcs.end())
		return false;

	CallbackData data;
	data.m_passesElement = false;

	// Allocate a spot for the function 
	auto funcPtr = Pool<std::function<void(T...)>>::alloc();
	*funcPtr = func;
	data.m_funcPtr = funcPtr;

	// Argument types
	PARAM_EXPAND(data.m_argTypes.push_back(TypeInfo::getId<T>()));

	s_funcs[funcName] = std::move(data);

	return true;
}


///////////////////////////////////////////////////////////
template <typename... T>
inline bool UIParser::addCallback(const std::string& funcName, std::function<void(UIElement*, T...)> func)
{
	if (s_funcs.find(funcName) != s_funcs.end())
		return false;

	CallbackData data;
	data.m_passesElement = true;

	// Allocate a spot for the function
	auto funcPtr = Pool<std::function<void(UIElement*, T...)>>::alloc();
	*funcPtr = func;
	data.m_funcPtr = funcPtr;

	// Argument types
	PARAM_EXPAND(data.m_argTypes.push_back(TypeInfo::getId<T>()));

	s_funcs[funcName] = std::move(data);

	return true;
}


}