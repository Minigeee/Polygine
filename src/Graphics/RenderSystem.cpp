#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>

namespace poly
{


///////////////////////////////////////////////////////////
RenderPass operator&(RenderPass a, RenderPass b)
{
	return (RenderPass)((Uint32)a & (Uint32)b);
}


///////////////////////////////////////////////////////////
RenderPass operator|(RenderPass a, RenderPass b)
{
	return (RenderPass)((Uint32)a | (Uint32)b);
}


///////////////////////////////////////////////////////////
RenderPass operator~(RenderPass a)
{
	return (RenderPass)(~(Uint32)a);
}


///////////////////////////////////////////////////////////
RenderSystem::RenderSystem() :
	m_scene		(0)
{

}


///////////////////////////////////////////////////////////
void RenderSystem::applyLighting(Shader* shader)
{
	// Need a scene
	if (!m_scene) return;

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


}