#include <poly/Core/Profiler.h>

#include "EditSystem.h"

#define HISTORY_MEMORY_SIZE 64 * 1024 * 1024


///////////////////////////////////////////////////////////
void blendHeightMaps(Image& canvas, Image& input, Image& output, Uint32 func, const Vector2i& min, const Vector2i& max)
{
    Uint32 size = canvas.getWidth() * canvas.getHeight();
    float* canvasData = (float*)canvas.getData();
    float* inData = (float*)input.getData();
    float* outData = (float*)output.getData();

    if (func == 0)
    {
        for (int r = min.y; r < max.y; ++r)
        {
            for (int c = min.x; c < max.x; ++c)
            {
                Uint32 index = r * canvas.getWidth() + c;
                outData[index] = inData[index] + canvasData[index];
            }
        }
    }
    else if (func == 1)
    {
        for (int r = min.y; r < max.y; ++r)
        {
            for (int c = min.x; c < max.x; ++c)
            {
                Uint32 index = r * canvas.getWidth() + c;
                outData[index] = inData[index] - canvasData[index];
            }
        }
    }
    else if (func == 2)
    {
        int w = canvas.getWidth();
        int h = canvas.getHeight();

        for (int r = min.y; r < max.y; ++r)
        {
            for (int c = min.x; c < max.x; ++c)
            {
                // Calculate kernel size
                Uint32 kernel = (Uint32)(canvasData[r * w + c] * 20 + 3);

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
                int i = 0;
                for (int r2 = rBounds.x; r2 <= rBounds.y; ++r2)
                {
                    for (int c2 = cBounds.x; c2 <= cBounds.y; ++c2, ++i)
                        avg += inData[r2 * w + c2];
                }

                outData[r * w + c] = avg / (float)i;
            }
        }
    }
}


///////////////////////////////////////////////////////////
void blendColorMaps(Image& canvas, Image& input, Image& output, const Vector3f& color, const Vector2i& min, const Vector2i& max)
{
    float* canvasData = (float*)canvas.getData();
    Vector3<Uint8>* inData = (Vector3<Uint8>*)input.getData();
    Vector3<Uint8>* outData = (Vector3<Uint8>*)output.getData();

    for (int r = min.y; r < max.y; ++r)
    {
        for (int c = min.x; c < max.x; ++c)
        {
            int index = r * canvas.getWidth() + c;
            float alpha = canvasData[index];
            Vector3f in = Vector3f(inData[index]) / 255.0f;

            outData[index] = Vector3<Uint8>((color * alpha + in * (1.0f - alpha)) * 255.0f);
        }
    }
}


///////////////////////////////////////////////////////////
EditSystem::EditSystem(Terrain* terrain, BrushPanel* panel) :
	m_terrain		    (terrain),
	m_panel			    (panel),
    m_mapSize           (1024),
    m_brushPos          (0),
    m_brushMin          (-1),
    m_brushMax          (-1),
	m_historyIndex	    (0),
    m_historyMemSize    (0)
{
    m_canvasMap.create(0, m_mapSize, m_mapSize, 1, GLType::Float);

    m_heightMap.create(0, m_mapSize, m_mapSize, 1, GLType::Float);
    m_heightMapSrc.create(0, m_mapSize, m_mapSize, 1, GLType::Float);

    m_colorMap.create(0, m_mapSize, m_mapSize, 3);
    m_colorMapSrc.create(0, m_mapSize, m_mapSize, 3);

    memset(m_canvasMap.getData(), 0, m_mapSize * m_mapSize * sizeof(float));

    memset(m_heightMap.getData(), 0, m_mapSize * m_mapSize * sizeof(float));
    memset(m_heightMapSrc.getData(), 0, m_mapSize * m_mapSize * sizeof(float));

    memset(m_colorMap.getData(), 0xFF, m_mapSize * m_mapSize * 3);
    memset(m_colorMapSrc.getData(), 0xFF, m_mapSize * m_mapSize * 3);

    // Set terrain maps
    m_terrain->setHeightMap(m_heightMap);
    m_terrain->setColorMap(m_colorMap);
}


///////////////////////////////////////////////////////////
EditSystem::~EditSystem()
{
    // Free history
    for (Uint32 i = 0; i < m_history.size(); ++i)
        // Free memory
        free(m_history[i].m_data);
}


