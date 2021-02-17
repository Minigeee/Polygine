#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Grass.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader Grass::s_shader;


///////////////////////////////////////////////////////////
Grass::Grass() :
	m_scene			(0),
	m_terrain		(0),
	m_ambientColor	(0.02f)
{

}


///////////////////////////////////////////////////////////
void Grass::init(Scene* scene)
{
	m_scene = scene;

	const float m_grassRadius = 50.0f;
	const float m_grassSpacing = 0.1f;

	float lodDists[] = { 10.0f, 25.0f, m_grassRadius };

	// Add grass data around origin
	std::vector<Vector3f> grassData;

	// Add grass for each lod layer
	for (Uint32 lodLevel = 0; lodLevel < 3; ++lodLevel)
	{
		float radius = lodDists[lodLevel];
		float spacing = m_grassSpacing * powf(2.0f, (float)lodLevel);

		for (float y = -radius; y < radius; y += spacing)
		{
			float w = sqrtf(radius * radius - y * y);

			for (float x = -w; x < w; x += spacing)
			{
				float dist = sqrtf(x * x + y * y);

				if (lodLevel == 0 || dist > lodDists[lodLevel - 1])
					grassData.push_back(Vector3f(x, y, spacing));
			}
		}
	}

	m_vertexBuffer.create(grassData);

	// Create vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 2, sizeof(Vector3f), 0 * sizeof(float));
	m_vertexArray.addBuffer(m_vertexBuffer, 1, 1, sizeof(Vector3f), 2 * sizeof(float));
	m_vertexArray.setDrawMode(DrawMode::Points);
}


///////////////////////////////////////////////////////////
void Grass::render(Camera& camera)
{
	Shader& shader = getShader();

	// Apply shader uniforms
	shader.bind();
	shader.setUniform("u_projView", camera.getProjMatrix() * camera.getViewMatrix());
	shader.setUniform("u_cameraPos", camera.getPosition());
	shader.setUniform("u_ambient", m_ambientColor);
	shader.setUniform("u_grassRadius", 50.0f);
	shader.setUniform("u_grassSpacing", 0.1f);
	shader.setUniform("u_grassWidth", 0.15f);
	shader.setUniform("u_grassHeight", 0.8f);
	shader.setUniform("u_time", m_clock.getElapsedTime().toSeconds());

	// Apply directional lights
	int i = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id id, DirLightComponent& light)
		{
			std::string prefix = "u_dirLights[" + std::to_string(i++) + "].";

			shader.setUniform(prefix + "diffuse", light.m_diffuse);
			shader.setUniform(prefix + "specular", light.m_specular);
			shader.setUniform(prefix + "direction", normalize(light.m_direction));
		}
	);
	shader.setUniform("u_numDirLights", i);

	// Terrain maps
	shader.setUniform("u_terrainSize", m_terrain->getSize());
	shader.setUniform("u_terrainHeight", m_terrain->getHeight());
	shader.setUniform("u_heightMap", m_terrain->getHeightMap());
	shader.setUniform("u_normalMap", m_terrain->getNormalMap());
	shader.setUniform("u_colorMap", m_terrain->getColorMap());

	// Double side render
	glCheck(glDisable(GL_CULL_FACE));
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