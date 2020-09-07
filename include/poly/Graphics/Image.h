#ifndef POLY_IMAGE_H
#define POLY_IMAGE_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/GLType.h>

#include <poly/Math/Vector2.h>

namespace poly
{

class Image
{
public:
	Image();
	~Image();

	bool load(const std::string& fname, GLType dtype = GLType::Uint8);

	void free();

	void setData(void* data, Uint32 w, Uint32 h, Uint32 c, GLType dtype = GLType::Uint8);

	void setSize(const Vector2u& size);

	void setSize(Uint32 w, Uint32 h);

	void setDataType(GLType type);

	void setNumChannels(Uint32 num);

	void* getData() const;

	Uint32 getWidth() const;

	Uint32 getHeight() const;

	GLType getDataType() const;

	Uint32 getNumChannels() const;

	void* getPixel(Uint32 w, Uint32 h) const;

private:
	void* m_data;
	Uint32 m_width;
	Uint32 m_height;
	GLType m_dataType;

	Uint32 m_numChannels	: 31;
	Uint32 m_ownsData		: 1;
};

}

#endif