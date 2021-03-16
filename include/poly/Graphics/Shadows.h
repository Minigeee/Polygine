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
		FrameBuffer* m_shadowMap;
		Matrix4f m_lightProjView;
		float m_shadowDist;
	};

private:
	HashMap<Entity::Id, ShadowInfo> m_shadowInfo;
};

}

#endif