#ifndef POLY_XML_ATTRIBUTE_H
#define POLY_XML_ATTRIBUTE_H

namespace poly
{


class XmlNode;

///////////////////////////////////////////////////////////
/// \brief A RapidXML attribute wrapper
///
///////////////////////////////////////////////////////////
class XmlAttribute
{
	friend XmlNode;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default destructor
	///
	///////////////////////////////////////////////////////////
	XmlAttribute();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~XmlAttribute();

	///////////////////////////////////////////////////////////
	/// \brief Set the name of the attribute
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The new name of the attribute
	///
	///////////////////////////////////////////////////////////
	void setName(const char* name);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of the attribute
	///
	/// \note Memory allocated by this function will not be freed until the XML document is destroyed
	///
	/// \param name The new value of the attribute
	///
	///////////////////////////////////////////////////////////
	void setValue(const char* name);

	///////////////////////////////////////////////////////////
	/// \brief Get the name of the attribute
	///
	/// \return The name string
	///
	///////////////////////////////////////////////////////////
	char* getName() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the value of the attribute
	///
	/// \return The value string
	///
	///////////////////////////////////////////////////////////
	char* getValue() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the next attribute within the same node
	///
	/// This function returns the next XML attribute that belongs
	/// to the same node. If a name is specified, then the next
	/// attribute with the given name will be returned.
	///
	/// \param name The optional name of the next XML attribute
	///
	/// \return The next XML attribute
	///
	///////////////////////////////////////////////////////////
	XmlAttribute getNextAttribute(const char* name = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the previous attribute within the same node
	///
	/// This function returns the previous XML attribute that belongs
	/// to the same node. If a name is specified, then the previous
	/// attribute with the given name will be returned.
	///
	/// \param name The optional name of the previous XML attribute
	///
	/// \return The next XML attribute
	///
	///////////////////////////////////////////////////////////
	XmlAttribute getPrevAttribute(const char* name = 0) const;

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

private:
	XmlAttribute(void* attr, void* document);

private:
	void* m_attribute;		//!< The internal XML attribute pointer
	void* m_document;		//!< The internal RapidXML document
};


}

#endif

///////////////////////////////////////////////////////////
/// \class poly::XmlAttribute
/// \ingroup Core
///
/// This class is a wrapper around rapidxml::xml_attribute<>
/// class, so many of their accessor functions will be similar.
/// The class represents a single XML attribute in a node.
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