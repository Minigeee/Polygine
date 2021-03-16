#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/PostProcess.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Shadows.h>

#include <poly/Math/Functions.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shadows::Shadows(Scene* scene) :
	Extension			(scene)
{

}


///////////////////////////////////////////////////////////
Shadows::~Shadows()
{
	for (auto it = m_shadowInfo.begin(); it != m_shadowInfo.end(); ++it)
	{
		Pool<Texture>::free(it.value().m_shadowMap->getDepthTexture());
		Pool<FrameBuffer>::free(it.value().m_shadowMap);
	}
}


///////////////////////////////////////////////////////////
void Shadows::render(Camera& camera)
{
	// Calculate parameters
	Matrix4f invCameraView = inverse(camera.getViewMatrix());
	float tanHalfFovH = tan(rad(0.5f * camera.getFov()));
	float tanHalfFovV = tan(rad(0.5f * camera.getFov() / camera.getAspectRatio()));

	Uint32 lightNum = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id& id, const DirLightComponent& light)
		{
			// Get shadow info
			ShadowInfo* info = 0;

			auto it = m_shadowInfo.find(id);
			if (it == m_shadowInfo.end())
			{
				// Create the info struct if it doesn't exist
				info = &m_shadowInfo[id];
				info->m_lightProjView = Matrix4f(1.0f);
				info->m_shadowDist = 0.0f;

				info->m_shadowMap = Pool<FrameBuffer>::alloc();
				info->m_shadowMap->create(light.m_shadowResolution, light.m_shadowResolution);
				info->m_shadowMap->attachDepth(Pool<Texture>::alloc());
			}
			else
				info = &it.value();

			// Calculate vertices
			float near = camera.getNear();
			float far = light.m_shadowDistance;
			float xn = near * tanHalfFovH;
			float yn = near * tanHalfFovV;
			float xf = far * tanHalfFovH;
			float yf = far * tanHalfFovV;

			Vector4f vertices[] =
			{
				Vector4f(-xn, -yn, -near, 1.0f),
				Vector4f(-xn, yn, -near, 1.0f),
				Vector4f(xn, -yn, -near, 1.0f),
				Vector4f(xn, yn, -near, 1.0f),
				Vector4f(-xf, -yf, -far, 1.0f),
				Vector4f(-xf, yf, -far, 1.0f),
				Vector4f(xf, -yf, -far, 1.0f),
				Vector4f(xf, yf, -far, 1.0f)
			};

			// Create light camera
			Camera lightCamera;
			lightCamera.setDirection(light.m_direction);
			lightCamera.setPosition(camera.getPosition());

			// Calculate light ortho bounds
			Vector3f min(std::numeric_limits<float>::max());
			Vector3f max(-std::numeric_limits<float>::max());
			for (Uint32 i = 0; i < 8; ++i)
			{
				Vector4f vertex = lightCamera.getViewMatrix() * invCameraView * vertices[i];

				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);
				min.z = std::min(min.z, vertex.z);
				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
				max.z = std::max(max.z, vertex.z);
			}

			lightCamera.setOrthographic(min.x, max.x, min.y, max.y, -200.0f, -min.z);
			info->m_lightProjView = lightCamera.getProjMatrix() * lightCamera.getViewMatrix();
			info->m_shadowDist = light.m_shadowDistance;

			// Render the scene
			m_scene->render(lightCamera, *info->m_shadowMap, RenderPass::Shadow);
		}
	);
}


///////////////////////////////////////////////////////////
void Shadows::apply(Shader* shader)
{
	shader->setUniform("u_numShadows", (int)m_shadowInfo.size());

	Uint32 i = 0;
	for (auto it = m_shadowInfo.begin(); it != m_shadowInfo.end(); ++it)
	{
		std::string indexStr = '[' + std::to_string(i++) + ']';
		shader->setUniform("u_shadowMaps" + indexStr, *it.value().m_shadowMap->getDepthTexture());
		shader->setUniform("u_lightProjViews" + indexStr, it.value().m_lightProjView);
		shader->setUniform("u_shadowDists" + indexStr, it.value().m_shadowDist);
	}
}


}