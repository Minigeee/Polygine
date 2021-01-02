#include <poly/Math/Functions.h>

#include <poly/UI/Font.h>
#include <poly/UI/Text.h>

namespace poly
{


///////////////////////////////////////////////////////////
Text::Text() :
	m_font				(0),
	m_characterSize		(12),
	m_characterSpacing	(0.0f),
	m_lineSpacing		(3.0f),
	m_stringChanged		(false)
{
	m_blendFactor = BlendFactor::One;
	m_isTextureTransparent = true;
}


///////////////////////////////////////////////////////////
void Text::setFont(Font* font)
{
	m_font = font;
	m_stringChanged = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void Text::setString(const std::string& string)
{
	m_string = string;
	m_stringChanged = true;
	markDirty();

	// Reset character colors
	m_characterColors.resize(m_string.size(), Vector4f(0.0f, 0.0f, 0.0f, -1.0f));
}


///////////////////////////////////////////////////////////
void Text::setCharacterSize(Uint32 size)
{
	m_characterSize = size;
	m_stringChanged = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void Text::setCharacterSpacing(float spacing)
{
	m_characterSpacing = spacing;
	m_stringChanged = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void Text::setLineSpacing(float spacing)
{
	m_lineSpacing = spacing;
	m_stringChanged = true;
	markDirty();
}


///////////////////////////////////////////////////////////
void Text::setCharacterColor(const Vector4f& color, Uint32 index)
{
	m_characterColors[index] = color;

	// Ensure the quads are correct
	updateProperties();

	// Update the quad colors
	m_quads[index].m_color = color;
}


///////////////////////////////////////////////////////////
void Text::setCharacterColor(const Vector4f& color, Uint32 start, Uint32 end)
{
	for (Uint32 i = start; i < end; ++i)
		m_characterColors[i] = color;

	// Ensure the quads are correct
	updateProperties();

	// Update the quad colors
	for (Uint32 i = start; i < end; ++i)
		m_quads[i].m_color = color;
}


///////////////////////////////////////////////////////////
Font* Text::getFont() const
{
	return m_font;
}


///////////////////////////////////////////////////////////
const std::string& Text::getString() const
{
	return m_string;
}


///////////////////////////////////////////////////////////
Uint32 Text::getCharacterSize() const
{
	return m_characterSize;
}


///////////////////////////////////////////////////////////
float Text::getCharacterSpacing() const
{
	return m_characterSpacing;
}


///////////////////////////////////////////////////////////
float Text::getLineSpacing() const
{
	return m_lineSpacing;
}


///////////////////////////////////////////////////////////
const Vector4f& Text::getCharacterColor(Uint32 index) const
{
	return m_characterColors[index].a < 0.0f ? m_color : m_characterColors[index];
}


///////////////////////////////////////////////////////////
const Vector2f& Text::getCharacterOffset(Uint32 index)
{
	updateProperties();
	return m_characterOffsets[index];
}


///////////////////////////////////////////////////////////
float Text::getGlyphYMax()
{
	updateProperties();
	return m_glyphYMax;
}


///////////////////////////////////////////////////////////
float Text::getGlyphYMin()
{
	updateProperties();
	return m_glyphYMin;
}


///////////////////////////////////////////////////////////
void Text::getQuads(std::vector<UIQuad>& quads)
{
	updateProperties();

	for (Uint32 i = 0; i < m_quads.size(); ++i)
	{
		// Skip empty quads
		if (m_quads[i].m_texture)
			quads.push_back(m_quads[i]);
	}
}


///////////////////////////////////////////////////////////
void Text::updateProperties()
{
	bool transformChanged = m_transformChanged;

	// Call default update first
	UIElement::updateProperties();

	// Quit early if no font
	if (!m_font) return;

	if (m_stringChanged)
	{
		m_quads.resize(m_string.size());
		m_characterOffsets.resize(m_string.size() + 1);

		// Set texture
		m_texture = &m_font->getTexture(m_characterSize);

		// Keep track of x position
		Vector2f currentPos(0.0f);
		m_glyphYMax = 0.0f;
		m_glyphYMin = 0.0f;

		// Keep track of size
		float maxWidth = 0.0f;

		// Calculate quad positions
		for (Uint32 i = 0; i < m_string.size(); ++i)
		{
			if (m_string[i] == '\n')
			{
				// Update current position and continue
				currentPos.x = 0.0f;
				currentPos.y += (float)m_characterSize + m_lineSpacing;

				continue;
			}

			// Get the character glyph
			const Font::Glyph& glyph = m_font->getGlyph((Uint32)m_string[i], m_characterSize);
			const Vector4f& color = m_characterColors[i];

			// Set quad properties
			m_quads[i].m_origin = Vector2f(0.0f);
			m_quads[i].m_color = color.a < 0.0f ? m_color : color;
			m_quads[i].m_texture = m_texture;
			m_quads[i].m_blendFactor = BlendFactor::One;
			m_quads[i].m_transparent = true;

			// Set size
			m_quads[i].m_size.x = glyph.m_glyphRect.z;
			m_quads[i].m_size.y = glyph.m_glyphRect.w;
			// If the area is zero, skip the quad by setting an empty texture
			if (fabsf(glyph.m_glyphRect.z * glyph.m_glyphRect.w) <= FLT_EPSILON)
				m_quads[i].m_texture = 0;

			// Set texture rectangle
			m_quads[i].m_textureRect = glyph.m_textureRect;

			// Set character offsets
			m_characterOffsets[i].x = currentPos.x + glyph.m_glyphRect.x;
			m_characterOffsets[i].y = currentPos.y - glyph.m_glyphRect.y;
			currentPos.x += glyph.m_advance + m_characterSpacing;
			if (i < m_string.size() - 1)
				currentPos.x += m_font->getKerning((Uint32)m_string[i], (Uint32)m_string[i + 1], m_characterSize);

			// Glyph y bounds
			if (glyph.m_glyphRect.y > m_glyphYMax)
				m_glyphYMax = glyph.m_glyphRect.y;
			if (glyph.m_glyphRect.y - glyph.m_glyphRect.w < m_glyphYMin)
				m_glyphYMin = glyph.m_glyphRect.y - glyph.m_glyphRect.w;

			// Text bounds
			if (currentPos.x > maxWidth)
				maxWidth = currentPos.x;
		}

		// Update character y-offsets
		for (Uint32 i = 0; i < m_string.size(); ++i)
			m_characterOffsets[i].y += m_glyphYMax;

		// Add offset of last character
		m_characterOffsets.back() = currentPos;

		// Set text size
		m_pixelSize = Vector2f(maxWidth, currentPos.y + m_characterSize);
		m_useRelSize = Vector2b(false);

		m_stringChanged = false;
	}

	if (transformChanged)
	{
		// Text rotation
		float angle = -rad(m_absRotation);
		float ca = cos(angle);
		float sa = sin(angle);

		// Add quads
		for (Uint32 i = 0; i < m_quads.size(); ++i)
		{
			// Calculate rotated position
			Vector2f p = m_characterOffsets[i];
			p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);
			p += m_absPosition;

			// Set position
			m_quads[i].m_position = p;

			// Set rotation
			m_quads[i].m_rotation = m_absRotation;
		}
	}
}


}