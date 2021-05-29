#ifndef POLY_UI_PARSER_H
#define POLY_UI_PARSER_H

#include <poly/Core/XmlAttribute.h>
#include <poly/Core/XmlNode.h>

#include <poly/UI/UIElement.h>

#include <functional>

namespace poly
{


class Font;
class Texture;

///////////////////////////////////////////////////////////
/// \brief A utility class that is used for loading UI layouts from XML
/// \ingroup UI
///
///////////////////////////////////////////////////////////
class UIParser
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Parse the UI element type from an XML node
	///
	/// This will allocate an element based on the name of the
	/// XML node, and store the results in \a out.
	///
	/// \param node The node to parse the element type from
	/// \param out The UI element pointer where the results will be stored
	///
	/// \return True if the node contained a valid UI element type
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlNode node, UIElement*& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a boolean from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out. The value of a boolean attribute should be
	/// either "true" or "false". This attribute is case insensitive.
	///
	/// \param attr The attribute to parse a boolean from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid boolean
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, bool& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a integer from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out. The value of the integer can be any type
	/// of integer, positive or negative, but has to be base 10.
	///
	/// \param attr The attribute to parse an integer from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid integer
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, int& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a float from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out.
	///
	/// \param attr The attribute to parse a float from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid float
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, float& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a Vector2f from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out. The values of the vector can be separated
	/// by spaces, commas, or both.
	///
	/// \param attr The attribute to parse a Vector2f from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid 2d vector
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, Vector2f& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a Vector3f from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out. The values of the vector can be separated
	/// by spaces, commas, or both.
	///
	/// \param attr The attribute to parse a Vector3f from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid 3d vector
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, Vector3f& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a Vector4f from the value of an XML attribute
	///
	/// This parses the value of the given attribute and stores the
	/// result in \a out. The values of the vector can be separated
	/// by spaces, commas, or both.
	///
	/// \param attr The attribute to parse a Vector4f from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a valid 4d vector
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, Vector4f& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a font from an XML node
	///
	/// This parses and loads a font from a font file, specified
	/// by the XML node. The font node can have two attributes: the
	/// "file" and the "charcter_set" attribute. The file attribute
	/// should specify the filepath of the font file, and the character
	/// set attribute should specify which character set to load from
	/// the font file. By default, the character set is English (this
	/// is the only available one at the moment).
	///
	/// \param node The XML node to load a font from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the font was parsed and loaded successfully
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlNode node, Font*& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse and load a texture from an XML attribute
	///
	/// This parses and loads a texture from an image file, where the
	/// file is specified by the value of the XML attribute.
	///
	/// \param attr The attribute to parse and load a texture from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the texture was parsed and loaded successfully
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, Texture*& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a shader from an XML node
	///
	/// This parses and loads a shader program from a set of shaders,
	/// which are specified in the XML node. The XML node should have
	/// up to three attributes: "vert", "geom", and "frag", and each
	/// attribute should contain a filepath to their respective shader
	/// code files. The "vert" attribute is the only necessary attribute,
	/// as the geometry and fragment shaders are not necessary to compile
	/// a shader. Note that the shader node only allows a single
	/// shader of each type to be compiled.
	///
	/// \param node The XML node to load a shader program from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the shader was parsed and loaded successfully
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlNode node, Shader*& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a UIPosition from an XML attribute
	///
	/// This parses a UIPosition from the value of an XML attribute.
	/// The value of the attribute must be a string with one of the
	/// following values:
	///
	/// \li `top_left`
	/// \li `top_center`
	/// \li `top_right`
	/// \li `left`
	/// \li `center`
	/// \li `right`
	/// \li `bot_left`
	/// \li `bot_center`
	/// \li `bot_right`
	///
	/// \param attr The attribute to to parse a UIPosition from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the UIPosition was a valid value
	///
	///////////////////////////////////////////////////////////
	static bool parse(XmlAttribute attr, UIPosition& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a color from an XML attribute
	///
	/// This parses a color from the value of an XML attribute.
	/// The color may be represented several ways. The options are:
	///
	/// \li A 3D vector (R, G, B)
	/// \li A 4D vector (R, G, B, A)
	/// \li A 3-byte hex code starting with '#' (#RRGGBB)
	/// \li A 4-byte hex code starting with '#' (#RRGGBBAA)
	/// \li A pre-defined color
	///
	/// The list of valid predefined colors contains 139 entries,
	/// to see which colors are included, please look at the implementation
	/// of this function.
	///
	/// \param attr The attribute to to parse a color from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the color was a valid value
	///
	///////////////////////////////////////////////////////////
	static bool parseColor(XmlAttribute attr, Vector4f& out);

	///////////////////////////////////////////////////////////
	/// \brief Parse a callback function and connect it to a registered function
	///
	/// This function parses the value in the XML attribute and returns
	/// a c++ function object if the function name has been registered
	/// in the parser. To register a function, use addCallback().
	///
	/// \param attr The attribute to to parse a callback function from
	/// \param out The variable where the results will be stored
	///
	/// \return True if the attribute contained a function that was registered
	///
	///////////////////////////////////////////////////////////
	template <typename... T>
	static bool parse(XmlAttribute attr, UIElement* element, std::function<void(T...)>& out);

	///////////////////////////////////////////////////////////
	/// \brief Register a function callback under a certain name
	///
	/// Callback functions must be registered using this function for
	/// them to be used in UI XML files. UI elements that reference the
	/// \a funcName will be connected to the given callback function.
	///
	/// \param funcName The name to register the callback function under
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	template <typename... T>
	static bool addCallback(const std::string& funcName, const std::function<void(T...)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Register a function callback under a certain name
	///
	/// Callback functions must be registered using this function for
	/// them to be used in UI XML files. UI elements that reference the
	/// \a funcName will be connected to the given callback function.
	///
	/// \param funcName The name to register the callback function under
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	template <typename... T>
	static bool addCallback(const std::string& funcName, std::function<void(UIElement*, T...)> func);

private:
	struct CallbackData
	{
		void* m_funcPtr;
		std::vector<Uint32> m_argTypes;
		bool m_passesElement;
	};

private:
	static HashMap<std::string, Font*> s_fonts;			
	static HashMap<std::string, Texture*> s_textures;
	static HashMap<std::string, Shader*> s_shaders;
	static HashMap<std::string, CallbackData> s_funcs;
};


}


///////////////////////////////////////////////////////////
/// \brief A convenience macro function for registering non-member functions
///
/// This macro only works for non-member functions, and registers
/// the function under the same name.
///
///////////////////////////////////////////////////////////
#define UI_XML_CALLBACK(func) poly::UIParser::addCallback(STR(func), std::function<decltype(func)>(func));


#include <poly/UI/UIParser.inl>

#endif