#include <poly/Math/Functions.h>

#include <poly/Graphics/Shaders/ui.vert.h>
#include <poly/Graphics/Shaders/ui.geom.h>
#include <poly/Graphics/Shaders/text.frag.h>

#include <poly/UI/Font.h>
#include <poly/UI/Text.h>
#include <poly/UI/UIParser.h>

#include <SFML/System/Utf.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
Shader Text::s_textShader;

///////////////////////////////////////////////////////////
Font* Text::s_defaultFont = 0;


///////////////////////////////////////////////////////////
Text::Text() :
	m_font				(s_defaultFont),
	m_characterSize		(12),
	m_characterSpacing	(0.0f),
	m_lineSpacing		(3.0f),
	m_textureHeight		(0),
	m_stringChanged		(false),
	m_isCentered		(false)
{
	m_srcBlend = BlendFactor::ConstColor;
	m_dstBlend = BlendFactor::OneMinusSrcColor;
	m_isTextureTransparent = true;

	// Shader
	m_shader = getTextShader();
}


///////////////////////////////////////////////////////////
void Text::parse(XmlNode node, const UITemplateMap& templates)
{
	// Default parse
	UIElement::parse(node, templates);

	// Value
	if (strlen(node.getValue()) > 0)
		setString(node.getValue());

	else
	{
		XmlAttribute valueAttr = node.getFirstAttribute("value");
		if (valueAttr.exists())
			setString(valueAttr.getValue());
	}

	// Character size
	XmlAttribute charSizeAttr = node.getFirstAttribute("character_size");
	if (charSizeAttr.exists())
	{
		int size;
		if (UIParser::parse(charSizeAttr, size))
			setCharacterSize((Uint32)size);
	}

	// Character spacing
	XmlAttribute charSpacingAttr = node.getFirstAttribute("character_spacing");
	if (charSpacingAttr.exists())
	{
		float spacing;
		if (UIParser::parse(charSpacingAttr, spacing))
			setCharacterSpacing(spacing);
	}

	// Line spacing
	XmlAttribute lineSpacingAttr = node.getFirstAttribute("line_spacing");
	if (lineSpacingAttr.exists())
	{
		float spacing;
		if (UIParser::parse(lineSpacingAttr, spacing))
			setLineSpacing(spacing);
	}

	// Font
	XmlNode fontNode = node.getFirstNode("font");
	if (fontNode.exists())
	{
		Font* font;
		if (UIParser::parse(fontNode, font))
			setFont(font);
	}
}


///////////////////////////////////////////////////////////
void Text::setFont(Font* font)
{
	m_font = font;
	m_stringChanged = true;
}


///////////////////////////////////////////////////////////
void Text::setString(const std::string& string)
{
	m_string = Utf32::fromUtf8(string);
	m_stringChanged = true;

	// Resize the character colors array
	m_characterColors.resize(m_string.size(), Vector3f(m_color));
}


///////////////////////////////////////////////////////////
void Text::setString(const Utf32String& string)
{
	m_string = string;
	m_stringChanged = true;

	// Resize the character colors array
	m_characterColors.resize(m_string.size(), Vector3f(m_color));
}


///////////////////////////////////////////////////////////
void Text::setCharacterSize(Uint32 size)
{
	m_characterSize = size;
	m_stringChanged = true;
}


///////////////////////////////////////////////////////////
void Text::setCharacterSpacing(float spacing)
{
	m_characterSpacing = spacing;
	m_stringChanged = true;
}


///////////////////////////////////////////////////////////
void Text::setLineSpacing(float spacing)
{
	m_lineSpacing = spacing;
	m_stringChanged = true;
}


///////////////////////////////////////////////////////////
void Text::setOrigin(const Vector2f& origin)
{
	m_isCentered = false;
	UIElement::setOrigin(origin);
}


///////////////////////////////////////////////////////////
void Text::setOrigin(float x, float y)
{
	m_isCentered = false;
	UIElement::setOrigin(x, y);
}


///////////////////////////////////////////////////////////
void Text::setOrigin(UIPosition origin)
{
	m_isCentered = 
		origin == UIPosition::Left ||
		origin == UIPosition::Center || 
		origin == UIPosition::Right;

	UIElement::setOrigin(origin);
}


///////////////////////////////////////////////////////////
void Text::setCharacterColors(const Vector3f& color, Uint32 offset, Uint32 num)
{
	// The string and colors array will always have the same size

	Uint32 end = offset + num;
	if (end > m_string.size())
		end = m_string.size();

	// Update colors
	for (Uint32 i = offset; i < end; ++i)
	{
		m_characterColors[i] = color;
		if (i < m_quads.size())
			m_quads[i].m_color = Vector4f(color, 1.0f);
	}
}


