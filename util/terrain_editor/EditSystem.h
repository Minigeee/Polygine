#ifndef TERRAIN_EDITOR_EDIT_SYSTEM_H
#define TERRAIN_EDITOR_EDIT_SYSTEM_H

#include <poly/Graphics/Terrain.h>

#include "BrushPanel.h"

using namespace poly;

class EditSystem
{
public:
	EditSystem(Terrain* terrain, BrushPanel* panel);

	~EditSystem();

	void setTerrainSize(float size);

	void setTerrainHeight(float height);

	void setHeightMapSize(Uint32 size);

	void setColorMapSize(Uint32 size);

	void finishStroke();

	void moveBrush(const Vector2f& pos);

	void undo();

	void redo();

private:
	struct MapState
	{
		Uint32 m_mode;
		Vector2i m_min;
		Vector2i m_max;
		void* m_data;
	};

private:
	Terrain* m_terrain;
	BrushPanel* m_panel;
	Image m_heightMap;
	Image m_colorMap;
	Image m_heightMapCanvas;
	Image m_colorMapCanvas;
	Image m_heightMapSrc;
	Image m_colorMapSrc;
	Uint32 m_heightMapSize;
	Uint32 m_colorMapSize;

	Vector2i m_brushPos;
	Vector2i m_brushMin;
	Vector2i m_brushMax;

	Uint32 m_historyIndex;
	Uint32 m_historyMemSize;
	std::deque<MapState> m_history;
};

#endif