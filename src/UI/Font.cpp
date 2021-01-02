#include <poly/Core/Logger.h>

#include <poly/UI/Font.h>

#include <freetype/freetype.h>
#include FT_BITMAP_H

namespace poly
{

#ifndef DOXYGEN_SKIP

namespace priv
{

struct FTGlyph
{
    Uint8* m_data;
    float m_advance;
    Vector4i m_rect;
    int m_pitch;
};

}

#endif


///////////////////////////////////////////////////////////
Font::Glyph::Glyph() :
    m_advance       (0.0f),
    m_glyphRect     (0.0f),
    m_textureRect   (0.0f)
{

}


///////////////////////////////////////////////////////////
Font::Font() :
    m_library       (0),
    m_face          (0),
    m_pool          (sizeof(Page), 4),
    m_characterSize (0)
{

}


///////////////////////////////////////////////////////////
Font::~Font()
{
    // Remove font face
    if (m_face)
        FT_Done_Face(static_cast<FT_Face>(m_face));

    // Close the library
    if (m_library)
        FT_Done_FreeType(static_cast<FT_Library>(m_library));
}


///////////////////////////////////////////////////////////
bool Font::load(const std::string& fname, CharacterSet set)
{
    if (set == CharacterSet::English)
    {
        m_characters.reserve(128 - 32);
        for (Uint32 i = 32; i < 127; ++i)
            m_characters.push_back(i);

        return load(fname, m_characters);
    }

    return false;
}


///////////////////////////////////////////////////////////
bool Font::load(const std::string& fname, const std::vector<Uint32>& set)
{
    // Initialize library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOG_ERROR("Could not initialize FreeType");
        return false;
    }
    m_library = ft;

    // Load font face
    FT_Face face;
    if (FT_New_Face(ft, fname.c_str(), 0, &face))
    {
        LOG_ERROR("Failed to load font %s", fname.c_str());
        return false;
    }
    m_face = face;

    // Set character set
    m_characters = set;

    LOG("Loaded font: %s", fname.c_str());
    return true;
}


///////////////////////////////////////////////////////////
const Font::Glyph& Font::getGlyph(Uint32 c, Uint32 size)
{
    // Load the glyphs if it isn't loaded yet
    auto it = m_pages.find(size);
    if (it == m_pages.end())
    {
        loadGlyphs(size);
        it = m_pages.find(size);
    }

    // Return the glyph
    return it.value()->m_glyphs[c];
}


///////////////////////////////////////////////////////////
Texture& Font::getTexture(Uint32 size)
{
    // Load the glyphs if it isn't loaded yet
    auto it = m_pages.find(size);
    if (it == m_pages.end())
    {
        loadGlyphs(size);
        it = m_pages.find(size);
    }

    // Return the texture
    return it.value()->m_texture;
}


///////////////////////////////////////////////////////////
void Font::loadGlyphs(Uint32 size)
{
    FT_Library library = (FT_Library)m_library;
    FT_Face face = (FT_Face)m_face;

    if (m_characterSize != size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);
        m_characterSize = size;
    }

    // A vector of glyphs
    std::vector<priv::FTGlyph> glyphData(m_characters.size());

    // Keep track of overall size of texture
    Vector2u textureSize;

    // Load the set of glyphs
    for (Uint32 i = 0; i < m_characters.size(); ++i)
    {
        // Get character
        Uint32 c = m_characters[i];

        // Load the glyph
        if (FT_Load_Char(face, c, FT_LOAD_DEFAULT))
        {
            LOG_WARNING("Failed to load character: %d", c);
            continue;
        }

        // Render the glyph
        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD))
        {
            LOG_WARNING("Failed to render character: %d", c);
            continue;
        }

        // Create the glyph object
        priv::FTGlyph& glyph = glyphData[i];
        glyph.m_rect.x = face->glyph->bitmap_left;
        glyph.m_rect.y = face->glyph->bitmap_top;
        glyph.m_rect.z = face->glyph->bitmap.width;
        glyph.m_rect.w = face->glyph->bitmap.rows;
        glyph.m_advance = (float)(face->glyph->advance.x >> 6);
        glyph.m_pitch = face->glyph->bitmap.pitch;
        if (glyph.m_pitch < 0)
            glyph.m_pitch = -glyph.m_pitch;

        // Allocate space for glyph
        Uint32 numPixels = glyph.m_pitch * glyph.m_rect.w;
        glyph.m_data = (Uint8*)malloc(numPixels);
        memcpy(glyph.m_data, face->glyph->bitmap.buffer, numPixels);

        // Keep track of texture size
        textureSize.x += face->glyph->bitmap.width + 3;
        if (face->glyph->bitmap.rows > textureSize.y)
            textureSize.y = face->glyph->bitmap.rows;
    }

    // Create a page
    Page* page = (Page*)m_pool.alloc();
    new(page)Page();

    // Create a texture
    textureSize.x = textureSize.x / 3 * 4;
    Uint8* data = (Uint8*)malloc(textureSize.x * textureSize.y);
    memset(data, 0, textureSize.x * textureSize.y);
    Uint32 currentX = 0;

    for (Uint32 i = 0; i < m_characters.size(); ++i)
    {
        priv::FTGlyph& ftGlyph = glyphData[i];

        // Copy the data from the glyph
        Uint32 width = ftGlyph.m_rect.z / 3;
        for (Uint32 r = 0; r < (Uint32)ftGlyph.m_rect.w; ++r)
        {
            Uint8* dstRow = data + r * textureSize.x + currentX;
            Uint8* srcRow = ftGlyph.m_data + r * ftGlyph.m_pitch;

            for (Uint32 c = 0; c < width; ++c)
            {
                Uint8* dst = dstRow + c * 4;
                Uint8* src = srcRow + c * 3;

                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];

                // Set alpha value
                dst[3] = (Uint8)(((float)dst[0] + (float)dst[1] + (float)dst[2]) / 3.0f);
            }
        }

        // Free the glyph memory
        free(ftGlyph.m_data);

        // Create an actual glyph object
        Glyph& glyph = page->m_glyphs[m_characters[i]];
        glyph.m_advance = ftGlyph.m_advance;
        glyph.m_glyphRect = ftGlyph.m_rect;
        // glyph.m_glyphRect.x /= 3.0f;
        glyph.m_glyphRect.z /= 3.0f;

        // Texture rectangle
        glyph.m_textureRect.x = (float)currentX / textureSize.x;
        glyph.m_textureRect.y = 0.0f;
        glyph.m_textureRect.z = (float)ftGlyph.m_rect.z / textureSize.x * 4.0f / 3.0f;
        glyph.m_textureRect.w = (float)ftGlyph.m_rect.w / textureSize.y;

        // Update current x
        currentX += ftGlyph.m_rect.z / 3 * 4 + 4;
    }

    // Upload data to the texture
    page->m_texture.create(data, PixelFormat::Rgba, textureSize.x / 4, textureSize.y);

    // Add page to pages
    m_pages[size] = page;

    // Free texture data
    free(data);
}


///////////////////////////////////////////////////////////
float Font::getKerning(Uint32 c1, Uint32 c2, Uint32 size)
{
    FT_Face face = (FT_Face)m_face;

    if (m_characterSize != size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);
        m_characterSize = size;
    }

    if (face && FT_HAS_KERNING(face))
    {
        Uint32 index1 = FT_Get_Char_Index(face, c1);
        Uint32 index2 = FT_Get_Char_Index(face, c2);

        FT_Vector kerning;
        FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        return (float)kerning.x;
    }

    return 0.0f;
}


}