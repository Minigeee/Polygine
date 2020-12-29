#ifndef POLY_SKYBOX_H
#define POLY_SKYBOX_H

#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class ProceduralSkybox;


///////////////////////////////////////////////////////////
/// \brief A texture cube map used to display a textured sky
///
///////////////////////////////////////////////////////////
class Skybox : public RenderSystem
{
	friend ProceduralSkybox;

public:
	///////////////////////////////////////////////////////////
	/// \brief An enum representing the six sides of a cube
	///
	///////////////////////////////////////////////////////////
	enum Side
	{
		Right,		//!< The positive x side of a box
		Left,		//!< The negative x side of a box
		Top,		//!< The positive y side of a box
		Bottom,		//!< The negative y side of a box
		Front,		//!< The positive z side of a box
		Back		//!< The negative z side of a box
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Skybox();

	///////////////////////////////////////////////////////////
	/// \brief Deinitializes the cubemap
	///
	///////////////////////////////////////////////////////////
	~Skybox();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the skybox
	///
	/// The skybox does not depend on the scene
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Render the skybox from the perspective of a camera
	///
	/// \param camera The camera to render from
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera) override;

	///////////////////////////////////////////////////////////
	/// \brief Load a single side of the cube map from an image file
	///
	/// Internally, this uses Image::load() to load images from
	/// image files.
	///
	/// \param fname The image file name
	/// \param side The side of the cube to load
	///
	/// \return True if the image was loaded successfully
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, Side side);

private:
	static VertexArray s_vertexArray;
	static VertexBuffer s_vertexBuffer;
	static Shader s_shader;

	static VertexArray& getVertexArray();

	Shader& getShader();

private:
	Uint32 m_id;		//!< The OpenGL cubemap id
};


