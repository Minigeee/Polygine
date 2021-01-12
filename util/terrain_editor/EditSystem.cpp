#include <poly/Core/Profiler.h>

#include "EditSystem.h"


///////////////////////////////////////////////////////////
void blendHeightMaps(Image& src, Image& dst, Image& output, Uint32 func)
{
    Uint32 size = src.getWidth() * src.getHeight();
    float* srcData = (float*)src.getData();
    float* dstData = (float*)dst.getData();
    float* outData = (float*)output.getData();

    if (func == 0)
    {
        for (Uint32 i = 0; i < size; ++i)
            outData[i] = dstData[i] + srcData[i];
    }
    else if (func == 1)
    {
        for (Uint32 i = 0; i < size; ++i)
            outData[i] = dstData[i] - srcData[i];
    }
    else if (func == 2)
    {
        int w = src.getWidth();
        int h = src.getHeight();

        for (int r = 0; r < h; ++r)
        {
            for (int c = 0; c < w; ++c)
            {
                // Calculate kernel size
                Uint32 kernel = (Uint32)(srcData[r * w + c] * 20 + 1);

                // Calculate area bounds
                Vector2i rBounds = Vector2i((int)r - kernel / 2, (int)r + kernel / 2);
                Vector2i cBounds = Vector2i((int)c - kernel / 2, (int)c + kernel / 2);
                if (rBounds.x < 0)
                    rBounds.x = 0;
                if (rBounds.y >= h)
                    rBounds.y = h - 1;
                if (cBounds.x < 0)
                    cBounds.x = 0;
                if (cBounds.y >= w)
                    cBounds.y = w;

                // Calculate average of kernel
                float avg = 0.0f;
                for (int r2 = rBounds.x; r2 <= rBounds.y; ++r)
                {
                    for (int c2 = cBounds.x; c2 <= cBounds.y; ++c)
                        avg += dstData[r2 * w + c2];
                }

                outData[r * w + c] = avg / (kernel * kernel);
            }
        }
    }
}


///////////////////////////////////////////////////////////
void blendColorMaps(Image& src, Image& dst, Image& output, const Vector3f& color)
{
    Uint32 size = src.getWidth() * src.getHeight();
    Uint8* srcData = (Uint8*)src.getData();
    Vector3<Uint8>* dstData = (Vector3<Uint8>*)dst.getData();
    Vector3<Uint8>* outData = (Vector3<Uint8>*)output.getData();

    for (Uint32 i = 0; i < size; ++i)
    {
        float alpha = srcData[i] / 255.0f;
        Vector3f dstPixel = Vector3f(dstData[i]) / 255.0f;

        // Set output data
        outData[i] = Vector3<Uint8>(255.0f * (color * alpha + dstPixel * (1.0f - alpha)));
    }
}


///////////////////////////////////////////////////////////
EditSystem::EditSystem(Terrain* terrain, BrushPanel* panel) :
	m_terrain		(terrain),
	m_panel			(panel),
    m_heightMapSize (1024),
    m_colorMapSize  (1024),
    m_brushPos      (0),
	m_historyIndex	(0)
{
    m_heightMap.create(0, m_heightMapSize, m_heightMapSize, 1, GLType::Float);
    m_heightMapCanvas.create(0, m_heightMapSize, m_heightMapSize, 1, GLType::Float);
    m_heightMapSrc.create(0, m_heightMapSize, m_heightMapSize, 1, GLType::Float);

    m_colorMap.create(0, m_colorMapSize, m_colorMapSize, 3);
    m_colorMapCanvas.create(0, m_colorMapSize, m_colorMapSize, 1);
    m_colorMapSrc.create(0, m_colorMapSize, m_colorMapSize, 3);

    memset(m_heightMap.getData(), 0, m_heightMapSize * m_heightMapSize * sizeof(float));
    memset(m_heightMapCanvas.getData(), 0, m_heightMapSize * m_heightMapSize * sizeof(float));
    memset(m_heightMapSrc.getData(), 0, m_heightMapSize * m_heightMapSize * sizeof(float));

    memset(m_colorMap.getData(), 0xFF, m_colorMapSize * m_colorMapSize * 3);
    memset(m_colorMapCanvas.getData(), 0xFF, m_colorMapSize * m_colorMapSize * 1);
    memset(m_colorMapSrc.getData(), 0xFF, m_colorMapSize * m_colorMapSize * 3);

    // Set terrain maps
    m_terrain->setHeightMap(m_heightMap);
    m_terrain->setColorMap(m_colorMap);
}


