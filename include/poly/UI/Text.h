#ifndef POLY_TEXT_H
#define POLY_TEXT_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Font;

///////////////////////////////////////////////////////////
/// \brief A UI element that renders text
///
///////////////////////////////////////////////////////////
class Text : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Text();

	///////////////////////////////////////////////////////////
	/// \brief Set the default font that every text element will use on construction
	///
	/// \param font The default font to set
	///
	///////////////////////////////////////////////////////////
	static void setDefaultFont(Font* font);

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the font to render the text with
	///
	/// A font is required for text to be rendered, if the text
	/// element does not have a font, it will not be rendered.
	///
	/// \param font A pointer to a font object
	///
	///////////////////////////////////////////////////////////
	void setFont(Font* font);

	///////////////////////////////////////////////////////////
	/// \brief Set the text string
	///
	/// Changing the text string will cause the pixel size of the
	/// element to be changed to match the bounds of the new
	/// string. Changing the text string will also reset any
	/// characters with a different color.
	///
	/// \param string The string to render
	///
	///////////////////////////////////////////////////////////
	void setString(const std::string& string);

	///////////////////////////////////////////////////////////
	/// \brief Set the text character size
	///
	/// The text character size is the property that determines
	/// the size of the UI element. Whenever the character size
	/// is changed, the pixel size of the element will be updated
	/// to match the bounds of the string.
	///
	/// This function replaces setSize() for Text elements, so
	/// attempting to change the size of the Text element with
	/// setSize(), or scale() will result in nothing changing.
	///
	/// \param size The text character size in pixels
	///
	///////////////////////////////////////////////////////////
	void setCharacterSize(Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Set the amount of extra space between text characters
	///
	/// This value determines the extra offset to add between each character,
	/// on top of the default amount of spacing. This value can
	/// be negative to reduce the amount of space between characters.
	///
	/// \param spacing The amount of extra space between characters, in pixels
	///
	///////////////////////////////////////////////////////////
	void setCharacterSpacing(float spacing);

	///////////////////////////////////////////////////////////
	/// \brief Set the amount of extra space between two lines of text
	///
	/// This value determines the extra offset to add betwen two
	/// lines of text in multiline text. This value can be negative
	/// to reduce the amount of space between two lines.
	///
	/// \param spacing The amount of extra space between two lines, in pixels
	///
	///////////////////////////////////////////////////////////
	void setLineSpacing(float spacing);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the text element
	///
	/// \param origin The origin of the text element
	///
	/// \see UIElement::setOrigin
	///
	///////////////////////////////////////////////////////////
	void setOrigin(const Vector2f& origin);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the text element
	///
	/// \param x The x-coordinate of the offset
	/// \param y The y-coordinate of the offset
	///
	/// \see UIElement::setOrigin
	///
	///////////////////////////////////////////////////////////
	void setOrigin(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the text element with a special offset when origin is vertically centered
	///
	/// When the origin value is vertically centered, such as for
	/// Left, Center, or Right, an offset is applied to the origin
	/// to make it look more centered.
	///
	/// \param origin A predefined origin value
	///
	///////////////////////////////////////////////////////////
	void setOrigin(UIPosition origin);

	///////////////////////////////////////////////////////////
	/// \brief Get the text font
	///
	/// \return A pointer to the text font
	///
	///////////////////////////////////////////////////////////
	Font* getFont() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the string that is being rendered by the text element
	///
	/// \return The display string
	///
	///////////////////////////////////////////////////////////
	const std::string& getString() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the text character size
	///
	/// \return The character size in pixels
	///
	///////////////////////////////////////////////////////////
	Uint32 getCharacterSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the amount of extra space between two characters
	///
	/// \return The amount of extra space between two characters in pixels
	///
	///////////////////////////////////////////////////////////
	float getCharacterSpacing() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the amount of extra space between two lines
	///
	/// \return The amount of extra space between two lines
	///
	///////////////////////////////////////////////////////////
	float getLineSpacing() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the vertical and horizontal offset of a character
	///
	/// \param index The index of the character to retrieve an offset for
	///
	/// \return The horizontal and vertical offset of a character
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getCharacterOffset(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Get the maximum height of all characters in the string, starting from the baseline of the glyph
	///
	/// \return The maximum glyph y-value
	///
	///////////////////////////////////////////////////////////
	float getGlyphYMax();

	///////////////////////////////////////////////////////////
	/// \brief Get the minimum height of all characters in the string, starting from the baseline of the glyph
	///
	/// \return The minimum glyph y-value
	///
	///////////////////////////////////////////////////////////
	float getGlyphYMin();

private:
	void getQuads(std::vector<UIQuad>& quads) override;

	void updateQuads();

	void updateTransforms() override;

	Shader* getTextShader();

	static Shader s_textShader;
	static Font* s_defaultFont;

private:
	Font* m_font;								//!< A text font
	std::string m_string;						//!< The string to render
	Uint32 m_characterSize;						//!< The character size
	float m_characterSpacing;					//!< The character spacing
	float m_lineSpacing;						//!< The line spacing

	std::vector<UIQuad> m_quads;				//!< A cache of UI quads
	std::vector<Vector2f> m_characterOffsets;	//!< A list of character offsets
	float m_glyphYMax;							//!< The clyph's y-max
	float m_glyphYMin;							//!< The clyph's y-min
	bool m_stringChanged;						//!< True if the string was changed
	bool m_isCentered;							//!< True if the text is vertically centered
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Text
/// \ingroup UI
///
/// A UI element that renders text under a certain font. Like
/// the font class, text is rendered using subpixel text rendering
/// for slightly higher resolution.
///
/// The text element is mostly the same as a normal UI element,
/// with a few exceptions. The 2D pixel size of the text can
/// not be changed directly. Instead, the character size of the
/// text must be changed for the pixel size to be changed.
/// Also, when setting the origin of the text using a predefined
/// enum, such as UIPosition::Center, the actual offset will be
/// slightly adjusted when the text is vertically centered to
/// make it look more centered.
///
/// To use a text element, the string must be set with setString(),
/// and the text element must have a font. A default font for all
/// text elements can also be set with Text::setDefaultFont().
/// If a text element does not have a valid font, it will not
/// be rendered.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load a font
/// Font font;
/// font.load("fonts/default.ttf");
/// Text::setDefaultFont(&font);
///
/// Text text;
/// text.setPosition(30.0f, 30.0f);
/// text.setString("Hello World!");
/// text.setCharacterSize(13);
///
/// // Set some optional parameters
/// // Add an extra offset between characters
/// text.setCharacterSpacing(1.0f);
///
/// // Add an extra offset between two lines of text
/// text.setLineSpacing(5.0f);
///
/// \endcode
///
///////////////////////////////////////////////////////////