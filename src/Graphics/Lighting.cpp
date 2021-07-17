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
	m_ambientColor			(0.02f)
{
	// Make enough space for 10 blocks
	Uint32 align = UniformBuffer::getUniformBlockAlignment();
	Uint32 size = (sizeof(UniformBlock_Lights) + align - 1) / align * align;
	m_uniformBuffer.create(size * 10);
}


///////////////////////////////////////////////////////////
void Lighting::update(Camera& camera, Uint32 maxPointLights)
{
	// Need a scene
	if (!m_scene) return;

	// Create uniform block
	UniformBlock_Lights block;
	bool blockChanged = false;

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

			blockChanged |= (
				dst.m_diffuse != m_cache.m_dirLights[i].m_diffuse ||
				dst.m_specular != m_cache.m_dirLights[i].m_specular ||
				dst.m_direction != m_cache.m_dirLights[i].m_direction
			);

			++i;
		}
	);
	block.m_numDirLights = i;
	blockChanged |= (block.m_numDirLights != m_cache.m_numDirLights);

	// Maximum number of point lights allowed
	int maxNumPointLights = sizeof(UniformBlock_Lights::m_pointLights) / sizeof(UniformStruct_PointLight);
	maxNumPointLights = std::min((int)maxPointLights, maxNumPointLights);

	// Frustum for culling point lights
	const Frustum& frustum = camera.getFrustum();

	// Apply point lights
	i = 0;
	m_scene->system<TransformComponent, PointLightComponent>(
		[&](const Entity::Id id, TransformComponent& t, PointLightComponent& light)
		{
			// Can't add too many lights
			if (i >= maxNumPointLights)
				return;

			// Frustum culling (using sphere of radius where contributed brightness < 5% of vec3(1, 1, 1))
			const Vector3f& c = light.m_coefficients;
			float brightness = std::max(light.m_diffuse.r, std::max(light.m_diffuse.g, light.m_diffuse.b));
			float radius = -c.y + sqrtf(c.y * c.y - 4.0f * c.z * (c.x - 20.0f * brightness)) / (2.0f * c.z);

			if (frustum.contains(Sphere(t.m_position, radius)))
			{
				UniformStruct_PointLight& dst = block.m_pointLights[i];
				dst.m_position = t.m_position;
				dst.m_diffuse = light.m_diffuse;
				dst.m_specular = light.m_specular;
				dst.m_coefficients = light.m_coefficients;

				blockChanged |= (
					dst.m_position != m_cache.m_pointLights[i].m_position ||
					dst.m_diffuse != m_cache.m_pointLights[i].m_diffuse ||
					dst.m_specular != m_cache.m_pointLights[i].m_specular ||
					dst.m_coefficients != m_cache.m_pointLights[i].m_coefficients
					);

				++i;
			}
		}
	);
	block.m_numPointLights = i;
	blockChanged |= (block.m_numPointLights != m_cache.m_numPointLights);

	// Push data if changed
	if (blockChanged)
	{
		m_uniformBuffer.pushData(block);
		m_cache = block;
	}
}


///////////////////////////////////////////////////////////
void Lighting::apply(Shader* shader)
{
	// Bind uniform block
	shader->bindUniformBlock("Lights", m_uniformBuffer);
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