#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Texture.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 Texture::currentBound[] = {};


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

	return fmt;
}


///////////////////////////////////////////////////////////
Texture::Texture() :
	m_id			(0),
	m_width			(0),
	m_height		(0),
	m_depth			(0),
	m_dimensions	(2),
	m_format		(PixelFormat::Rgb),
	m_dataType		(GLType::Uint8),
	m_wrap			(TextureWrap::ClampToEdge),
	m_filter		(TextureFilter::Linear)
{

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
		LOG_WARNING("Trying to bind a texture that doesn't exist yet");
		return;
	}

	ASSERT(slot < 100, "Texture slot number is too big");

	// Only bind if it is not bound yet
	if (currentBound[slot] != m_id)
	{
		// Get dimensions enum
		GLenum dims = GL_TEXTURE_1D;
		if (m_dimensions == 2)
			dims = GL_TEXTURE_2D;
		else if (m_dimensions == 3)
			dims = GL_TEXTURE_3D;

		// Bind texture
		glCheck(glActiveTexture(GL_TEXTURE0 + slot));
		glCheck(glBindTexture(dims, m_id));

		currentBound[slot] = m_id;
	}
}


///////////////////////////////////////////////////////////
void Texture::create(void* data, PixelFormat fmt, Uint32 w, Uint32 h, Uint32 d, GLType dtype, TextureFilter filter, TextureWrap wrap)
{
	// Can only create texture once
	if (m_id) return;

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
		glCheck(glTexImage2D(dims, 0, internalFmt, w, h, 0, (GLenum)fmt, (GLenum)dtype, data));

	else
		glCheck(glTexImage3D(dims, 0, internalFmt, w, h, d, 0, (GLenum)fmt, (GLenum)dtype, data));

	// Set filter parameter
	glCheck(glTexParameteri(dims, GL_TEXTURE_MIN_FILTER, (GLint)filter));
	glCheck(glTexParameteri(dims, GL_TEXTURE_MAG_FILTER, (GLint)filter));

	// Set wrap parameter
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_S, (GLint)wrap));
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_T, (GLint)wrap));
	glCheck(glTexParameteri(dims, GL_TEXTURE_WRAP_R, (GLint)wrap));

	// Set member variables
	m_width = w;
	m_height = h;
	m_depth = d;
	m_format = fmt;
	m_dataType = dtype;
	m_wrap = wrap;
	m_filter = filter;
}


///////////////////////////////////////////////////////////
void Texture::create(const Image& image, TextureFilter filter, TextureWrap wrap)
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
		wrap
	);
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
	return m_format;
}


///////////////////////////////////////////////////////////
GLType Texture::getDataType() const
{
	return m_dataType;
}


///////////////////////////////////////////////////////////
TextureWrap Texture::getWrap() const
{
	return m_wrap;
}


///////////////////////////////////////////////////////////
TextureFilter Texture::getFilter() const
{
	return m_filter;
}


}