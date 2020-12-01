#ifndef POLY_FRAMEBUFFER_H
#define POLY_FRAMEBUFFER_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/Texture.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Window;

///////////////////////////////////////////////////////////
/// \brief A framebuffer acts as a render target with optional color, depth, and stencil texture attachments
///
///////////////////////////////////////////////////////////
class FrameBuffer
{
	friend Window;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	FrameBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// This deletes the internal OpenGL framebuffer object
	///
	///////////////////////////////////////////////////////////
	~FrameBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Bind this framebuffer and make it the current render target
	///
	/// This function makes this framebuffer the current render
	/// target, with an optional z-slice parameter for 3D framebuffers.
	/// 3D framebuffers are equivalent to an array of 2D framebuffers,
	/// where each slice along the z-axis has to be rendered to
	/// seperately.
	///
	/// \param z The optional z-slice parameter
	///
	///////////////////////////////////////////////////////////
	void bind(Uint32 z = 0);

	///////////////////////////////////////////////////////////
	/// \brief Initialize the framebuffer with the given size
	///
	/// The width and height of the framebuffer are required,
	/// and the depth can be given a value other than 0 if a
	/// 3D framebuffer is desired.
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 w, Uint32 h, Uint32 d = 0);

	///////////////////////////////////////////////////////////
	/// \brief Attach a color buffer, either as a texture or renderbuffer
	///
	/// A color texture attachment should be used if the resulting
	/// color values are needed (i.e. for postprocessing), otherwise,
	/// a renderbuffer can be used. To use a texture attachment,
	/// a pointer to an uninitialized texture must be passed into
	/// the first parameter position, and other texture parameters
	/// may be specified for customization. If no texture pointer
	/// is provided, a renderbuffer will be used by default.
	///
	/// \param texture An optional texture pointer for texture attachments
	/// \param fmt The pixel format to use (R, RG, RGB, or RGBA)
	/// \param dtype The data type to use for pixel values (8-bit, 16-bit, or 32-bit pixels)
	/// \param filter The filter method to use when sampling in between two pixels
	/// \param wrap The wrap method to use when sampling outside the bounds of the texture
	///
	///////////////////////////////////////////////////////////
	void attachColor(
		Texture* texture = 0,
		PixelFormat fmt = PixelFormat::Rgb,
		GLType dtype = GLType::Uint8,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge
	);

	///////////////////////////////////////////////////////////
	/// \brief Attach a depth buffer, either as a texture or renderbuffer
	///
	/// A depth texture attachment should be used if the resulting
	/// depth values are needed (i.e. for postprocessing), otherwise,
	/// a renderbuffer can be used. To use a texture attachment,
	/// a pointer to an uninitialized texture must be passed into
	/// the first parameter position, and other texture parameters
	/// may be specified for customization. If no texture pointer
	/// is provided, a renderbuffer will be used by default.
	///
	/// \param texture An optional texture pointer for texture attachments
	/// \param dtype The data type to use for pixel values (8-bit, 16-bit, or 32-bit pixels)
	/// \param filter The filter method to use when sampling in between two pixels
	/// \param wrap The wrap method to use when sampling outside the bounds of the texture
	///
	///////////////////////////////////////////////////////////
	void attachDepth(
		Texture* texture = 0,
		GLType dtype = GLType::Float,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge
	);

	///////////////////////////////////////////////////////////
	/// \brief Get the internal framebuffer id
	///
	/// \return The framebuffer id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the framebuffer width
	///
	/// \return The framebuffer width
	///
	///////////////////////////////////////////////////////////
	Uint32 getWidth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the framebuffer height
	///
	/// \return The framebuffer height
	///
	///////////////////////////////////////////////////////////
	Uint32 getHeight() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the framebuffer depth
	///
	/// \return The framebuffer depth
	///
	///////////////////////////////////////////////////////////
	Uint32 getDepth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of color texture attachments
	///
	/// \return The number of color texture attachments
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumColorTextures() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the color texture at the specified index
	///
	/// \return A color texture
	///
	///////////////////////////////////////////////////////////
	Texture* getColorTexture(Uint32 index = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the depth texture at the specified index
	///
	/// \return A depth texture
	///
	///////////////////////////////////////////////////////////
	Texture* getDepthTexture() const;

public:
	static FrameBuffer Default;				//!< The default framebuffer created by GLFW

private:
	Uint32 m_id;							//!< The framebuffer id
	Vector3u m_size;						//!< The framebuffer size

	std::vector<Texture*> m_colorTextures;	//!< The list of color textures
	Texture* m_depthTexture;				//!< The depth texture
	std::vector<Uint32> m_colorIds;			//!< A list of color renderbuffers
	Uint32 m_depthId;						//!< The depth renderbuffer

	static Uint32 currentBound;				//!< The current bound framebuffer
};

}

#endif