///////////////////////////////////////////////////////////
void EditSystem::finishStroke()
{
    Uint32 mode = m_panel->getMode();

    // If the history pointer is not a the end, clear all after the current
    if (m_historyIndex < m_history.size())
    {
        Uint32 size = m_history.size();
        for (Uint32 i = m_historyIndex; i < size; ++i)
        {
            // Free data
            free(m_history.back().m_data);

            // Pop
            m_history.pop_back();
        }

        m_historyIndex = m_history.size();
    }

    if (mode == 0)
    {
        // Copy data to a separate buffer
        Uint32 width = m_brushMax.x - m_brushMin.x;
        Uint32 height = m_brushMax.y - m_brushMin.y;
        Uint32 size = width * height * sizeof(float);
        float* data = (float*)malloc(size);
        float* newData = (float*)m_heightMap.getData();
        float* oldData = (float*)m_heightMapSrc.getData();

        for (int r = m_brushMin.y, i = 0; r < m_brushMax.y; ++r)
        {
            for (int c = m_brushMin.x; c < m_brushMax.x; ++c, ++i)
            {
                Uint32 index = r * m_heightMap.getWidth() + c;
                data[i] = newData[index] - oldData[index];
            }
        }

        // Update memory used
        m_historyMemSize += size;

        // If over the limit, remove earliest states
        if (m_historyMemSize > HISTORY_MEMORY_SIZE)
        {
            free(m_history.front().m_data);
            m_history.pop_front();

            --m_historyIndex;
        }

        // Save map state
        MapState state;
        state.m_mode = m_panel->getMode();
        state.m_min = m_brushMin;
        state.m_max = m_brushMax;
        state.m_data = data;
        m_history.push_back(state);

        // Copy output to source
        memcpy(m_heightMapSrc.getData(), m_heightMap.getData(), m_mapSize * m_mapSize * sizeof(float));
    }
    else if (mode == 1)
    {
        // Copy data to a separate buffer
        Uint32 width = m_brushMax.x - m_brushMin.x;
        Uint32 height = m_brushMax.y - m_brushMin.y;
        Uint32 size = width * height * 3;
        Vector3<Uint8>* data = (Vector3<Uint8>*)malloc(size);
        Vector3<Uint8>* newData = (Vector3<Uint8>*)m_colorMap.getData();
        Vector3<Uint8>* oldData = (Vector3<Uint8>*)m_colorMapSrc.getData();

        for (int r = m_brushMin.y, i = 0; r < m_brushMax.y; ++r)
        {
            for (int c = m_brushMin.x; c < m_brushMax.x; ++c, ++i)
            {
                Uint32 index = r * m_colorMap.getWidth() + c;
                data[i] = newData[index] - oldData[index];
            }
        }

        // Update memory used
        m_historyMemSize += size;

        // If over the limit, remove earliest states
        if (m_historyMemSize > HISTORY_MEMORY_SIZE)
        {
            free(m_history.front().m_data);
            m_history.pop_front();

            --m_historyIndex;
        }

        // Save map state
        MapState state;
        state.m_mode = m_panel->getMode();
        state.m_min = m_brushMin;
        state.m_max = m_brushMax;
        state.m_data = data;
        m_history.push_back(state);

        // Copy output to source
        memcpy(m_colorMapSrc.getData(), m_colorMap.getData(), m_mapSize * m_mapSize * 3);
    }

    // Reset canvas
    memset(m_canvasMap.getData(), 0, m_mapSize* m_mapSize * sizeof(float));

    m_brushMin = Vector2i(-1);
    m_brushMax = Vector2i(-1);

    // Update history index
    ++m_historyIndex;
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

    // Clamp texture coordinate
    if (texCoord.x < 0)
        texCoord.x = 0;
    if (texCoord.x > m_heightMap.getWidth())
        texCoord.x = m_heightMap.getWidth();
    if (texCoord.y < 0)
        texCoord.y = 0;
    if (texCoord.y > m_heightMap.getHeight())
        texCoord.y = m_heightMap.getHeight();

    if (texCoord == m_brushPos) return;
    m_brushPos = texCoord;

    START_PROFILING_FUNC;

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
            float offset = 1.0f / (1.0f + exp(0.5f * gradient));
            factor = (factor - offset) / (1.0f / (1.0f + exp(-0.5f * gradient)) - offset);

            float& pixel = *(float*)m_canvasMap.getPixel(r, c);
            if (pixel < factor * strength)
                pixel = factor * strength;
        }
    }

    // Calculate update bounds
    Vector2i brushBoundsPos, brushBoundsSize;
    brushBoundsPos.x = texCoord.x > radius ? texCoord.x - radius : 0;
    brushBoundsPos.y = texCoord.y > radius ? texCoord.y - radius : 0;
    brushBoundsSize.x = texCoord.x + radius < (int)m_heightMap.getWidth() ? 2 * radius : m_heightMap.getWidth() - texCoord.x;
    brushBoundsSize.y = texCoord.y + radius < (int)m_heightMap.getHeight() ? 2 * radius : m_heightMap.getHeight() - texCoord.y;

    if (m_brushMin.x < 0 || brushBoundsPos.x < m_brushMin.x)
        m_brushMin.x = brushBoundsPos.x;
    if (m_brushMin.y < 0 || brushBoundsPos.y < m_brushMin.y)
        m_brushMin.y = brushBoundsPos.y;
    if (m_brushMax.x < 0 || brushBoundsPos.x + brushBoundsSize.x > m_brushMax.x)
        m_brushMax.x = brushBoundsPos.x + brushBoundsSize.x;
    if (m_brushMax.y < 0 || brushBoundsPos.y + brushBoundsSize.y > m_brushMax.y)
        m_brushMax.y = brushBoundsPos.y + brushBoundsSize.y;

    if (mode == 0)
    {
        blendHeightMaps(m_canvasMap, m_heightMapSrc, m_heightMap, m_panel->getHeightFunc(), m_brushMin, m_brushMax);
        m_terrain->updateHeightMap(m_heightMap, brushBoundsPos, brushBoundsSize);
    }
    else if (mode == 1)
    {
        blendColorMaps(m_canvasMap, m_colorMapSrc, m_colorMap, m_panel->getSelectedColor(), m_brushMin, m_brushMax);
        m_terrain->updateColorMap(m_colorMap, brushBoundsPos, brushBoundsSize);
    }
}


