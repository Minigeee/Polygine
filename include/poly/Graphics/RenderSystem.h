#ifndef POLY_RENDER_SYSTEM_H
#define POLY_RENDER_SYSTEM_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/RenderState.h>

namespace poly
{

class Scene;

class RenderSystem
{
public:
	virtual void init(Scene* scene) = 0;

	virtual void render(Camera& camera, FrameBuffer& target = FrameBuffer::Default) = 0;
};

}

#endif