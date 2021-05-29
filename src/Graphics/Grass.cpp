#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Grass.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/Shadows.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader Grass::s_shader;


///////////////////////////////////////////////////////////
Grass::Grass() :
	m_terrain			(0),
	m_ambientColor		(0.02f),
	m_grassSpacing		(0.1f),
	m_grassWidth		(0.15f),
	m_grassHeight		(0.8f),
	m_color				(0.0f),
	m_colorMap			(0),
	m_densityMap		(0),
	m_sizeMap			(0),
	m_hasDefaultColor	(false)
{
	m_lodDists.push_back(10.0f);
	m_lodDists.push_back(25.0f);
	m_lodDists.push_back(50.0f);
}


///////////////////////////////////////////////////////////
void Grass::init(Scene* scene)
{
	m_scene = scene;

	// Add grass data around origin
	std::vector<Vector3f> grassData;

	// Add grass for each lod layer
	for (Uint32 lodLevel = 0; lodLevel < m_lodDists.size(); ++lodLevel)
	{
		float radius = m_lodDists[lodLevel];
		float spacing = m_grassSpacing * powf(2.0f, (float)lodLevel);

		for (float y = -radius; y < radius; y += spacing)
		{
			float w = sqrtf(radius * radius - y * y);

			for (float x = -w; x < w; x += spacing)
			{
				float dist = sqrtf(x * x + y * y);

				if (lodLevel == 0 || dist > m_lodDists[lodLevel - 1])
					grassData.push_back(Vector3f(x, y, spacing));
			}
		}
	}

	// Sort front to back for more efficiency (far away transparency isn't noticeable)
	std::sort(grassData.begin(), grassData.end(),
		[](const Vector3f& a, const Vector3f& b) -> bool
		{
			float distA = a.x * a.x + a.y * a.y;
			float distB = b.x * b.x + b.y * b.y;

			return distA < distB;
		}
	);

	m_vertexBuffer.create(grassData);

	// Create vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 2, sizeof(Vector3f), 0 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 1, 1, sizeof(Vector3f), 2 * sizeof(float));
	m_vertexArray.setDrawMode(DrawMode::Points);
}


///////////////////////////////////////////////////////////
void Grass::render(Camera& camera, RenderPass pass)
{
	// Only render grass for default pass
	if (pass != RenderPass::Default) return;

	// Need to have terrain to render
	if (!m_terrain) return;

	Shader& shader = getShader();

	// Apply shader uniforms
	shader.bind();
	shader.setUniform("u_grassRadius", m_lodDists.back());
	shader.setUniform("u_grassSpacing", m_grassSpacing);
	shader.setUniform("u_grassWidth", m_grassWidth);
	shader.setUniform("u_grassHeight", m_grassHeight);
	shader.setUniform("u_time", m_clock.getElapsedTime().toSeconds());

	// Camera
	camera.apply(&shader);

	// Lighting
	m_scene->getExtension<Lighting>()->apply(&shader);
	m_scene->getExtension<Shadows>()->apply(&shader);

	// Terrain maps
	shader.setUniform("u_terrainSize", m_terrain->getSize());
	shader.setUniform("u_terrainHeight", m_terrain->getHeight());
	shader.setUniform("u_heightMap", m_terrain->getHeightMap());
	shader.setUniform("u_normalMap", m_terrain->getNormalMap());

	// Set color map
	if (m_colorMap)
	{
		shader.setUniform("u_colorMap", *m_colorMap);
		shader.setUniform("u_useColorMap", true);
	}
	else
	{
		if (m_hasDefaultColor)
		{
			shader.setUniform("u_grassColor", m_color);
			shader.setUniform("u_useColorMap", false);
		}
		else
		{
			shader.setUniform("u_colorMap", m_terrain->getColorMap());
			shader.setUniform("u_useColorMap", true);
		}
	}

	// Set density map
	if (m_densityMap)
	{
		shader.setUniform("u_densityMap", *m_densityMap);
		shader.setUniform("u_useDensityMap", true);
	}
	else
		shader.setUniform("u_useDensityMap", false);

	// Set size map
	if (m_densityMap)
	{
		shader.setUniform("u_sizeMap", *m_densityMap);
		shader.setUniform("u_useSizeMap", true);
	}
	else
		shader.setUniform("u_useSizeMap", false);


	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Double side render
	glCheck(glDisable(GL_CULL_FACE));

	// Enable alpha blending
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Draw vertex array
	m_vertexArray.draw();

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glDisable(GL_BLEND));
}


///////////////////////////////////////////////////////////
void Grass::setTerrain(Terrain* terrain)
{
	m_terrain = terrain;
}


///////////////////////////////////////////////////////////
void Grass::setAmbientColor(const Vector3f& color)
{
	m_ambientColor = color;
}


///////////////////////////////////////////////////////////
void Grass::setGrassSpacing(float spacing)
{
	m_grassSpacing = spacing;

	// Recreate the grass mesh
	if (m_scene)
		init(m_scene);
}


///////////////////////////////////////////////////////////
void Grass::setGrassWidth(float width)
{
	m_grassWidth = width;
}


///////////////////////////////////////////////////////////
void Grass::setGrassHeight(float height)
{
	m_grassHeight = height;
}


///////////////////////////////////////////////////////////
void Grass::setLodDistance(float dist, Uint32 lodLevel)
{
	if (lodLevel < m_lodDists.size())
		m_lodDists[lodLevel] = dist;
	else if (lodLevel == m_lodDists.size())
		m_lodDists.push_back(dist);

	// Recreate the grass mesh
	if (m_scene)
		init(m_scene);
}


///////////////////////////////////////////////////////////
void Grass::setColor(float r, float g, float b)
{
	m_color = Vector3f(r, g, b);
	m_hasDefaultColor = true;
}


///////////////////////////////////////////////////////////
void Grass::setColor(const Vector3f& color)
{
	m_color = color;
	m_hasDefaultColor = true;
}


///////////////////////////////////////////////////////////
void Grass::setColorMap(Texture* cmap)
{
	m_colorMap = cmap;
}


///////////////////////////////////////////////////////////
void Grass::setDensityMap(Texture* dmap)
{
	m_densityMap = dmap;
}


///////////////////////////////////////////////////////////
void Grass::setSizeMap(Texture* smap)
{
	m_sizeMap = smap;
}


///////////////////////////////////////////////////////////
float Grass::getGrassSpacing() const
{
	return m_grassSpacing;
}


///////////////////////////////////////////////////////////
float Grass::getGrassWidth() const
{
	return m_grassWidth;
}


///////////////////////////////////////////////////////////
float Grass::getGrassHeight() const
{
	return m_grassHeight;
}


///////////////////////////////////////////////////////////
float Grass::getLodDistance(Uint32 lodLevel) const
{
	return m_lodDists[lodLevel];
}


///////////////////////////////////////////////////////////
const Vector3f& Grass::getColor() const
{
	return m_color;
}


///////////////////////////////////////////////////////////
Texture* Grass::getColorMap() const
{
	return m_colorMap;
}


///////////////////////////////////////////////////////////
Texture* Grass::getDensityMap() const
{
	return m_densityMap;
}


///////////////////////////////////////////////////////////
Texture* Grass::getSizeMap() const
{
	return m_sizeMap;
}


///////////////////////////////////////////////////////////
Shader& Grass::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/grass.vert", Shader::Vertex);
		s_shader.load("shaders/grass.geom", Shader::Geometry);
		s_shader.load("shaders/grass.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


}