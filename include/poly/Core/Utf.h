#ifndef POLY_UTF_H
#define POLY_UTF_H

#include <poly/Core/DataTypes.h>

#include <string>

namespace poly
{


typedef std::basic_string<Uint16>	Utf16String;
typedef std::basic_string<Uint32>	Utf32String;


///////////////////////////////////////////////////////////
/// \brief A utility class used to convert UTF strings
/// \ingroup Core
///
///////////////////////////////////////////////////////////
class Utf8
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-16 string into a UTF-8 string
	///
	/// \param str A string encoded in UTF-16
	///
	/// \return The string converted to UTF-8
	///
	///////////////////////////////////////////////////////////
	static std::string fromUtf16(const Utf16String& str);

	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-32 string into a UTF-8 string
	///
	/// \param str A string encoded in UTF-32
	///
	/// \return The string converted to UTF-8
	///
	///////////////////////////////////////////////////////////
	static std::string fromUtf32(const Utf32String& str);
};


///////////////////////////////////////////////////////////
/// \brief A utility class used to convert UTF strings
/// \ingroup Core
///
///////////////////////////////////////////////////////////
class Utf16
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-8 string into a UTF-16 string
	///
	/// \param str A string encoded in UTF-8
	///
	/// \return The string converted to UTF-16
	///
	///////////////////////////////////////////////////////////
	static Utf16String fromUtf8(const std::string& str);

	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-32 string into a UTF-16 string
	///
	/// \param str A string encoded in UTF-32
	///
	/// \return The string converted to UTF-16
	///
	///////////////////////////////////////////////////////////
	static Utf16String fromUtf32(const Utf32String& str);
};


///////////////////////////////////////////////////////////
/// \brief A utility class used to convert UTF strings
/// \ingroup Core
///
///////////////////////////////////////////////////////////
class Utf32
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-8 string into a UTF-32 string
	///
	/// \param str A string encoded in UTF-8
	///
	/// \return The string converted to UTF-32
	///
	///////////////////////////////////////////////////////////
	static Utf32String fromUtf8(const std::string& str);

	///////////////////////////////////////////////////////////
	/// \brief Convert a UTF-16 string into a UTF-32 string
	///
	/// \param str A string encoded in UTF-16
	///
	/// \return The string converted to UTF-32
	///
	///////////////////////////////////////////////////////////
	static Utf32String fromUtf16(const Utf16String& str);
};


}

#endif