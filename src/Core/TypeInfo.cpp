#include <poly/Core/TypeInfo.h>

namespace poly
{

Uint32 TypeInfo::typeCounter = 0;
HashMap<Uint32, TypeInfo> TypeInfo::idToInfo;

///////////////////////////////////////////////////////////

const TypeInfo& TypeInfo::getInfo(Uint32 id)
{
	return idToInfo[id];
}

///////////////////////////////////////////////////////////

}