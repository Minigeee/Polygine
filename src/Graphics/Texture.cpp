#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Texture.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 Texture::s_currentBound[] = {};

///////////////////////////////////////////////////////////
Uint32 Texture::s_currentSlot = 0;


///////////////////////////////////////////////////////////
Uint32 getInternalFormat(PixelFormat format, GLType dtype)
{
	Uint32 fmt = (Uint32)format;
	if (dtype == GLType::Uint16 || dtype == GLType::Int16)
	{
		if (format == PixelFormat::R)
			fmt = GL_R16F;
		else if (format == PixelFormat::Rg)
			fmt = GL_RG16F;
		else if (format == PixelFormat::Rgb)
			fmt = GL_RGB16F;
		else if (format == PixelFormat::Rgba)
			fmt = GL_RGBA16F;
	}
	else if (dtype == GLType::Float)
	{
		if (format == PixelFormat::R)
			fmt = GL_R32F;
		else if (format == PixelFormat::Rg)
			fmt = GL_RG32F;
		else if (format == PixelFormat::Rgb)
			fmt = GL_RGB32F;
		else if (format == PixelFormat::Rgba)
			fmt = GL_RGBA32F;
	}
	else if (dtype == GLType::HalfFloat)
	{
		if (format == PixelFormat::R)
			fmt = GL_R16F;
		else if (format == PixelFormat::Rg)
			fmt = GL_RG16F;
		else if (format == PixelFormat::Rgb)
			fmt = GL_RGB16F;
		else if (format == PixelFormat::Rgba)
			fmt = GL_RGBA16F;
	}

	return fmt;
}


///////////////////////////////////////////////////////////
Texture::Texture() :
	m_id			(0),
	m_width			(0),
	m_height		(0),
	m_depth			(0),
	m_dimensions	(2),
	m_format		((Uint16)PixelFormat::Rgb),
	m_dataType		(GLType::Uint8),
	m_wrap			((Uint16)TextureWrap::ClampToEdge),
	m_filter		((Uint16)TextureFilter::Linear),
	m_data			(0),
	m_multisampled	(false),
	m_hasMipmaps	(false)
{

}


///////////////////////////////////////////////////////////
Texture::Texture(const std::string& fname, GLType dtype, TextureFilter filter, TextureWrap wrap, bool mipmap, float adjustForGamma) :
	m_id			(0),
	m_width			(0),
	m_height		(0),
	m_depth			(0),
	m_dimensions	(2),
	m_format		((Uint16)PixelFormat::Rgb),
	m_dataType		(GLType::Uint8),
	m_wrap			((Uint16)filter),
	m_filter		((Uint16)wrap),
	m_data			(0),
	m_multisampled	(false),
	m_hasMipmaps	(false)
{
	load(fname, dtype, filter, wrap, mipmap, 1.0f);
}


///////////////////////////////////////////////////////////
Texture::~Texture()
{
	if (m_id)
		glCheck(glDeleteTextures(1, &m_id));

	m_id = 0;
}


///////////////////////////////////////////////////////////
void Texture::bind(Uint32 slot)
{
	// Need to create a texture first
	if (!m_id)
	{
		// Finish loading if data is waiting
		if (m_data)
			finish();

		LOG_WARNING("Trying to bind a texture that doesn't exist yet");
		return;
	}

	ASSERT(slot < 100, "Texture slot number is too big");

	// Make sure the correct texture slot is active
	if (s_currentSlot != slot)
	{
		// Set slot as active
		glCheck(glActiveTexture(GL_TEXTURE0 + slot));

		s_currentSlot = slot;
	}

	// Only bind if it is not bound yet
	if (s_currentBound[slot] != m_id)
	{
		// Get dimensions enum
		GLenum dims = GL_TEXTURE_1D;
		if (m_dimensions == 2)
		{
			if (m_multisampled)
				dims = GL_TEXTURE_2D_MULTISAMPLE;
			else
				dims = GL_TEXTURE_2D;
		}
		else if (m_dimensions == 3)
			dims = GL_TEXTURE_3D;

		// Bind texture
		glCheck(glBindTexture(dims, m_id));

		s_currentBound[slot] = m_id;
	}
}


