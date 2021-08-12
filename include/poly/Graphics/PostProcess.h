#ifndef POLY_POST_PROCESS_H
#define POLY_POST_PROCESS_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Skybox.h>

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
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect that applies gamma correction and HDR rendering
/// \ingroup Graphics
///
/// See PostProcess for an example of how to use post processing effects.
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
/// \brief A post processing fog effect
/// \ingroup Graphics
///
/// Depth fog requires a pointer to the camera that is currently active
/// in the input framebuffer, a pointer to the depth texture
/// associated with the input framebuffer, and a pointer to the
/// current scene, to work properly.
///
/// See PostProcess for an example of how to use post processing effects.
///
/// \see setScene, setCamera, setDepthTexture
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
	/// \brief Apply fog effect to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the camera pointer for directional light effects
	///
	/// \param scene A camera pointer
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Set the entity containing the main directional light component
	///
	/// This directional light will be used to render the light scattering
	/// effect.
	///
	/// \param entity The entity containing the directional light component
	///
	///////////////////////////////////////////////////////////
	void setDirLight(Entity entity);

	///////////////////////////////////////////////////////////
	/// \brief Set the depth texture to be used to apply the depth fog effect
	///
	/// \param A pointer to the depth texture
	///
	///////////////////////////////////////////////////////////
	void setDepthTexture(Texture* texture);

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
	/// scattering from the directional light is. By default,
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
	/// \brief Get the entity used to render the light scattering effect
	///
	/// \return The entity containing a directional light used to render the scattering effect
	///
	///////////////////////////////////////////////////////////
	Entity getDirLight() const;

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
	Camera* m_camera;			//!< A camera pointer for calculating depth information
	Texture* m_depthTexture;	//!< The depth texture used for the depth fog effect
	Entity m_dirLight;			//!< An entity containing the directional light

	Vector3f m_color;			//!< The color of the fog
	float m_density;			//!< The density of the fog
	float m_scatterStrength;	//!< The light scatter strength for lights
	bool m_applyToSkybox;		//!< A boolean that controls if the fog should be applied to the skybox
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for fast approximate anti-aliasing
/// \ingroup Graphics
///
/// See PostProcess for an example of how to use post processing effects.
///
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
	/// \brief Apply anti-aliasing to the input framebuffer
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
	float m_threshold;		//!< The luma threshold for detecting edges
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for applying either a vertical or horizontal blur
/// \ingroup Graphics
///
/// See PostProcess for an example of how to use post processing effects.
///
///////////////////////////////////////////////////////////
class Blur : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief An enum of distribution types for blur weights
	///
	///////////////////////////////////////////////////////////
	enum DistType
	{
		Uniform,		//!< A uniform distribution
		Gaussian		//!< A gaussian distribution
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Blur();

	///////////////////////////////////////////////////////////
	/// \brief Apply either a vertical or horizontal blur to the input framebuffer
	///
	/// To do a full blur effect, apply a vertical blur, followed
	/// by a horizontal blur on the output target of this first blur.
	/// The combination of both blurs will result in a full blur effect.
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the weight distribution type
	///
	/// The blur weight distribution type determines how the kernel
	/// weights are calculated. For a uniform distribution, every
	/// weight in the kernel has a value equal to each other, and
	/// in a gaussian distribution, each weight is calculated using
	/// the gaussian distribution, with the center of the kernel
	/// as the mean.
	///
	/// These weights determine how much of each surrounding pixel
	/// to use when generating the blur effect.
	///
	/// The default distribution type is DistType::Gaussian
	///
	/// \param type The weight distribution type
	///
	///////////////////////////////////////////////////////////
	void setDistType(DistType type);

	///////////////////////////////////////////////////////////
	/// \brief Set the side length of the square blurring kernel
	///
	/// The kernel is the square from which surrounding pixels
	/// are used in a weighted average to generate a blur effect.
	///
	/// The default kernel size is 11.
	///
	/// \param size The size of the blurring kernel in pixels
	///
	///////////////////////////////////////////////////////////
	void setKernelSize(Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Set the amount of spacing between each kernel sample (in pixels)
	///
	/// Increasing the amount of spacing between each sampling location
	/// within the kernel is an effective to increase the blur radius
	/// while keepping the number of samples required the same.
	/// This will allow for stronger blur effects while keeping the
	/// performance relatively low. To make up for the increased
	/// space between each sampling location, and the noticeable
	/// pattern that arises as a result, a small random positional
	/// offset is applied to each sampling location. The amount
	/// of positional offset can be controlled with the noise factor.
	///
	/// The default spacing value is 1.
	///
	/// \param spacing The amount of space between each sampling location (in pixels)
	///
	///////////////////////////////////////////////////////////
	void setKernelSpacing(float spacing);

	///////////////////////////////////////////////////////////
	/// \brief Set the factor that is multiplied by the random positional offset when taking kernel samples
	///
	/// A small random positional offset is applied to each kernel
	/// sample to break up the noticeable pattern when large kernel
	/// spacing is used. This creates a slight noise pattern in
	/// the resulting blur, but it is unnoticeable in most cases.
	///
	/// The default noise factor is 1.
	///
	/// \param factor The noise multiplier used when offseting sample locations
	///
	///////////////////////////////////////////////////////////
	void setNoiseFactor(float factor);

	///////////////////////////////////////////////////////////
	/// \brief Set the amount of weight spread for non-uniform distribution types
	///
	/// The spread is the equivalent to the standard deviation
	/// in the gaussian distribution. A spread value does not apply
	/// for uniform distributions.
	///
	/// The default spread value is 1.7.
	///
	/// \param spread The spread of the weight distribution
	///
	///////////////////////////////////////////////////////////
	void setSpread(float spread);

	///////////////////////////////////////////////////////////
	/// \brief Set whether the blur should be applied vertically or horizontally
	///
	/// When this value is true, the blur effect will be applied
	/// vertically.
	///
	/// \param vertical A boolean indicating whether blur should be applied vertically
	///
	///////////////////////////////////////////////////////////
	void setVerticalBlur(bool vertical);

	///////////////////////////////////////////////////////////
	/// \brief Get the weight distribution type
	///
	/// \return The distribution type
	///
	///////////////////////////////////////////////////////////
	DistType getDistType() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the blurring kernel size in pixels
	///
	/// \return The kernel size in pixels
	///
	///////////////////////////////////////////////////////////
	Uint32 getKernelSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the amount of spacing between each kernel sample in pixels
	///
	/// \return The kernel spacing value in pixels
	///
	///////////////////////////////////////////////////////////
	float getKernelSpacing() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the noise factor multiplier
	///
	/// \return The noise offset factor
	///
	///////////////////////////////////////////////////////////
	float getNoiseFactor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the distribution spread value (standard deviation)
	///
	/// \return The distribution spread value
	///
	///////////////////////////////////////////////////////////
	float getSpread() const;

	///////////////////////////////////////////////////////////
	/// \brief Get whether the blur effect should be applied vertically
	///
	/// \return A boolean indicating if the blur effect should be applied vertically
	///
	///////////////////////////////////////////////////////////
	bool usesVerticalBlur() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	DistType m_distType;			//!< Blur weight distrubution type
	Uint32 m_kernelSize;			//!< The kernel size (how much to blur)
	float m_kernelSpacing;			//!< The amount of spacing between each sample location in a kernel
	float m_noiseFactor;			//!< The amount of sampling offset noise has on the blur
	float m_spread;					//!< The amount of weight spread
	bool m_verticalBlur;			//!< Should the effect apply vertical blur?
	bool m_paramsDirty;				//!< True if blur parameters are different

	std::vector<float> m_weights;	//!< The blur weights
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for the bloom effect
/// \ingroup Graphics
///
/// See PostProcess for an example of how to use post processing effects.
///
///////////////////////////////////////////////////////////
class Bloom : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	Bloom();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// This is used to clean up the extra framebuffer used for
	/// the bloom effect.
	///
	///////////////////////////////////////////////////////////
	~Bloom();

	///////////////////////////////////////////////////////////
	/// \brief Apply the bloom effect to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the intensity of the bloom effect
	///
	/// The intensity of the bloom determines how strong the bleeding
	/// effect is overlayed onto the original scene. The bleeding
	/// effect is combined with the original image by adding the value
	/// of each pixel, and the intensity value is multiplied into
	/// the bleeding effect before the combination.
	///
	/// \code
	/// finalColor = inputColor + intensity * bloom;
	/// \endcode
	///
	/// The default value is 1.
	///
	/// \param intensity The intensity of the bloom effect
	///
	///////////////////////////////////////////////////////////
	void setIntensity(float intensity);

	///////////////////////////////////////////////////////////
	/// \brief Set the luminosity threshold above which colors are added to the bloom effect
	///
	/// This threshold determines which pixels will be used in the
	/// bloom effect, where if the luminosity value of the pixels
	/// is higher than the threshold, it will be used. A soft threshold
	/// can be used by setting the threshold interval, where pixels
	/// that are the below the threshold will still be included if
	/// they are still within the given interval below the threshold.
	/// These pixels will have a dampened value though.
	///
	/// The luminosity value of a pixel is calculated like this:
	/// \code
	/// float luminosity = dot(vec3(0.299, 0.587, 0.114), color.rgb);
	/// \endcode
	///
	/// The default value is 1.
	///
	/// \param threshold The luminosity threshold
	///
	///////////////////////////////////////////////////////////
	void setThreshold(float threshold);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the range beneath the luminosity threshold, where pixels are slowly blended into the bloom effect
	///
	/// Pixel values within this range are slowly blended into the
	/// bloom effect by multiplying their inital values by the smoothstep
	/// function. Pixels with a luminosity value on the lower end of
	/// the range will have a multiplier close to 0, while pixels
	/// on the higher end will have a multiplier close to 1.
	///
	/// The default value is 0.8.
	///
	/// \param interval The range over which pixels are slowly blended into the bloom effect
	///
	///////////////////////////////////////////////////////////
	void setThresholdInterval(float interval);

	///////////////////////////////////////////////////////////
	/// \brief Set the blur radius of the bloom effect, in a resolution independent way
	///
	/// The blur radius is independent of screen resolution, so the
	/// true blur radius will be scaled according to screen size.
	/// Changing this value will not effect the performance of the
	/// bloom effect, as the kernel size is actually kept constant.
	/// The kernel spacing is increased or decreased according to
	/// the radius.
	///
	/// Another way to increase blur radius is by increasing the number
	/// of blurs, but that option can be more expensive for performance.
	///
	/// The default value is 0.05.
	///
	/// \param radius The blur radius
	///
	///////////////////////////////////////////////////////////
	void setRadius(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of times to apply the blur effect
	///
	/// An easy way to increase the bloom bleed effect, but is more
	/// expensive for performance.
	///
	/// The default value is 3.
	///
	/// \param numBlurs The number of times to apply the blur
	///
	///////////////////////////////////////////////////////////
	void setNumBlurs(Uint32 numBlurs);

	///////////////////////////////////////////////////////////
	/// \brief Get the intesity of the bloom effect
	///
	/// \return The intensity of the bloom effect
	///
	///////////////////////////////////////////////////////////
	float getIntensity() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the luminosity threshold of the bloom efect
	///
	/// \return The luminosity threshold of the bloom effect
	///
	///////////////////////////////////////////////////////////
	float getThreshold() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the interval over which pixels are multipled by a smoothing factor when added to the bloom effect
	///
	/// \return The threshold interval value
	///
	///////////////////////////////////////////////////////////
	float getThresholdInterval() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the resolution independent blur radius of the bloom effect
	///
	/// \return The resolution independent blur radius of the bloom effect
	///
	///////////////////////////////////////////////////////////
	float getRadius() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of times the bloom effect gets blurred
	///
	/// \return The number of times the bloom effect is blurred
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumBlurs() const;

private:
	static Shader s_thresholdShader;
	static Shader s_addShader;

	static Shader& getThresholdShader();

	static Shader& getAddShader();

private:
	Blur m_blurEffect;				//!< The blur effect used to create the bleeding effect
	FrameBuffer* m_blurTargets[2];	//!< An extra framebuffers to apply the blur
	Texture* m_blurTextures[2];		//!< The color textures of the extra framebuffers

	float m_intensity;				//!< The intensity of the bloom effect
	float m_threshold;				//!< The luminosity threshold above which pixels are added to the effect
	float m_thresholdInterval;		//!< The interval over which pixels are blended into the effect
	float m_radius;					//!< The blur radius, independent of screen size
	Uint32 m_numBlurs;				//!< The number of times to apply the blur
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for screen space ambient occlusion
/// \ingroup Graphics
///
/// SSAO requires a pointer to the camera that is currently active
/// in the input framebuffer, as well a pointer to the depth texture
/// associated with the input framebuffer, to work properly.
///
/// See PostProcess for an example of how to use post processing effects.
///
/// \see setCamera, setDepthTexture
///
///////////////////////////////////////////////////////////
class Ssao : public PostProcess
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	Ssao();

	///////////////////////////////////////////////////////////
	/// \brief Apply screen space ambient occlusion to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the camera that the scene is currently being rendered from
	///
	/// The camera is needed to do spatial calculations.
	///
	/// \param camera A pointer to the current camera
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Set the depth texture that should be used to apply SSAO
	///
	/// \param texture A pointer to the depth texture
	///
	///////////////////////////////////////////////////////////
	void setDepthTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the soft radius within which objects should occlude lighting (in world space units)
	///
	/// For each pixel, random points are sampled within a hemisphere
	/// with the given radius, and tested for lighting occlusion.
	/// Choosing a larger radius will allow objects from further away
	/// to contribute to ambient occlusion, but the occlusion edges
	/// will be much softer. A smaller radius will do the opposite.
	///
	/// The default value is 0.2.
	///
	/// \param radius The occlusion radius
	///
	///////////////////////////////////////////////////////////
	void setRadius(float radius);

	///////////////////////////////////////////////////////////
	/// \brief Set the depth bias that gets added to each sampled position when testing for occlusion
	///
	/// This parameter is mostly used to fix artifacts in more complex
	/// scenes, but is not too important in most cases.
	///
	/// The default value is 0.
	///
	/// \parameter bias The depth bias
	///
	///////////////////////////////////////////////////////////
	void setBias(float bias);

	///////////////////////////////////////////////////////////
	/// \brief Set the range away from the camera where SSAO is not applied (in world space units)
	///
	/// The method of screen space ambient occlusion this engine
	/// uses causes a lot of artifacts at far distances, and in
	/// most cases, ambient occlusion is not noticeable at large
	/// distances. So a distance range is set so that any pixels
	/// that are outside the set range will not have ambient occlusion
	/// applied.
	///
	/// The default value is 30.
	///
	/// \param range The cutoff range for SSAO
	///
	///////////////////////////////////////////////////////////
	void setRange(float range);

	///////////////////////////////////////////////////////////
	/// \brief Set the falloff factor for the intensity of the SSAO effect
	///
	/// In some cases, a slight falloff of the intensity of the SSAO
	/// effect is desired to smooth the transition towards the cutoff
	/// range, and in some scenes, a smaller intesity at further distances
	/// might be desireable.
	///
	/// The intensity of the effect is calculated using the distance from the camera:
	/// \code
	/// occlusion *= 1.0 / (1.0 + falloff * distance);
	/// \endcode
	///
	/// The default value is 0.1.
	///
	/// \param falloff The falloff factor
	///
	///////////////////////////////////////////////////////////
	void setFalloff(float falloff);

	///////////////////////////////////////////////////////////
	/// \brief Set the intensity of the SSAO effect
	///
	/// This parameter determines how strong the occlusion effect
	/// is applied, where higher values will make occluded pixels
	/// darker, and lower values will make occluded pixels closer
	/// to their original value.
	///
	/// The default value is 0.8.
	///
	/// \param intensity The intensity of the SSAO effect
	///
	///////////////////////////////////////////////////////////
	void setIntensity(float intensity);

	///////////////////////////////////////////////////////////
	/// \brief Set the random positional sampling offset factor
	///
	/// The noise factor determines how much to apply a random positional
	/// offset to each sampled location within the occlusion hemisphere.
	/// When sampling depths to check for occlusion, a small random
	/// offset is added to the sampling position to break up the banding
	/// patterns that occur as a result of a low sampling rate.
	/// When the noies factor is high, the magnitude of the random
	/// offset will be higher, and as a result, the edges between
	/// occluded regions and non-occluded regions will be much softer.
	/// The oppsite applies for smaller noise factors, but noise
	/// factors too close to 0 will begin experience banding patterns.
	///
	/// The default value is 0.1.
	///
	/// \param factor The noise factor
	///
	///////////////////////////////////////////////////////////
	void setNoiseFactor(float factor);

	///////////////////////////////////////////////////////////
	/// \brief Get the sampling radius for occlusion tests (in world space units)
	///
	/// \return The sampling radius
	///
	///////////////////////////////////////////////////////////
	float getRadius() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the depth bias
	///
	/// \return The depth bias
	///
	///////////////////////////////////////////////////////////
	float getBias() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the cutoff range for the SSAO effect (in world space coordinates)
	///
	/// \return The cutoff range for the effect
	///
	///////////////////////////////////////////////////////////
	float getRange() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the falloff factor used to reduce intensity at further distances
	///
	/// \return The falloff factor
	///
	///////////////////////////////////////////////////////////
	float getFalloff() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the intensity of the SSAO effect
	///
	/// \return The intensity of the effect
	///
	///////////////////////////////////////////////////////////
	float getIntensity() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the random positional sampling offset factor
	///
	/// \return The noise factor
	///
	///////////////////////////////////////////////////////////
	float getNoiseFactor() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	Camera* m_camera;			//!< A pointer to the camera used for spatial calculations
	Texture* m_depthTexture;	//!< The depth texture to use for SSAO

	float m_radius;				//!< The sampling radius used when testing for occlusion
	float m_bias;				//!< The depth bias used to fix visual artifacts
	float m_range;				//!< The cutoff range of the SSAO effect
	float m_falloff;			//!< The falloff factor to reduce intensity at further distances
	float m_intensity;			//!< The intensity of the SSAO effect
	float m_noiseFactor;		//!< The random positional sampling offset factor
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for applying lens flare to a single directional light
/// \ingroup Graphics
///
/// The lens flare effect requires a scene pointer and a camera
/// pointer to work. Note that lens flare will only be applied
/// to the first directional light it finds.
///
/// See PostProcess for an example of how to use post processing effects.
///
/// \see setScene, setCamera
///
///////////////////////////////////////////////////////////
class LensFlare : public PostProcess
{
public:
	LensFlare();

	///////////////////////////////////////////////////////////
	/// \brief Apply the lens flare effect to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the scene pointer
	///
	/// A scene pointer is needed to access directional lights.
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	void setScene(Scene* scene);

	///////////////////////////////////////////////////////////
	/// \brief Set a pointer to a camera
	///
	/// The camera is needed to calculate the directional light's position
	/// in screen space.
	///
	/// \param camera A pointer to a camera
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the lens flare
	///
	/// By default, the color of the lens flare is set to be the
	/// color of the light it is being applied to.
	///
	/// \param r The red component of the lens flare color
	/// \param g The green component of the lens flare color
	/// \param b The blue component of the lens flare color
	///
	///////////////////////////////////////////////////////////
	void setColor(float r, float g, float b);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the lens flare
	///
	/// By default, the color of the lens flare is set to be the
	/// color of the light it is being applied to.
	///
	/// \param color The color of the lens flare
	///
	///////////////////////////////////////////////////////////
	void setColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the intensity of the lens flare effect
	///
	/// The intensity is the multiplier to the effect when it is
	/// overlayed onto the main scene.
	///
	/// The default value is 0.8.
	///
	/// \param intensity The intensity of the lens flare effect
	///
	///////////////////////////////////////////////////////////
	void setIntensity(float intensity);

	///////////////////////////////////////////////////////////
	/// \brief Set the luminosity factor
	///
	/// When the effect is applied, the location the light is coming
	/// from is sampled and its luminosity is calculated. This
	/// luminosity is multiplied by the luminosity factor, which
	/// is then applied to the intensity. This will make it so that
	/// lens flare is not applied when the light is blocked by an object.
	///
	/// The default value is 0.5.
	///
	/// \param factor The luminosity factor
	///
	///////////////////////////////////////////////////////////
	void setLuminosityFactor(float factor);

	///////////////////////////////////////////////////////////
	/// \brief Set the screen space cutoff bounds
	///
	/// The bounds are used to set boundaries to stop showing the
	/// lens flare effect. This is needed so that the effect
	/// doesn't continue while the light is off screen. The
	/// x-component should contain the horizontal cutoff value,
	/// and the y-component should contain the vertical cutoff
	/// value. The cutoff values should be in screen space
	/// coordinates, where a value of 1 in the x-component would
	/// indicate bounds of [-1, 1], meaning the bounds are the
	/// left and right sides of the screen.
	///
	/// The default value is (0.9, 1.3).
	///
	/// \param bounds The effect cutoff bounds
	///
	///////////////////////////////////////////////////////////
	void setBounds(const Vector2f& bounds);

	///////////////////////////////////////////////////////////
	/// \brief Get the color of the lens flare effect
	///
	/// \return The color of the lens flare effect
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the intensity of the lens flare effect
	///
	/// \return The intensity of the lens flare effect
	///
	///////////////////////////////////////////////////////////
	float getIntensity() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the luminosity factor
	///
	/// \return The luminosity factor
	///
	///////////////////////////////////////////////////////////
	float getLuminosityFactor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the screen space cutoff bound of the effect
	///
	/// \return The screen space cutoff bounds
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getBounds() const;

private:
	static Shader s_shader;

	static Shader& getShader();

private:
	Scene* m_scene;				//!< A pointer to a scene
	Camera* m_camera;			//!< A pointer to a camera

	Vector3f m_color;			//!< The color of the lens flare
	float m_intensity;			//!< The intensity of the lens flare
	float m_luminosityFactor;	//!< The luminosity factor
	Vector2f m_bounds;			//!< The screen space cutoff bounds
};


///////////////////////////////////////////////////////////
/// \brief A post processing effect for screen-space reflections
/// \ingroup Graphics
///
/// The reflection effect requires the scene g-buffer to retrieve
/// normals and reflectivity data, a camera to calculate reflection rays
/// and reconstruct position data, and optionally, cubemaps to fill in
/// areas of the screen where no reflections could be found.
///
/// See PostProcess for an example of how to use post processing effects.
///
/// \see setScene, setCamera
///
///////////////////////////////////////////////////////////
class Reflections : public PostProcess
{
public:
	Reflections();

	///////////////////////////////////////////////////////////
	/// \brief Apply the lens reflection to the input framebuffer
	///
	/// \param input The input framebuffer
	/// \param output The output framebuffer
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& input, FrameBuffer& output = FrameBuffer::Default) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the scene g-buffer that will be used to apply the effect
	///
	/// The g-buffer is needed for the scene normals and reflectivity
	/// data.
	///
	/// \param buffer A pointer to the scene g-buffer
	///
	///////////////////////////////////////////////////////////
	void setGBuffer(FrameBuffer* buffer);

	///////////////////////////////////////////////////////////
	/// \brief Set the camera that will be used to apply the effect
	///
	/// The camera is needed to calculate position data from the depth
	/// map, and to calculate reflection vectors.
	///
	/// \param camera A pointer to a camera
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Set an optional procedural skybox to fill in pixels that have no reflection
	///
	/// For areas that don't reflect any other pixel in the screen,
	/// the procedural skybox will be sampled using the reflected ray
	/// instead. This does not stack with other environment maps.
	///
	/// \param skybox A pointer to a procedural skybox
	///
	///////////////////////////////////////////////////////////
	void setCubemap(ProceduralSkybox* skybox);

	///////////////////////////////////////////////////////////
	/// \brief Set the maximum distance from a reflector that objects can be reflected
	///
	/// This property determines how far away from a reflector pixel
	/// that another pixel can be reflected. This value should be provided
	/// in world space units.
	///
	/// \param dist The maximum distance that objects can be reflected
	///
	///////////////////////////////////////////////////////////
	void setMaxSteps(Uint32 steps);

	///////////////////////////////////////////////////////////
	/// \brief Set the fixed stepping size of the raycast operation
	///
	/// A raycast is used to sample reflected data, and the reflected
	/// ray is traversed in fixed distance intervals (fixed pixel intervals).
	/// This property determines the raycast step size in pixels.
	///
	/// \param size The stepping size of the raycast operation in pixels
	///
	///////////////////////////////////////////////////////////
	void setStepSize(float size);

	///////////////////////////////////////////////////////////
	/// \brief Set the maximum difference in raycast intersection point and depth sampled point that counts as a raycast hit
	///
	/// The maximum depth difference defines the maximum allowed difference
	/// in the depth of the raycast point (the point where an intersection
	/// was detected along a point), and the depth sampled from the
	/// depth map. When this difference is too large, it indicates
	/// that the raycast hit will be reflecting the wrong fragment,
	/// so that fragment is marked as no reflection.
	///
	/// \param diff The maximum depth difference in world space units
	///
	///////////////////////////////////////////////////////////
	void setMaxDepthDiff(float diff);

	///////////////////////////////////////////////////////////
	/// \brief Set the fresnel effect factor
	///
	/// The fresnel effect makes reflective surfaces more reflective
	/// at a shallower angle, and less reflective at a steeper angle.
	/// The reflection multiplicative factor is calculated using this equation:
	///
	/// \code
	///	reflFactor = 1 - pow(dot(-viewDir, normal), fresnelFactor);
	/// \endcode
	///
	/// So when the fresnel factor has a larger value, the reflection
	/// factor will be higher in general, and when the fresnel factor
	/// is lower, the reflection factor will be lower in general.
	///
	/// \param factor The fresnel factor
	///
	///////////////////////////////////////////////////////////
	void setFresnelFactor(float factor);

	///////////////////////////////////////////////////////////
	/// \brief Get the maximum number of raycast steps allowed
	///
	/// \return The maximum number of raycast steps allowed
	///
	///////////////////////////////////////////////////////////
	Uint32 getMaxSteps() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the raycast step size in pixels
	///
	/// \return The raycast step size in pixels
	///
	///////////////////////////////////////////////////////////
	float getStepSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the maximum depth difference property
	///
	/// \return The maximum depth difference property
	///
	/// \see setMaxDepthDiff
	///
	///////////////////////////////////////////////////////////
	float getMaxDepthDiff() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the fresnel effect factor
	///
	/// \return The fresnel effect factor
	///
	///////////////////////////////////////////////////////////
	float getFresnelFactor() const;

private:
	static Shader& getShader();

	static Shader s_shader;

private:
	FrameBuffer* m_gBuffer;					//!< A pointer to the scene g-buffer
	Camera* m_camera;						//!< A pointer to the camera that will be used to apply the reflections effect
	ProceduralSkybox* m_proceduralSkybox;	//!< An optional procedural skybox to fill in areas without a reflection

	Uint32 m_maxSteps;						//!< The maximum number of steps the raycast is allowed to perform
	float m_stepSize;						//!< The step size of the raycast (in pixels)
	float m_maxDepthDiff;					//!< The maximum allowed difference in depth from camera (between the ray intersection point and the depth sampled point)
	float m_fresnelFactor;					//!< The fresnel effect factor
};


}

#endif

///////////////////////////////////////////////////////////
/// \class poly::PostProcess
/// \ingroup Graphics
///
/// This is the base class for all post processing effects.
///
/// Most post processing effects will not require any extra
/// setup or initialization, with the exception of any effects
/// that require extra information, such as a depth buffer,
/// or a camera.
///
/// To use the post processing effect, use PostProcess::render(),
/// passing a reference to the input framebuffer and the output
/// framebuffer. The effect will use the contents of the input,
/// and the resulting content will be stored in the output buffer.
///
/// It is common to use ping pong framebuffers to apply multiple
/// effects.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Setup scene
/// Scene scene;
/// Camera camera;
///
/// ...
///
/// // Setup framebuffers
/// FrameBuffer framebuffers[2];
/// Texture textures[4];
/// for (Uint32 i = 0; i < 2; ++i)
/// {
/// 	framebuffers[i].create(1280, 720);
/// 	framebuffers[i].attachColor(&textures[2 * i + 1], PixelFormat::Rgb, GLType::Uint16);
/// 	framebuffers[i].attachDepth(&textures[2 * i + 2]);
/// }
///
/// // HDR rendering and gamma correction
/// ColorAdjust adjust;
///
/// // Fast approximate anti aliasing
/// Fxaa fxaa;
///
/// // Screen space ambient occlusion (this requires a depth buffer and camera)
/// Ssao ssao;
/// ssao.setCamera(&camera);
///
/// // We will render the scene into the first framebuffer, so
/// // it is ok to set the depth texture now because the location
/// // of the depth texture will not change, and writing to the
/// // depth buffer should always be disabled during the rendering
/// // of the effects. So the first framebuffer will always contains
/// // the correct depth information
/// ssao.setDepthTexture(framebuffers[0].getDepthTexture());
///
///
/// // Game loop
/// while (true)
/// {
///		// Input and updates
///
///		...
///
///		// Render scene into the first framebuffer
///		scene.render(framebuffers[0]);
///
///		// Render effects using ping pong buffers
///		ssao.render(framebuffers[0], framebuffers[1]);
///		adjust.render(framebuffers[1], framebuffers[0]);
///
///		// Make the last effect render to the default framebuffer
///		fxaa.render(framebuffers[0]);
/// }
///
/// \endcode
///
/// To create custom post processing effects, make the class inherit
/// from the PostProcess class, and override the PostProcess::render()
/// function. Use PostProcess::getVertexArray() to get a renderable
/// vertex array with the vertices of a screen size quad.
/// Setup all necessary shader uniforms and then draw the vertex
/// array. Make sure to disable depth testing.
///
/// For an example, look at how any of the built in post processing
/// effects were implemented.
///
///////////////////////////////////////////////////////////