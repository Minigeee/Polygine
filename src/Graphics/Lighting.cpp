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

}


///////////////////////////////////////////////////////////
void Lighting::apply(Camera& camera, Shader* shader)
{
	// Need a scene
	if (!m_scene) return;

	// Apply main ambient color
	shader->setUniform("u_ambient", m_ambientColor);

	// Apply directional lights
	int i = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id id, DirLightComponent& light)
		{
			std::string prefix = "u_dirLights[" + std::to_string(i++) + "].";

			shader->setUniform(prefix + "diffuse", light.m_diffuse);
			shader->setUniform(prefix + "specular", light.m_specular);
			shader->setUniform(prefix + "direction", normalize(light.m_direction));
		}
	);
	shader->setUniform("u_numDirLights", i);

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

			std::string prefix = "u_pointLights[" + std::to_string(i++) + "].";

			shader->setUniform(prefix + "position", t.m_position);
			shader->setUniform(prefix + "diffuse", light.m_diffuse * intensity);
			shader->setUniform(prefix + "specular", light.m_specular * intensity);
			shader->setUniform(prefix + "coefficients", light.m_coefficients);
		}
	);
	shader->setUniform("u_numPointLights", i);
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