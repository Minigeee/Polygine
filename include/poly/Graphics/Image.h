#ifndef POLY_IMAGE_H
#define POLY_IMAGE_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/GLType.h>

#include <poly/Math/Vector2.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief Holds pixel data for 2D images
///
///////////////////////////////////////////////////////////
class Image
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Image();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// If the image owns the pixel data, it will be freed when the
	/// destructor is called. The image is determined to be the owner
	/// of the data when it is loaded from a file, or if the \a manage
	/// parameter is set to true in setData().
	///
	///////////////////////////////////////////////////////////
	~Image();

	///////////////////////////////////////////////////////////
	/// \brief Load 2D image data from a file
	///
	/// When an image is loaded from a file, memeory is allocated
	/// and freed automatically by the image class. Supported image
	/// formats are: PNG, JPEG, BMP, TGA, PSD, GIF, HDR, PIC, and PNM.
	///
	/// \param fname The file path to load
	/// \param dtype The data type to store the pixels in
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, GLType dtype = GLType::Uint8);

	///////////////////////////////////////////////////////////
	/// \brief Free pixel data if it is owned by the image
	///
	/// If the image owns the pixel data, it will be freed when the
	/// destructor is called. The image is determined to be the owner
	/// of the data when it is loaded from a file, or if the \a manage
	/// parameter is set to true in setData().
	///
	///////////////////////////////////////////////////////////
	void free();

	///////////////////////////////////////////////////////////
	/// \brief Create a new image from pixel data
	///
	/// Set a pointer to the pixel data and set width, height,
	/// color channels, and data type. The user is responsible for
	/// managing memory unless \a is et to true. This includes freeing
	/// the memory.
	///
	/// If a NULL pointer is set for data, an empty black image will be
	/// allocated and image will automatically manage the allocated memory.
	///
	/// \param data A pointer to the location pixel data is stored
	/// \param w The width of the 2D image
	/// \param h The hgieht of the 2D image
	/// \param c The number of color channels per pixel
	/// \param dtype The data type of each color
	/// \param manage Determines if data is owned by image or by the user. If set to true, the data will be freed on destruction
	///
	///////////////////////////////////////////////////////////
	void create(void* data, Uint32 w, Uint32 h, Uint32 c, GLType dtype = GLType::Uint8, bool manage = false);

	///////////////////////////////////////////////////////////
	/// \brief Set image size
	///
	/// This does not modfiy any pixel values, rescale the image,
	/// or pad the image. It changes how the pixel data is interpreted.
	///
	/// \param size The new image size
	///
	///////////////////////////////////////////////////////////
	void setSize(const Vector2u& size);

	///////////////////////////////////////////////////////////
	/// \brief Set image size
	///
	/// This does not modfiy any pixel values, rescale the image,
	/// or pad the image. It affects how pixels are retrieved.
	///
	/// \param w The new width
	/// \param h The new height
	///
	///////////////////////////////////////////////////////////
	void setSize(Uint32 w, Uint32 h);

	///////////////////////////////////////////////////////////
	/// \brief Set image data type
	///
	/// This does not modfiy or rescale any pixel values. It affects
	/// how pixels are retrieved.
	///
	/// \param type The new data type
	///
	///////////////////////////////////////////////////////////
	void setDataType(GLType type);

	///////////////////////////////////////////////////////////
	/// \brief Set image data type
	///
	/// This does not modfiy pixel values or fill in any missing
	/// color channels. It affects how pixels are retrieved.
	///
	/// \param num The new number of color channels
	///
	///////////////////////////////////////////////////////////
	void setNumChannels(Uint32 num);

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the pixel data
	///
	/// \return A pointer to pixel data
	///
	///////////////////////////////////////////////////////////
	void* getData() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the image width
	///
	/// \return Image width
	///
	///////////////////////////////////////////////////////////
	Uint32 getWidth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the image width
	///
	/// \return Image width
	///
	///////////////////////////////////////////////////////////
	Uint32 getHeight() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the image data type
	///
	/// \return The image data type
	///
	///////////////////////////////////////////////////////////
	GLType getDataType() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of color channels per pixel
	///
	/// \return The number of color channels per pixel
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumChannels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to a pixel at specified row and coluomn
	///
	/// \param r The row index of the pixel
	/// \param c The column index of the pixel
	///
	/// \return A pointer to the pixel
	///
	///////////////////////////////////////////////////////////
	void* getPixel(Uint32 r, Uint32 c) const;

private:
	void* m_data;					//!< A pointer to pixel data
	Uint32 m_width;					//!< THe image width
	Uint32 m_height;				//!< The image height
	GLType m_dataType;				//!< The pixel data tyoe

	Uint32 m_numChannels	: 31;	//!< The number of color channels per pixel
	Uint32 m_ownsData		: 1;	//!< True if the image owns the data
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Image
/// \ingroup Graphics
///
/// An image stores pixel data. Because images are CPU side pixel
/// data, they can't be rendered like textures. Their purpose
/// is to load data from certain image formats and for pixel
/// manipulation. It is possible to create a texture and from an
/// image and use the texture for rendering.
///
/// More functionality will probably be added in the future.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load an image
/// Image image;
/// image.load("image.png");
///
/// // Get all pixel data
/// Vector3<Uint8>* pixels = (Vector3<Uint8>*)image.getData();
///
/// // Get a specific pixel
/// Vector3<Uint8>* pixel = (Vector3<Uint8>*)image.getPixel(45, 50);
/// pixel->r = 255;
/// pixel->g = 200;
/// pixel->b = 100;
///
/// \endcode
///
///////////////////////////////////////////////////////////