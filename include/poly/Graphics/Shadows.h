#ifndef POLY_SHADOWS_H
#define POLY_SHADOWS_H

#include <poly/Engine/Entity.h>
#include <poly/Engine/Extension.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/UniformBuffer.h>

namespace poly
{

class FrameBuffer;
class Shader;


#ifndef DOXYGEN_SKIP

///////////////////////////////////////////////////////////
struct UniformBlock_Shadows
{
	UniformBufferType<Matrix4f>		m_lightProjViews[6];
	UniformBufferType<float, 4>		m_shadowDists[6];
	UniformBufferType<float, 4>		m_shadowStrengths[2];
	UniformBufferType<int, 4>		m_numShadowCascades[2];
	UniformBufferType<int>			m_numShadows;
};

#endif


///////////////////////////////////////////////////////////
/// \brief A scene extension used for rendering shadows
///
///////////////////////////////////////////////////////////
class Shadows : public Extension
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	Shadows(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~Shadows();

	///////////////////////////////////////////////////////////
	/// \brief Render the scene into shadow maps (only for directional lights)
	///
	/// The scene is rendered using an orthographic projection matrix,
	/// from the perspective of each directional light that has
	/// shadows enabled. A shadow map is rendered for each cascade
	/// within the directional light.
	///
	/// This function should be called before doing any other
	/// render calls so that update shadow maps are available.
	/// The shadow extension can only store shadow maps for a
	/// single camera at a time (it can't render shadows for
	/// multiple cameras).
	///
	/// At the end of the render, the new data is pushed to the
	/// uniform buffer.
	///
	/// \param camera The camera to render shadow maps for
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera);

	///////////////////////////////////////////////////////////
	/// \brief Apply shadow maps and uniforms to a shader
	///
	/// This will apply the current shadow maps and shadow parameters
	/// from the latest shadow render to the specified shader.
	///
	/// \param shader The shader to apply the shadow maps and parameters to
	///
	///////////////////////////////////////////////////////////
	void apply(Shader* shader);

private:
	struct ShadowInfo
	{
		std::vector<FrameBuffer*> m_shadowMaps;		//!< Shadow maps, one for each cascade level
		std::vector<Matrix4f> m_lightProjViews;		//!< The matrices used to transform world space -> light clip space
		std::vector<float> m_shadowDists;			//!< The cascade distances
		Matrix4f m_cameraProj;						//!< The (normal) camera projection matrix
		float m_shadowStrength;						//!< The blocking strength of the shadows
	};

private:
	UniformBuffer m_uniformBuffer;					//!< A uniform block for shader uniforms
	HashMap<Entity::Id, ShadowInfo> m_shadowInfo;	//!< Shadow info
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Shadows
/// \ingroup Graphics
///
/// The shadows scene extension is used to render shadows and
/// and manage shadow parameters for directional lights in the
/// scene. To use the shadows extension, call render() with the
/// desired camera before rendering anything else in the scene.
/// Rendering shadows is only supported for a single camera, so
/// shadow maps can not be rendered for multiple different
/// cameras.
///
/// Use Scene::getExtension() to access the shadows extension.
///
/// \code
///
/// using namespace poly;
///
/// Scene scene;
/// Camera camera;
///
/// // Setup the rest of the scene...
///
/// while (true)
/// {
///		// Render shadows for scene
///		scene.getExtension<Shadows>()->render(camera);
///
///		// Default render
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////