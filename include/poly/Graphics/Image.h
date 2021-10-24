#ifndef POLY_IMAGE_H
#define POLY_IMAGE_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/GLType.h>

#include <poly/Math/Vector2.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief Provides a convenient way to store and manipulate image pixel data
///
///////////////////////////////////////////////////////////
template <typename T>
class ImageBuffer
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates an empty buffer with no size
	///
	///////////////////////////////////////////////////////////
	ImageBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Creates a buffer with the specified width and height and filled with the given value
	///
	/// \param w The width of the image in pixels
	/// \param h The height of the image in pixels
	/// \param value The value to fill the buffer with
	///
	///////////////////////////////////////////////////////////
	ImageBuffer(Uint32 w, Uint32 h, const T& value = T(0));

	///////////////////////////////////////////////////////////
	/// \brief Create an image buffer with the specified size and filled with the given value
	///
	/// \param size The width and height of the image buffer
	/// \param value The value to fill the buffer with
	///
	///////////////////////////////////////////////////////////
	ImageBuffer(Uint32 size, const T& value = T(0));

	///////////////////////////////////////////////////////////
	/// \brief Create an image buffer from the specified data and image size
	///
	/// Any buffer created with data allocated from an outside source
	/// will be freed during the destruction of this buffer.
	///
	/// \param data A pointer to the data
	/// \param w The width of the image buffer in pixels
	/// \param h The height of the image buffer in pixels
	///
	///////////////////////////////////////////////////////////
	ImageBuffer(T* data, Uint32 w, Uint32 h);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~ImageBuffer();

#ifndef DOXYGEN_SKIP
	ImageBuffer(const ImageBuffer<T>&);
	ImageBuffer& operator=(const ImageBuffer<T>&);
	ImageBuffer(ImageBuffer<T>&&);
	ImageBuffer& operator=(ImageBuffer<T>&&);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Convert to a different type
	///
	///////////////////////////////////////////////////////////
	template <typename U>
	operator ImageBuffer<U>() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to a row in the image buffer
	///
	/// The returned row pointer can be used to access a specific
	/// pixel within the row
	///
	/// \param index The row index
	///
	///////////////////////////////////////////////////////////
	T* operator[](Uint32 index) const;

	///////////////////////////////////////////////////////////
	/// \brief Create an image buffer with the specified size and filled with the given value
	///
	/// \param w The width of the image buffer in pixels
	/// \param h The height of the image buffer in pixels
	/// \param value The value to fill the buffer with
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 w, Uint32 h, const T& value = T(0));

	///////////////////////////////////////////////////////////
	/// \brief Create an image buffer with the specified size and filled with the given value
	///
	/// \param size The width and height of the image buffer
	/// \param value The value to fill the buffer with
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 size, const T& value = T(0));

	///////////////////////////////////////////////////////////
	/// \brief Create an image buffer from the specified data and image size
	///
	/// Any buffer created with data allocated from an outside source
	/// will be freed during the destruction of this buffer.
	///
	/// \param data A pointer to the data
	/// \param w The width of the image buffer in pixels
	/// \param h The height of the image buffer in pixels
	///
	///////////////////////////////////////////////////////////
	void create(T* data, Uint32 w, Uint32 h);

	///////////////////////////////////////////////////////////
	/// \brief Get width of the image buffer in pixels
	///
	/// \return The width of the image buffer in pixels
	///
	///////////////////////////////////////////////////////////
	Uint32 getWidth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get height of the image buffer in pixels
	///
	/// \return The height of the image buffer in pixels
	///
	///////////////////////////////////////////////////////////
	Uint32 getHeight() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the data
	///
	///////////////////////////////////////////////////////////
	T* getData() const;

	///////////////////////////////////////////////////////////
	/// \brief Execute a segment of code for every pixel in subrectangle of the image buffer
	///
	/// The function must accept: \a pixel, \a row, \a col parameters.
	/// The subrectangle must be specified in pixels. The subrectangle
	/// will automatically be clamped to buffer bounds. If \a dr or \a dc, the
	/// subrectangle sizes, are negative, the full image will be used.
	///
	/// \param func The function to execute for each pixel
	/// \param r The subrectangle row position
	/// \param c The subrectangle col position
	/// \param dr The subrectangle size along the row axis
	/// \param dr The subrectangle size along the col axis
	///
	///////////////////////////////////////////////////////////
	template <typename Func>
	void forEach(Func&& func, int r = 0, int c = 0, int dr = -1, int dc = -1) const;

private:
	T* m_data;			//!< A pointer to the data
	Uint32 m_width;		//!< The height of the image buffer in pixels
	Uint32 m_height;	//!< The height of the image buffer in pixels
	bool m_ownsData;	//!< True if the image buffer owns the data
};


///////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////

