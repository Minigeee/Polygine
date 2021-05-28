#ifndef POLY_XML_NODE_H
#define POLY_XML_NODE_H

#include <poly/Core/XmlAttribute.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A wrapper around the RapidXML node class
///
///////////////////////////////////////////////////////////
class XmlNode
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	XmlNode();

	///////////////////////////////////////////////////////////
	/// \brief The destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~XmlNode();

	///////////////////////////////////////////////////////////
	/// \brief Set the name of the node
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The new name of the node
	///
	///////////////////////////////////////////////////////////
	void setName(const char* name);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of the node
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The new value of the node
	///
	///////////////////////////////////////////////////////////
	void setValue(const char* name);

	///////////////////////////////////////////////////////////
	/// \brief Create a child XML node with the given name and value and return a node object
	///
	/// This function will allocate space for a new node, and append
	/// the new node as a child of the current node. The new node
	/// will have the given name and value, or be left empty if
	/// the name or value is not specified.
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The name of the new node
	/// \param value The value of the new node
	///
	/// \return The new node
	///
	///////////////////////////////////////////////////////////
	XmlNode addNode(const char* name = 0, const char* value = 0);

	///////////////////////////////////////////////////////////
	/// \brief Create an XML attribute with the given name and value and return an attribute object
	///
	/// This function will allocate space for a new attribute, and append
	/// it to the node's list of attributes. The new attribute will have
	/// the given name and value, or left empty if none are provided.
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The name of the new attribute
	/// \param value The value of the new attribute
	///
	/// \return The new attribute
	///
	///////////////////////////////////////////////////////////
	XmlAttribute addAttribute(const char* name = 0, const char* value = 0);

	///////////////////////////////////////////////////////////
	/// \brief Remove the specified child node from the current node
	///
	/// \param child The child node to remove
	///
	///////////////////////////////////////////////////////////
	void removeNode(const XmlNode& child);

	///////////////////////////////////////////////////////////
	/// \brief Remove the specified attribute from the current node
	///
	/// \param attribute The attribute to remove
	///
	///////////////////////////////////////////////////////////
	void removeAttribute(const XmlAttribute& attribute);

	///////////////////////////////////////////////////////////
	/// \brief Get the name of the node
	///
	/// \return The name string
	///
	///////////////////////////////////////////////////////////
	char* getName() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the value of the node
	///
	/// \return The value string
	///
	///////////////////////////////////////////////////////////
	char* getValue() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the first child node
	///
	/// This function returns the first child node. If a node name
	/// is given, then the first child node with the specified name
	/// will be returned.
	///
	/// \param name The optional name of the child node to find
	///
	/// \return The first child node
	///
	///////////////////////////////////////////////////////////
	XmlNode getFirstNode(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the last child node
	///
	/// This function returns the last child node. If a node name
	/// is given, then the last child node with the specified name
	/// will be returned.
	///
	/// \param name The optional name of the child node to find
	///
	/// \return The last child node
	///
	///////////////////////////////////////////////////////////
	XmlNode getLastNode(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the next sibling node
	///
	/// This function returns the next sibling node. If a node name
	/// is given, then the next sibling node with the specified name
	/// will be returned.
	///
	/// \param name The optional name of the sibiling node to find
	///
	/// \return The next sibling node
	///
	///////////////////////////////////////////////////////////
	XmlNode getNextSibling(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the previous sibling node
	///
	/// This function returns the previous sibling node. If a node name
	/// is given, then the previous sibling node with the specified name
	/// will be returned.
	///
	/// \param name The optional name of the sibiling node to find
	///
	/// \return The previous sibling node
	///
	///////////////////////////////////////////////////////////
	XmlNode getPrevSibling(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the first node attribute
	///
	/// This function returns the first node attribute. If a name
	/// is given, then the first node attribute with the given
	/// name is returned
	///
	/// \param name The optional name of the node attribute to find
	///
	/// \return The first node attribute
	///
	///////////////////////////////////////////////////////////
	XmlAttribute getFirstAttribute(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the last node attribute
	///
	/// This function returns the last node attribute. If a name
	/// is given, then the last node attribute with the given
	/// name is returned
	///
	/// \param name The optional name of the node attribute to find
	///
	/// \return The last node attribute
	///
	///////////////////////////////////////////////////////////
	XmlAttribute getLastAttribute(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the parent node
	///
	/// \return The parent node
	///
	///////////////////////////////////////////////////////////
	XmlNode getParent() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the XML attribute has been initialized
	///
	/// If the XML attribute hasn't been initialized, then it won't
	/// have any valid data.
	///
	/// \return True if the XML attribute has been initialized
	///
	///////////////////////////////////////////////////////////
	bool exists() const;

protected:
	XmlNode(void* node, void* m_document);

protected:
	void* m_node;		//!< The internal RapidXML node
	void* m_document;	//!< The internal RapidXML document
};


}

#endif

///////////////////////////////////////////////////////////
/// \class poly::XmlNode
/// \ingroup Core
///
/// This class is a wrapper around rapidxml::xml_node<>, so many
/// of its accessor functions are similar. It represents a node
/// in an XML node tree.
/// 
/// Example XML file:
/// \code
///
/// <file>
///		<node attribute="Attribute Value">Node Value</node>
///	</file>
///
/// \endcode
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// XmlDocument doc;
/// if (!doc.load("file.xml"))
///		std::cout << "Failed to load XML file\n";
///
/// XmlNode fileNode = doc.getFirstNode("file");
///
/// XmlNode node1 = fileNode.getFirstNode("node");
/// if (node1.exists())
/// {
///		std::cout << node1.getValue() << '\n';
///		std::cout << node1.getFirstAttribute("attribute").getValue() << '\n';
/// }
///
/// XmlNode node2 = fileNode.getFirstNode("node2");
/// if (node2.exists())
/// {
///		std::cout << node2.getValue() << '\n';
///		std::cout << node2.getFirstAttribute("attribute").getValue() << '\n';
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////