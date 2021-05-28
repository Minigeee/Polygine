#include <poly/Core/XmlNode.h>

#include <rapidxml.hpp>

#define XML_ATTR_CAST(x) static_cast<rapidxml::xml_attribute<>*>(x)
#define XML_DOC_CAST(x) static_cast<rapidxml::xml_document<>*>(x)
#define XML_NODE_CAST(x) static_cast<rapidxml::xml_node<>*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
XmlNode::XmlNode() :
	m_node			(0),
	m_document		(0)
{

}


///////////////////////////////////////////////////////////
XmlNode::XmlNode(void* node, void* document) :
	m_node			(node),
	m_document		(document)
{

}


///////////////////////////////////////////////////////////
XmlNode::~XmlNode()
{

}


///////////////////////////////////////////////////////////
void XmlNode::setName(const char* name)
{
	if (m_node && m_document)
		XML_NODE_CAST(m_node)->name(XML_DOC_CAST(m_document)->allocate_string(name));
}


///////////////////////////////////////////////////////////
void XmlNode::setValue(const char* value)
{
	if (m_node && m_document)
		XML_NODE_CAST(m_node)->value(XML_DOC_CAST(m_document)->allocate_string(value));
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::addNode(const char* name, const char* value)
{
	if (m_node && m_document)
	{
		if (name)
			name = XML_DOC_CAST(m_document)->allocate_string(name);
		if (value)
			value = XML_DOC_CAST(m_document)->allocate_string(value);

		// Create and append the node
		rapidxml::xml_node<>* node = XML_DOC_CAST(m_document)->allocate_node(rapidxml::node_element, name, value);
		XML_NODE_CAST(m_node)->append_node(node);

		return XmlNode(node, m_document);
	}

	return XmlNode();
}


///////////////////////////////////////////////////////////
XmlAttribute XmlNode::addAttribute(const char* name, const char* value)
{
	if (m_node && m_document)
	{
		if (name)
			name = XML_DOC_CAST(m_document)->allocate_string(name);
		if (value)
			value = XML_DOC_CAST(m_document)->allocate_string(value);

		// Create and append the node
		rapidxml::xml_attribute<>* attr = XML_DOC_CAST(m_document)->allocate_attribute(name, value);
		XML_NODE_CAST(m_node)->append_attribute(attr);

		return XmlAttribute(attr, m_document);
	}

	return XmlAttribute();
}


///////////////////////////////////////////////////////////
void XmlNode::removeNode(const XmlNode& child)
{
	if (child.m_node)
		XML_NODE_CAST(m_node)->remove_node(XML_NODE_CAST(child.m_node));
}


///////////////////////////////////////////////////////////
void XmlNode::removeAttribute(const XmlAttribute& attr)
{
	if (attr.m_attribute)
		XML_NODE_CAST(m_node)->remove_attribute(XML_ATTR_CAST(attr.m_attribute));
}


///////////////////////////////////////////////////////////
char* XmlNode::getName() const
{
	return m_node ? XML_NODE_CAST(m_node)->name() : 0;
}


///////////////////////////////////////////////////////////
char* XmlNode::getValue() const
{
	return m_node ? XML_NODE_CAST(m_node)->value() : 0;
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getFirstNode(const char* name) const
{
	return XmlNode(m_node ? XML_NODE_CAST(m_node)->first_node(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getLastNode(const char* name) const
{
	return XmlNode(m_node ? XML_NODE_CAST(m_node)->last_node(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getNextSibling(const char* name) const
{
	return XmlNode(m_node ? XML_NODE_CAST(m_node)->next_sibling(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getPrevSibling(const char* name) const
{
	return XmlNode(m_node ? XML_NODE_CAST(m_node)->previous_sibling(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlNode::getFirstAttribute(const char* name) const
{
	return XmlAttribute(m_node ? XML_NODE_CAST(m_node)->first_attribute(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlAttribute XmlNode::getLastAttribute(const char* name) const
{
	return XmlAttribute(m_node ? XML_NODE_CAST(m_node)->last_attribute(name) : 0, m_document);
}


///////////////////////////////////////////////////////////
XmlNode XmlNode::getParent() const
{
	return XmlNode(m_node ? XML_NODE_CAST(m_node)->parent() : 0, m_document);
}


///////////////////////////////////////////////////////////
bool XmlNode::exists() const
{
	return (bool)m_node;
}


}