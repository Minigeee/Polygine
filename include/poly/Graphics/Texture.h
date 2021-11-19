#ifndef POLY_TEXTURE_H
#define POLY_TEXTURE_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/NonCopyable.h>

#include <poly/Graphics/GLType.h>
#include <poly/Graphics/Image.h>

namespace poly
{

enum class TextureWrap
{
	Repeat				= 0x2901,
	MirroredRepeat		= 0x8370,
	ClampToEdge			= 0x812F,
	ClampToBorder		= 0x812D
};

enum class TextureFilter
{
	Linear		= 0x2601,
	Nearest		= 0x2600
};

enum class PixelFormat
{
	R		= 0x1903,
	Rg		= 0x8227,
	Rgb		= 0x1907,
	Rgba	= 0x1908,
	Depth	= 0x1902
};

///////////////////////////////////////////////////////////
/// \brief A class that creates renderable images on the GPU
///
///////////////////////////////////////////////////////////
class Texture : public NonMoveable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Texture();

	///////////////////////////////////////////////////////////
	/// \brief Create the texture by loading data from an image file
	///
	/// \param fname The relative path to the image file
	/// \param dtype The data type to use when loading the image
	/// \param mipmap Determines if mipmaps should be generated for the texture
	/// \param adjustForGamma The gamma factor that is used in post-processing
	///
	/// \see load
	///
	///////////////////////////////////////////////////////////
	Texture(const std::string& fname, GLType dtype = GLType::Uint8, bool mipmap = false, float adjustForGamma = 1.0f);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// This will destroy the internal texture object, if it exists
	///
	///////////////////////////////////////////////////////////
	~Texture();

	///////////////////////////////////////////////////////////
	/// \brief Bind texture to a certain texture slot
	///
	/// \param slot The texture slot to bind the texture to
	///
	///////////////////////////////////////////////////////////
	void bind(Uint32 slot = 0);

