#include <type_traits>

///////////////////////////////////////////////////////////

template <typename T>
inline void TypeInfo::init(const char* name)
{
	// Create info struct
	TypeInfo info;
	info.m_id = id<T>();
	info.m_size = sizeof(T);
	info.m_align = alignof(T);
	info.m_name = name;

	info.m_isPod = std::is_pod<T>::value;
	info.m_isLiteral = std::is_literal_type<T>::value;
	info.m_isAbstract = std::is_abstract<T>::value;
	info.m_isPolymorphic = std::is_polymorphic<T>::value;

	// Add to map
	idToInfo[info.m_id] = info;
}

template <typename T>
inline Uint32 TypeInfo::id()
{
	static Uint32 id = ++typeCounter;
	return id;
}

///////////////////////////////////////////////////////////

template <typename T>
inline const TypeInfo& TypeInfo::getInfo()
{
	return idToInfo[id<T>()];
}

///////////////////////////////////////////////////////////