///////////////////////////////////////////////////////////
/// \brief A procedurally generated skybox that is based on a few colors and the geometry of a planet
///
///////////////////////////////////////////////////////////
class ProceduralSkybox : public RenderSystem
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ProceduralSkybox();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the skybox
	///
	/// The skybox uses the scene to get the first directional light
	/// to calculate where in the sky to apply the sun bloom effect.
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Render the skybox from the perspective of a camera
	///
	/// \param camera The camera to render from
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the zenith color
	///
	/// The zenith is the point directly above the camera's position,
	/// or the view vector that has a 90 degree angle with the ground.
	///
	/// \param color The zenith color
	///
	///////////////////////////////////////////////////////////
	void setZenithColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the horizon color
	///
	/// The horizon is the point where the sky meets the ground.
	///
	/// \param color The horizon color
	///
	///////////////////////////////////////////////////////////
	void setHorizonColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the ground color
	///
	/// \param color The ground color
	///
	///////////////////////////////////////////////////////////
	void setGroundColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the sun bloom color
	///
	/// This is the color of the area surrounding the direction
	/// of the incoming light. This effect is normally caused by
	/// the mie scattering because this type of scattering scatters
	/// light in the forward direction more than any other direction.
	///
	/// \param color The sun bloom color
	///
	///////////////////////////////////////////////////////////
	void setBloomColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the size and relative brightness of the sun bloom effect
	///
	/// \param size The size and relative size of the bloom factor
	///
	///////////////////////////////////////////////////////////
	void setBloomSize(float size);

	///////////////////////////////////////////////////////////
	/// \brief Set light strength
	///
	/// The light strength is equivalent to the color multiplier
	///
	/// \param strength The light strength multiplier
	///
	///////////////////////////////////////////////////////////
	void setLightStrength(float strength);

	///////////////////////////////////////////////////////////
	/// \brief Set the radius of the top of the atmosphere
	///
	/// \param radius The radius of the top of the atmosphere in kilometers (default 6420)
	///
	///////////////////////////////////////////////////////////
	void setTopRadius(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Set the radius of the planet
	///
	/// \param radius The radius of the planet in kilometers (default 6360)
	///
	///////////////////////////////////////////////////////////
	void setBotRadius(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Set the camera altitude in kilometers
	///
	/// Changing this value will cause the skybox to look different,
	/// depending on the altitude
	///
	/// \param alt The altitude in kilometers
	///
	///////////////////////////////////////////////////////////
	void setAltitude(float alt);

	///////////////////////////////////////////////////////////
	/// \brief Get the zenith color
	///
	/// \return The zenith color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getZenithColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the horizon color
	///
	/// \return The horizon color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getHorizonColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the ground color
	///
	/// \return The ground color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getGroundColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the sun bloom color
	///
	/// \return The sun bloom color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getBloomColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the sun bloom size and brightness
	///
	/// \return The sun bloom size and brightness
	///
	///////////////////////////////////////////////////////////
	float getBloomSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the light strength multiplier
	///
	/// \return The light strength multiplier
	///
	///////////////////////////////////////////////////////////
	float getLightStrength() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the radius to the top of the atmosphere in kilometers
	///
	/// \return The radius to the top of the atmosphere in kilometers
	///
	///////////////////////////////////////////////////////////
	float getTopRadius() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the radius of the planet in kilometers
	///
	/// \return The radius of the planet in kilometers
	///
	///////////////////////////////////////////////////////////
	float getBotRadius() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the camera altitude in kilometers
	///
	/// \return The camera altitude in kilometers
	///
	///////////////////////////////////////////////////////////
	float getAltitude() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the ambient color of the sky
	///
	/// This will often be very close to the zenith color
	///
	/// \return The ambient color of the sky
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getAmbientColor();

private:
	static Shader s_shader;

	Shader& getShader();

private:
	Scene* m_scene;				//!< A pointer to a scene

	Vector3f m_zenithColor;		//!< The zenith color
	Vector3f m_horizonColor;	//!< The horizon color
	Vector3f m_groundColor;		//!< The ground color
	Vector3f m_bloomColor;		//!< The sun bloom color
	float m_bloomSize;			//!< Mie phase function "g" factor
	float m_lightStrength;		//!< Color multiplier

	float m_topRadius;			//!< Atmosphere radius
	float m_botRadius;			//!< Planet radius
	float m_altitude;			//!< Camera altitude

	Vector3f m_ambient;			//!< The ambient color of the sky
	bool m_colorsChanged;		//!< True if the zenith or horizon color changed
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Skybox
/// \ingroup Graphics
///
/// A skybox is a textured cubemap that surrounds the scene.
/// To use the skybox, load all six sides of the cube using
/// load(). The sides of the skybox are defined from the perspective
/// of a person standing inside the cube box, where positive x is
/// Side::Right, positive y is Side::Top, and positive z is
/// Side::Front.
///
/// Then the skybox must be added to a scene as a render system,
/// using Scene::addRenderSystem(). The skybox will then be rendered
/// everytime Scene::render() is called.
///
/// Usage example:
/// \code
///
/// Scene scene;
///
/// Skybox skybox;
/// skybox.load("right.png", Skybox::Right);
/// skybox.load("left.png", Skybox::Left);
/// skybox.load("top.png", Skybox::Top);
/// skybox.load("bottom.png", Skybox::Bottom);
/// skybox.load("front.png", Skybox::Front);
/// skybox.load("back.png", Skybox::Back);
/// scene.addRenderSystem(&skybox);
///
/// \endcode
///
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \class poly::ProceduralSkybox
/// \ingroup Graphics
///
/// The procedural skybox generates a skybox based on a few colors
/// and the geometry of a planet. The zenith and horizon colors are
/// interpolated by using the distance from the view to the top of the
/// atmosphere as the interpolation factor. This gives the effect of
/// slow change in interpolation factor for most of the sky, but results
/// in much faster change when the view direction is near the horizon.
/// This gives slightly more realistic results.
///
/// The procedural skybox must be added to a scene as a render system,
/// using Scene::addRenderSystem(). The skybox will then be rendered
/// everytime Scene::render() is called.
///
/// Usage example:
/// \code
///
/// Scene scene;
///
/// ProceduralSkybox skybox;
///
/// // No extra setup is needed unless custom colors are needed (which they probably are)
/// scene.addRenderSystem(&skybox);
///
/// \endcode
///
///////////////////////////////////////////////////////////