///////////////////////////////////////////////////////////
bool Texture::load(const std::string& fname, GLType dtype, TextureFilter filter, TextureWrap wrap, bool mipmap, float adjustForGamma)
{
	// Load the image
	Image img;
	if (!img.load(fname, dtype))
		return false;

	// Adjust pixels
	if (adjustForGamma != 1.0f)
	{
		Uint32 numPixels = img.getWidth() * img.getHeight();
		Uint32 numValues = numPixels * img.getNumChannels();

		if (dtype == GLType::Uint8)
		{
			Uint8* data = (Uint8*)img.getData();

			for (Uint32 i = 0; i < numValues; ++i)
				data[i] = (Uint8)(powf((float)data[i] / 255.0f, adjustForGamma) * 255.0f);
		}
		else if (dtype == GLType::Uint16)
		{
			Uint16* data = (Uint16*)img.getData();

			for (Uint32 i = 0; i < numValues; ++i)
				data[i] = (Uint16)(powf((float)data[i] / 65535.0f, adjustForGamma) * 65535.0f);
		}
		else if (dtype == GLType::Float)
		{
			float* data = (float*)img.getData();

			for (Uint32 i = 0; i < numValues; ++i)
				data[i] = powf(data[i], adjustForGamma);
		}
	}


	if (Window::isContextActive())
		create(img, filter, wrap, mipmap);

	else
	{
		// Get texture format
		PixelFormat fmt;
		if (img.getNumChannels() == 1)
			fmt = PixelFormat::R;
		else if (img.getNumChannels() == 2)
			fmt = PixelFormat::Rg;
		else if (img.getNumChannels() == 3)
			fmt = PixelFormat::Rgb;
		else
			fmt = PixelFormat::Rgba;

		// Store settings
		m_data = img.getData();
		m_width = img.getWidth();
		m_height = img.getHeight();
		m_format = (Uint16)fmt;
		m_dataType = img.getDataType();
		m_filter = (Uint16)filter;
		m_wrap = (Uint16)wrap;
		m_hasMipmaps = mipmap;

		// Make sure the image doesn't free data
		img.setOwnsData(false);
	}

	return true;
}


///////////////////////////////////////////////////////////
bool Texture::finish()
{
	if (!m_data)
		return true;
	if (!Window::isContextActive())
		return false;

	// Create texture
	create(
		m_data,
		(PixelFormat)m_format,
		m_width,
		m_height,
		0,
		m_dataType,
		(TextureFilter)m_filter,
		(TextureWrap)m_wrap,
		m_hasMipmaps
	);

	// Free data
	free(m_data);
	m_data = 0;

	return true;
}


