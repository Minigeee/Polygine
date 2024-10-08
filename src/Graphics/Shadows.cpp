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
	// Make enough space for 10 blocks
	Uint32 align = UniformBuffer::getUniformBlockAlignment();
	Uint32 size = (sizeof(UniformBlock_Shadows) + align - 1) / align * align;
	m_uniformBuffer.create(size * 10);
}


///////////////////////////////////////////////////////////
Shadows::~Shadows()
{
	for (auto it = m_shadowInfo.begin(); it != m_shadowInfo.end(); ++it)
	{
		std::vector<FrameBuffer*>& shadowMaps = it.value().m_shadowMaps;

		for (Uint32 i = 0; i < shadowMaps.size(); ++i)
		{
			Pool<Texture>::free(shadowMaps[i]->getDepthTexture());
			Pool<FrameBuffer>::free(shadowMaps[i]);
		}
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
			// Add number of lights used (even if shadows are disabled)
			++lightNum;

			// Skip if disabled, or if 2 lights already enabled
			if (!light.m_shadowsEnabled || lightNum > 2)
			{
				auto it = m_shadowInfo.find(id);
				if (it != m_shadowInfo.end())
					// Set as inactive
					it.value().m_isActive = false;

				return;
			}

			// Create light camera
			Camera lightCamera;
			lightCamera.setDirection(light.m_direction);
			lightCamera.setPosition(camera.getPosition());

			// Calculate base distance unit
			float denom = 0.0f;
			for (Uint32 i = 0; i < light.m_shadowCascades; ++i)
				denom += powf(light.m_cascadeDistMultiplier, (float)i);
			float distUnit = (light.m_shadowDistance - camera.getNear()) / denom;

			// Initial near and far plane
			float near = camera.getNear();
			float far = near + distUnit;

			// Get shadow info
			ShadowInfo* info = &m_shadowInfo[id];
			info->m_shadowStrength = light.m_shadowStrength;
			info->m_cameraProj = camera.getProjMatrix();
			info->m_lightIndex = lightNum - 1;
			info->m_isActive = true;

			for (Uint32 cascade = 0; cascade < light.m_shadowCascades; ++cascade)
			{
				if (cascade >= info->m_shadowMaps.size())
				{
					FrameBuffer* shadowMap = Pool<FrameBuffer>::alloc();
					shadowMap->create(light.m_shadowResolution, light.m_shadowResolution);
					shadowMap->attachDepth(Pool<Texture>::alloc());

					info->m_shadowMaps.push_back(shadowMap);
					info->m_lightProjViews.push_back(Matrix4f(1.0f));
					info->m_shadowDists.push_back(0.0f);
					info->m_isActive = true;
				}

				// Resize framebuffers if needed
				if (light.m_shadowResolution != info->m_shadowMaps[cascade]->getWidth())
				{
					FrameBuffer* shadowMap = info->m_shadowMaps[cascade];
					shadowMap->reset();
					shadowMap->create(light.m_shadowResolution, light.m_shadowResolution);
					shadowMap->attachDepth(shadowMap->getDepthTexture());
				}

				// Calculate vertices
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

				// Update new near and far planes
				near = far;
				far += powf(light.m_cascadeDistMultiplier, (float)(cascade + 1)) * distUnit;

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
				info->m_lightProjViews[cascade] = lightCamera.getProjMatrix() * lightCamera.getViewMatrix();
				info->m_shadowDists[cascade] = near;

				// Render the scene
				m_scene->render(lightCamera, *info->m_shadowMaps[cascade], RenderPass::Shadow);
			}
		}
	);


	// Only update uniform buffer if shadows are enabled (when there's a positive number of active lights)
	if (m_shadowInfo.size())
	{
		// Update uniform block
		UniformBlock_Shadows block;

		// Apply shadow maps to shader
		for (auto it = m_shadowInfo.begin(); it != m_shadowInfo.end(); ++it)
		{
			ShadowInfo& info = it.value();
			Uint32 i = info.m_lightIndex;

			// Skip if inactive
			if (!info.m_isActive)
			{
				block.m_shadowsEnabled[i] = false;
				continue;
			}

			for (Uint32 cascade = 0; cascade < info.m_shadowMaps.size(); ++cascade)
			{
				Vector4f projCoords = info.m_cameraProj * Vector4f(0.0f, 0.0f, -info.m_shadowDists[cascade], 1.0f);

				Uint32 index = i * 3 + cascade;
				block.m_lightProjViews[index] = info.m_lightProjViews[cascade];
				block.m_shadowDists[index] = projCoords.z;
			}

			block.m_shadowStrengths[i] = info.m_shadowStrength;
			block.m_numShadowCascades[i] = (int)info.m_shadowMaps.size();
			block.m_shadowsEnabled[i] = true;
		}

		// Push data
		m_uniformBuffer.pushData(block);
	}
}


///////////////////////////////////////////////////////////
void Shadows::apply(Shader* shader)
{
	// Bind uniform block
	shader->bindUniformBlock("Shadows", m_uniformBuffer);

	// Apply shadow maps to shader
	for (auto it = m_shadowInfo.begin(); it != m_shadowInfo.end(); ++it)
	{
		ShadowInfo& info = it.value();
		Uint32 i = info.m_lightIndex;

		// Skip if inactive
		if (!info.m_isActive)
			continue;

		for (Uint32 cascade = 0; cascade < info.m_shadowMaps.size(); ++cascade)
		{
			Uint32 index = i * 3 + cascade;
			std::string indexStr = '[' + std::to_string(index) + ']';
			shader->setUniform("u_shadowMaps" + indexStr, *info.m_shadowMaps[cascade]->getDepthTexture());
		}
	}
}


}