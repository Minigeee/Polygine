#ifndef POLY_POST_PROCESS_H
#define POLY_POST_PROCESS_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Scene;


///////////////////////////////////////////////////////////
/// \brief The base class for all post processing effects
///
///////////////////////////////////////////////////////////
class PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Render a post processing effect, given an input framebuffer and an output framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	virtual void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) = 0;

private:
	static VertexArray s_quadVao;
	static VertexBuffer s_quadVbo;

protected:
	static VertexArray& getVertexArray();
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect that applies gamma correction and HDR rendering
///
///////////////////////////////////////////////////////////
class ColorAdjust : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ColorAdjust();

	///////////////////////////////////////////////////////////
	/// \brief Create the effect from a gamma factor
	///
	///////////////////////////////////////////////////////////
	ColorAdjust(float gamma);

	///////////////////////////////////////////////////////////
	/// \brief Apply gamma correction and HDR rendering to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the gamma factor
	///
	/// \param gamma The gamma factor
	///
	///////////////////////////////////////////////////////////
	void setGamma(float gamma);

	///////////////////////////////////////////////////////////
	/// \brief Get the gamma factor
	///
	/// \return The gamma factor
	///
	///////////////////////////////////////////////////////////
	float getGamma() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	float m_gamma;		//!< The gamma factor
};


///////////////////////////////////////////////////////////
/// \brief A post-processing fog effect
///
///////////////////////////////////////////////////////////
class Fog : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Fog();

	///////////////////////////////////////////////////////////
	/// \brief Apply gamma correction and HDR rendering to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the scene pointer for directional light effects
	///
	/// \param scene A scene pointer
	///
	///////////////////////////////////////////////////////////
	void setScene(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Set the camera pointer for directional light effects
	///
	/// \param scene A camera pointer
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Set the fog color
	///
	/// \param r The r-component of the fog color
	/// \param g The g-component of the fog color
	/// \param b The b-component of the fog color
	///
	///////////////////////////////////////////////////////////
	void setColor(float r, float g, float b);

	///////////////////////////////////////////////////////////
	/// \brief Set the fog density
	///
	/// Lower values make the fog less dense. The default value
	/// is 0.0005.
	///
	/// \param density The fog density
	///
	///////////////////////////////////////////////////////////
	void setDensity(float density);

	///////////////////////////////////////////////////////////
	/// \brief Set the fog light scatter strength
	///
	/// The scatter strength determines how bright the light
	/// scattering from a directional light is. By default,
	/// this value is 0.
	///
	/// \param strength The scattering strength
	///
	///////////////////////////////////////////////////////////
	void setScatterStrength(float strength);

	///////////////////////////////////////////////////////////
	/// \brief Set the property that determines if the fog should be applied to the skybox
	///
	/// \param applyFog A boolean for if the skybox should have fog applied
	///
	///////////////////////////////////////////////////////////
	void setSkyboxFog(bool applyFog);

	///////////////////////////////////////////////////////////
	/// \brief Get the fog color
	///
	/// \return Fog color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the fog density
	///
	/// \return Fog density
	///
	///////////////////////////////////////////////////////////
	float getDensity() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the fog light scattering strength
	///
	/// \return Fog light scattering strength
	///
	///////////////////////////////////////////////////////////
	float getScatterStrength() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	Scene* m_scene;
	Camera* m_camera;

	Vector3f m_color;
	float m_density;
	float m_scatterStrength;
	bool m_applyToSkybox;
};


///////////////////////////////////////////////////////////
class Fxaa : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Fxaa();

	///////////////////////////////////////////////////////////
	/// \brief Apply gamma correction and HDR rendering to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the luma difference threshold that decides which pixels are edges
	///
	/// By default, the value is 0.1.
	///
	/// \param threshold The threshold value
	///
	///////////////////////////////////////////////////////////
	void setLumaThreshold(float threshold);

	///////////////////////////////////////////////////////////
	/// \brief Get the luma threhold value
	///
	/// \return The luma threhold value
	///
	///////////////////////////////////////////////////////////
	float getLumaThreshold() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	float m_threshold;
};


///////////////////////////////////////////////////////////
class Blur : public PostProcess
{
public:
	enum DistType
	{
		Uniform,
		Gaussian
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Blur();

	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	void setDistType(DistType type);

	void setKernelSize(Uint32 size);

	void setSpread(float spread);

	void setVerticalBlur(bool vertical);

	DistType getDistType() const;

	Uint32 getKernelSize() const;

	float getSpread() const;

	bool usesVerticalBlur() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	DistType m_distType;			//!< Blur weight distrubution type
	Uint32 m_kernelSize;			//!< The kernel size (how much to blur)
	float m_spread;					//!< The amount of weight spread
	bool m_verticalBlur;			//!< Should the effect apply vertical blur?
	bool m_paramsDirty;				//!< True if blur parameters are different

	std::vector<float> m_weights;	//!< The blur weights
};


///////////////////////////////////////////////////////////
class Bloom : public PostProcess
{
public:
	Bloom();

	~Bloom();

	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	void setIntensity(float intensity);

	void setThreshold(float threshold);

	void setThresholdInterval(float interval);

	void setRadius(float radius);

	void setNumBlurs(Uint32 numBlurs);

	float getIntensity() const;

	float getThreshold() const;

	float getThresholdInterval() const;

	float getRadius() const;

	Uint32 getNumBlurs() const;

private:
	static Shader s_thresholdShader;
	static Shader s_addShader;

	static Shader& getThresholdShader();

	static Shader& getAddShader();

private:
	Blur m_blurEffect;
	FrameBuffer* m_blurTarget;
	Texture* m_blurTexture;

	float m_intensity;
	float m_threshold;
	float m_thresholdInterval;
	float m_radius;
	Uint32 m_numBlurs;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::PostProcess
/// \ingroup Graphics
///
/// This is the base class for all post processing effects.
///
///////////////////////////////////////////////////////////