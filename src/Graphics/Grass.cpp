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

	const float m_grassRadius = 20.0f;
	const float m_grassSpacing = 0.1f;

	// Add grass data around origin
	std::vector<Vector2f> grassData;
	for (float y = -m_grassRadius; y < m_grassRadius; y += m_grassSpacing)
	{
		float w = sqrtf(m_grassRadius * m_grassRadius - y * y);

		for (float x = -w; x < w; x += m_grassSpacing)
			grassData.push_back(Vector2f(x, y));
	}

	m_vertexBuffer.create(grassData);

	// Create vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 2, sizeof(Vector2f), 0 * sizeof(Vector2f));
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
	shader.setUniform("u_grassRadius", 20.0f);
	shader.setUniform("u_grassSpacing", 0.1f);
	shader.setUniform("u_grassWidth", 0.03f);
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

	// Double side render
	glCheck(glDisable(GL_CULL_FACE));

	// Draw vertex array
	m_vertexArray.draw();

	glCheck(glEnable(GL_CULL_FACE));
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