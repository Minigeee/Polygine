#ifndef POLY_FONT_H
#define POLY_FONT_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Texture.h>

#include <poly/Math/Vector4.h>

#include <freetype/freetype.h>

#include <string>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief Predefined character sets
///
///////////////////////////////////////////////////////////
enum class CharacterSet
{
	English
};


///////////////////////////////////////////////////////////
/// \brief A class for loading and handling font faces
///
///////////////////////////////////////////////////////////
class Font
{
public:
	struct Glyph
	{
		float m_advance;
		Vector4f m_textureRect;
		Vector4f m_glyphRect;
	};

public:
	Font();

	~Font();

	bool load(const std::string& fname, CharacterSet set = CharacterSet::English);

	bool load(const std::string& fname, const std::vector<Uint32>& set);

	const Glyph& getGlyph(Uint32 c, Uint32 size);

	Texture& getTexture(Uint32 size);

private:
	struct Page
	{
		Texture m_texture;
		HashMap<Uint32, Glyph> m_glyphs;
	};

	void loadGlyphs(Uint32 size);

private:
	void* m_library;
	void* m_face;

	ObjectPool m_pool;
	std::vector<Uint32> m_characters;
	HashMap<Uint32, Page*> m_pages;
};

}

#endif