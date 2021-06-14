#ifndef POLY_AUDIO_BUFFER_H
#define POLY_AUDIO_BUFFER_H

#include <poly/Core/Stream.h>

#include <SFML/Audio/SoundBuffer.hpp>

namespace poly
{


class Sound;


///////////////////////////////////////////////////////////
/// \brief A data buffer used to hold static audio data that can quickly be played
///
///////////////////////////////////////////////////////////
class AudioBuffer : public ReadStream
{
	friend Sound;

public:
	///////////////////////////////////////////////////////////
	/// \brief Load audio data from a file
	///
	/// This function loads audio data from an acceptable audio
	/// file format, and stores all data in a buffer. Audio buffers
	/// should be used for relatively short audio clips, that last
	/// no more than a few seconds. The acceptable file formats are:
	/// WAV, OGG/Vorbis, and FLAC.
	///
	/// \param fname The path of the file to load
	///
	/// \return True if the file was loaded without error
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Read data from the read stream into an output buffer
	///
	/// This function is part of the read stream class. Data is
	/// always read starting at the beggining of the buffer because
	/// there shouldn't be much data anyways. Note that this will treat
	/// the data as bytes, not samples, so the max bytes to read will
	/// be equal to twice the number of samples to read.
	///
	/// \param buffer A pointer to the output buffer to read the data into
	/// \param max The maximum number of bytes to read
	///
	/// \return The actual number of bytes that was read into the output buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 read(void* buffer, Uint32 max) override;

	Uint32 read(Int16* samples, Uint32 max);

	bool setSamples(Int16* samples, Uint32 num, Uint32 numChannels = 1, Uint32 sampleRate = 44100);

	Uint32 getNumSamples() const;

	Uint32 getNumChannels() const;

	Uint32 getSampleRate() const;

private:
	sf::SoundBuffer m_buffer;
};


}

#endif