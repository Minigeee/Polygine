#ifndef POLY_NOISE_H
#define POLY_NOISE_H

#include <poly/Core/DataTypes.h>

#include <FastNoiseLite.h>

namespace poly
{

class FractalNoise
{
public:
	FractalNoise();

	float generate(float x);

	float generate(float x, float y);

	float generate(float x, float y, float z);

	void generateImage(float* data, Uint32 w, Uint32 h);

	void generateImage(float* data, Uint32 w, Uint32 h, Uint32 z);

	void setFrequency(float freq);

	void setOctaves(int octaves);

	void setLacunarity(float lacunarity);

	void setGain(float gain);

private:
	FastNoiseLite m_noise;
};

}

#endif