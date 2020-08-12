#ifndef POLY_TUPLE_H
#define POLY_TUPLE_H

#include <tuple>

namespace poly
{

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

template <typename T>
struct TupleData
{
	TupleData() = default;
	TupleData(const T& data);

	T m_data;
};

}

///////////////////////////////////////////////////////////

template <typename... Ts>
class Tuple :
	public priv::TupleData<Ts>...
{
	static_assert(priv::IsUnique<Ts...>::value, "tuples are not allowed to have duplicate types");

public:
	Tuple() = default;
	Tuple(Ts&&... args);

	template <typename T>
	void set(const T& value);

	template <typename T>
	T& get();

	template <typename T>
	const T& get() const;
};

template <typename... Ts>
Tuple<Ts...> makeTuple(Ts&&... args);

template <int N, typename... Ts>
typename std::tuple_element_t<N, std::tuple<Ts...>> get(Tuple<Ts...>& t);

}

#include <poly/Core/Tuple.inl>

#endif