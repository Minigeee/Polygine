
namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline T clamp(T val, T min, T max)
{
	return val < min ? min : (val > max ? max : val);
}


///////////////////////////////////////////////////////////
template <typename T>
inline T mix(T a, T b, float factor)
{
	return T(a + factor * (b - a));
}



}