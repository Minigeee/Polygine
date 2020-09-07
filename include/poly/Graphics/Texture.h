#ifndef POLY_TEXTURE_H
#define POLY_TEXTURE_H

#include <poly/Core/DataTypes.h>

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

enum class TextureFormat
{
	R		= 0x1903,
	Rg		= 0x8227,
	Rgb		= 0x1907,
	Rgba	= 0x1908,
	Depth	= 0x1902
};

class Texture
{
public:
	Texture();
	~Texture();

	void bind(Uint32 slot = 0);

	void create(
		void* data,
		TextureFormat fmt,
		Uint32 w,
		Uint32 h = 0,
		Uint32 d = 0,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge,
		GLType dtype = GLType::Uint8
	);

	void create(
		const Image& image,
		TextureFilter filter = TextureFilter::Linear,
		TextureWrap wrap = TextureWrap::ClampToEdge
	);

	Uint32 getId() const;

	Uint32 getNumDimensions() const;

private:
	Uint32 m_id;
	Uint32 m_dimensions;

	static Uint32 currentBound[100];
};

}

#endif