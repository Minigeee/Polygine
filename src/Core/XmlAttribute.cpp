#include <poly/Core/XmlAttribute.h>

#include <rapidxml.hpp>

#define XML_ATTR_CAST(x) static_cast<rapidxml::xml_attribute<>*>(x)
#define XML_DOC_CAST(x) static_cast<rapidxml::xml_document<>*>(x)


namespace poly
{


///////////////////////////////////////////////////////////
XmlAttribute::XmlAttribute() :
	m_attribute			(0),
	m_document			(0)
{

}


///////////////////////////////////////////////////////////
XmlAttribute::XmlAttribute(void* attr, void* document) :
	m_attribute			(attr),
	m_document			(document)
{

}


///////////////////////////////////////////////////////////
XmlAttribute::~XmlAttribute()
{

}


///////////////////////////////////////////////////////////
void XmlAttribute::setName(const char* name)
{
	if (m_attribute && m_document)
		XML_ATTR_CAST(m_attribute)->name(XML_DOC_CAST(m_document)->allocate_string(name));
}


///////////////////////////////////////////////////////////
void XmlAttribute::setValue(const char* value)
{
	if (m_attribute && m_document)
		XML_ATTR_CAST(m_attribute)->value(XML_DOC_CAST(m_document)->allocate_string(value));
}


///////////////////////////////////////////////////////////
char* XmlAttribute::getName() const
{
	return m_attribute ? XML_ATTR_CAST(m_attribute)->name() : 0;
}


///////////////////////////////////////////////////////////
char* XmlAttribute::getValue() const
{
	return m_attribute ? XML_ATTR_CAST(m_attribute)->value() : 0;
}


///////////////////////////////////////////////////////////
XmlAttribute XmlAttribute::getNextAttribute(const char* name) const
{
	return XmlAttribute(m_attribute ? XML_ATTR_CAST(m_attribute)->next_attribute(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlAttribute::getPrevAttribute(const char* name) const
{
	return XmlAttribute(m_attribute ? XML_ATTR_CAST(m_attribute)->previous_attribute(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
bool XmlAttribute::exists() const
{
	return (bool)m_attribute;
}


}