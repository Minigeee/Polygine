#include <poly/Core/Logger.h>
#include <poly/Core/Utf.h>

#include <poly/UI/Font.h>

#include <freetype/freetype.h>
#include FT_BITMAP_H

#define MAX_TEXTURE_WIDTH 2048

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
    m_textureRecti  (0),
    m_textureRectf  (0.0f)
{

}


///////////////////////////////////////////////////////////
Font::Font() :
    m_library       (0),
    m_face          (0),
    m_pagePool      (4),
    m_texturePool   (4),
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

    m_face = 0;
    m_library = 0;
}


///////////////////////////////////////////////////////////
bool Font::load(const std::string& fname)
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

    LOG("Loaded font: %s", fname.c_str());
    return true;
}


///////////////////////////////////////////////////////////
const Font::Glyph& Font::getGlyph(Uint32 c, Uint32 size)
{
    // Try to find the glyph page
    auto it1 = m_pages.find(size);
    if (it1 == m_pages.end())
    {
        // If the page wasn't created load the glyph and return a ref
        loadGlyph(c, size);
        return m_pages[size]->m_glyphs[c];
    }

    Page* page = it1.value();

    // Try to find the glyph within the page
    auto it2 = page->m_glyphs.find(c);
    if (it2 == page->m_glyphs.end())
    {
        loadGlyph(c, size);
        return page->m_glyphs[c];
    }
    else
        return it2->second;
}


///////////////////////////////////////////////////////////
Texture* Font::getTexture(Uint32 size) const
{
    auto it = m_pages.find(size);
    return it == m_pages.end() ? 0 : it->second->m_texture;
}


