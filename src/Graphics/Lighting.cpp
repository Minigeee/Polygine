#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/Shader.h>

namespace poly
{


///////////////////////////////////////////////////////////
Lighting::Lighting(Scene* scene) :
	Extension		(scene),
	m_ambientColor	(0.02f)
{

}


///////////////////////////////////////////////////////////
void Lighting::apply(Shader* shader)
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
const Vector3f& Lighting::getAmbientColor() const
{
	return m_ambientColor;
}


}