#include <poly/Core/TypeInfo.h>

#include <poly/Math/Functions.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
template <typename T>
struct PixelMatchesType
{
	static bool check(GLType dtype, Uint32 c) { return getGLType<T>() == dtype && c == 1; }
};


///////////////////////////////////////////////////////////
template <typename T>
struct PixelMatchesType<Vector2<T>>
{
	static bool check(GLType dtype, Uint32 c) { return getGLType<T>() == dtype && c == 2; }
};


///////////////////////////////////////////////////////////
template <typename T>
struct PixelMatchesType<Vector3<T>>
{
	static bool check(GLType dtype, Uint32 c) { return getGLType<T>() == dtype && c == 3; }
};


///////////////////////////////////////////////////////////
template <typename T>
struct PixelMatchesType<Vector4<T>>
{
	static bool check(GLType dtype, Uint32 c) { return getGLType<T>() == dtype && c == 4; }
};


}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer() :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_ownsData		(false)
{

}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer(Uint32 w, Uint32 h, const T& value) :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_ownsData		(false)
{
	// Create map
	create(w, h, value);
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer(Uint32 size, const T& value) :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_ownsData		(false)
{
	// Create map
	create(size, value);
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer(T* data, Uint32 w, Uint32 h) :
	m_data			(0),
	m_width			(0),
	m_height		(0),
	m_ownsData		(false)
{
	// Create map
	create(data, w, h);
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer(const ImageBuffer<T>& other) :
	m_data			(0),
	m_width			(other.m_width),
	m_height		(other.m_height),
	m_ownsData		(true)
{
	// Allocate space
	Uint32 numPixels = m_width * m_height;
	m_data = (T*)malloc(numPixels * sizeof(T));

	// Copy data
	memcpy(m_data, other.m_data, numPixels * sizeof(T));
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>& ImageBuffer<T>::operator=(const ImageBuffer<T>& other)
{
	if (this != &other)
	{
		// Free previous
		if (m_data && m_ownsData)
			::free(m_data);

		m_data = 0;
		m_width = other.m_width;
		m_height = other.m_height;
		m_ownsData = true;

		// Allocate space
		Uint32 numPixels = m_width * m_height;
		m_data = (T*)malloc(numPixels * sizeof(T));

		// Copy data
		memcpy(m_data, other.m_data, numPixels * sizeof(T));
	}

	return *this;
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::ImageBuffer(ImageBuffer<T>&& other) :
	m_data			(other.m_data),
	m_width			(other.m_width),
	m_height		(other.m_height),
	m_ownsData		(other.m_ownsData)
{
	other.m_data = 0;
	other.m_width = 0;
	other.m_height = 0;
	other.m_ownsData = false;
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>& ImageBuffer<T>::operator=(ImageBuffer<T>&& other)
{
	if (this != &other)
	{
		// Free previous
		if (m_data && m_ownsData)
			::free(m_data);

		m_data = other.m_data;
		m_width = other.m_width;
		m_height = other.m_height;
		m_ownsData = other.m_ownsData;

		other.m_data = 0;
		other.m_width = 0;
		other.m_height = 0;
		other.m_ownsData = false;
	}

	return *this;
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T>::~ImageBuffer()
{
	// All data types should be POD so no destructor
	if (m_data && m_ownsData)
		::free(m_data);

	m_width = 0;
	m_height = 0;
	m_data = 0;
	m_ownsData = false;
}


///////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline ImageBuffer<T>::operator ImageBuffer<U>() const
{
	ImageBuffer<U> result(m_width, m_height);
	U* dst = result.getData();

	Uint32 numPixels = m_width * m_height;

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = (U)m_data[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T* ImageBuffer<T>::operator[](Uint32 index) const
{
	return m_data + m_width * index;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ImageBuffer<T>::create(Uint32 w, Uint32 h, const T& value)
{
	// Set size
	m_width = w;
	m_height = h;
	m_ownsData = true;

	// Allocate space
	Uint32 numPixels = w * h;
	m_data = (T*)malloc(numPixels * sizeof(T));

	// Construct all objects
	for (Uint32 i = 0; i < numPixels; ++i)
		new(m_data + i)T(value);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ImageBuffer<T>::create(Uint32 size, const T& value)
{
	// Set size
	m_width = size;
	m_height = size;
	m_ownsData = true;

	// Allocate space
	Uint32 numPixels = size * size;
	m_data = (T*)malloc(numPixels * sizeof(T));

	// Construct all objects
	for (Uint32 i = 0; i < numPixels; ++i)
		new(m_data + i)T(value);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ImageBuffer<T>::create(T* data, Uint32 w, Uint32 h)
{
	m_data = data;
	m_width = w;
	m_height = h;
	m_ownsData = false;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 ImageBuffer<T>::getWidth() const
{
	return m_width;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 ImageBuffer<T>::getHeight() const
{
	return m_height;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T* ImageBuffer<T>::getData() const
{
	return m_data;
}


///////////////////////////////////////////////////////////
template <typename T>
template <typename Func>
inline void ImageBuffer<T>::forEach(Func&& func, int r, int c, int dr, int dc) const
{
	// Calculate subrectangle bounds
	int rs = clamp(r, 0, (int)m_height - 1);
	int cs = clamp(c, 0, (int)m_width - 1);
	dr = dr < 0 ? m_height : dr;
	dc = dc < 0 ? m_width : dc;
	int rf = clamp(r + dr, 0, (int)m_height);
	int cf = clamp(c + dc, 0, (int)m_width);

	// Execute segment for each pixel
	for (r = rs; r < rf; ++r)
	{
		for (c = cs; c < cf; ++c)
			func(m_data[r * m_width + c], r, c);
	}
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator+=(ImageBuffer<T>& a, U b)
{
	T* dst = a.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] += b;

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator+=(ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	T* dst = a.getData();
	T* src = b.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] += src[i];

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator-=(ImageBuffer<T>& a, U b)
{
	T* dst = a.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] -= b;

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator-=(ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	T* dst = a.getData();
	T* src = b.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] -= src[i];

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator*=(ImageBuffer<T>& a, U b)
{
	T* dst = a.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] *= b;

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator*=(ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	T* dst = a.getData();
	T* src = b.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] *= src[i];

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator/=(ImageBuffer<T>& a, U b)
{
	T* dst = a.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] /= b;

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T>& operator/=(ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	T* dst = a.getData();
	T* src = b.getData();
	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] /= src[i];

	return a;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator+(const ImageBuffer<T>& a, U b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] + b;

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator+(U a, const ImageBuffer<T>& b)
{
	ImageBuffer<T> result(b.getWidth(), b.getHeight());
	T* dst = result.getData();
	T* _2 = b.getData();

	Uint32 numPixels = b.getWidth() * b.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = a + _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator+(const ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();
	T* _2 = b.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] + _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator-(const ImageBuffer<T>& a, U b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] - b;

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator-(U a, const ImageBuffer<T>& b)
{
	ImageBuffer<T> result(b.getWidth(), b.getHeight());
	T* dst = result.getData();
	T* _2 = b.getData();

	Uint32 numPixels = b.getWidth() * b.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = a - _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator-(const ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();
	T* _2 = b.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] - _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator*(const ImageBuffer<T>& a, U b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] * b;

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator*(U a, const ImageBuffer<T>& b)
{
	ImageBuffer<T> result(b.getWidth(), b.getHeight());
	T* dst = result.getData();
	T* _2 = b.getData();

	Uint32 numPixels = b.getWidth() * b.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = a * _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator*(const ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();
	T* _2 = b.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] * _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator/(const ImageBuffer<T>& a, U b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] / b;

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator/(U a, const ImageBuffer<T>& b)
{
	ImageBuffer<T> result(b.getWidth(), b.getHeight());
	T* dst = result.getData();
	T* _2 = b.getData();

	Uint32 numPixels = b.getWidth() * b.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = a / _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> operator/(const ImageBuffer<T>& a, const ImageBuffer<U>& b)
{
	ImageBuffer<T> result(a.getWidth(), a.getHeight());
	T* dst = result.getData();
	T* _1 = a.getData();
	T* _2 = b.getData();

	Uint32 numPixels = a.getWidth() * a.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = _1[i] / _2[i];

	return result;
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T> sqrt(const ImageBuffer<T>& x)
{
	ImageBuffer<T> result(x.getWidth(), x.getHeight());
	T* dst = result.getData();
	T* data = x.getData();

	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = sqrt(data[i]);

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename P>
inline ImageBuffer<T> pow(const ImageBuffer<T>& b, const P& p)
{
	ImageBuffer<T> result(b.getWidth(), b.getHeight());
	T* dst = result.getData();
	T* data = b.getData();

	Uint32 numPixels = b.getWidth() * b.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = ::pow(data[i], p);

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename A, typename B>
inline ImageBuffer<T> clamp(const ImageBuffer<T>& x, const A& a, const B& b)
{
	ImageBuffer<T> result(x.getWidth(), x.getHeight());
	T* dst = result.getData();
	T* data = x.getData();

	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 0; i < numPixels; ++i)
		dst[i] = clamp(data[i], a, b);

	return result;
}


///////////////////////////////////////////////////////////
template <typename T, typename A1, typename B1, typename A2, typename B2>
inline ImageBuffer<T> remap(const ImageBuffer<T>& x, const A1& a1, const B1& b1, const A2& a2, const B2& b2)
{
	return a1 + (b2 - a2) * (x - a1) / (b1 - a1);
}


///////////////////////////////////////////////////////////
template <typename T, typename U>
inline ImageBuffer<T> mix(const ImageBuffer<T>& a, const U& b, float factor)
{
	return a + (b - a) * factor;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T min(const ImageBuffer<T>& x)
{
	T* data = x.getData();

	T min = data[0];
	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 1; i < numPixels; ++i)
	{
		if (data[i] < min)
			min = data[i];
	}

	return min;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T max(const ImageBuffer<T>& x)
{
	T* data = x.getData();

	T max = data[0];
	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 1; i < numPixels; ++i)
	{
		if (data[i] > max)
			max = data[i];
	}

	return max;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Vector2u argmin(const ImageBuffer<T>& x)
{
	T* data = x.getData();

	T min = data[0];
	Uint32 minIndex = 0;
	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 1; i < numPixels; ++i)
	{
		if (data[i] < min)
		{
			min = data[i];
			minIndex = i;
		}
	}

	return Vector2u(minIndex / x.getWidth(), minIndex % x.getWidth());
}


///////////////////////////////////////////////////////////
template <typename T>
inline Vector2u argmax(const ImageBuffer<T>& x)
{
	T* data = x.getData();

	T max = data[0];
	Uint32 maxIndex = 0;
	Uint32 numPixels = x.getWidth() * x.getHeight();

	for (Uint32 i = 1; i < numPixels; ++i)
	{
		if (data[i] > max)
		{
			max = data[i];
			maxIndex = i;
		}
	}

	return Vector2u(maxIndex / x.getWidth(), maxIndex % x.getWidth());
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T> resize(const ImageBuffer<T>& buffer, Uint32 w, Uint32 h)
{
	// Resize image
	ImageBuffer<T> dst(w, h);
	priv::resize(buffer.getData(), dst.getData(), buffer.getWidth(), buffer.getHeight(), w, h, 1, getGLType<T>());

	// Return new buffer
	return dst;
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<Vector2<T>> resize(const ImageBuffer<Vector2<T>>& buffer, Uint32 w, Uint32 h)
{
	// Resize image
	ImageBuffer<Vector2<T>> dst(w, h);
	priv::resize(buffer.getData(), dst.getData(), buffer.getWidth(), buffer.getHeight(), w, h, 2, getGLType<T>());

	// Return new buffer
	return dst
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<Vector3<T>> resize(const ImageBuffer<Vector3<T>>& buffer, Uint32 w, Uint32 h)
{
	// Resize image
	ImageBuffer<Vector3<T>> dst(w, h);
	priv::resize(buffer.getData(), dst.getData(), buffer.getWidth(), buffer.getHeight(), w, h, 3, getGLType<T>());

	// Return new buffer
	return dst
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<Vector4<T>> resize(const ImageBuffer<Vector4<T>>& buffer, Uint32 w, Uint32 h)
{
	// Resize image
	ImageBuffer<Vector4<T>> dst(w, h);
	priv::resize(buffer.getData(), dst.getData(), buffer.getWidth(), buffer.getHeight(), w, h, 4, getGLType<T>());

	// Return new buffer
	return dst
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Image::create(ImageBuffer<T>& buffer)
{
	// Free previous data if needed
	if (m_data && m_ownsData)
		::free(m_data);

	// Common properties
	m_data = buffer.m_data;
	m_width = buffer.m_width;
	m_height = buffer.m_height;
	m_ownsData = buffer.m_ownsData;
	buffer.m_ownsData = false;

	// Type specific properties
	m_numChannels = 1;
	m_dataType = getGLType<T>();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Image::create(ImageBuffer<Vector2<T>>& buffer)
{
	// Free previous data if needed
	if (m_data && m_ownsData)
		::free(m_data);

	// Common properties
	m_data = buffer.m_data;
	m_width = buffer.m_width;
	m_height = buffer.m_height;
	m_ownsData = buffer.m_ownsData;
	buffer.m_ownsData = false;

	// Type specific properties
	m_numChannels = 2;
	m_dataType = getGLType<T>();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Image::create(ImageBuffer<Vector3<T>>& buffer)
{
	// Free previous data if needed
	if (m_data && m_ownsData)
		::free(m_data);

	// Common properties
	m_data = buffer.m_data;
	m_width = buffer.m_width;
	m_height = buffer.m_height;
	m_ownsData = buffer.m_ownsData;
	buffer.m_ownsData = false;

	// Type specific properties
	m_numChannels = 3;
	m_dataType = getGLType<T>();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void Image::create(ImageBuffer<Vector4<T>>& buffer)
{
	// Free previous data if needed
	if (m_data && m_ownsData)
		::free(m_data);

	// Common properties
	m_data = buffer.m_data;
	m_width = buffer.m_width;
	m_height = buffer.m_height;
	m_ownsData = buffer.m_ownsData;
	buffer.m_ownsData = false;

	// Type specific properties
	m_numChannels = 4;
	m_dataType = getGLType<T>();
}


///////////////////////////////////////////////////////////
template <typename T>
inline ImageBuffer<T> Image::getBuffer() const
{
	return ImageBuffer<T>((T*)m_data, m_width, m_height);
}


///////////////////////////////////////////////////////////
template <typename T>
inline T Image::sample(const Vector2f& uv) const
{
#ifndef NDEBUG
	ASSERT(priv::PixelMatchesType<T>::check(m_dataType, m_numChannels), "Requested image pixel using the wrong data type: %s", TypeInfo::get<T>().m_name.c_str());
#endif

	float r = clamp((1.0f - uv.y) * m_height - 0.5f, 0.0f, (float)m_height - 1.0f);
	float c = clamp(uv.x * m_width - 0.5f, 0.0f, (float)m_width - 1.0f);
	Uint32 ri = (Uint32)r;
	Uint32 ci = (Uint32)c;
	float rf = r - (float)ri;
	float cf = c - (float)ci;

	ASSERT(ri < m_height&& ci < m_width, "Requested image pixel row or column is out of bounds: %d, %d", ri, ci);

	Uint32 ro = (ri == m_height - 1 ? 0 : 1);
	Uint32 co = (ci == m_width - 1 ? 0 : 1);

	// Get surrounding pixels
	T* p1 = (T*)m_data + (ri + 0) * m_width + (ci + 0);
	T* p2 = (T*)m_data + (ri + ro) * m_width + (ci + 0);
	T* p3 = (T*)m_data + (ri + 0) * m_width + (ci + co);
	T* p4 = (T*)m_data + (ri + ro) * m_width + (ci + co);

	// Interpolate
	T a1 = mix(*p1, *p2, rf);
	T a2 = mix(*p3, *p4, rf);

	return mix(a1, a2, cf);
}


}