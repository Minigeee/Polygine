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
	// Make enough space for 10 blocks
	Uint32 align = UniformBuffer::getUniformBlockAlignment();
	Uint32 size = (sizeof(UniformBlock_Lights) + align - 1) / align * align;
	m_uniformBuffer.setSize(size * 10);
}


///////////////////////////////////////////////////////////
void Lighting::update(Camera& camera)
{
	// Need a scene
	if (!m_scene) return;

	// Create uniform block
	UniformBlock_Lights block;

	// Apply main ambient color
	block.m_ambient = m_ambientColor;

	// Apply directional lights
	int i = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id id, DirLightComponent& light)
		{
			UniformStruct_DirLight& dst = block.m_dirLights[i];
			dst.m_diffuse = light.m_diffuse;
			dst.m_specular = light.m_specular;
			dst.m_direction = normalize(light.m_direction);

			++i;
		}
	);
	block.m_numDirLights = i;

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

			UniformStruct_PointLight& dst = block.m_pointLights[i];
			dst.m_position = t.m_position;
			dst.m_diffuse = light.m_diffuse * intensity;
			dst.m_specular = light.m_specular * intensity;
			dst.m_coefficients = light.m_coefficients;

			++i;
		}
	);
	block.m_numPointLights = i;

	// Push data
	m_uniformBuffer.pushData(block);
}


///////////////////////////////////////////////////////////
void Lighting::apply(Shader* shader)
{
	// Bind uniform block
	shader->setUniformBlock("Lights", m_uniformBuffer);
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