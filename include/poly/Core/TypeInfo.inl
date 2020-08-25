#include <type_traits>

namespace poly
{

namespace priv
{

// Code snippet from murmurhash: https://sites.google.com/site/murmurhash/
unsigned int MurmurHash2(const void* key, int len, unsigned int seed);

}

///////////////////////////////////////////////////////////

template <typename T>
inline Uint32 TypeInfo::getId()
{
	static Uint32 typeId = [&]()
	{
		// Create info struct
		TypeInfo info;
		info.m_size = sizeof(T);
		info.m_align = alignof(T);
		info.m_name = typeid(T).name();

		info.m_isPod = std::is_pod<T>::value;
		info.m_isLiteral = std::is_literal_type<T>::value;
		info.m_isAbstract = std::is_abstract<T>::value;
		info.m_isPolymorphic = std::is_polymorphic<T>::value;

		// Generate id
		Uint32 id = priv::MurmurHash2(info.m_name.c_str(), info.m_name.size(), 0x64F621AEu);

		// Make sure there isn't a hash collision
		while (idToInfo.find(id) != idToInfo.end()) ++id;
		info.m_id = id;

		// Add to map
		idToInfo[id] = info;

		return id;
	}();

	return typeId;
}

template <typename T>
inline const TypeInfo& TypeInfo::get()
{
	return idToInfo[getId<T>()];
}

///////////////////////////////////////////////////////////

}