///////////////////////////////////////////////////////////
Font* Text::getFont() const
{
	return m_font;
}


///////////////////////////////////////////////////////////
const Utf32String& Text::getString() const
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
const Vector2f& Text::getCharacterOffset(Uint32 index)
{
	updateQuads();
	return m_characterOffsets[index];
}


///////////////////////////////////////////////////////////
const Vector3f& Text::getCharacterColor(Uint32 index) const
{
	return m_characterColors[index];
}


///////////////////////////////////////////////////////////
float Text::getGlyphYMax()
{
	updateQuads();
	return m_glyphYMax;
}


///////////////////////////////////////////////////////////
float Text::getGlyphYMin()
{
	updateQuads();
	return m_glyphYMin;
}


///////////////////////////////////////////////////////////
void Text::getQuads(std::vector<UIQuad>& quads)
{
	// Update transforms because quad positions and rotations are needed
	updateTransforms();

	for (Uint32 i = 0; i < m_quads.size(); ++i)
		quads.push_back(m_quads[i]);
}


///////////////////////////////////////////////////////////
void Text::updateQuads()
{
	// Quit early if no font
	if (!m_font) return;

	Texture* texture = m_font->getTexture(m_characterSize);
	if (m_stringChanged || (texture && texture->getHeight() != m_textureHeight))
	{
		// Get height before updating quads
		if (texture)
			m_textureHeight = texture->getHeight();

		// Resize arrays
		m_quads.resize(m_string.size());
		m_characterOffsets.resize(m_string.size() + 1);

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

			// Set quad properties
			m_quads[i].m_origin = Vector2f(0.0f);
			m_quads[i].m_color = Vector4f(m_characterColors[i], 1.0f);

			// Set size
			m_quads[i].m_size.x = glyph.m_glyphRect.z;
			m_quads[i].m_size.y = glyph.m_glyphRect.w;

			// Set texture rectangle
			m_quads[i].m_textureRect = glyph.m_textureRectf;

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

		// Set texture (this has to be after the glyph loop to ensure the texture has been generated)
		m_texture = m_font->getTexture(m_characterSize);

		// Update character y-offsets
		for (Uint32 i = 0; i < m_string.size(); ++i)
			m_characterOffsets[i].y += m_glyphYMax;

		// Add offset of last character
		m_characterOffsets.back() = currentPos;

		// Set text size
		m_pixelSize = Vector2f(maxWidth, currentPos.y + m_characterSize);
		m_useRelSize = Vector2b(false);

		// The size probably changed, so mark transforms dirty
		markTransformDirty();

		// Check if height changed during the time the quad update occured
		if (texture && m_textureHeight != m_texture->getHeight())
			// If it did, need to update again
			updateQuads();

		// Update texture height;
		if (m_texture)
			m_textureHeight = m_texture->getHeight();

		m_stringChanged = false;
	}
}


///////////////////////////////////////////////////////////
void Text::updateTransforms()
{
	// Update quads before updating transforms to ensure correct bounds are used
	updateQuads();

	if (m_transformChanged)
	{
		// Update origin before updating other transforms
		if (m_isCentered)
			m_origin.y = 0.5f * m_glyphYMax / m_characterSize;

		// Regular update
		UIElement::updateTransforms();

		// Snap position to pixel
		m_absPosition.x = std::round(m_absPosition.x);
		m_absPosition.y = std::round(m_absPosition.y);

		// Calculate origin offset
		Vector2f offset = -m_origin * m_pixelSize;
		offset.x = std::round(offset.x);
		offset.y = std::round(offset.y);

		// Text rotation
		float angle = -rad(m_absRotation);
		float ca = cos(angle);
		float sa = sin(angle);

		// Add quads
		for (Uint32 i = 0; i < m_quads.size(); ++i)
		{
			// Calculate rotated position
			Vector2f p = m_characterOffsets[i] + offset;
			p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);
			p += m_absPosition;

			// Set position
			m_quads[i].m_position = p;

			// Set rotation
			m_quads[i].m_rotation = m_absRotation;
		}
	}
}


///////////////////////////////////////////////////////////
Shader* Text::getTextShader()
{
	if (!s_textShader.getId())
	{
		s_textShader.load("poly/ui.vert", SHADER_UI_VERT, Shader::Vertex);
		s_textShader.load("poly/ui.geom", SHADER_UI_GEOM, Shader::Geometry);
		s_textShader.load("poly/text.frag", SHADER_TEXT_FRAG, Shader::Fragment);
		s_textShader.compile();
	}

	return &s_textShader;
}


///////////////////////////////////////////////////////////
void Text::setDefaultFont(Font* font)
{
	s_defaultFont = font;
}



}