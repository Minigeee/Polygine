#ifndef POLY_FONT_H
#define POLY_FONT_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Texture.h>

#include <poly/Math/Vector4.h>

#include <string>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A class for loading and handling font faces
///
///////////////////////////////////////////////////////////
class Font
{
public:
	///////////////////////////////////////////////////////////
	/// \brief A struct containing information on character
	///
	///////////////////////////////////////////////////////////
	struct Glyph
	{
		Glyph();

		float m_advance;			//!< The number of pixels between the start of this character and the next character
		Vector4i m_textureRecti;	//!< The texture rectangle that determines which part of the texture to display (in pixels)
		Vector4f m_textureRectf;	//!< The texture rectangle that determines which part of the texture to display (in texture coordinates)
		Vector4f m_glyphRect;		//!< The rectangle describing the relative position and size of the character in pixels
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes the FreeType library
	///
	///////////////////////////////////////////////////////////
	Font();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Cleans up the resource used by the FreeType library
	///
	///////////////////////////////////////////////////////////
	~Font();

	///////////////////////////////////////////////////////////
	/// \brief 
	///
	/// \param fname The file name of the font file
	///
	/// \return True if the file loads successfully
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Get a character glyph for a certain font size
	///
	/// The glyph will be generated into the font object.
	///
	/// \param c The character to retrieve a glyph for
	/// \param size The font size to retrieve the glyph
	///
	/// \return A reference to the character glyph
	///
	///////////////////////////////////////////////////////////
	const Glyph& getGlyph(Uint32 c, Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Get the font texture atlas for the specified font size
	///
	/// \param size The font size to retrieve a texture atlas for
	///
	/// \return A pointer to the texture
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture(Uint32 size) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the extra offset between two specific characters
	///
	/// \param c1 The first character
	/// \param c2 The second character
	/// \param size The charaacter size to load kerning for
	///
	///////////////////////////////////////////////////////////
	float getKerning(Uint32 c1, Uint32 c2, Uint32 size);

private:
	struct Page
	{
		Page();

		~Page();

		Vector2u m_currentLoc;				//!< The current location in the texture, the location to add the next glyph
		Uint32 m_currentRowSize;			//!< The height of the current row
		Texture* m_texture;					//!< The font page texture
		Uint8* m_textureData;				//!< Texture data
		HashMap<Uint32, Glyph> m_glyphs;	//!< Map of codepoints to their glyph information
	};

	void loadGlyph(Uint32 c, Uint32 size);

private:
	void* m_library;
	void* m_face;

	TypePool<Page> m_pagePool;
	TypePool<Texture> m_texturePool;
	HashMap<Uint32, Page*> m_pages;
	Uint32 m_characterSize;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Font
/// \ingroup UI
///
/// The font class loads font files and renders glyphs into
/// textures for use by Text elements. To use a font, first load
/// a font from a file using load(), then glyph geometry and
/// textures can be loaded for specific character sizes using
/// getGlyph() and getTexture(). But most of the time, these
/// functions are only used internally by the Text element.
///
/// When loading the font, a limited character set must be chosen.
/// By default, the english character set is used, which includes
/// ASCII characters from 32 to 127. When rendering the glyphs,
/// the font renders using subpixel rendering for slightly higher
/// resolution text. A default text font can be set using
/// Text::setDefaultFont().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load a font
/// Font font;
/// font.load("fonts/default.ttf");
///
/// // Set the default text font
/// Text::setDefaultFont(&font);
///
/// \endcode
///
///////////////////////////////////////////////////////////