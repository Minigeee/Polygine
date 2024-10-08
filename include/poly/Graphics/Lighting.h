#ifndef POLY_LIGHTING_H
#define POLY_LIGHTING_H

#include <poly/Engine/Extension.h>

#include <poly/Graphics/UniformBuffer.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Shader;


#ifndef DOXYGEN_SKIP

///////////////////////////////////////////////////////////
struct UniformStruct_DirLight
{
	UniformBufferType<Vector3f, 4>		m_diffuse;
	UniformBufferType<Vector3f, 4>		m_specular;
	UniformBufferType<Vector3f, 4>		m_direction;
};


///////////////////////////////////////////////////////////
struct UniformStruct_PointLight
{
	UniformBufferType<Vector3f, 4>		m_position;
	UniformBufferType<Vector3f, 4>		m_diffuse;
	UniformBufferType<Vector3f, 4>		m_specular;
	UniformBufferType<Vector3f, 4>		m_coefficients;
};


///////////////////////////////////////////////////////////
struct UniformBlock_Lights
{
	UniformBufferType<Vector3f, 4>	m_ambient;
	UniformStruct_DirLight			m_dirLights[2];
	UniformStruct_PointLight		m_pointLights[100];
	UniformBufferType<int>			m_numDirLights;
	UniformBufferType<int>			m_numPointLights;
};

#endif


///////////////////////////////////////////////////////////
/// \brief A scene extension for lighting related features
///
///////////////////////////////////////////////////////////
class Lighting : public Extension
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	/// \param scene A pointer a scene
	///
	///////////////////////////////////////////////////////////
	Lighting(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Update the lighting uniform buffer
	///
	/// This will push all lighting data to the uniform buffer,
	/// and as this can be an expensive operation, it should not
	/// be used too often.
	///
	/// Most of the time, the user will not need to use this function
	/// because the scene automatically calls this at the beginning
	/// of each render.
	///
	/// \param camera The camera to update lighting system for (culling point lights)
	/// \param maxPointLights The maximum number of point lights to apply
	///
	///////////////////////////////////////////////////////////
	void update(Camera& camera, Uint32 maxPointLights = 100);

	///////////////////////////////////////////////////////////
	/// \brief Apply lighting parameters to a shader
	///
	/// This function should be called as convenience function to
	/// apply all necessary lighting uniforms to a shader.
	///
	/// \param shader A pointer to a shader to apply uniforms to
	///
	///////////////////////////////////////////////////////////
	void apply(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Set the scene ambient color
	///
	/// The ambient color is the default color that gets applied
	/// to a surface when no light is illuminating it.
	///
	/// \param color The ambient color
	///
	///////////////////////////////////////////////////////////
	void setAmbientColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the scene ambient color
	///
	/// The ambient color is the default color that gets applied
	/// to a surface when no light is illuminating it.
	///
	/// \param r The r-component of the ambient color
	/// \param g The g-component of the ambient color
	/// \param b The b-component of the ambient color
	///
	///////////////////////////////////////////////////////////
	void setAmbientColor(float r, float g, float b);

	///////////////////////////////////////////////////////////
	/// \brief Get the scene ambient color
	///
	/// \return The scene ambient color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getAmbientColor() const;

private:
	UniformBlock_Lights m_cache;	//!< Cache data to only update when the data changes
	UniformBuffer m_uniformBuffer;	//!< A uniform buffer for storing lighting uniform data
	Vector3f m_ambientColor;		//!< The ambient color
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Lighting
/// \ingroup Graphics
///
/// The lighting scene extension is used for lighting related
/// features. At the moment, it is only used to specify the
/// scene ambient color though.
///
/// Use Scene::getExtension() to access the lighting extension.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Scene scene;
/// scene.getExtension<Lighting>()->setAmbientColor(0.05f, 0.02f, 0.1f);
///
/// \endcode
///
///////////////////////////////////////////////////////////