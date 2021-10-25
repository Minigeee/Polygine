#include <poly/Math/Noise.h>

#include <FastNoiseLite.h>

#define NOISE_CAST(x) reinterpret_cast<FastNoiseLite*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
FractalNoise::FractalNoise() :
	m_generator		(0)
{
	m_generator = new FastNoiseLite();
	NOISE_CAST(m_generator)->SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
}


///////////////////////////////////////////////////////////
FractalNoise::~FractalNoise()
{
	delete (NOISE_CAST(m_generator));
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x) const
{
	return NOISE_CAST(m_generator)->GetNoise(x, 0.0f) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x, float y) const
{
	return NOISE_CAST(m_generator)->GetNoise(x, y) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
float FractalNoise::generate(float x, float y, float z) const
{
	return NOISE_CAST(m_generator)->GetNoise(x, y, z) * 0.5f + 0.5f;
}


///////////////////////////////////////////////////////////
void FractalNoise::generateImage(float* data, Uint32 w, Uint32 h) const
{
	for (Uint32 y = 0, i = 0; y < h; ++y)
	{
		for (Uint32 x = 0; x < w; ++x, ++i)
			data[i] = NOISE_CAST(m_generator)->GetNoise((float)x, (float)y) * 0.5f + 0.5f;
	}
}


///////////////////////////////////////////////////////////
void FractalNoise::generateImage(float* data, Uint32 w, Uint32 h, Uint32 d) const
{
	for (Uint32 z = 0, i = 0; z < d; ++z)
	{
		for (Uint32 y = 0; y < h; ++y)
		{
			for (Uint32 x = 0; x < w; ++x, ++i)
				data[i] = NOISE_CAST(m_generator)->GetNoise((float)x, (float)y, (float)z) * 0.5f + 0.5f;
		}
	}
}


///////////////////////////////////////////////////////////
void FractalNoise::setSeed(int seed)
{
	NOISE_CAST(m_generator)->SetSeed(seed);
}


///////////////////////////////////////////////////////////
void FractalNoise::setFrequency(float freq)
{
	NOISE_CAST(m_generator)->SetFrequency(freq);
}


///////////////////////////////////////////////////////////
void FractalNoise::setOctaves(int octaves)
{
	NOISE_CAST(m_generator)->SetFractalOctaves(octaves);
}


///////////////////////////////////////////////////////////
void FractalNoise::setLacunarity(float lacunarity)
{
	NOISE_CAST(m_generator)->SetFractalLacunarity(lacunarity);
}


///////////////////////////////////////////////////////////
void FractalNoise::setGain(float gain)
{
	NOISE_CAST(m_generator)->SetFractalGain(gain);
}


}