#ifndef POLY_NOISE_H
#define POLY_NOISE_H

#include <poly/Core/DataTypes.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A fractal random noise generator
///
///////////////////////////////////////////////////////////
class FractalNoise
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Initializes the noise generator with:
	/// \li Seed: 1337
	/// \li Frequency: 0.01
	/// \li Octaves: 3
	/// \li Lacunarity: 2.0
	/// \li Gain: 0.5
	///
	///////////////////////////////////////////////////////////
	FractalNoise();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Destroys the noise generator object
	///
	///////////////////////////////////////////////////////////
	~FractalNoise();

	///////////////////////////////////////////////////////////
	/// \brief Generate noise from a 1D point
	///
	/// The returned value is a number between 0 and 1.
	///
	/// \param x The x coordinate of the point
	///
	/// \return A random noise value
	///
	///////////////////////////////////////////////////////////
	float generate(float x);

	///////////////////////////////////////////////////////////
	/// \brief Generate noise from a 2D point
	///
	/// The returned value is a number between 0 and 1.
	///
	/// \param x The x coordinate of the point
	/// \param y The y coordinate of the point
	///
	/// \return A random noise value
	///
	///////////////////////////////////////////////////////////
	float generate(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Generate noise from a 3D point
	///
	/// The returned value is a number between 0 and 1.
	///
	/// \param x The x coordinate of the point
	/// \param y The y coordinate of the point
	/// \param z The z coordinate of the point
	///
	/// \return A random noise value
	///
	///////////////////////////////////////////////////////////
	float generate(float x, float y, float z);

	///////////////////////////////////////////////////////////
	/// \brief Generate a 2D noise image
	///
	/// All noise values are numbers between 0 and 1
	///
	/// \param data A pointer to the image data to generate noise
	/// \param w The width of the image to generate
	/// \param h The height of the image to generate
	///
	///////////////////////////////////////////////////////////
	void generateImage(float* data, Uint32 w, Uint32 h);

	///////////////////////////////////////////////////////////
	/// \brief Generate a 3D noise image
	///
	/// All noise values are numbers between 0 and 1
	///
	/// \param data A pointer to the image data to generate noise
	/// \param w The width of the image to generate
	/// \param h The height of the image to generate
	/// \param d The depth of the image to generate
	///
	///////////////////////////////////////////////////////////
	void generateImage(float* data, Uint32 w, Uint32 h, Uint32 d);

	///////////////////////////////////////////////////////////
	/// \brief Set the noise seed
	///
	/// This value determines the pseudorandom values outputted
	/// by the random number generator used in the noise generator.
	///
	/// \param seed The seed value
	///
	///////////////////////////////////////////////////////////
	void setSeed(int seed);

	///////////////////////////////////////////////////////////
	/// \brief Set noise frequency
	///
	/// This value determines the granularity of the noise, where
	/// higher values lead to coarse results, and smaller values
	/// closer to 0 result in smoother results.
	///
	/// \param freq The frequency value
	///
	///////////////////////////////////////////////////////////
	void setFrequency(float freq);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of octaves
	///
	/// This value determines how many layers of noise are overlayed
	/// generate the final result.
	///
	/// \param octaves The octave value
	///
	///////////////////////////////////////////////////////////
	void setOctaves(int octaves);

	///////////////////////////////////////////////////////////
	/// \brief Set the noise lacunarity
	///
	/// This value is the frequency multiplier between noise layers
	/// when the octave value is higher than 1.
	///
	/// \param lacunarity The lacunarity value
	///
	///////////////////////////////////////////////////////////
	void setLacunarity(float lacunarity);

	///////////////////////////////////////////////////////////
	/// \brief Set the noise gain
	///
	/// This value is the amplitude multiplier between noise layers
	/// when the octave value is higher than 1.
	///
	/// \param gain The gain value
	///
	///////////////////////////////////////////////////////////
	void setGain(float gain);

private:
	void* m_generator;	//!< The noise generator
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::FractalNoise
/// \ingroup Math
///
/// This class is a fractal noise generator that generates random noise
/// values based on the seed value and the input coordinate. The noise
/// generated from this generator depends on the input coordinates
/// and is continuous relative to the coordinates. All noise values
/// range from 0 to 1. This is the fractal variant of the noise
/// generator
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// FractalNoise noise;
///
/// // Generate from a 2D point
/// float n = noise.generate(0.1f, 3.14f);
///
/// // Allocate space and generate a noise image
/// float* image = (float*)malloc(1024 * 1024 * sizeof(float));
/// noise.generateImage(image, 1024, 1024);
///
/// // Free image after done using it
/// free(image);
///
/// \endcode
///
///////////////////////////////////////////////////////////