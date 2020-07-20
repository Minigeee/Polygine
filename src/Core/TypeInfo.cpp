#include <poly/Core/TypeInfo.h>

using namespace poly;

Uint32 TypeInfo::typeCounter = 0;
std::unordered_map<Uint32, TypeInfo> TypeInfo::idToInfo;

///////////////////////////////////////////////////////////

const TypeInfo& TypeInfo::get(Uint32 id)
{
	return idToInfo[id];
}

///////////////////////////////////////////////////////////