	///////////////////////////////////////////////////////////
	/// \brief Load a texture from an image file
	///
	/// This creates a temporary image object and loads the file
	/// using Image::load(). The function returns \a true if the image
	/// was successfully loaded, and \a false if an error occured.
	/// As with the create() functions, it is possible to generate
	/// mipmaps for the texture.
	///
	/// \see Image::load
	///
	/// \param fname The relative path to the image file
	/// \param dtype The data type to use when loading the image
	/// \param mipmap Determines if mipmaps should be generated for the texture
	/// \param adjustForGamma The gamma factor that is used in post-processing
	///
	/// \return True if the file was successfully loaded
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, GLType dtype = GLType::Uint8, bool mipmap = false, float adjustForGamma = 1.0f);

	///////////////////////////////////////////////////////////
	/// \brief Create a new texture from pixel data
	///
	/// This will create a new 1D, 2D, or 3D texture, depending on
	/// how many dimensions are greater than 0. A new texture object
	/// is created and it is given the specified sampling parameters,
	/// pixel format, and data type. If the pointer to the pixel data
	/// is NULL, then an empty texture will be created.
	///
	/// To generate mipmaps, set the \a mipmap parameter to true.
	///
	/// \param data A pointer to the pixel data to create the texture with
	/// \param fmt The pixel format to use
	/// \param w Texture width
	/// \param h Texture height (for 2D and 3D textures)
	/// \param d Texture depth (for 3D textures)
	/// \param dtype The texture data type
	/// \param filter The sampling filter type for choosing a pixel when in between pixels
	/// \param wrap The sampling wrap type for when sampling outside the texture bounds
	/// \param mipmap Determines if mipmaps should be generated for the texture
	/// \param multisampled Set to true to create a multisampled texture (only works for 2D)
	///
	///////////////////////////////////////////////////////////
	void create(
		void* data,
		PixelFormat fmt,
		Uint32 w,
		Uint32 h = 0,
		Uint32 d = 0,
		GLType dtype = GLType::Uint8,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge,
		bool mipmap = false,
		bool multisampled = false
	);

	///////////////////////////////////////////////////////////
	/// \brief Create a new texture from a 2D image
	///
	/// This will create a new 2D texture. A new texture object
	/// is created and it is given the specified sampling parameters, and
	/// pixel format and data type taken from the image.
	///
	/// \param image An image containing pixel data to create the texture
	/// \param filter The sampling filter type for choosing a pixel when in between pixels
	/// \param wrap The sampling wrap type for when sampling outside the texture bounds
	/// \param mipmap Determines if mipmaps should be generated for the texture
	///
	///////////////////////////////////////////////////////////
	void create(
		const Image& image,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge,
		bool mipmap = false
	);

	///////////////////////////////////////////////////////////
	/// \brief Update all of texture data
	///
	/// This will cause data to be uploaded to the texture, but maintain
	/// the same properties as when the texture was created.
	///
	/// \param data A pointer to the new texture data
	///
	///////////////////////////////////////////////////////////
	void update(void* data);

	///////////////////////////////////////////////////////////
	/// \brief Update a subregion of the texture (1D)
	///
	/// This will cause data from only the subregion to be updated,
	/// where pos is pixel offset of the subregion from the top left
	/// of the texture, and size is the region size in pixels.
	/// The pixel data must be layed out in continuous row-major order.
	///
	/// \param data A pointer to the new texture data
	/// \param pos The position of the section to update (x)
	/// \param size The dimensions of the section to update (w)
	///
	///////////////////////////////////////////////////////////
	void update(void* data, Uint32 pos, Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Update a subregion of the texture (2D)
	///
	/// This will cause data from only the subregion to be updated,
	/// where pos is pixel offset of the subregion from the top left
	/// of the texture, and size is the region size in pixels.
	/// The pixel data must be layed out in continuous row-major order.
	///
	/// \note \a pos must be specified in (x, y) coordinates
	///
	/// \param data A pointer to the new texture data
	/// \param pos The position of the section to update (x, y)
	/// \param size The dimensions of the section to update (w, h)
	///
	///////////////////////////////////////////////////////////
	void update(void* data, const Vector2u& pos, const Vector2u& size);

	///////////////////////////////////////////////////////////
	/// \brief Update a subregion of the texture (3D)
	///
	/// This will cause data from only the subregion to be updated,
	/// where pos is pixel offset of the subregion from the top left
	/// of the texture, and size is the region size in pixels.
	/// The pixel data must be layed out in continuous row-major order.
	///
	/// \param data A pointer to the new texture data
	/// \param pos The position of the section to update (x, y, z)
	/// \param size The dimensions of the section to update (w, h, d)
	///
	///////////////////////////////////////////////////////////
	void update(void* data, const Vector3u& pos, const Vector3u& size);

	///////////////////////////////////////////////////////////
	/// \brief Set the texture filter method
	///
	/// This property determines how to choose the value of a sampled pixel
	/// when sampling between pixels. When TextureFilter::Linear is used,
	/// pixels will be linearly interpolated when sampling between
	/// two pixels, and when TextureFilter::Linear is used, the nearest
	/// pixel to the sample location will be used.
	///
	/// This property can be:
	/// * TextureFilter::Linear
	/// * TextureFilter::Nearest
	///
	/// \param filter The texture filter type to use
	///
	///////////////////////////////////////////////////////////
	void setFilter(TextureFilter filter);

	///////////////////////////////////////////////////////////
	/// \brief Set the texture wrap method
	///
	/// This property determines how the value of a pixel is chosen
	/// when sampling outside the bounds of the texture
	///
	/// This property can be:
	/// * TextureWrap::Repeat
	/// * TextureWrap::MirroredRepeat
	/// * TextureWrap::ClampToEdge
	/// * TextureWrap::ClampToBorder
	///
	/// \param wrap The texture wrap method to use
	///
	///////////////////////////////////////////////////////////
	void setWrap(TextureWrap wrap);

	///////////////////////////////////////////////////////////
	/// \brief Get the internal texture id
	///
	/// \return The internal OpenGL texture id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture width
	///
	/// \return The texture width
	///
	///////////////////////////////////////////////////////////
	Uint32 getWidth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture height
	///
	/// \return The texture height, 0 if the texture is a 1D texture
	///
	///////////////////////////////////////////////////////////
	Uint32 getHeight() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture depth
	///
	/// \return The texture depth, 0 if the texture is a 1D or 2D texture
	///
	///////////////////////////////////////////////////////////
	Uint32 getDepth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of texture dimensions
	///
	/// \return The number of texture dimensions
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumDimensions() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the pixel format
	///
	/// \return The texture pixel format
	///
	///////////////////////////////////////////////////////////
	PixelFormat getFormat() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the pixel data type
	///
	/// \return The pixel data type
	///
	///////////////////////////////////////////////////////////
	GLType getDataType() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the wrap sampling method
	///
	/// \return The wrap sampling method for when sampling out of the texture bounds
	///
	///////////////////////////////////////////////////////////
	TextureWrap getWrap() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the filter sampling method
	///
	/// \return The filter sampling method for when sampling pixels
	///
	///////////////////////////////////////////////////////////
	TextureFilter getFilter() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the texture is multisampled
	///
	/// \return True if the texture is multisampled and is a 2D texture
	///
	///////////////////////////////////////////////////////////
	bool isMultisampled() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if mipmaps were generated for the texture
	///
	/// \return True if mipmaps were generated for the texture
	///
	///////////////////////////////////////////////////////////
	bool hasMipmaps() const;

private:
	Uint32 m_id;			//!< The texture id
	Uint32 m_width;			//!< Texture width
	Uint32 m_height;		//!< Texture height
	Uint32 m_depth;			//!< Texture depth
	Uint32 m_dimensions;	//!< Number of dimensions
	PixelFormat m_format;	//!< The pixel format
	GLType m_dataType;		//!< The pixel data type
	TextureWrap m_wrap;		//!< The wrap sampling method
	TextureFilter m_filter;	//!< The filter sampling method
	bool m_multisampled;	//!< True if the texture is multisampled
	bool m_hasMipmaps;		//!< True if mipmaps were generated for the texture

	static Uint32 s_currentSlot;
	static Uint32 s_currentBound[100];
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Texture
/// \ingroup Graphics
///
/// A texture manages pixel data on the GPU and enables the
/// data to be rendered onto a surface. It is possible to create
/// 1D, 2D, and 3D textures, but only 2D textures are supported
/// when using an image to create a texture.
///
/// The filter sampling method determines which color to
/// choose from a texture when sampling between pixels.
/// TextureFilter::Linear will use bilinear interpolation
/// when between pixels and TextureFilter::Nearest will choose
/// the closest pixel. The linear filtering method produces
/// smoother looking textures, so it is desired in most cases.
///
/// The wrap sampling method determines the color of a pixel
/// when sampling outside the bounds of a texture. The default
/// is TextureWrap::ClampToEdge, which chooses the color of the
/// pixel closest to the location that was sampled.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load image
/// Image image;
/// image.load("image.png");
///
/// // Create texture
/// Texture texture;
/// texture.create(image);
///
/// \endcode
///
///////////////////////////////////////////////////////////