#include <poly/Core/Allocate.h>
#include <poly/Core/DataTypes.h>
#include <poly/Core/XmlDocument.h>

#include <rapidxml.hpp>

#include <fstream>

#define XML_CAST(x) static_cast<rapidxml::xml_document<>*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
XmlDocument::XmlDocument() :
	XmlNode		(0)
{
	m_document = new rapidxml::xml_document<>;
}


///////////////////////////////////////////////////////////
XmlDocument::~XmlDocument()
{
	delete XML_CAST(m_document);
	FREE_DBG(m_buffer);
}


///////////////////////////////////////////////////////////
bool XmlDocument::load(const std::string& fname)
{
	std::ifstream file(fname, std::ios::binary | std::ios::ate);
	if (!file.is_open()) return false;

	// Get file size
	Uint32 fsize = (Uint32)file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = (char*)MALLOC_DBG(fsize + 1);
	file.read(buffer, fsize);
	buffer[fsize] = 0;

	file.close();

	// Parse
	XML_CAST(m_document)->parse<0>(buffer);

	m_node = XML_CAST(m_document);
	m_buffer = buffer;

	return true;
}


}