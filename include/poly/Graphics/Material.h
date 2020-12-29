#ifndef POLY_MATERIAL_H
#define POLY_MATERIAL_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Shader;
class Texture;

///////////////////////////////////////////////////////////
/// \brief A class that defines the properties of a model surface
///
///////////////////////////////////////////////////////////
class Material
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Material();

	///////////////////////////////////////////////////////////
	/// \brief Set the diffuse color of the material
	///
	/// The diffuse color is the main color that appears on a model,
	/// and when a diffuse texture is used, the diffuse color is multiplied
	/// by the texture colors to produce the final diffuse color.
	///
	/// \param color The diffuse color
	///
	/// \note Transparent colors are currently not supported
	///
	///////////////////////////////////////////////////////////
	void setDiffuse(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the diffuse color of the material
	///
	/// The diffuse color is the main color that appears on a model,
	/// and when a diffuse texture is used, the diffuse color is multiplied
	/// by the texture colors to produce the final diffuse color.
	///
	/// \param r The r component of the diffuse color
	/// \param g The g component of the diffuse color
	/// \param b The b component of the diffuse color
	///
	/// \note Transparent colors are currently not supported
	///
	///////////////////////////////////////////////////////////
	void setDiffuse(float r, float g, float b);

	///////////////////////////////////////////////////////////
	/// \brief Set the specular color of the material
	///
	/// The specular color affects the color that is reflected
	/// off the surface of the model. The final reflection color
	/// is a combination of a light's specular color and the material's
	/// specular color, and the specular texture if it exists.
	///
	/// \param color The specular color
	///
	///////////////////////////////////////////////////////////
	void setSpecular(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the specular color of the material
	///
	/// The specular color affects the color that is reflected
	/// off the surface of the model. The final reflection color
	/// is a combination of a light's specular color and the material's
	/// specular color, and the specular texture if it exists.
	///
	/// \param r The r component of the specular color
	/// \param g The g component of the specular color
	/// \param b The b component of the specular color
	///
	///////////////////////////////////////////////////////////
	void setSpecular(float r, float g, float b);

	///////////////////////////////////////////////////////////
	/// \brief Set the shininess of the material
	///
	/// Higher values of shininess will cause the material to look
	/// more metallic, where the reflection of light affects a smaller
	/// area, has a sharper cutoff, but is brighter. This does not
	/// affect the brightness of light reflections.
	///
	/// \param shininess The shininess value
	///
	///////////////////////////////////////////////////////////
	void setShininess(float shininess);
	
	///////////////////////////////////////////////////////////
	/// \brief Set the material diffuse texture
	///
	/// The diffuse texture is similar to the diffuse color,
	/// except the diffuse colors are sampled from a texture.
	///
	/// \param texture The diffuse texture
	///
	///////////////////////////////////////////////////////////
	void setDiffTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the material specular texture
	///
	/// The specular texture is similar to the specular color,
	/// except the specular colors are sampled from a texture.
	///
	/// \param texture The specular texture
	///
	///////////////////////////////////////////////////////////
	void setSpecTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Add a texture to the material to map to a shader uniform
	///
	/// Add a texture to the material that gets mapped to the specified
	/// sampler uniform when apply() is called. If the specified uniform
	/// is already being used by the material, the new texture overrides
	/// the previous texture.
	///
	/// \param uniform The uniform name
	/// \param texture A pointer to the texture
	///
	///////////////////////////////////////////////////////////
	void addTexture(const std::string& uniform, Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Remove a texture from the material
	///
	/// Remove the texture bound to the specified uniform name.
	///
	/// \param uniform The uniform name of the texture to remove
	///
	///////////////////////////////////////////////////////////
	void removeTexture(const std::string& uniform);

	///////////////////////////////////////////////////////////
	/// \brief Get the diffuse color
	///
	/// \return The diffuse color
	///
	///////////////////////////////////////////////////////////
	Vector3f& getDiffuse();

	///////////////////////////////////////////////////////////
	/// \brief Get the specular color
	///
	/// \return The specular color
	///
	///////////////////////////////////////////////////////////
	Vector3f& getSpecular();

	///////////////////////////////////////////////////////////
	/// \brief Get shininess value
	///
	/// \return The shininess value
	///
	///////////////////////////////////////////////////////////
	float getShininess() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture bound to the specified uniform
	///
	/// \param The name of the uniform to retrieve a texture from
	///
	/// \return A texture pointer
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture(const std::string& uniform) const;

	///////////////////////////////////////////////////////////
	/// \brief Apply the material to a shader
	///
	/// Models are allowed to have multiple materials, so use the \a index
	/// parameter to specify which material index to apply.
	///
	/// The shader should be set up like this:
	/// \code
	///
	/// // shader.frag
	///
	/// // Set this to however many materials you want to support
	/// #define MAX_NUM_MATERIALS 4
	///
	/// struct Material
	/// {
	/// 	vec3 diffuse;
	/// 	vec3 specular;
	/// 	float shininess;
	/// };
	///
	/// // The materials
	/// uniform Material u_materials[MAX_NUM_MATERIALS];
	///
	/// // Textures used by model loader
	/// uniform sampler2D u_diffuseMaps[MAX_NUM_MATERIALS];
	/// uniform sampler2D u_specularMaps[MAX_NUM_MATERIALS];
	///
	/// // Any other optional textures
	/// uniform sampler1D u_texture1d;
	/// uniform sampler2D u_texture2d;
	/// uniform sampler3D u_texture3d;
	///
	/// ...
	///
	/// \endcode
	///
	///////////////////////////////////////////////////////////
	void apply(Shader* shader, int index = 0) const;

private:
	Vector3f m_diffuse;							//!< The diffuse color
	Vector3f m_specular;						//!< The specular color
	float m_shininess;							//!< The shininess value

	Texture* m_diffTexture;						//!< The diffuse texture
	Texture* m_specTexture;						//!< The specular texture

	HashMap<std::string, Texture*> m_textures;	//!< Map of uniform to texture
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Material
/// \ingroup Graphics
///
/// A material defines the properties of the surface of a model.
/// It defines properties such as the mesh diffuse color, specular color,
/// shininess, and any textures the mesh may use. Both the diffuse
/// and specular colors use RGB values and they don't support
/// transparency.
///
/// It's possible to add custom textures to a material using
/// addTexture(). These textures can be used for any purpose,
/// such as for a skybox, height maps, normal maps, etc. They
/// don't necessarily have to be used for a diffuse or specular
/// map.
///
/// To use the material with a Model, the function apply() must be called.
/// Each model can have multiple materials affecting different
/// vertices, so if the model contains multiple materials, it
/// will be necessary to specify which index material is being
/// applied when calling apply().
///
/// The shader the material is being applied to must be set up
/// like this (most of the time the material affects the fragment
/// shader):
///
/// \code
///
/// // shader.frag
///
/// // Set this to however many materials you want to support
/// #define MAX_NUM_MATERIALS 4
///
/// struct Material
/// {
/// 	vec3 diffuse;
/// 	vec3 specular;
/// 	float shininess;
/// };
///
/// // The materials
/// uniform Material u_materials[MAX_NUM_MATERIALS];
///
/// // Textures used by model loader
/// uniform sampler2D u_diffuseMaps[MAX_NUM_MATERIALS];
/// uniform sampler2D u_specularMaps[MAX_NUM_MATERIALS];
///
/// // Any other optional textures
/// uniform sampler1D u_texture1d;
/// uniform sampler2D u_texture2d;
/// uniform sampler3D u_texture3d;
///
/// ...
///
/// \endcode
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Material material;
/// material.setDiffuse(1.0f, 0.0f, 0.0f);
/// material.setSpecular(0.2f, 0.2f, 0.2f);
/// material.setShininess(4.0f);
///
/// // Add a texture
/// Image image;
/// image.load("image.png");
///
/// Texture texture;
/// texture.create(image);
///
/// material.addTexture("u_diffuseMaps[0]", &texture);
///
/// // Apply to shader
/// Shader shader;
/// shader.load("shader.vert", Shader::Vertex);
/// shader.load("shader.frag", Shader::Fragment);
/// shader.compile();
///
/// shader.bind();
/// material.apply(&shader);
///
/// // Use these if more than 1 material is required
/// // material.apply(&shader, 0);
/// // material.apply(&shader, 1);
/// // material.apply(&shader, 2);
/// // material.apply(&shader, 3);
///
/// \endcode
///
///////////////////////////////////////////////////////////