///////////////////////////////////////////////////////////
void EditSystem::undo()
{
    // Don't undo if at beggining
    if (m_historyIndex == 0) return;

    // Get previous state
    const MapState& state = m_history[--m_historyIndex];

    // Subtract state offset
    if (state.m_mode == 0)
    {
        float* offset = (float*)state.m_data;
        float* data = (float*)m_heightMap.getData();
        float* src = (float*)m_heightMapSrc.getData();

        for (int r = state.m_min.y, i = 0; r < state.m_max.y; ++r)
        {
            for (int c = state.m_min.x; c < state.m_max.x; ++c, ++i)
            {
                int index = r * m_heightMap.getWidth() + c;
                src[index] = (data[index] -= offset[i]);
            }
        }

        // Update map
        m_terrain->updateHeightMap(m_heightMap, state.m_min, state.m_max - state.m_min);
    }
    else if (state.m_mode == 1)
    {
        Vector3<Uint8>* offset = (Vector3<Uint8>*)state.m_data;
        Vector3<Uint8>* data = (Vector3<Uint8>*)m_colorMap.getData();
        Vector3<Uint8>* src = (Vector3<Uint8>*)m_colorMapSrc.getData();

        for (int r = state.m_min.y, i = 0; r < state.m_max.y; ++r)
        {
            for (int c = state.m_min.x; c < state.m_max.x; ++c, ++i)
            {
                int index = r * m_colorMap.getWidth() + c;
                src[index] = (data[index] -= offset[i]);
            }
        }

        // Update map
        m_terrain->updateColorMap(m_colorMap, state.m_min, state.m_max - state.m_min);
    }
}


///////////////////////////////////////////////////////////
void EditSystem::redo()
{
    // Don't redo if at end
    if (m_historyIndex == m_history.size()) return;

    // Get previous state
    const MapState& state = m_history[m_historyIndex++];

    // Subtract state offset
    if (state.m_mode == 0)
    {
        float* offset = (float*)state.m_data;
        float* data = (float*)m_heightMap.getData();
        float* src = (float*)m_heightMapSrc.getData();

        for (int r = state.m_min.y, i = 0; r < state.m_max.y; ++r)
        {
            for (int c = state.m_min.x; c < state.m_max.x; ++c, ++i)
            {
                int index = r * m_heightMap.getWidth() + c;
                src[index] = (data[index] += offset[i]);
            }
        }

        // Update map
        m_terrain->updateHeightMap(m_heightMap, state.m_min, state.m_max - state.m_min);
    }
    else if (state.m_mode == 1)
    {
        Vector3<Uint8>* offset = (Vector3<Uint8>*)state.m_data;
        Vector3<Uint8>* data = (Vector3<Uint8>*)m_colorMap.getData();
        Vector3<Uint8>* src = (Vector3<Uint8>*)m_colorMapSrc.getData();

        for (int r = state.m_min.y, i = 0; r < state.m_max.y; ++r)
        {
            for (int c = state.m_min.x; c < state.m_max.x; ++c, ++i)
            {
                int index = r * m_colorMap.getWidth() + c;
                src[index] = (data[index] += offset[i]);
            }
        }

        // Update map
        m_terrain->updateColorMap(m_colorMap, state.m_min, state.m_max - state.m_min);
    }
}