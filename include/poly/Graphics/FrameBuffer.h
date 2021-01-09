#ifndef POLY_FRAMEBUFFER_H
#define POLY_FRAMEBUFFER_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/Texture.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Window;

///////////////////////////////////////////////////////////
/// \brief A class that acts as a render target with optional color, depth, and stencil texture attachments
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
	/// 3D framebuffer is desired. To create a multisampled
	/// framebuffer, set \a multisampled to true.
	///
	/// \param w The width of the framebuffer
	/// \param h The height of the framebuffer
	/// \param d The depth of the framebuffer (for 3D framebuffers)
	/// \param multisampled Set to true if the framebuffer is multisampled
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 w, Uint32 h, Uint32 d = 0, bool multisampled = false);

	///////////////////////////////////////////////////////////
	/// \brief Reset the framebuffer so that the framebuffer can be resized
	///
	///////////////////////////////////////////////////////////
	void reset();

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
	/// \brief Blit contents from the current framebuffer to a target framebuffer
	///
	/// This function copies contents of the current framebuffer
	/// to a new one. Blitting is mostly used to copy contents of
	/// a multisampled framebuffer to a normal one.
	///
	/// \param target The target framebuffer to blit to
	///
	///////////////////////////////////////////////////////////
	void blitTo(FrameBuffer& target);

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
	/// \brief Check if the framebuffer is multisampled
	///
	/// \return True if the framebuffer is multisampled
	///
	///////////////////////////////////////////////////////////
	bool isMultisampled() const;

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
	bool m_multisampled;					//!< True if the framebuffer is multisampled

	std::vector<Texture*> m_colorTextures;	//!< The list of color textures
	Texture* m_depthTexture;				//!< The depth texture
	std::vector<Uint32> m_colorIds;			//!< A list of color renderbuffers
	Uint32 m_depthId;						//!< The depth renderbuffer

	static Uint32 currentBound;				//!< The current bound framebuffer
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::FrameBuffer
/// \ingroup Graphics
///
/// A framebuffer is a render target that supports color, depth,
/// and stencil buffers. Currently, only color and depth buffer
/// attachments are supported, but stencil buffers may be supported
/// in the future.
///
/// To use the framebuffer, it must first be created with create(),
/// where the width and height of the framebuffer must be specified.
/// To create a 3D framebuffer, set the third depth parameter to
/// any value above 0. To create a multisampled framebuffer for
/// anti-aliasing, set the fourth parameter to true. After calling
/// create(), the framebuffer is still incomplete. At least one color
/// buffer must be attached, and a depth buffer can be added if needed.
/// If the contents of any of the buffer attachments are needed
/// (i.e. for postprocessing), then a pointer to an uninitialized
/// texture must be provided. The framebuffer will initialize the
/// texture with the correct parameters, and that texture can then
/// be used for other purposes. If the contents are not needed,
/// then the texture pointer can be left as null, and a renderbuffer
/// will be created in its place.
///
/// If a multisampled framebuffer is created, its contents cannot
/// directly be used, as multisampled framebuffers hold much more
/// data than normal framebuffers, and it has to be resolved.
/// To do this, use blitTo() to resolve the multisampled framebuffer's
/// contents to a different framebuffer. To render to a framebuffer,
/// call bind(), and all following renders will affect the bound
/// framebuffer.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a few textures to store framebuffer contents
/// Texture textures[3];
///
/// // Create a normal 1280x720 framebuffer
/// FrameBuffer framebuffer;
/// framebuffer.create(1280, 720);
///
/// // Create a color attachment with 16-bit color depth
/// framebuffer.attachColor(&textures[0], PixelFormat::Rgb, GLType::Uint16);
///
/// // Skip the depth attachment
///
///
/// // Create a multisampled 1280x720 framebuffer
/// FrameBuffer multisampled;
/// multisampled.create(1280, 720, 0, true);
///
/// // Create a color attachment with 16-bit color depth
/// multisampled.attachColor(&textures[1], PixelFormat::Rgb, GLType::Uint16);
///
/// // Create a depth attachment
/// multisampled.attachDepth(&textures[2]);
///
///
/// // Game loop
/// while (true)
/// {
///		// Bind the multisampled framebuffer
///		multisampled.bind();
///
///		// Render stuff to the multisampled framebuffer...
///
///		// Blit the multisampled framebuffer to the normal one
///		multisampled.blitTo(framebuffer);
///
///		// Do some postprocessing stuff with the normal framebuffer...
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////