///////////////////////////////////////////////////////////
void Texture::create(void* data, PixelFormat fmt, Uint32 w, Uint32 h, Uint32 d, GLType dtype, TextureFilter filter, TextureWrap wrap, bool mipmap, bool multisampled)
{
	// Set multisampled
	m_multisampled = multisampled;

	// Create texture
	glCheck(glGenTextures(1, &m_id));

	// Unpack odd sized images correctly
	glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	// Determine number of dimensions
	GLenum dims;
	if (!h)
	{
		m_dimensions = 1;
		dims = GL_TEXTURE_1D;
	}
	else if (!d)
	{
		m_dimensions = 2;
		if (m_multisampled)
			dims = GL_TEXTURE_2D_MULTISAMPLE;
		else
			dims = GL_TEXTURE_2D;
	}
	else
	{
		m_dimensions = 3;
		dims = GL_TEXTURE_3D;
	}

	// Get the internal format
	Uint32 internalFmt = getInternalFormat(fmt, dtype);

	// Bind the texture
	bind();

	// Buffer data
	if (m_dimensions == 1)
		glCheck(glTexImage1D(dims, 0, internalFmt, w, 0, (GLenum)fmt, (GLenum)dtype, data));

	else if (m_dimensions == 2)
	{
		// Different texture creation based on if the texture is multisampled
		if (m_multisampled)
			glCheck(glTexImage2DMultisample(dims, 4, internalFmt, w, h, GL_TRUE));
		else
			glCheck(glTexImage2D(dims, 0, internalFmt, w, h, 0, (GLenum)fmt, (GLenum)dtype, data));
	}

	else
		glCheck(glTexImage3D(dims, 0, internalFmt, w, h, d, 0, (GLenum)fmt, (GLenum)dtype, data));

	// If the texture is not multisampled, then set texture filter and wrap parameters
	if (!m_multisampled)
	{
		// Set wrap parameter
		glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_S, (GLint)wrap));
		glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_T, (GLint)wrap));
		glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_R, (GLint)wrap));

		// Generate mipmap if enabled
		if (mipmap)
		{
			glCheck(glGenerateMipmap(dims));

			// Set filter parameters
			GLint magFilter = filter == TextureFilter::Nearest ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
			glCheck(glTexParameteri(dims, GL_TEXTURE_MIN_FILTER, magFilter));
			glCheck(glTexParameteri(dims, GL_TEXTURE_MAG_FILTER, (GLint)filter));
		}
		else
		{
			// Set filter parameters
			glCheck(glTexParameteri(dims, GL_TEXTURE_MIN_FILTER, (GLint)filter));
			glCheck(glTexParameteri(dims, GL_TEXTURE_MAG_FILTER, (GLint)filter));
		}
	}

	// Set member variables
	m_width = w;
	m_height = h;
	m_depth = d;
	m_format = (Uint16)fmt;
	m_dataType = dtype;
	m_wrap = (Uint16)wrap;
	m_filter = (Uint16)filter;
}


///////////////////////////////////////////////////////////
void Texture::create(const Image& image, TextureFilter filter, TextureWrap wrap, bool mipmap)
{
	// Can only create texture once
	if (m_id) return;

	ASSERT(image.getNumChannels() <= 4 && image.getNumChannels() > 0, "Image has invalid number of color channels");

	// Get texture format
	PixelFormat fmt;
	if (image.getNumChannels() == 1)
		fmt = PixelFormat::R;
	else if (image.getNumChannels() == 2)
		fmt = PixelFormat::Rg;
	else if (image.getNumChannels() == 3)
		fmt = PixelFormat::Rgb;
	else
		fmt = PixelFormat::Rgba;

	// Use the other create function
	create(
		image.getData(),
		fmt,
		image.getWidth(),
		image.getHeight(),
		0,
		image.getDataType(),
		filter,
		wrap,
		mipmap,
		false
	);
}


///////////////////////////////////////////////////////////
void Texture::update(void* data)
{
	// Don't update before creating
	if (!m_id) return;

	// Bind the texture
	bind();

	// Get the internal format
	Uint32 internalFmt = getInternalFormat((PixelFormat)m_format, m_dataType);

	// Buffer data
	if (m_dimensions == 1)
		glCheck(glTexImage1D(GL_TEXTURE_1D, 0, internalFmt, m_width, 0, (GLenum)m_format, (GLenum)m_dataType, data));

	else if (m_dimensions == 2)
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, m_width, m_height, 0, (GLenum)m_format, (GLenum)m_dataType, data));

	else
		glCheck(glTexImage3D(GL_TEXTURE_3D, 0, internalFmt, m_width, m_height, m_depth, 0, (GLenum)m_format, (GLenum)m_dataType, data));
}


///////////////////////////////////////////////////////////
void Texture::update(void* data, Uint32 pos, Uint32 size)
{
	// Don't update before creating
	if (!m_id) return;

	// Make sure the texture is 2D
	ASSERT(m_dimensions == 1, "Updating a texture using a 1D region requires having a 1D texture");

	// Bind the texture
	bind();

	// Update the subregion
	glCheck(glTexSubImage1D(GL_TEXTURE_1D, 0, (int)pos, (int)size, (GLenum)m_format, (GLenum)m_dataType, data));
}


///////////////////////////////////////////////////////////
void Texture::update(void* data, const Vector2u& pos, const Vector2u& size)
{
	// Don't update before creating
	if (!m_id) return;

	// Make sure the texture is 2D
	ASSERT(m_dimensions == 2, "Updating a texture using a 2D region requires having a 2D texture");

	// Bind the texture
	bind();

	// Update the subregion
	glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, (int)pos.x, (int)pos.y, (int)size.x, (int)size.y, (GLenum)m_format, (GLenum)m_dataType, data));
}


