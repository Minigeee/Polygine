#ifndef POLY_RENDER_SYSTEM_H
#define POLY_RENDER_SYSTEM_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/RenderState.h>

namespace poly
{

class Scene;

///////////////////////////////////////////////////////////
/// \brief The base class for all rendering procedure classes
///
///////////////////////////////////////////////////////////
class RenderSystem
{
public:
	///////////////////////////////////////////////////////////
	/// \brief This function should initialize anything that is scene dependent,
	///        such as access to entities
	///
	/// \param scene A pointer to the scene to initialize the system for
	///
	///////////////////////////////////////////////////////////
	virtual void init(Scene* scene) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Execute the rendering procedures
	///
	/// \param camera The camera to render from the perspective of
	///
	///////////////////////////////////////////////////////////
	virtual void render(Camera& camera) = 0;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::RenderSystem
/// \ingroup Graphics
///
/// The RenderSystem is the base class for any class that needs
/// a custom rendering procedure, such as terrain, the skybox,
/// a spatial structure for rendering (the octree), and anything
/// else that needs custom render procedures. Advanced knowledge
/// of OpenGL will be helpful if custom render systems are needed.
///
/// Look at the source code of Octree, Terrain, or Skybox for examples
/// of how to implement a render system.
///
///////////////////////////////////////////////////////////