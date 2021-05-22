#include <poly/Core/XmlAttribute.h>

#include <rapidxml.hpp>

#define XML_CAST(x) static_cast<rapidxml::xml_attribute<>*>(x)


namespace poly
{


///////////////////////////////////////////////////////////
XmlAttribute::XmlAttribute() :
	m_attribute			(0)
{

}


///////////////////////////////////////////////////////////
XmlAttribute::XmlAttribute(void* attr) :
	m_attribute			(attr)
{

}


///////////////////////////////////////////////////////////
XmlAttribute::~XmlAttribute()
{

}


///////////////////////////////////////////////////////////
char* XmlAttribute::getName() const
{
	return m_attribute ? XML_CAST(m_attribute)->name() : 0;
}


///////////////////////////////////////////////////////////
char* XmlAttribute::getValue() const
{
	return m_attribute ? XML_CAST(m_attribute)->value() : 0;
}


///////////////////////////////////////////////////////////
XmlAttribute XmlAttribute::getNextAttribute(const char* name) const
{
	return XmlAttribute(m_attribute ? XML_CAST(m_attribute)->next_attribute(name) : 0);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlAttribute::getPrevAttribute(const char* name) const
{
	return XmlAttribute(m_attribute ? XML_CAST(m_attribute)->previous_attribute(name) : 0);
}


///////////////////////////////////////////////////////////
bool XmlAttribute::exists() const
{
	return (bool)m_attribute;
}


}