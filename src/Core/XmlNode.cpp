#include <poly/Core/XmlNode.h>

#include <rapidxml.hpp>

#define XML_CAST(x) static_cast<rapidxml::xml_node<>*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
XmlNode::XmlNode() :
	m_node			(0)
{

}


///////////////////////////////////////////////////////////
XmlNode::XmlNode(void* node) :
	m_node			(node)
{

}


///////////////////////////////////////////////////////////
XmlNode::~XmlNode()
{

}


///////////////////////////////////////////////////////////
char* XmlNode::getName() const
{
	return m_node ? XML_CAST(m_node)->name() : 0;
}


///////////////////////////////////////////////////////////
char* XmlNode::getValue() const
{
	return m_node ? XML_CAST(m_node)->value() : 0;
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getFirstNode(const char* name) const
{
	return XmlNode(m_node ? XML_CAST(m_node)->first_node(name) : 0);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getLastNode(const char* name) const
{
	return XmlNode(m_node ? XML_CAST(m_node)->last_node(name) : 0);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getNextSibling(const char* name) const
{
	return XmlNode(m_node ? XML_CAST(m_node)->next_sibling(name) : 0);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getPrevSibling(const char* name) const
{
	return XmlNode(m_node ? XML_CAST(m_node)->previous_sibling(name) : 0);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlNode::getFirstAttribute(const char* name) const
{
	return XmlAttribute(m_node ? XML_CAST(m_node)->first_attribute(name) : 0);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlNode::getLastAttribute(const char* name) const
{
	return XmlAttribute(m_node ? XML_CAST(m_node)->last_attribute(name) : 0);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getParent() const
{
	return XmlNode(m_node ? XML_CAST(m_node)->parent() : 0);
}


///////////////////////////////////////////////////////////
bool XmlNode::exists() const
{
	return (bool)m_node;
}


}