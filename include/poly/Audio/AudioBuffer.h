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
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	AudioBuffer();

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
	/// This function is part of the read stream class. Note that this will treat
	/// the data as bytes, not samples, so the max bytes to read will
	/// be equal to twice the number of samples to read. All read operations
	/// will continue at the location where the last read operation stopped.
	///
	/// \param buffer A pointer to the output buffer to read the data into
	/// \param max The maximum number of bytes to read
	///
	/// \return The actual number of bytes that was read into the output buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 read(void* buffer, Uint32 max) override;

	///////////////////////////////////////////////////////////
	/// \brief Read data from the audio buffer starting at the beggining
	///
	/// The data is read as samples, which is equal to a 16-bit integer.
	///
	/// \param buffer A pointer to the output buffer to read the data into
	/// \param max The maximum number of samples to read
	/// \param offset The offset to start reading data at (in number of samples)
	///
	/// \return The actual number of samples that was read into the output buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 read(Int16* samples, Uint32 max, Uint32 offset = 0);

	///////////////////////////////////////////////////////////
	/// \brief Set the location of where to start reading data from
	///
	/// Note that this function will not affect setSamples() as that
	/// function will always override all existing data. This is used
	/// for reading stream data.
	///
	/// \param offset The read offset in number of samples
	///
	///////////////////////////////////////////////////////////
	void seek(Uint32 offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the samples in the buffer
	///
	/// This set the audio samples in the buffer. All data previously
	/// stored in the buffer will be completely removed. The samples
	/// should be 16-bit signed integers.
	///
	/// \param samples A pointer to the audio samples
	/// \param num The number of samples to store
	/// \param numChannels The number of channels (1 or 2)
	/// \param sampleRate The sample rate
	///
	/// \return True if the samples were successfully set
	///
	///////////////////////////////////////////////////////////
	bool setSamples(Int16* samples, Uint32 num, Uint32 numChannels = 1, Uint32 sampleRate = 44100);

	///////////////////////////////////////////////////////////
	/// \brief Get the number of samples stored in the buffer
	///
	/// \return The number of samples stored in the buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumSamples() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of channels
	///
	/// \return The number of channels
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumChannels() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the audio sample rate
	///
	/// \return The audio sample rate
	///
	///////////////////////////////////////////////////////////
	Uint32 getSampleRate() const;

private:
	sf::SoundBuffer m_buffer;		//!< The internal SFML sound buffer
	Uint32 m_offset;				//!< The buffer read offset
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::AudioBuffer
/// \ingroup Audio
///
/// An audio buffer stores audio samples in an array. Audio data
/// can be stored in the buffer by using open() to load audio sampels
/// from a file with a supported format, or by setting the samples
/// using setSamples(). Data can be read from the buffer as a read stream.
/// See open() for more details on which file formats are supported.
///
/// The audio buffer should be used to store relatively small amounts
/// of audio data so that they can be played with Sound. Uncompressed
/// audio data takes up quite a bit of memory so the stored sounds
/// can't be too long. To play the audio data, connect it to a sound
/// using Sound::setBuffer().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Load audio data
/// AudioBuffer buffer;
/// buffer.open("audio/footsteps.wav");
///
/// // Play a sound
/// Sound sound;
/// sound.setBuffer(&buffer);
/// sound.play();
///
/// \endcode
///
///////////////////////////////////////////////////////////