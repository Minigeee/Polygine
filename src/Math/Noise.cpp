#include <poly/Math/Noise.h>

namespace poly
{


///////////////////////////////////////////////////////////
FractalNoise::FractalNoise()
{
	m_noise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x)
{
	return m_noise.GetNoise(x, 0.0f) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x, float y)
{
	return m_noise.GetNoise(x, y) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x, float y, float z)
{
	return m_noise.GetNoise(x, y, z) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
void FractalNoise::generateImage(float* data, Uint32 w, Uint32 h)
{
	for (Uint32 y = 0, i = 0; y < h; ++y)
	{
		for (Uint32 x = 0; x < w; ++x, ++i)
			data[i] = m_noise.GetNoise((float)x, (float)y) * 0.5f + 0.5f;
	}
}


///////////////////////////////////////////////////////////
void FractalNoise::generateImage(float* data, Uint32 w, Uint32 h, Uint32 d)
{
	for (Uint32 z = 0, i = 0; z < d; ++z)
	{
		for (Uint32 y = 0; y < h; ++y)
		{
			for (Uint32 x = 0; x < w; ++x, ++i)
				data[i] = m_noise.GetNoise((float)x, (float)y, (float)z) * 0.5f + 0.5f;
		}
	}
}


///////////////////////////////////////////////////////////
void FractalNoise::setSeed(int seed)
{
	m_noise.SetSeed(seed);
}


///////////////////////////////////////////////////////////
void FractalNoise::setFrequency(float freq)
{
	m_noise.SetFrequency(freq);
}


///////////////////////////////////////////////////////////
void FractalNoise::setOctaves(int octaves)
{
	m_noise.SetFractalOctaves(octaves);
}


///////////////////////////////////////////////////////////
void FractalNoise::setLacunarity(float lacunarity)
{
	m_noise.SetFractalLacunarity(lacunarity);
}


///////////////////////////////////////////////////////////
void FractalNoise::setGain(float gain)
{
	m_noise.SetFractalGain(gain);
}


}