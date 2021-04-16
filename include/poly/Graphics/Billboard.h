#ifndef POLY_BILLBOARD_H
#define POLY_BILLBOARD_H

#include <poly/Graphics/Renderable.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Material;
class Texture;

///////////////////////////////////////////////////////////
/// \brief A billboard object displays a texture that always faces the camera
///
///////////////////////////////////////////////////////////
class Billboard : public Renderable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	Billboard();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~Billboard();

	///////////////////////////////////////////////////////////
	/// \brief Load a diffuse texture from an image file
	///
	/// This function loads a diffuse texture from an image file
	/// and stores the texture in the diffuse texture slot of the
	/// billboard material. The texture pointer can be accessed
	/// with getTexture() or through the diffuse texture in
	/// getMaterial().
	///
	/// The function internally loads image files using Image::load(),
	/// so refer to that function for more details.
	///
	/// \param fname The file name of the image to load
	///
	/// \return True if the image was successfully loaded, false if there was an error
	///
	/// \see Image::load()
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the billboard in world space units
	///
	/// \param size The size of the billboard
	///
	///////////////////////////////////////////////////////////
	void setSize(const Vector2f& size);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the billboard in world space units
	///
	/// \param w The width of the billboard in world space units
	/// \param h The height of the billboard in world space units
	///
	///////////////////////////////////////////////////////////
	void setSize(float w, float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the billboard as a percentage value
	///
	/// The origin is the location that is defined as the center of
	/// the local coordinate system of the billboard. The values of
	/// the origin should be a value between 0 and 1, where 0 and 1
	/// would be the edges of the billboard, and 0.5 would be the
	/// center of the billboard. An origin of (0, 0) would be at
	/// the bottom-left corner of the billboard, and a value of
	/// (1, 1) would be at the top-right corner of the billboard.
	///
	/// By default, the origin is set to the center of the billboard.
	///
	/// \param origin The origin of the billboard
	///
	///////////////////////////////////////////////////////////
	void setOrigin(const Vector2f& origin);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the billboard as a percentage value
	///
	/// The origin is the location that is defined as the center of
	/// the local coordinate system of the billboard. The values of
	/// the origin should be a value between 0 and 1, where 0 and 1
	/// would be the edges of the billboard, and 0.5 would be the
	/// center of the billboard. An origin of (0, 0) would be at
	/// the bottom-left corner of the billboard, and a value of
	/// (1, 1) would be at the top-right corner of the billboard.
	///
	/// By default, the origin is set to the center of the billboard.
	///
	/// \param x The x-component of the origin
	/// \param y The y-component of the origin
	///
	///////////////////////////////////////////////////////////
	void setOrigin(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set whether rotation along the x-axis of the billboard should be locked
	///
	/// Locking the rotational x-axis would still allow the billboard
	/// to rotate along the y-axis, but would prevent rotation along
	/// the x-axis of the billboard. This could be useful for billboards
	/// that are far away, but at the same y-level, such as trees.
	///
	/// \param locked A boolean indicating whether to lock the rotational x-axis
	///
	///////////////////////////////////////////////////////////
	void setAxisLocked(bool locked);

	///////////////////////////////////////////////////////////
	/// \brief Set whether the billboard should be affected by lighting effects
	///
	/// By default, lighting does not affect billboards.
	///
	/// \param enabled A boolean indicating whether lighting affects billboards
	///
	///////////////////////////////////////////////////////////
	void setLightingEnabled(bool enabled);

	///////////////////////////////////////////////////////////
	/// \brief Set whether the billboard should be affected by shadow effects
	///
	/// This does not control whether or not billboards cast shadows
	/// because billboards are never rendered into the shadow maps.
	/// Enabling this will allow shadows to be cast onto the billboard.
	/// By default, shadows are not enabled for billboards.
	///
	/// \param enabled A boolean indicating whether shadowing affects billboards
	///
	///////////////////////////////////////////////////////////
	void setShadowingEnabled(bool enabled);

	///////////////////////////////////////////////////////////
	/// \brief Get access to the billboard material
	///
	/// This can be used to change the billboard's diffuse color,
	/// specular color, shininess, textures, etc.
	///
	/// \return A pointer to the billboard material
	///
	///////////////////////////////////////////////////////////
	Material* getMaterial() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the billboard diffuse texture
	///
	/// \return A pointer to the billboard diffuse texture
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the billboard size in world space units
	///
	/// \return The billboard size in world space units
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the origin of the billboard
	///
	/// \return The origin of the billboard
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getOrigin() const;

	///////////////////////////////////////////////////////////
	/// \brief Get whether the rotational x-axis is locked
	///
	/// \return A boolean indicating whether the rotational x-axis is locked
	///
	///////////////////////////////////////////////////////////
	bool isAxisLocked() const;

	///////////////////////////////////////////////////////////
	/// \brief Get whether lighting effects affect the billboard
	///
	/// \return A boolean indicating whether lighting effects affect the billboard
	///
	///////////////////////////////////////////////////////////
	bool isLightingEnabled() const;

	///////////////////////////////////////////////////////////
	/// \brief Get whether shadowing effects affect the billboard
	///
	/// \return A boolean indicating whether shadowing effects affect the billboard
	///
	///////////////////////////////////////////////////////////
	bool isShadowingEnabled() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the global billboard vertex array
	///
	/// \return The global billboard vertex array
	///
	///////////////////////////////////////////////////////////
	static VertexArray& getVertexArray();

	///////////////////////////////////////////////////////////
	/// \brief Get the default billboard shader
	///
	/// \return The default billboard shader
	///
	///////////////////////////////////////////////////////////
	static Shader& getDefaultShader();

private:
	void updateBoundingVolumes();

private:
	Material* m_material;				//!< The billboard material
	Texture* m_texture;					//!< The diffuse texture
	Vector2f m_size;					//!< The billboard size
	Vector2f m_origin;					//!< The billbaord origin
	bool m_axisLocked;					//!< Indicates if the rotational x-axis is locked
	bool m_lightingEnabled;				//!< Indicates if lighting affects the billboard
	bool m_shadowingEnabled;			//!< Indicates if shadowing affects the billboard

	static VertexArray s_vertexArray;	//!< The global billboard vertex array
	static Shader s_shader;				//!< The default billboard shader
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Billboard
/// \ingroup Graphics
///
/// A billboard is a renderable object that displays a texture
/// that always faces the camera. Billboards are often used
/// to render far away objects to save the GPU from processing
/// extra vertices, or to render other special effects.
///
/// The Billboard inherits from the Renderable class, so it
/// has to be attached to a RenderComponent, and the scene must
/// have an octree for the billboard to be rendered. 
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Billboard billboard;
///
/// // Load a diffuse image
/// billboard.load("images/billboard_d.png");
/// // Add a slight red tint
/// billboard.getMaterial()->setDiffuse(1.0f, 0.95f, 0.95f);
///
/// // Create the scene
/// Scene scene;
/// Octree octree;
/// Camera cmaera;
/// scene.addRenderSystem(&octree);
///
/// // Create an entity
/// TransformComponent t;
/// t.m_position.y = 5.0f;
/// RenderComponent r;
/// r.m_renderable = &billboard;
/// r.m_shader = &Billboard::getDefaultShader();
///
/// scene.createEntity(t, r);
///
///
/// // Game loop
/// while (true)
/// {
///		// Do some game logic + other stuff...
///
///		// Rendering the scene will render the billboard
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////