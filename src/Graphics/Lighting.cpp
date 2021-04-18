#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/Shader.h>

namespace poly
{


///////////////////////////////////////////////////////////
Lighting::Lighting(Scene* scene) :
	Extension				(scene),
	m_ambientColor			(0.02f),
	m_pointLightMaxDist		(30.0f)
{
	// Calculate uniform data size
	Uint32 size = 0;
	size += 4 * sizeof(float);
	size += 12 * 2 * sizeof(float);
	size += 16 * 32 * sizeof(float);
	size += 2 *sizeof(float);

	size = (size + 15) / 16 * 16;

	m_uniformBlock.setBufferSize(size * 10);
}


///////////////////////////////////////////////////////////
void Lighting::update(Camera& camera)
{
	// Need a scene
	if (!m_scene) return;

	// Apply main ambient color
	m_uniformBlock.addData(m_ambientColor);

	// Apply directional lights
	int i = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id id, DirLightComponent& light)
		{
			m_uniformBlock.addData(light.m_diffuse);
			m_uniformBlock.addData(light.m_specular);
			m_uniformBlock.addData(normalize(light.m_direction));

			++i;
		}
	);
	int numDirLights = i;

	// Fill the rest of the empty lights
	for (int i = numDirLights; i < 2; ++i)
	{
		m_uniformBlock.addData(Vector3f());
		m_uniformBlock.addData(Vector3f());
		m_uniformBlock.addData(Vector3f());
	}

	// Apply point lights
	i = 0;
	m_scene->system<TransformComponent, PointLightComponent>(
		[&](const Entity::Id id, TransformComponent& t, PointLightComponent& light)
		{
			float distance = dist(t.m_position, camera.getPosition());

			// Check if the light is within the max range
			if (distance > m_pointLightMaxDist)
				return;

			// Create a gradual fade out
			float intensity = 1.0f - (distance - 0.8f * m_pointLightMaxDist) / (0.2f * m_pointLightMaxDist);
			if (intensity > 1.0f)
				intensity = 1.0f;

			m_uniformBlock.addData(t.m_position);
			m_uniformBlock.addData(light.m_diffuse * intensity);
			m_uniformBlock.addData(light.m_specular * intensity);
			m_uniformBlock.addData(light.m_coefficients);

			++i;
		}
	);
	int numPointLights = i;

	// Fill the rest of the empty lights
	for (int i = numPointLights; i < 32; ++i)
	{
		m_uniformBlock.addData(Vector3f());
		m_uniformBlock.addData(Vector3f());
		m_uniformBlock.addData(Vector3f());
		m_uniformBlock.addData(Vector3f());
	}

	// Number of lights
	m_uniformBlock.addData(numDirLights);
	m_uniformBlock.addData(numPointLights);

	// Push data
	m_uniformBlock.update();
}


///////////////////////////////////////////////////////////
void Lighting::apply(Shader* shader)
{
	// Bind uniform block
	shader->setUniformBlock("Lights", m_uniformBlock);
}


///////////////////////////////////////////////////////////
void Lighting::setAmbientColor(const Vector3f& color)
{
	m_ambientColor = color;
}


///////////////////////////////////////////////////////////
void Lighting::setAmbientColor(float r, float g, float b)
{
	m_ambientColor = Vector3f(r, g, b);
}


///////////////////////////////////////////////////////////
void Lighting::setPointLightMaxDist(float dist)
{
	m_pointLightMaxDist = dist;
}


///////////////////////////////////////////////////////////
const Vector3f& Lighting::getAmbientColor() const
{
	return m_ambientColor;
}


}