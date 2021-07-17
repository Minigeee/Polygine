#include <poly/Core/TypeInfo.h>

#include <poly/Math/Functions.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline void Image::getPixel(float r, float c, T* out) const
{
	Uint32 ri = (Uint32)r;
	Uint32 ci = (Uint32)r;
	float rf = r - (float)ri;
	float cf = c - (float)ci;

	ASSERT(ri < m_height&& ci < m_width, "Requested image pixel row or column is out of bounds: %d, %d", ri, ci);

#ifndef NDEBUG
	bool correctType = false;

	if (std::is_same<T, Int8>::value)
		correctType = m_dataType == GLType::Int8;
	else if (std::is_same<T, Uint8>::value)
		correctType = m_dataType == GLType::Uint8;
	else if (std::is_same<T, Int16>::value)
		correctType = m_dataType == GLType::Int16;
	else if (std::is_same<T, Uint16>::value)
		correctType = m_dataType == GLType::Uint16;
	else if (std::is_same<T, Int32>::value)
		correctType = m_dataType == GLType::Int32;
	else if (std::is_same<T, Uint32>::value)
		correctType = m_dataType == GLType::Uint32;
	else if (std::is_same<T, float>::value)
		correctType = m_dataType == GLType::Float;
	else if (std::is_same<T, double>::value)
		correctType = m_dataType == GLType::Double;
	
	ASSERT(correctType, "Requested image pixel using the wrong data type: %s", TypeInfo::get<T>().m_name.c_str());
#endif

	// Get surrounding pixels
	T* p1 = (T*)m_data + ((ri + 0) * m_width + (ci + 0)) * m_numChannels;
	T* p2 = (T*)m_data + ((ri + 1) * m_width + (ci + 0)) * m_numChannels;
	T* p3 = (T*)m_data + ((ri + 0) * m_width + (ci + 1)) * m_numChannels;
	T* p4 = (T*)m_data + ((ri + 1) * m_width + (ci + 1)) * m_numChannels;

	// Interpolate
	for (Uint32 i = 0; i < m_numChannels; ++i)
	{
		T a1 = mix(p1[i], p2[i], rf);
		T a2 = mix(p3[i], p4[i], rf);
		out[i] = mix(a1, a2, cf);
	}
}


}