///////////////////////////////////////////////////////////
void Texture::update(void* data, const Vector3u& pos, const Vector3u& size)
{
	// Don't update before creating
	if (!m_id) return;

	// Make sure the texture is 2D
	ASSERT(m_dimensions == 3, "Updating a texture using a 3D region requires having a 3D texture");

	// Bind the texture
	bind();

	// Update the subregion
	glCheck(glTexSubImage3D(GL_TEXTURE_3D, 0, (int)pos.x, (int)pos.y, (int)pos.z, (int)size.x, (int)size.y, (int)size.z, (GLenum)m_format, (GLenum)m_dataType, data));
}


///////////////////////////////////////////////////////////
void Texture::setFilter(TextureFilter filter)
{
	// Don't set filter before creating
	if (!m_id) return;

	m_filter = (Uint16)filter;

	// Bind the texture
	bind();

	// Get dimensions enum
	GLenum dims = GL_TEXTURE_1D;
	if (m_dimensions == 2)
	{
		if (m_multisampled)
			dims = GL_TEXTURE_2D_MULTISAMPLE;
		else
			dims = GL_TEXTURE_2D;
	}
	else if (m_dimensions == 3)
		dims = GL_TEXTURE_3D;

	// Use different methods depending on if mipmaps were generated
	if (m_hasMipmaps)
	{
		glCheck(glGenerateMipmap(dims));

		// Set filter parameters
		GLint magFilter = filter == TextureFilter::Nearest ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
		glCheck(glTexParameteri(dims, GL_TEXTURE_MIN_FILTER, magFilter));
		glCheck(glTexParameteri(dims, GL_TEXTURE_MAG_FILTER, (GLint)filter));
	}
	else
	{
		// Set filter parameters
		glCheck(glTexParameteri(dims, GL_TEXTURE_MIN_FILTER, (GLint)filter));
		glCheck(glTexParameteri(dims, GL_TEXTURE_MAG_FILTER, (GLint)filter));
	}
}


///////////////////////////////////////////////////////////
void Texture::setWrap(TextureWrap wrap)
{
	// Don't set wrap before creating
	if (!m_id) return;

	m_wrap = (Uint16)wrap;

	// Bind the texture
	bind();

	// Get dimensions enum
	GLenum dims = GL_TEXTURE_1D;
	if (m_dimensions == 2)
	{
		if (m_multisampled)
			dims = GL_TEXTURE_2D_MULTISAMPLE;
		else
			dims = GL_TEXTURE_2D;
	}
	else if (m_dimensions == 3)
		dims = GL_TEXTURE_3D;

	// Set wrap parameter
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_S, (GLint)wrap));
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_T, (GLint)wrap));
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_R, (GLint)wrap));
}


///////////////////////////////////////////////////////////
Uint32 Texture::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
Uint32 Texture::getWidth() const
{
	return m_width;
}


///////////////////////////////////////////////////////////
Uint32 Texture::getHeight() const
{
	return m_height;
}


///////////////////////////////////////////////////////////
Uint32 Texture::getDepth() const
{
	return m_depth;
}


///////////////////////////////////////////////////////////
Uint32 Texture::getNumDimensions() const
{
	return m_dimensions;
}


///////////////////////////////////////////////////////////
PixelFormat Texture::getFormat() const
{
	return (PixelFormat)m_format;
}


///////////////////////////////////////////////////////////
GLType Texture::getDataType() const
{
	return m_dataType;
}


///////////////////////////////////////////////////////////
TextureWrap Texture::getWrap() const
{
	return (TextureWrap)m_wrap;
}


///////////////////////////////////////////////////////////
TextureFilter Texture::getFilter() const
{
	return (TextureFilter)m_filter;
}


///////////////////////////////////////////////////////////
bool Texture::isMultisampled() const
{
	return m_multisampled;
}


///////////////////////////////////////////////////////////
bool Texture::hasMipmaps() const
{
	return m_hasMipmaps;
}


}