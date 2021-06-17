#include <poly/Core/Utf.h>

#include <SFML/System/Utf.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
std::string Utf8::fromUtf16(const Utf16String& str)
{
	std::string output;
	output.reserve(str.size());

	// Convert
	sf::Utf16::toUtf8(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


///////////////////////////////////////////////////////////
std::string Utf8::fromUtf32(const Utf32String& str)
{
	std::string output;
	output.reserve(str.size());

	// Convert
	sf::Utf32::toUtf8(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


///////////////////////////////////////////////////////////
Utf16String Utf16::fromUtf8(const std::string& str)
{
	Utf16String output;
	output.reserve(str.size());

	// Convert
	sf::Utf8::toUtf16(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


///////////////////////////////////////////////////////////
Utf16String Utf16::fromUtf32(const Utf32String& str)
{
	Utf16String output;
	output.reserve(str.size());

	// Convert
	sf::Utf32::toUtf16(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


///////////////////////////////////////////////////////////
Utf32String Utf32::fromUtf8(const std::string& str)
{
	Utf32String output;
	output.reserve(str.size());

	// Convert
	sf::Utf8::toUtf32(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


///////////////////////////////////////////////////////////
Utf32String Utf32::fromUtf16(const Utf16String& str)
{
	Utf32String output;
	output.reserve(str.size());

	// Convert
	sf::Utf16::toUtf32(str.begin(), str.end(), std::back_inserter(output));

	return output;
}


}