#ifndef POLY_UTF_H
#define POLY_UTF_H

#include <poly/Core/DataTypes.h>

#include <string>

namespace poly
{


typedef std::basic_string<Uint16>	Utf16String;
typedef std::basic_string<Uint32>	Utf32String;


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class Utf8
{
public:
	static std::string fromUtf16(const Utf16String& str);

	static std::string fromUtf32(const Utf32String& str);
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class Utf16
{
public:
	static Utf16String fromUtf8(const std::string& str);

	static Utf16String fromUtf32(const Utf32String& str);
};


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
class Utf32
{
public:
	static Utf32String fromUtf8(const std::string& str);

	static Utf32String fromUtf16(const Utf16String& str);
};


}

#endif