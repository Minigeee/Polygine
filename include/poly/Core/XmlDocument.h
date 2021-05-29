#ifndef POLY_XML_DOCUMENT_H
#define POLY_XML_DOCUMENT_H

#include <poly/Core/XmlNode.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A wrapper around the RapidXML document class
///
///////////////////////////////////////////////////////////
class XmlDocument : public XmlNode
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	XmlDocument();

	///////////////////////////////////////////////////////////
	/// brief The desctructor
	///
	/// This frees the buffer data used by the XML document
	///
	///////////////////////////////////////////////////////////
	~XmlDocument();

	///////////////////////////////////////////////////////////
	/// \brief Load an XML document file and create a node tree
	///
	/// This function loads an XML document file and stores all the
	/// data in an internal buffer. A node tree is created to access
	/// the data, with the XML document as the root node. The XML
	/// document inherits from a node itself, so it can be used
	/// like a node.
	///
	/// \param fname The path to the file to load
	///
	/// \return True if the file was successfully loaded
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

private:
	char* m_buffer;			//!< The buffer used to store loaded data
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::XmlDocument
/// \ingroup Core
///
/// This class is a wrapper to rapidxml::xml_document<>, so
/// most of the parsing related functions will be similar.
///
/// To load an XML file, use load(), and all data management
/// will automatically be handled. Any loaded data will be freed
/// when the object is destroyed.
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