///////////////////////////////////////////////////////////
void Font::loadGlyph(Uint32 c, Uint32 size)
{
    FT_Library library = (FT_Library)m_library;
    FT_Face face = (FT_Face)m_face;

    // Find or create a page
    Page* page = 0;
    Vector2u textureSize;

    auto it = m_pages.find(size);
    if (it == m_pages.end())
    {
        // Create a new page
        page = m_pagePool.alloc();

        // Allocate texture space
        // Allocate twice as much height, just in case some characters are larger
        textureSize = Vector2u(MAX_TEXTURE_WIDTH, 2 * size);
        page->m_textureData = (Uint8*)MALLOC_DBG(textureSize.x * textureSize.y * 3);
        memset(page->m_textureData, 0, textureSize.x * textureSize.y * 3);

        // Create the initial texture
        page->m_texture = m_texturePool.alloc();
        page->m_texture->create(page->m_textureData, PixelFormat::Rgb, textureSize.x, textureSize.y);

        // Add to map
        m_pages[size] = page;
    }
    else
    {
        // Get page
        page = it->second;
        textureSize = Vector2u(page->m_texture->getWidth(), page->m_texture->getHeight());
    }

    // Set the character size
    if (m_characterSize != size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);
        m_characterSize = size;
    }

    // Load the glyph
    if (FT_Load_Char(face, c, FT_LOAD_DEFAULT))
    {
        LOG_WARNING("Failed to load character: %d", c);
        return;
    }

    // Render the glyph
    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD))
    {
        LOG_WARNING("Failed to render character: %d", c);
        return;
    }

    // Create the glyph object
    Glyph& glyph = page->m_glyphs[c];

    // Get data
    Uint8* data = face->glyph->bitmap.buffer;

    // Get the glyph rectangle
    glyph.m_glyphRect.x = (float)face->glyph->bitmap_left;
    glyph.m_glyphRect.y = (float)face->glyph->bitmap_top;
    glyph.m_glyphRect.z = (float)face->glyph->bitmap.width / 3.0f;
    glyph.m_glyphRect.w = (float)face->glyph->bitmap.rows;

    // Get the texture dimensions (not position yet because this can change)
    glyph.m_textureRecti.z = face->glyph->bitmap.width / 3;
    glyph.m_textureRecti.w = face->glyph->bitmap.rows;

    // The advance
    glyph.m_advance = (float)(face->glyph->advance.x >> 6);

    // Get the pitch value (the bitmap row size in bytes)
    int pitch = face->glyph->bitmap.pitch;
    if (pitch < 0)
        pitch = -pitch;

    // Check if the current texture coords have to be updated
    if (page->m_currentLoc.x + glyph.m_textureRecti.z > MAX_TEXTURE_WIDTH)
    {
        // Go down one row
        page->m_currentLoc.x = 0;
        page->m_currentLoc.y += page->m_currentRowSize + 1;

        // Reset the row size
        page->m_currentRowSize = 0;

        // Update texture size
        Uint32 oldHeight = textureSize.y;
        textureSize.y = page->m_currentLoc.y + 2 * size;

        // Allocate new data
        Uint8* newData = (Uint8*)MALLOC_DBG(textureSize.x * textureSize.y * 3);

        // Copy old data and free old data
        memcpy(newData, page->m_textureData, textureSize.x * oldHeight * 3);
        memset(newData + textureSize.x * oldHeight * 3, 0, textureSize.x * (textureSize.y - oldHeight) * 3);
        FREE_DBG(page->m_textureData);
        page->m_textureData = newData;

        // Recreate the texture with the new size
        page->m_texture->create(page->m_textureData, PixelFormat::Rgb, textureSize.x, textureSize.y);

        // Update the texture rectangles of each glyph
        HashMap<Uint32, Glyph>& glyphs = page->m_glyphs;
        for (auto it = glyphs.begin(); it != glyphs.end(); ++it)
        {
            Glyph& g = it.value();
            g.m_textureRectf.y = (float)g.m_textureRecti.y / textureSize.y;
            g.m_textureRectf.w = (float)g.m_textureRecti.w / textureSize.y;
            Uint32 a = 0;
        }
    }

    // Set texture rectangle pixel location
    glyph.m_textureRecti.x = page->m_currentLoc.x;
    glyph.m_textureRecti.y = page->m_currentLoc.y;
    if (glyph.m_textureRecti.w > page->m_currentRowSize)
        page->m_currentRowSize = glyph.m_textureRecti.w;

    // Set the texture rectangle in texture coords
    glyph.m_textureRectf.x = (float)glyph.m_textureRecti.x / textureSize.x;
    glyph.m_textureRectf.y = (float)glyph.m_textureRecti.y / textureSize.y;
    glyph.m_textureRectf.z = (float)glyph.m_textureRecti.z / textureSize.x;
    glyph.m_textureRectf.w = (float)glyph.m_textureRecti.w / textureSize.y;

    // Copy the data from the bitmap to the texture
    Uint8* tempData = (Uint8*)MALLOC_DBG(glyph.m_textureRecti.z * glyph.m_textureRecti.w * 3);
    for (Uint32 r = 0; r < (Uint32)glyph.m_textureRecti.w; ++r)
    {
        // Copy to the texture data
        Uint8* dstRow = page->m_textureData + (page->m_currentLoc.y + r) * textureSize.x * 3 + page->m_currentLoc.x * 3;
        Uint8* srcRow = data + r * pitch;
        memcpy(dstRow, srcRow, glyph.m_textureRecti.z * 3);

        // Copy to temporary data
        dstRow = tempData + r * glyph.m_textureRecti.z * 3;
        memcpy(dstRow, srcRow, glyph.m_textureRecti.z * 3);
    }

    // Update a subsection of the texture
    page->m_texture->update(tempData, page->m_currentLoc, Vector2u(glyph.m_textureRecti.z, glyph.m_textureRecti.w));
    FREE_DBG(tempData);

    // Update current location
    page->m_currentLoc.x += glyph.m_textureRecti.z + 1;
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

        return (float)(kerning.x >> 6);
    }

    return 0.0f;
}


///////////////////////////////////////////////////////////
Font::Page::Page() :
    m_currentLoc        (0),
    m_currentRowSize    (0),
    m_textureData       (0)
{

}


///////////////////////////////////////////////////////////
Font::Page::~Page()
{
    if (m_textureData)
        FREE_DBG(m_textureData);

    m_textureData = 0;
}


}