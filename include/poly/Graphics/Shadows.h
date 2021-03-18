#ifndef POLY_SHADOWS_H
#define POLY_SHADOWS_H

#include <poly/Engine/Entity.h>
#include <poly/Engine/Extension.h>

#include <poly/Graphics/Camera.h>

namespace poly
{

class FrameBuffer;
class Shader;

class Shadows : public Extension
{
public:
	Shadows(Scene* scene);

	~Shadows();

	void render(Camera& camera);

	void apply(Shader* shader);

private:
	struct ShadowInfo
	{
		std::vector<FrameBuffer*> m_shadowMaps;
		std::vector<Matrix4f> m_lightProjViews;
		std::vector<float> m_shadowDists;
		Matrix4f m_cameraProj;
		float m_shadowStrength;
	};

private:
	HashMap<Entity::Id, ShadowInfo> m_shadowInfo;
};

}

#endif