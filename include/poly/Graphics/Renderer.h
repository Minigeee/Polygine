#ifndef POLY_RENDERER_H
#define POLY_RENDERER_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A graphics utility struct for rendering a fullscreen quad
///
///////////////////////////////////////////////////////////
struct FullscreenQuad
{
public:
	static void draw();

private:
	static VertexArray s_quadVao;
	static VertexBuffer s_quadVbo;
};


///////////////////////////////////////////////////////////
/// \brief Manages the rendering pipeline for scenes
///
///////////////////////////////////////////////////////////
class Renderer
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Renderer(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Add a render system
	///
	/// Render systems define custom rendering procedures.
	///
	/// This function calls RenderSystem::init(), so the user
	/// shouldn't have to initialize the system manually.
	///
	/// \param system A pointer to a render system
	///
	/// \see RenderSystem
	///
	///////////////////////////////////////////////////////////
	void addRenderSystem(RenderSystem* system);

	///////////////////////////////////////////////////////////
	/// \brief Render all added render systems in the order they were added
	///
	/// This function simply renders all render systems in the order they were added.
	/// First, all render systems that have enabled deferred rendering will
	/// be rendered into a g-buffer, and deferred shading will be applied to the
	/// results. Note that transparent objects can't be rendered in this stage.
	/// After finishing the deferred render pass, a forward render pass will be
	/// run for the systems that have enabled forward rendering. This will allow
	/// render systems that require a forward pass to execute, and it will allow
	/// all transparent objects to be rendered. The results are rendered into
	/// the output \a target framebuffer.
	///
	/// \param camera The camera used to render the scene
	/// \param target The target framebuffer to render the scene to
	/// \param pass The render pass that should be executed
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera, FrameBuffer& target = FrameBuffer::Default, RenderPass pass = RenderPass::Default);

	///////////////////////////////////////////////////////////
	/// \brief Get the g-buffer of the specified output target framebuffer
	///
	/// Each output target buffer has its own g-buffer, except for
	/// targets that are rendered using the RenderPass::Shadow render pass.
	/// Each g-buffer has the following textures attached to it, with the
	/// texture indices given in parentheses:
	///
	/// * Normal + Specular power    (0)
	/// * Diffuse + Occlusion factor (1)
	/// * Specular + Reflectivity    (2)
	/// * Depth
	///
	/// The first color buffer (normal + specular power) is a texture
	/// with 4 half-float components, giving the normal in the RGB components
	/// and the specular power (shininess) in the A component. The second
	/// color buffer (diffuse + occlusion factor) is a texture with
	/// 4 unsigned 8-bit components, with the diffuse color in the RGB
	/// components and the occlusion factor in the A component. The
	/// third color buffer (specular + reflectivity) is a texture with 3 unsigned
	/// 8-bit components, and contains the specular color in the RGB components,
	/// and the material reflectivity (for reflections) in the A component. The
	/// depth buffer is a standard depth buffer accessed with
	/// FrameBuffer::getDepthTexture(), and it a depth buffer that
	/// is shared with the output \a target framebuffer it is associated
	/// with.
	///
	/// There is no position buffer to keep the buffer as small and performant
	/// as possible. Position data can be recalculated from the depth buffer and
	/// inverse projection-view matrix.
	///
	/// \param target The framebuffer associeated with the g-buffer
	///
	/// \return A pointer to the associated g-buffer, or NULL if none exists
	///
	///////////////////////////////////////////////////////////
	FrameBuffer* getGBuffer(const FrameBuffer& target) const;

private:
	static Shader& getDeferredShader();

	static Shader s_deferredShader;

private:
	Scene* m_scene;										//!< The scene pointer
	std::vector<RenderSystem*> m_renderSystems;			//!< List of render systems
	HashMap<Uint32, FrameBuffer*> m_gBuffers;			//!< Map each output target framebuffer to its own deferred framebuffer
};


}

#endif