template <typename T, typename U>
ImageBuffer<T>& operator+=(ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T>& operator+=(ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T>& operator-=(ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T>& operator-=(ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T>& operator*=(ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T>& operator*=(ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T>& operator/=(ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T>& operator/=(ImageBuffer<T>& a, const ImageBuffer<U>& b);


template <typename T, typename U>
ImageBuffer<T> operator+(const ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T> operator+(U a, const ImageBuffer<T>& b);

template <typename T, typename U>
ImageBuffer<T> operator+(const ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T> operator-(const ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T> operator-(U a, const ImageBuffer<T>& b);

template <typename T, typename U>
ImageBuffer<T> operator-(const ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T> operator*(const ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T> operator*(U a, const ImageBuffer<T>& b);

template <typename T, typename U>
ImageBuffer<T> operator*(const ImageBuffer<T>& a, const ImageBuffer<U>& b);

template <typename T, typename U>
ImageBuffer<T> operator/(const ImageBuffer<T>& a, U b);

template <typename T, typename U>
ImageBuffer<T> operator/(U a, const ImageBuffer<T>& b);

template <typename T, typename U>
ImageBuffer<T> operator/(const ImageBuffer<T>& a, const ImageBuffer<U>& b);


///////////////////////////////////////////////////////////
/// \brief Perform a square root operation on every pixel in the input image buffer
///
/// \param x The input image buffer
///
/// \return A new image buffer containing the operation results
///
///////////////////////////////////////////////////////////
template <typename T>
ImageBuffer<T> sqrt(const ImageBuffer<T>& x);

///////////////////////////////////////////////////////////
/// \brief Perform a power operation on every pixel in the input image buffer
///
/// \param x The input image buffer containing the base value
/// \param p The power value
///
/// \return A new image buffer containing the operation results
///
///////////////////////////////////////////////////////////
template <typename T, typename P>
ImageBuffer<T> pow(const ImageBuffer<T>& b, const P& p);

///////////////////////////////////////////////////////////
/// \brief Clamp every pixel within the image buffer to the given range
///
/// \param x The input image buffer
/// \param a The minimum value of the clamp range
/// \param b The maximum value of the clamp range
///
/// \return A new image buffer containing the operation results
///
///////////////////////////////////////////////////////////
template <typename T, typename A, typename B>
ImageBuffer<T> clamp(const ImageBuffer<T>& x, const A& a, const B& b);

///////////////////////////////////////////////////////////
/// \brief Remap every pixel within the image buffer from the original range to a new range
///
/// This operation does not perform any clamping before or
/// after the remap operation.
///
/// \param x The input image buffer
/// \param a1 The minimum value of the original range
/// \param b1 The maximum value of the original range
/// \param a2 The minimum value of the new range
/// \param b2 The maximum value of the new range
///
/// \return A new image buffer containing the operation results
///
///////////////////////////////////////////////////////////
template <typename T, typename A1, typename B1, typename A2, typename B2>
ImageBuffer<T> remap(const ImageBuffer<T>& x, const A1& a1, const B1& b1, const A2& a2, const B2& b2);

///////////////////////////////////////////////////////////
/// \brief Linearly interpolate between an image buffer and another value
///
/// \param a The input image buffer
/// \param b The second interpolation operand
/// \param factor The interpolation factor
///
/// \return A new image buffer containing the operation results
///
///////////////////////////////////////////////////////////
template <typename T, typename U>
ImageBuffer<T> mix(const ImageBuffer<T>& a, const U& b, float factor);

///////////////////////////////////////////////////////////
/// \brief Get the minumum value in an image buffer
///
/// \param x The input image buffer
///
/// \return The minimum value in the image buffer
///
///////////////////////////////////////////////////////////
template <typename T>
T min(const ImageBuffer<T>& x);

///////////////////////////////////////////////////////////
/// \brief Get the maximum value in an image buffer
///
/// \param x The input image buffer
///
/// \return The maximum value in the image buffer
///
///////////////////////////////////////////////////////////
template <typename T>
T max(const ImageBuffer<T>& x);

///////////////////////////////////////////////////////////
/// \brief Get the pixel coordinate (r, c) of the minumum value in an image buffer
///
/// \param x The input image buffer
///
/// \return The pixel coordinate (r, c) of the minumum value
///
///////////////////////////////////////////////////////////
template <typename T>
Vector2u argmin(const ImageBuffer<T>& x);

///////////////////////////////////////////////////////////
/// \brief Get the pixel coordinate (r, c) of the maximum value in an image buffer
///
/// \param x The input image buffer
///
/// \return The pixel coordinate (r, c) of the maximum value
///
///////////////////////////////////////////////////////////
template <typename T>
Vector2u argmax(const ImageBuffer<T>& x);


///////////////////////////////////////////////////////////
/// \brief Holds pixel data for 2D images
///
///////////////////////////////////////////////////////////
class Image
{
	template <typename T>
	friend class ImageBuffer;

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
	/// \brief Load 2D image data from an image file
	///
	/// \param fname The file path to load
	/// \param dtype The data type to store the pixels in
	///
	/// \see load
	///
	///////////////////////////////////////////////////////////
	Image(const std::string& fname, GLType dtype = GLType::Uint8);

#ifndef DOXYGEN_SKIP
	Image(const Image& other);
	Image& operator=(const Image& other);
	Image(Image&& other);
	Image& operator=(Image&& other);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Load 2D image data from an image file
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
	/// \brief Load 2D image data from memory
	///
	/// When an image is loaded from memory, new memeory for the
	/// uncompressed pixels is allocated and freed automatically
	/// by the image class. Supported image formats are: PNG,
	/// JPEG, BMP, TGA, PSD, GIF, HDR, PIC, and PNM.
	///
	/// \param data A pointer to the unparsed image (compressed data)
	/// \param len The length of the data in memory (in bytes)
	/// \param dtype The data type to store the pixels in
	///
	///////////////////////////////////////////////////////////
	bool load(void* data, Uint32 len, GLType dtype = GLType::Uint8);

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
	/// \brief Create a new image from an image buffer
	///
	/// When creating an image using an image buffer, the current
	/// image will inherit ownership of the buffer data, only if
	/// the buffer has ownership of the data. The data type and number
	/// of color channels will be inferred from the buffer template type.
	///
	/// \param buffer The image buffer
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(ImageBuffer<T>& buffer);

	///////////////////////////////////////////////////////////
	/// \copydoc create(const ImageBuffer<T>&)
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(ImageBuffer<Vector2<T>>& buffer);

	///////////////////////////////////////////////////////////
	/// \copydoc create(const ImageBuffer<T>&)
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(ImageBuffer<Vector3<T>>& buffer);

	///////////////////////////////////////////////////////////
	/// \copydoc create(const ImageBuffer<T>&)
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(ImageBuffer<Vector4<T>>& buffer);

	///////////////////////////////////////////////////////////
	/// \brief Scales the image to a new size
	///
	/// This function allocates a new block of memory to store the
	/// data of the scaled image. After the image is rescaled, if the
	/// image owned the previous data, then it is freed. Otherwise,
	/// nothing happens. The image automatically takes ownership
	/// of the new data.
	///
	/// \note This function only works for GLType::Uint8 and GLType::Float, nothing happens if the incorrect data type is used
	///
	/// \param w The width of the new image
	/// \param h The height of the new image
	///
	///////////////////////////////////////////////////////////
	void resize(Uint32 w, Uint32 h);

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
	/// \brief Set whether image should take ownership of internal data
	///
	/// This can be used to force image to handle releasing memory
	/// during destruction, or to give up ownership of data.
	///
	/// \param owns A boolean specifying whether the image should take ownership of data
	///
	///////////////////////////////////////////////////////////
	void setOwnsData(bool owns);

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
	/// \brief Get an image buffer using the image data
	///
	/// The current image retains ownership of pixel data, if it
	/// currently has ownership.
	///
	/// \return An image buffer
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	ImageBuffer<T> getBuffer() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to a pixel at specified row and column
	///
	/// \param r The row index of the pixel
	/// \param c The column index of the pixel
	///
	/// \return A pointer to the pixel
	///
	///////////////////////////////////////////////////////////
	void* getPixel(Uint32 r, Uint32 c) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the value of a pixel using linear interpolation
	///
	/// \param r The row index of the pixel
	/// \param c The column index of the pixel
	/// \param out The location to store the interpolated value of the image at the given coordinates
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void getPixel(float r, float c, T* out) const;

private:
	void* m_data;					//!< A pointer to pixel data
	Uint32 m_width;					//!< THe image width
	Uint32 m_height;				//!< The image height
	GLType m_dataType;				//!< The pixel data type

	Uint32 m_numChannels	: 31;	//!< The number of color channels per pixel
	Uint32 m_ownsData		: 1;	//!< True if the image owns the data
};

}

#include <poly/Graphics/Image.inl>

#endif


///////////////////////////////////////////////////////////
/// \class poly::ImageBuffer
/// \ingroup Graphics
///
/// An image buffer provides a convenient way to store and
/// manipulate image pixel data. An image buffer can be created
/// using the create() function and providing the size, and optionally,
/// a pointer to existing continuous row-major pixel data. If a pointer to
/// existing data is given, you can tell the image buffer to
/// manage the freeing of the data during the buffer's destruction.
/// An image buffer can also be created by using Image::getBuffer(),
/// which uses the data already allocated by the image.
///
/// Images can be created from an image buffer using Image::create().
/// When an image is created from an image buffer, the image inherits
/// ownership of the internal data, if the buffer had original ownership.
/// Many common mathematical operations and functions can be performed
/// on an image buffer, but most of these operations and functions
/// create an entire new image buffer with the same size as the input
/// image buffers. So be careful when using mathematical operators
/// on large image buffers.
///
/// To operate on image buffers with more space or time efficiency,
/// it is often times better to loop through the data in a normal way, or
/// to use the forEach() function to loop through each pixel in the
/// buffer.
///
/// To access individual pixels, use the [r][c] operator with row-major
/// indexing.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a 512x512 float image with an intial value of 1.0
/// ImageBuffer<float> buffer(512, 512, 1.0f);
///
/// // Do some math operations
/// buffer = buffer * 2.0f - 1.0f;
///
/// // Get pixel at 5th row, 35th column
/// float p = buffer[4][34];
/// buffer[1][1] = p;
///
/// // Assign to image
/// Image img;
/// img.create(buffer);
///
/// \endcode
///
///////////////////////////////////////////////////////////


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