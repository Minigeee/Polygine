#include <poly/Core/DataTypes.h>
#include <poly/Core/Endian.h>

namespace poly
{


////////////////////////////////////////////////////////////
bool isBigEndian()
{
    union {
        Uint32 i;
        char c[4];
    } a = { 0x01020304 };

    return a.c[0] == 1;
}


////////////////////////////////////////////////////////////
Endian::Type Endian::getType()
{
    static Type s_type = isBigEndian() ? Big : Little;

    return s_type;
}


}