///////////////////////////////////////////////////////////
void EditSystem::finishStroke()
{
    Uint32 mode = m_panel->getMode();

    if (mode == 0)
    {
        // Reset canvas
        memset(m_heightMapCanvas.getData(), 0, m_heightMapSize * m_heightMapSize * sizeof(float));

        // Copy output to source
        memcpy(m_heightMapSrc.getData(), m_heightMap.getData(), m_heightMapSize * m_heightMapSize * sizeof(float));
    }
    else if (mode == 2)
    {
        memset(m_colorMapCanvas.getData(), 0xFF, m_colorMapSize * m_colorMapSize * 1);
    }
}


///////////////////////////////////////////////////////////
void EditSystem::moveBrush(const Vector2f& pos)
{
    // Get brush mode
    Uint32 mode = m_panel->getMode();

    // Get texture coordinates
    Vector2f size;
    if (mode == 0)
        size = Vector2f(m_heightMap.getWidth(), m_heightMap.getHeight());
    else if (mode == 1)
        size = Vector2f(m_colorMap.getWidth(), m_colorMap.getHeight());

    Vector2i texCoord = Vector2i((pos / m_terrain->getSize() + 0.5f) * size);

    if (texCoord == m_brushPos) return;
    m_brushPos = texCoord;

    // Fill in circle area
    int radius = (int)m_panel->getRadius();
    float gradient = m_panel->getGradient();
    float strength = m_panel->getStrength();

    for (int r = texCoord.y - radius; r <= texCoord.y + radius; ++r)
    {
        int y = r - texCoord.y;
        int W = lround(sqrt(radius * radius - y * y));

        for (int c = texCoord.x - W; c <= texCoord.x + W; ++c)
        {
            // Calculate pixel radius
            int x = c - texCoord.x;
            float R = (float)sqrt(x * x + y * y);

            // Calculate factor
            float factor = 1.0f / (1.0f + exp(-gradient * (1.0f - R / radius - 0.5f)));
            factor -= 1.0f / (1.0f + exp(0.5f * gradient));

            if (mode == 0)
            {
                float& pixel = *(float*)m_heightMapCanvas.getPixel(r, c);
                float value = factor * strength;

                if (pixel < value)
                    pixel = value;
            }
            else if (mode == 1)
            {
                Uint8& pixel = *(Uint8*)m_colorMapCanvas.getPixel(r, c);

                float value = (float)pixel / 255.0f + factor * strength;
                if (value > strength)
                    value = strength;

                pixel = (Uint8)(value * 255.0f);
            }
        }
    }

    START_PROFILING_FUNC;
    if (mode == 0)
        blendHeightMaps(m_heightMapCanvas, m_heightMapSrc, m_heightMap, 0);
    else if (mode == 1)
        blendColorMaps(m_colorMapCanvas, m_colorMapSrc, m_colorMap, Vector3f(0.0f));

    // Calculate update bounds
    Vector2u brushBoundsPos, brushBoundsSize;
    brushBoundsPos.x = texCoord.x > radius ? (Uint32)(texCoord.x - radius) : 0;
    brushBoundsPos.y = texCoord.y > radius ? (Uint32)(texCoord.y - radius) : 0;
    brushBoundsSize.x = texCoord.x + radius < (int)m_heightMap.getWidth() ? 2 * radius : m_heightMap.getWidth() - texCoord.x;
    brushBoundsSize.y = texCoord.y + radius < (int)m_heightMap.getHeight() ? 2 * radius : m_heightMap.getHeight() - texCoord.y;

    m_terrain->updateHeightMap(m_heightMap, brushBoundsPos, brushBoundsSize);
}