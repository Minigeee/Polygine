#include <poly/Core/Allocate.h>
#include <poly/Core/Logger.h>

#include <poly/Graphics/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace poly
{


///////////////////////////////////////////////////////////
Image::Image() :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_dataType		(GLType::Unknown),
	m_numChannels	(0),
	m_ownsData		(false)
{

}


///////////////////////////////////////////////////////////
Image::~Image()
{
	// Free data if image owns the data
	free();
}


///////////////////////////////////////////////////////////
Image::Image(const std::string& fname, GLType dtype) :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_dataType		(GLType::Unknown),
	m_numChannels	(0),
	m_ownsData		(false)
{
	load(fname, dtype);
}


///////////////////////////////////////////////////////////
bool Image::load(const std::string& fname, GLType dtype)
{
	int x = 0, y = 0, c = 0;
	void* data = 0;

	// Load image based on type
	if (dtype == GLType::Uint8)
		data = stbi_load(fname.c_str(), &x, &y, &c, 0);
	else if (dtype == GLType::Uint16)
		data = stbi_load_16(fname.c_str(), &x, &y, &c, 0);
	else if (dtype == GLType::Float)
		data = stbi_loadf(fname.c_str(), &x, &y, &c, 0);
	else
	{
		LOG_ERROR("Unsupported data type while loading image: %s", fname.c_str());
		return false;
	}

	if (!data)
	{
		LOG_ERROR("Failed to load image: %s", fname.c_str());
		return false;
	}

	m_data = data;
	m_width = x;
	m_height = y;
	m_numChannels = c;
	m_dataType = dtype;
	m_ownsData = true;

	LOG("Loaded image: %s", fname.c_str());
	return true;
}


///////////////////////////////////////////////////////////
bool Image::load(void* mem, Uint32 len, GLType dtype)
{
	int x = 0, y = 0, c = 0;
	void* data = 0;

	// Load image based on type
	if (dtype == GLType::Uint8)
		data = stbi_load_from_memory((Uint8*)mem, len, &x, &y, &c, 0);
	else if (dtype == GLType::Uint16)
		data = stbi_load_16_from_memory((Uint8*)mem, len, &x, &y, &c, 0);
	else if (dtype == GLType::Float)
		data = stbi_loadf_from_memory((Uint8*)mem, len, &x, &y, &c, 0);
	else
	{
		LOG_ERROR("Unsupported data type while loading image from memory");
		return false;
	}

	if (!data)
	{
		LOG_ERROR("Failed to load image from memory");
		return false;
	}

	m_data = data;
	m_width = x;
	m_height = y;
	m_numChannels = c;
	m_dataType = dtype;
	m_ownsData = true;

	return true;
}


///////////////////////////////////////////////////////////
void Image::free()
{
	if (m_ownsData && m_data)
		FREE_DBG(m_data);

	m_data = 0;
}


///////////////////////////////////////////////////////////
void Image::create(void* data, Uint32 w, Uint32 h, Uint32 c, GLType dtype, bool manage)
{
	// 32-bit colors are not supported
	ASSERT(dtype != GLType::Int32 && dtype != GLType::Uint32, "32-bit image colors are not supported");

	// Free previous data
	free();

	m_data = data;
	m_width = w;
	m_height = h;
	m_numChannels = c;
	m_dataType = dtype;
	m_ownsData = manage;
	
	// Make sure the data type is an unsigned variant
	if (m_dataType == GLType::Int8)
		m_dataType = GLType::Uint8;
	else if (m_dataType == GLType::Int16)
		m_dataType = GLType::Uint16;

	// Create an empty image if data is a null pointer
	if (!m_data)
	{
		Uint32 typeSize = 1;
		if (m_dataType == GLType::Uint16)
			typeSize = 2;
		else if (m_dataType == GLType::Float)
			typeSize = 4;

		// Allocate data
		Uint32 size = m_width * m_height * m_numChannels * typeSize;
		m_data = MALLOC_DBG(size);
		memset(m_data, 0, size);

		// Image owns data
		m_ownsData = true;
	}
}


///////////////////////////////////////////////////////////
void Image::setSize(const Vector2u& size)
{
	m_width = size.x;
	m_height = size.y;
}


///////////////////////////////////////////////////////////
void Image::setSize(Uint32 w, Uint32 h)
{
	m_width = w;
	m_height = h;
}


///////////////////////////////////////////////////////////
void Image::setDataType(GLType dtype)
{
	m_dataType = dtype;
}


///////////////////////////////////////////////////////////
void Image::setNumChannels(Uint32 c)
{
	m_numChannels = c;
}


///////////////////////////////////////////////////////////
void* Image::getData() const
{
	return m_data;
}


///////////////////////////////////////////////////////////
Uint32 Image::getWidth() const
{
	return m_width;
}


///////////////////////////////////////////////////////////
Uint32 Image::getHeight() const
{
	return m_height;
}


///////////////////////////////////////////////////////////
GLType Image::getDataType() const
{
	return m_dataType;
}


///////////////////////////////////////////////////////////
Uint32 Image::getNumChannels() const
{
	return m_numChannels;
}


///////////////////////////////////////////////////////////
void* Image::getPixel(Uint32 r, Uint32 c) const
{
	ASSERT(r < m_height&& c < m_width, "Requested image pixel row or column is out of bounds: %d, %d", r, c);

	Uint32 typeSize = 1;
	if (m_dataType == GLType::Uint16)
		typeSize = 2;
	else if (m_dataType == GLType::Float)
		typeSize = 4;

	return (Uint8*)m_data + (r * m_width + c) * m_numChannels * typeSize;
}


}