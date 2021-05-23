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
///////////////////////////////////////////////////////////
class UIParser
{
public:
	static bool parse(XmlNode node, UIElement*& out);

	static bool parse(XmlAttribute attr, int& out);

	static bool parse(XmlAttribute attr, float& out);

	static bool parse(XmlAttribute attr, Vector2f& out);

	static bool parse(XmlAttribute attr, Vector3f& out);

	static bool parse(XmlAttribute attr, Vector4f& out);

	static bool parse(XmlNode node, Font*& out);

	static bool parse(XmlAttribute attr, Texture*& out);

	static bool parse(XmlAttribute attr, UIPosition& out);

	static bool parseColor(XmlAttribute attr, Vector4f& out);

	template <typename... T>
	static bool parse(XmlAttribute attr, UIElement* element, std::function<void(T...)>& out);

	template <typename T>
	static void addElementType(const std::string& nodeName);

	template <typename... T>
	static bool addCallback(const std::string& funcName, const std::function<void(T...)>& func);

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
	static HashMap<std::string, std::function<UIElement*()>> s_elements;
	static HashMap<std::string, Font*> s_fonts;
	static HashMap<std::string, Texture*> s_textures;
	static HashMap<std::string, CallbackData> s_funcs;
};


}


#define UI_XML_CALLBACK(func) poly::UIParser::addCallback(STR(func), std::function<decltype(func)>(func));


#include <poly/UI/UIParser.inl>

#endif